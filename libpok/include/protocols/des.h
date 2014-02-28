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
 * Created by julien on Mon Nov 30 20:42:30 2009 
 */

#ifndef __LIBPOK_PROTOCOLS_DES_H__
#define __LIBPOK_PROTOCOLS_DES_H__

/**
 * \file    libpok/protocols/des.h
 * \author  Julien Delange
 * \date    2009
 * \brief   DES protocol.
 *
 * Implementation of the very basic DES crypto
 * protocol. This is a symetric crypto protocol
 * with a shared key so that receiver and sender
 * share the same key.
 *
 * More information at:
 * http://en.wikipedia.org/wiki/Data_Encryption_Standard
 */


#include <types.h>

#define pok_protocols_des_data_t unsigned long long

#ifdef POK_NEEDS_PROTOCOLS_DES
/**
 * Function that uncrypts data.
 */
void pok_protocols_des_unmarshall (void* crypted_data, pok_size_t crypted_size, void* uncrypted_data, size_t* uncrypted_size);

/**
 * Function that crypts data.
 */
void pok_protocols_des_marshall (void* uncrypted_data, pok_size_t uncrypted_size, void* crypted_data, size_t* crypted_size);

/**
 * The key for the DES protocol is on 8 bytes and is
 * defined by the macro POK_PROTOCOLS_DES_KEY
 */
#ifndef POK_PROTOCOLS_DES_KEY
#define POK_PROTOCOLS_DES_KEY {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef}
#endif

/**
 * The init vector for the DES protocol is on 8 bytes
 * defined by the macro POK_PROTOCOLS_DES_INIT
 */
#ifndef POK_PROTOCOLS_DES_INIT
#define POK_PROTOCOLS_DES_INIT {0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10}
#endif

#endif

#endif
