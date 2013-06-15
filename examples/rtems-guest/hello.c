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
 *                                      Copyright (c) 2007-2013 POK team 
 *
 * Created by phipse on Wed May 15 13:12:32 2013 
 */

#include <libc/stdio.h>

#include "virtLayerBSP.h"
//#include "virtLayerCPU.h"

int user_hello( void )
{
  virt_charWrite( "Hello POK world!\n");

  return 0;
}


/* Date:      06/10/2013
 * Author:    Philipp Eppelt
 * Purpose:   Implementation of the RTEMS virtualization layer.
 * Licensce:  see POK License.
 */


/**
 * \brief Initializes the console.
 *
 */
int
virt_consoleInit(void)
{
  return 0;
}

/**
 * \brief Reads a character from the console.
 */
char
virt_charRead(void)
{
  /* Console read is currently not supported by POK. -- phi 06/10/2013 */
  return '\0';
}

/**
 * \brief Writes a character to the console.
 */
void
virt_charWrite(char *c)
{
  printf("%s", c);
}
