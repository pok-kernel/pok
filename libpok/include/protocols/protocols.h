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
 * Created by julien on Thu Dec  3 10:21:25 2009 
 */

#ifndef __LIBPOK_PROTOCOLS_H__
#define __LIBPOK_PROTOCOLS_H__

/**
 * \file    libpok/protocols/protocols.h
 * \author  Julien Delange
 * \date    2009
 * \brief   Protocols to marshall/unmarshall data
 *
 * This file is a general-purpose file to include all
 * protocols in the same time. Protocols functions
 * provides features to encode and decode messages
 * before sending data through partitions. It is
 * especially useful when you want to encrypt data
 * over the network before sending or adapt application 
 * data to a particular protocol.
 *
 * For each protocol, we have:
 *   - One function to marshall data
 *   - One function to unmarshall data
 *   - One data type associated with the crypto protocol.
 *     This data type is used to store data when marshalling
 *     data and used as an input to unmarshall data.
 *
 * More documentation is available in the user manual.
 */

/*
 * The DES crypto protocol
 */
#include <protocols/des.h>

/*
 * The Blowfish crypto protocol
 */
#include <protocols/blowfish.h>

/*
 * The Ceasar crypto protocol
 */
#include <protocols/ceasar.h>

#endif
