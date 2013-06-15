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


#ifndef __POK_LOADER_H__
#define __POK_LOADER_H__

#include <types.h>

/**
 * \brief Load the program of the partition
 *
 * It loads the program of the partition \a part_id. In fact,
 * It will load the ELF file that corresponds to this partition.
 */
void pok_loader_load_partition (const uint8_t part_id,
                                     uint32_t offset,
                                     uint32_t *entry);
#endif /* !__POK_LOADER_H__ */

