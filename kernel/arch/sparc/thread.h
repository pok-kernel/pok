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
 * @file
 * @author Fabien Chouteau
 */

#ifndef __POK_SPARC_THREAD_H__
#define __POK_SPARC_THREAD_H__

#include <types.h>

uint32_t  pok_context_create(uint32_t id,
                             uint32_t stack_size,
                             uint32_t entry);

extern uint32_t pok_arch_sp;

void  pok_context_switch(uint32_t* old_sp,
                         uint32_t  new_sp);


#endif /* !__POK_PPC_THREAD_H__ */

