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
 * Created by julien on Fri Dec 11 16:32:31 2009 
 */


#include <types.h>

#define pok_protocols_blowfish_data_t uint64_t

#ifdef POK_NEEDS_PROTOCOLS_BLOWFISH

void pok_protocols_blowfish_marshall (void* uncrypted_data, size_t uncrypted_size, void* crypted_data, size_t* crypted_size);


void pok_protocols_blowfish_unmarshall (void* crypted_data, size_t crypted_size, void* uncrypted_data, size_t* uncrypted_size);

#ifndef POK_BLOWFISH_KEY
#define POK_BLOWFISH_KEY {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xf0,0xe1,0xd2,0xc3,0xb4,0xa5,0x96,0x87}
#endif

#ifndef POK_BLOWFISH_INIT
#define POK_BLOWFISH_INIT {0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10}
#endif

#endif
