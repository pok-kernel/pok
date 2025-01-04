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
 *                                      Copyright (c) 2007-2025 POK team
 */

#ifndef __POK_X86_PM_H__
#define __POK_X86_PM_H__

#define MEM_16MB 0x1000000

int pok_pm_init();
uint32_t pok_pm_sbrk(uint32_t increment);

#endif
