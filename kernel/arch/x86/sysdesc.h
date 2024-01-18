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
 *                                      Copyright (c) 2007-2024 POK team
 */

#ifndef __POK_X86_SYSDESC_H__
#define __POK_X86_SYSDESC_H__

typedef struct {
  uint16_t limit;
  uint32_t base;
  uint16_t padding;
} __attribute__((packed)) sysdesc_t;

#endif /* !__POK_X86_SYSDESC_H__ */
