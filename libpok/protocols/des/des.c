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
 * Created by julien on Mon Dec  7 15:16:48 2009 
 */


/**
 * \file    libpok/protocols/des/des.c
 * \author  Julien Delange
 * \date    2009
 * \brief   DES crypto protocol.
 *
 * This file is a wrapper that interfaces with OpenSSL functions.
 * It sets the crypto key, initialisation string and calls
 * OpenSSL function to crypts data.
 */


#include <protocols/des.h>
#include <libc/string.h>
#include <types.h>
#include "des.h"

#ifdef POK_NEEDS_PROTOCOLS_DES

unsigned char initVector[8] = POK_PROTOCOLS_DES_INIT;

static unsigned char cbc_key [8]= POK_PROTOCOLS_DES_KEY;

int pok_protocols_des_is_init = 0;

void pok_protocols_des_init ()
{
   if (pok_protocols_des_is_init == 1)
   {
      return;
   }

   pok_protocols_des_is_init = 1;

}

void pok_protocols_des_marshall (void* uncrypted_data, pok_size_t uncrypted_size, void* crypted_data, size_t* crypted_size)
{
   DES_cblock ivec;
   DES_key_schedule schedule;

   DES_set_key_checked (&cbc_key, &schedule);

   memcpy(ivec,initVector,sizeof(initVector));

   DES_ncbc_encrypt(uncrypted_data, crypted_data, uncrypted_size, &schedule, &ivec, DES_ENCRYPT);
   *crypted_size = 8;
}


void pok_protocols_des_unmarshall (void* crypted_data, pok_size_t crypted_size, void* uncrypted_data, size_t* uncrypted_size)
{
   DES_cblock ivec;
   DES_key_schedule schedule;

   DES_set_key_checked (&cbc_key, &schedule);

   memcpy(ivec,initVector,sizeof(initVector));
   DES_ncbc_encrypt(crypted_data, uncrypted_data, crypted_size, &schedule, &ivec, DES_DECRYPT);

   *uncrypted_size = 8;
}

#endif

