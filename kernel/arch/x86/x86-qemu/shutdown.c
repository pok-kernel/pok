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
 * \file kernel/arch/x86/x86-qemu/shutdown.c
 * \brief This file implements a shutdown function for x86-qemu BSP.
 * \author Romain Guilloteau
 */

#include <dependencies.h>

#ifdef POK_NEEDS_SHUTDOWN

#include <arch/x86/ioports.h>
#include <core/shutdown.h>

void pok_shutdown() { outw(0x604, 0x2000); }

#endif
