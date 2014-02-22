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
 * \file    arch/x86/space.h
 * \author  Julian Pidancet
 * \date    2008-2009
 */

#ifndef __POK_X86_SPACE_H__
#define __POK_X86_SPACE_H__

#include <types.h>
#include "thread.h"

typedef struct
{
  context_t ctx;
  uint32_t fake_ret;
  unsigned int partition_id;
  uint32_t user_pc;
  uint32_t user_sp;
  uint32_t kernel_sp;
  uint32_t arg1;
  uint32_t arg2;
} space_context_t;

#endif /* !__POK_X86_SPACE_H__ */

