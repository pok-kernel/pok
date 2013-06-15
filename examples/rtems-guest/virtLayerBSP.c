/* Date:      06/10/2013
 * Author:    Philipp Eppelt
 * Purpose:   Implementation of the RTEMS virtualization layer.
 * Licensce:  see POK License.
 */

#include "virtLayerBSP.h"


/**
 * \brief Initializes the console.
 *
 */
int
virt_consoleInit(void)
{
}

/**
 * \brief Reads a character from the console.
 */
char
virt_charRead(void)
{
  char read;
  sscanf("%c", &read);
  return read;
}

/**
 * \brief Writes a character to the console.
 */
void
virt_charWrite(char *c)
{
  printf("%c", c);
}
