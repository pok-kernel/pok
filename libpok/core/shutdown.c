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
 *                                      Copyright (c) 2007-2021 POK team
 */

/**
 * \file libpok/include/core/shutdown.c
 * \brief This file implements a shutdown function.
 * \author Romain Guilloteau
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_SHUTDOWN

#include <core/shutdown.h>
#include <core/syscall.h>

#ifdef POK_NEEDS_DEBUG
#include <libc/stdio.h>
#endif

void pok_shutdown() {
  pok_syscall0(POK_SYSCALL_SHUTDOWN);
  // The syscal should never return. If it does, this is an error, but there
  // is little we can do.
#ifdef POK_NEEDS_DEBUG
  printf("[LIBPOK] [SHUTDOWN] Shutdown system call failed\n");
#endif
  for (;;)
    ;
}

#endif
