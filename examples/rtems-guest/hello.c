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
#include "virtLayerCPU.h"

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
  return -1;
}

/**
 * \brief Writes a character to the console.
 */
void
virt_charWrite(char *c)
{
  printf("%s", c);
}

/* Date:      06/19/2013
 * Author:    Philipp Eppelt
 * Purpose:   CPU part of the virtualization layer.
 * Licencse:  see RTEMS License.
 */


/* Startup functions */

int 
virt_getWorkSpaceArea( void )
{
  return 0;
}



/* Interrupts */
int
virt_requestIrq( int vector )
{
  return 0;
}

void
virt_detachIrq( int vector )
{
}

void
virt_enableInterrupts( int _level )
{
}

void
virt_disableInterrupts( int _level )
{
}

void 
virt_flashInterrupts( int _level )
{
}

void
virt_openInterrupts( void )
{
}

void
virt_closeInterrupts( void )
{
}

int
virt_getInterruptLevel( int _level )
{
  return 0;
}



/* Idle Thread */

/**
 * @brief Lowest priority thread, doing nothing, never returns;
 */

void
virt_idleThread( void )
{
  while(1);
}


/* Error handling */

void
virt_execStopError( int _error )
{
  printf( "!!! An ERROR occured: %i\n", _error );
}
