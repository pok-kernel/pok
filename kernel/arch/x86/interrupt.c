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

#include "tss.h"
#include <arch/x86/interrupt.h>

void update_tss(interrupt_frame *frame) {
  // Update esp0 in TSS to the given frame if we are not executing in
  // code segment 1 (kernel).
  if ((frame->cs & 0xffff) != (1 << 3)) {
    pok_tss.esp0 = (uint32_t)frame + sizeof(interrupt_frame);
  }
}
