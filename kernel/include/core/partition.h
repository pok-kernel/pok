/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */

/**
 * \file    partition.h
 * \brief   Definition of structure for partitioning services.
 * \author  Julien Delange
 */

#ifndef __POK_PARTITION_H__
#define __POK_PARTITION_H__

#ifdef POK_NEEDS_PARTITIONS

#include <types.h>
#include <errno.h>
#include <core/error.h>
#include <core/thread.h>
#include <core/sched.h>


/**
 * \enum pok_partition_mode_t
 * \brief The different modes of a partition
 */
typedef enum
{
   POK_PARTITION_MODE_INIT_COLD = 1, /**< Partition is in cold mode (init mode),
                                       only the main thread is active */
   POK_PARTITION_MODE_INIT_WARM = 2, /**< Partition is in warm init mode,
                                       only the main thread is active */
   POK_PARTITION_MODE_NORMAL    = 3, /**< Partition is currently being executed,
                                       main thread is disabled, other threads
                                       are running
                                     */
   POK_PARTITION_MODE_IDLE      = 4, /**< Idle mode, the partition is not executed */
   POK_PARTITION_MODE_RESTART   = 5, /**< Currently being restarted */
   POK_PARTITION_MODE_STOPPED   = 6, /**< Partition is stopped, it will no longer be executed */
}pok_partition_mode_t;

typedef enum
{
  NORMAL_START          = 0,
  PARTITION_RESTART     = 1,
  HM_MODULE_RESTART     = 2,
  HM_PARTITION_RESTART  = 3
}pok_start_condition_t;


/*!
 * \struct pok_partition_t
 * \brief This structure contains all needed information for partition management
 */
typedef struct
{
   uint32_t              base_addr;    /**< The base address inside the whole memory (where the segment is in the whole memory ?) */
   uint32_t              base_vaddr;   /**< The virtual address of the partition. The address the threads sees when they are
                                        *    executed
                                       */

   uint32_t              size;         /**< Size of the allocated memory segment */

   const char            *name;        /**< Name of the partition */

   uint32_t              nthreads;     /**< Number of threads inside the partition */

   uint8_t               priority;     /**< Priority of the partition (unused at this time */
   uint32_t              period;       /**< Period of the partition, unused at this time */

   pok_sched_t           sched;       /**< The associated for the partition to schedule its threads */

   uint32_t (*sched_func)(uint32_t low, uint32_t high,uint32_t prev_thread, uint32_t cur_thread); /**< Scheduling function to scheduler threads */

   uint64_t              activation;                    /**< Last activation time of the partition */
   uint32_t              prev_thread;           /**< member for the scheduler (previous scheduled real thread inside the partition,i.e not the idle thread */
   uint32_t              current_thread;                /**< member for the scheduler (current executed thread inside the partition */

   uint32_t               thread_index_low;    /**< The low index in the threads table */
   uint32_t               thread_index_high;   /**< The high index in the threads table */
   uint32_t               thread_index;        /**< The thread index */

#if defined(POK_NEEDS_LOCKOBJECTS) || defined(POK_NEEDS_ERROR_HANDLING)
   uint8_t               lockobj_index_low;   /**< The low bound in the lockobject array. */
   uint8_t               lockobj_index_high;  /**< The high bound in the lockobject array */
   uint8_t               nlockobjs;           /**< The amount of lockobjects reserved for the partition */
#endif

#ifdef POK_NEEDS_SCHED_HFPPS
   uint64_t		payback; /**< Payback for HFPPS scheduling algorithm */
#endif /* POK_NEEDS_SCHED_HFPPS */

#ifdef POK_NEEDS_ERROR_HANDLING
   uint32_t             thread_error;         /**< The thread identifier used for error handling */
   pok_error_status_t   error_status;        /**< A pointer used to store information about errors */
#endif
   uint32_t             thread_main;          /**< The thread identifier of the main thread (initialization thread) */
   uint32_t             thread_main_entry;    /**< The entry-point of the main thread (useful for re-init) */
   pok_partition_mode_t mode;                 /**< Current mode of the partition */

#ifdef POK_NEEDS_IO
  uint16_t		io_min;                        /**< If the partition is allowed to perform I/O, the lower bound of the I/O */
  uint16_t		io_max;                        /**< If the partition is allowed to perform I/O, the uppder bound of the I/O */
#endif

  uint32_t		lock_level;
  pok_start_condition_t	start_condition;
} pok_partition_t;

extern pok_partition_t pok_partitions[POK_CONFIG_NB_PARTITIONS];

/**
 * Access to the current partition variable.
 * With that, you can do POK_CURRENT_PARTITION.nthreads of POK_CURRENT_PARTITION.mode
 * It avoids tedious syntax like pok_partitions[my_partition].blablabla
 */
#define POK_CURRENT_PARTITION pok_partitions[POK_SCHED_CURRENT_PARTITION]

/**
 * Check that [ \a ptr ; \a ptr + \a size [ is located in the address
 * space of partition \a pid. User partition \a ptr is relative to 0.
 */
static inline bool_t pok_check_ptr_in_partition(
   pok_partition_id_t pid, void *ptr, uint32_t size)
{
   if (pid >= POK_CONFIG_NB_PARTITIONS)
      return FALSE;

   uint32_t psize = pok_partitions[pid].size;

   if (size > psize)
      return FALSE;

   if ((uint32_t)ptr <= (psize - size))
      return TRUE;

   return FALSE;
}


/**
 * Initialize all partitions
 */
pok_ret_t pok_partition_init();

pok_ret_t pok_partition_set_mode (const uint8_t pid, const pok_partition_mode_t mode);
pok_ret_t pok_partition_set_mode_current (const pok_partition_mode_t mode);


pok_ret_t pok_partition_stop_thread (const uint32_t tid);

void pok_partition_reinit (const uint8_t);

void pok_partition_setup_main_thread (const uint8_t);

void pok_partition_setup_scheduler (const uint8_t pid);

pok_ret_t pok_partition_restart_thread (const uint32_t tid);

pok_ret_t pok_current_partition_get_id (uint8_t *id);

pok_ret_t pok_current_partition_get_period (uint64_t *period);

pok_ret_t pok_current_partition_get_duration (uint64_t *duration);

pok_ret_t pok_current_partition_get_operating_mode (pok_partition_mode_t *op_mode);

pok_ret_t pok_current_partition_get_lock_level (uint32_t *lock_level);

pok_ret_t pok_current_partition_get_start_condition (pok_start_condition_t *start_condition);

#endif /* __POK_NEEDS_PARTITIONS */

#endif /* __POK_PARTITION_H__ */
