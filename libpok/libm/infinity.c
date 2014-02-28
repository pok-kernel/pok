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
 * Created by julien on Fri Jan 30 14:41:34 2009 
 */

/*	$NetBSD: s_infinity.c,v 1.5 2003/07/26 19:25:05 salo Exp $	*/

/*
 * Written by J.T. Conklin <jtc@NetBSD.org>.
 * Public domain.
 */

#ifdef POK_NEEDS_LIBMATH

/*#if BYTE_ORDER == LITTLE_ENDIAN*/
char __infinity[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x7f };
/*#else
char __infinity[] = { 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
#endif*/

#endif
