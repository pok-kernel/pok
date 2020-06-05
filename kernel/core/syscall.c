/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2020 POK team
 */

#include <arch/x86/ioports.h>
#include <arch/x86/pci.h>
#include <bsp.h>
#include <libc.h>
#include <types.h>

#include <core/debug.h>
#include <core/error.h>
#include <core/lockobj.h>
#include <core/partition.h>
#include <core/shutdown.h>
#include <core/syscall.h>
#include <core/thread.h>
#include <core/time.h>
#include <errno.h>

#include <middleware/port.h>

#if defined POK_NEEDS_PORTS_QUEUEING || defined POK_NEEDS_PORTS_SAMPLING ||    \
    defined POK_NEEDS_PORTS_VIRTUAL
extern uint32_t pok_ports_names_max_len;
#endif

// Bellow, POK_NEEDS_PCI is kept for backward compatibility with code generators
#if (defined POK_NEEDS_RTL8029 || defined POK_NEEDS_PCI)
#include <drivers/rtl8029.h>
#endif

// Check that the syscall has been given the right number of arguments
#define CHECK_NARGS(expected_nargs)                                            \
  do {                                                                         \
    if (args->nargs != (expected_nargs)) {                                     \
      return POK_ERRNO_EINVAL;                                                 \
    }                                                                          \
  } while (0)

// Check that the given pointer and size belong to the calling partition
#define CHECK_PTR(ptr, sz)                                                     \
  do {                                                                         \
    if (!pok_check_ptr_in_partition(infos->partition, (ptr), (sz))) {          \
      return POK_ERRNO_EINVAL;                                                 \
    }                                                                          \
  } while (0)

/**
 * \file kernel/core/syscalls.c
 * \brief This file implement generic system calls
 * \author Julien Delange
 */

pok_ret_t pok_core_syscall(const pok_syscall_id_t syscall_id,
                           const pok_syscall_args_t *args,
                           const pok_syscall_info_t *infos) {
  switch (syscall_id) {
#if defined(POK_NEEDS_CONSOLE) || defined(POK_NEEDS_DEBUG)
  case POK_SYSCALL_CONSWRITE:
    CHECK_PTR((void *)args->arg1, args->arg2);

    if (pok_cons_write((const char *)args->arg1 + infos->base_addr,
                       args->arg2)) {
      return POK_ERRNO_OK;
    } else {
      return POK_ERRNO_EINVAL;
    }
#endif
#ifdef POK_NEEDS_CONSOLE
  case POK_SYSCALL_GETCHAR:
    CHECK_PTR((void *)args->arg1, NULL);

    pok_cons_get_char((char *)args->arg1 + infos->base_addr);
    return POK_ERRNO_OK;
#endif

#ifdef POK_NEEDS_PORTS_VIRTUAL
  case POK_SYSCALL_MIDDLEWARE_VIRTUAL_CREATE:
    CHECK_PTR((void *)args->arg1, pok_ports_names_max_len + 1);

    CHECK_PTR((void *)args->arg2, sizeof(pok_port_id_t));

    return pok_port_virtual_id(
        (char *)(args->arg1 + infos->base_addr),
        (pok_port_id_t *)(args->arg2 + infos->base_addr));

  case POK_SYSCALL_MIDDLEWARE_VIRTUAL_NB_DESTINATIONS:
    CHECK_PTR((void *)args->arg2, sizeof(uint32_t));

    return pok_port_virtual_nb_destinations(
        (pok_port_id_t)(args->arg1),
        (uint32_t *)(args->arg2 + infos->base_addr));

  case POK_SYSCALL_MIDDLEWARE_VIRTUAL_DESTINATION:
    CHECK_PTR((void *)args->arg3, sizeof(uint32_t));

    return pok_port_virtual_destination(
        (pok_port_id_t)(args->arg1), (uint32_t)(args->arg2),
        (uint32_t *)(args->arg3 + infos->base_addr));

  case POK_SYSCALL_MIDDLEWARE_VIRTUAL_GET_GLOBAL:
    CHECK_PTR((void *)args->arg2, sizeof(pok_port_id_t));

    return pok_port_virtual_get_global(
        (pok_port_id_t)(args->arg1),
        (pok_port_id_t *)(args->arg2 + infos->base_addr));

  case POK_SYSCALL_MIDDLEWARE_VIRTUAL_NODE:
    CHECK_PTR((void *)args->arg2, sizeof(uint32_t));

    return pok_port_virtual_node((uint32_t)(args->arg1),
                                 (uint8_t *)(args->arg2 + infos->base_addr));
#endif

#ifdef POK_NEEDS_MAC_ADDR
  case POK_SYSCALL_MIDDLEWARE_NODE_MAC_ADDR:
    CHECK_PTR((void *)args->arg1, 6);

    CHECK_PTR((void *)args->arg2, sizeof(uint32_t));

    return pok_node_mac_addr((uint8_t *)(args->arg1 + infos->base_addr),
                             (char *)(args->arg2 + infos->base_addr));
#endif

#if defined POK_NEEDS_GETTICK
  case POK_SYSCALL_GETTICK:
    CHECK_PTR((void *)args->arg1, sizeof(uint64_t));

    uint64_t *const addr = (uint64_t *)(args->arg1 + infos->base_addr);
    *addr = pok_gettick();
    return POK_ERRNO_OK;
#endif

  case POK_SYSCALL_THREAD_CREATE:
    CHECK_PTR((void *)args->arg1, sizeof(uint32_t));

    CHECK_PTR((void *)args->arg2, sizeof(pok_thread_attr_t));

    return pok_partition_thread_create(
        (uint32_t *)(args->arg1 + infos->base_addr),
        (pok_thread_attr_t *)(args->arg2 + infos->base_addr),
        (uint8_t)infos->partition);

#ifdef POK_NEEDS_THREAD_SLEEP
  case POK_SYSCALL_THREAD_SLEEP:
    return pok_thread_sleep(args->arg1);
#endif

#ifdef POK_NEEDS_THREAD_SLEEP_UNTIL
  case POK_SYSCALL_THREAD_SLEEP_UNTIL:
    return pok_thread_sleep_until(args->arg1);
#endif

  case POK_SYSCALL_THREAD_PERIOD:
    return pok_sched_end_period();

  case POK_SYSCALL_THREAD_SUSPEND:
    return pok_thread_suspend();

#ifdef POK_NEEDS_THREAD_ID
  case POK_SYSCALL_THREAD_ID:
    CHECK_PTR((void *)args->arg1, sizeof(uint32_t));

    return pok_sched_get_current((uint32_t *)(args->arg1 + infos->base_addr));
#endif
  case POK_SYSCALL_THREAD_STATUS:
    CHECK_PTR((void *)args->arg2, sizeof(pok_thread_attr_t));

    return pok_thread_get_status(
        args->arg1, (pok_thread_attr_t *)(args->arg2 + infos->base_addr));

  case POK_SYSCALL_THREAD_DELAYED_START:
    return pok_thread_delayed_start(args->arg1, args->arg2);
  case POK_SYSCALL_THREAD_SET_PRIORITY:
    return pok_thread_set_priority(args->arg1, args->arg2);
  case POK_SYSCALL_THREAD_RESUME:
    return pok_thread_resume(args->arg1);
  case POK_SYSCALL_THREAD_SUSPEND_TARGET:
    return pok_thread_suspend_target(args->arg1);
  case POK_SYSCALL_THREAD_RESTART:
    return pok_partition_restart_thread(args->arg1);
  case POK_SYSCALL_THREAD_STOP:
    return pok_partition_stop_thread(args->arg1);

  /**
   * STOPSELF used by the error thread
   */
  case POK_SYSCALL_THREAD_STOPSELF:
    pok_sched_stop_self();
    return POK_ERRNO_OK;

  case POK_SYSCALL_PARTITION_SET_MODE:
    return pok_partition_set_mode_current((pok_partition_mode_t)args->arg1);

  case POK_SYSCALL_PARTITION_GET_ID:
    CHECK_PTR((void *)args->arg1, sizeof(uint8_t));

    return pok_current_partition_get_id(
        (uint8_t *)(args->arg1 + infos->base_addr));

  case POK_SYSCALL_PARTITION_GET_PERIOD:
    CHECK_PTR((void *)args->arg1, sizeof(uint64_t));

    return pok_current_partition_get_period(
        (uint64_t *)(args->arg1 + infos->base_addr));

  case POK_SYSCALL_PARTITION_GET_DURATION:
    CHECK_PTR((void *)args->arg1, sizeof(uint64_t));

    return pok_current_partition_get_duration(
        (uint64_t *)(args->arg1 + infos->base_addr));

  case POK_SYSCALL_PARTITION_GET_LOCK_LEVEL:
    CHECK_PTR((void *)args->arg1, sizeof(uint32_t));

    return pok_current_partition_get_lock_level(
        (uint32_t *)(args->arg1 + infos->base_addr));

  case POK_SYSCALL_PARTITION_GET_OPERATING_MODE:
    CHECK_PTR((void *)args->arg1, sizeof(pok_partition_mode_t));

    return pok_current_partition_get_operating_mode(
        (pok_partition_mode_t *)(args->arg1 + infos->base_addr));

  case POK_SYSCALL_PARTITION_GET_START_CONDITION:
    CHECK_PTR((void *)args->arg1, sizeof(pok_start_condition_t));

    return pok_current_partition_get_start_condition(
        (pok_start_condition_t *)(args->arg1 + infos->base_addr));

  case POK_SYSCALL_ERROR_HANDLER_CREATE:
    return pok_error_thread_create(args->arg1, (void *)(args->arg2));

  case POK_SYSCALL_ERROR_RAISE_APPLICATION_ERROR:
    CHECK_PTR((void *)args->arg1, args->arg2);

    pok_error_raise_application_error((char *)(args->arg1 + infos->base_addr),
                                      args->arg2);
    return POK_ERRNO_OK;

  case POK_SYSCALL_ERROR_GET:
    CHECK_PTR((void *)args->arg1, sizeof(pok_error_status_t));

    return pok_error_get((pok_error_status_t *)(args->arg1 + infos->base_addr));

    /* Middleware syscalls */
#ifdef POK_NEEDS_PORTS_SAMPLING
  case POK_SYSCALL_MIDDLEWARE_SAMPLING_CREATE:
    CHECK_PTR((void *)args->arg1, pok_ports_names_max_len + 1);

    CHECK_PTR((void *)args->arg5, sizeof(pok_port_id_t));

    return pok_port_sampling_create(
        (char *)(args->arg1 + infos->base_addr), (pok_port_size_t)args->arg2,
        (pok_port_direction_t)args->arg3, (uint64_t)args->arg4,
        (pok_port_id_t *)(args->arg5 + infos->base_addr));

  case POK_SYSCALL_MIDDLEWARE_SAMPLING_WRITE:
    CHECK_PTR((void *)args->arg2, args->arg3);

    return pok_port_sampling_write(
        (const pok_port_id_t)args->arg1,
        (const void *)((void *)args->arg2 + infos->base_addr),
        (const uint8_t)args->arg3);

  case POK_SYSCALL_MIDDLEWARE_SAMPLING_READ:
    /* arg2 is checked later in pok_port_sampling_read() once we know access
     * size */

    CHECK_PTR((void *)args->arg3, sizeof(pok_port_size_t));

    CHECK_PTR((void *)args->arg4, sizeof(bool_t));

    return pok_port_sampling_read(
        (const pok_port_id_t)args->arg1, (void *)args->arg2 + infos->base_addr,
        (pok_port_size_t *)(args->arg3 + infos->base_addr),
        (bool_t *)(args->arg4 + infos->base_addr));

  case POK_SYSCALL_MIDDLEWARE_SAMPLING_ID:
    CHECK_PTR((void *)args->arg1, pok_ports_names_max_len + 1);

    CHECK_PTR((void *)args->arg2, sizeof(pok_port_id_t));

    return pok_port_sampling_id(
        (char *)(args->arg1 + infos->base_addr),
        (pok_port_id_t *)(args->arg2 + infos->base_addr));

#ifndef POK_GENERATED_CODE
  case POK_SYSCALL_MIDDLEWARE_SAMPLING_STATUS:
    CHECK_PTR((void *)args->arg2, sizeof(pok_port_sampling_status_t));

    return pok_port_sampling_status(
        (const pok_port_id_t)args->arg1,
        (pok_port_sampling_status_t *)(args->arg2 + infos->base_addr));
#endif /* POK_GENERATED_CODE */
#endif /* POK_NEEDS_PORTS_SAMPLING */

#ifdef POK_NEEDS_PORTS_QUEUEING
  case POK_SYSCALL_MIDDLEWARE_QUEUEING_CREATE:
    CHECK_PTR((void *)args->arg1, pok_ports_names_max_len + 1);

    CHECK_PTR((void *)args->arg5, sizeof(pok_port_id_t));

    return pok_port_queueing_create(
        (char *)(args->arg1 + infos->base_addr), (pok_port_size_t)args->arg2,
        (pok_port_direction_t)args->arg3,
        (pok_port_queueing_discipline_t)args->arg4,
        (pok_port_id_t *)(args->arg5 + infos->base_addr));

  case POK_SYSCALL_MIDDLEWARE_QUEUEING_SEND:
    CHECK_PTR((void *)args->arg2, args->arg3);

    return pok_port_queueing_send(
        (const pok_port_id_t)args->arg1,
        (const void *)((void *)args->arg2 + infos->base_addr),
        (const uint8_t)(args->arg3), (const uint64_t)args->arg4);

  case POK_SYSCALL_MIDDLEWARE_QUEUEING_RECEIVE:
    CHECK_PTR((void *)args->arg4, args->arg3);

    CHECK_PTR((void *)args->arg5, sizeof(pok_port_size_t));

    return pok_port_queueing_receive(
        (const pok_port_id_t)args->arg1, (uint64_t)args->arg2,
        (pok_port_size_t)args->arg3,
        (void *)((void *)args->arg4 + infos->base_addr),
        (pok_port_size_t *)(args->arg5 + infos->base_addr));

  case POK_SYSCALL_MIDDLEWARE_QUEUEING_ID:
    CHECK_PTR((void *)args->arg1, pok_ports_names_max_len + 1);

    CHECK_PTR((void *)args->arg2, sizeof(pok_port_id_t));

    return pok_port_queueing_id(
        (char *)(args->arg1 + infos->base_addr),
        (pok_port_id_t *)(args->arg2 + infos->base_addr));

#ifndef POK_GENERATED_CODE
  case POK_SYSCALL_MIDDLEWARE_QUEUEING_STATUS:
    CHECK_PTR((void *)args->arg2, sizeof(pok_port_queueing_status_t));

    return pok_port_queueing_status(
        (const pok_port_id_t)args->arg1,
        (pok_port_queueing_status_t *)(args->arg2 + infos->base_addr));
#endif
#endif /* POK_NEEDS_PORTS_QUEUEING */

#ifdef POK_NEEDS_LOCKOBJECTS
  case POK_SYSCALL_LOCKOBJ_CREATE:
    CHECK_PTR((void *)args->arg1, sizeof(pok_lockobj_id_t));

    CHECK_PTR((void *)args->arg2, sizeof(pok_lockobj_attr_t));

    return pok_lockobj_partition_create(
        (pok_lockobj_id_t *)(args->arg1 + infos->base_addr),
        (pok_lockobj_attr_t *)(args->arg2 + infos->base_addr));

  case POK_SYSCALL_LOCKOBJ_OPERATION:
    if (args->arg2 == NULL) {
      return pok_lockobj_partition_wrapper((const uint8_t)args->arg1, NULL);
    } else {
      CHECK_PTR((void *)args->arg2, sizeof(pok_lockobj_attr_t));

      return pok_lockobj_partition_wrapper(
          (const uint8_t)args->arg1,
          (pok_lockobj_lockattr_t *)(args->arg2 + infos->base_addr));
    }
#endif /* POK_NEEDS_LOCKOBJECTS */

#ifdef POK_NEEDS_IO
  case POK_SYSCALL_INB:
    if ((args->arg1 < pok_partitions[infos->partition].io_min) ||
        (args->arg1 > pok_partitions[infos->partition].io_max)) {
      return -POK_ERRNO_EPERM;
    } else {
      return inb((unsigned short)args->arg1);
    }

  case POK_SYSCALL_OUTB:
    if ((args->arg1 < pok_partitions[infos->partition].io_min) ||
        (args->arg1 > pok_partitions[infos->partition].io_max)) {
      return -POK_ERRNO_EPERM;
    } else {
      outb((unsigned short)args->arg1, (unsigned char)args->arg2);
      return POK_ERRNO_OK;
    }
#endif /* POK_NEEDS_IO */

#ifdef POK_NEEDS_PCI
  case POK_SYSCALL_PCI_REGISTER:
    CHECK_PTR((void *)args->arg1, sizeof(s_pci_device));

    return pci_register((void *)args->arg1 + infos->base_addr,
                        infos->partition);
#endif /* POK_NEEDS_PCI */

// Bellow, POK_NEEDS_PCI is kept for backward compatibility with code generators
#if (defined POK_NEEDS_RTL8029 || defined POK_NEEDS_PCI)
  case POK_SYSCALL_RTL8929_READ:
    CHECK_PTR((void *)args->arg2, sizeof(uint32_t));

    rtl8029_read((pok_port_id_t)(args->arg1),
                 (uint32_t *)(args->arg2 + infos->base_addr),
                 (uint32_t)(args->arg3));

    break;

  case POK_SYSCALL_RTL8929_POLL_AND_READ:
    CHECK_PTR((void *)args->arg2, sizeof(uint32_t));

    rtl8029_poll_and_read((pok_port_id_t)(args->arg1),
                          (uint32_t *)(args->arg2 + infos->base_addr),
                          (uint32_t)(args->arg3));

    break;

  case POK_SYSCALL_RTL8929_WRITE:
    CHECK_PTR((void *)args->arg2, sizeof(uint32_t));

    rtl8029_write((pok_port_id_t)(args->arg1),
                  (uint32_t *)(args->arg2 + infos->base_addr),
                  (uint32_t)(args->arg3));
    break;

  case POK_SYSCALL_RTL8929_POLL:
    rtl8029_poll();
    break;

  case POK_SYSCALL_RTL8929_CLEAR:
    rtl8029_clear();
    break;

  case POK_SYSCALL_RTL8929_INIT:
    rtl8029_init();
    break;
#endif

#ifdef POK_NEEDS_SHUTDOWN

  case POK_SYSCALL_SHUTDOWN:
    CHECK_NARGS(0);
    pok_shutdown();

    // If the execution reaches this point that means the shutdown has failed
    // or is asynchronous and takes time.
    pok_arch_preempt_disable();
    while (1)
      ;

#endif

  /**
   * Here is the default syscall handler. In this case, the syscall
   * ID was not properly identified and thus, we should return an
   * error. If error management is activated, we raise an error
   * in kernel of partitions, calling the error handler.
   */
  default:
    pok_error_declare(POK_ERROR_KIND_ILLEGAL_REQUEST);
    pok_sched_activate_error_thread();
    break;
  }

  return POK_ERRNO_EINVAL;
}
