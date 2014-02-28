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

#ifndef __LIBPOK_PROTOCOLS_SSL_H__
#define __LIBPOK_PROTOCOLS_SSL_H__

#include <types.h>

/**
 * \file    libpok/protocols/ssl.h
 * \author  Julien Delange
 * \date    2009
 * \brief   SSL crypto protocol.
 *
 * More information at:
 * http://en.wikipedia.org/wiki/Transport_Layer_Security
 */

#ifdef POK_NEEDS_PROTOCOLS
void pok_protocols_ssl_unmarshall (void* crypted_data, pok_size_t crypted_size, void* uncrypted_data, size_t* uncrypted_size);

void pok_protocols_ssl_marshall (void* uncrypted_data, pok_size_t uncrypted_size, void* crypted_data, size_t* crypted_size);

#define pok_protocols_ssl_data_t int

#endif

#endif
