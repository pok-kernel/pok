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

/**
 * Remember boys, blowfish encrypts data on 8 bytes !
 */

#ifdef POK_NEEDS_PROTOCOLS_BLOWFISH

#include "blowfish.h"

#include <types.h>
#include <libc/string.h>
#include <libc/stdio.h>

#include <protocols/blowfish.h>

static unsigned char cbc_key [16]=POK_BLOWFISH_KEY;

/*
static unsigned char cbc_iv [8]={0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
#define MYIV {0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
*/



void pok_protocols_blowfish_marshall (void* uncrypted_data, size_t uncrypted_size, void* crypted_data, size_t* crypted_size)
{
   (void) uncrypted_size;
   BF_KEY key;
/*   unsigned char iv[8] = POK_BLOWFISH_INIT; */
   BF_set_key(&key,16,cbc_key);

/*   memcpy(iv,cbc_iv,8); */
   BF_ecb_encrypt(uncrypted_data,crypted_data,&key,BF_ENCRYPT);

   *crypted_size = 8;
}

void pok_protocols_blowfish_unmarshall (void* crypted_data, size_t crypted_size, void* uncrypted_data, size_t* uncrypted_size)
{
/*   unsigned char iv[8] = POK_BLOWFISH_INIT; */
   (void) crypted_size;

   BF_KEY key;

   BF_set_key(&key,16,cbc_key);

/*   memcpy (iv,cbc_iv,8); */

   BF_ecb_encrypt(crypted_data,uncrypted_data, &key,BF_DECRYPT);

   *uncrypted_size = 8;
}

#endif
