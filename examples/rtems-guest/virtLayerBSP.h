/* Date:      06/10/2013
 * Author:    Philipp Eppelt
 * Purpose:   BSP part of the virtualization layer.
 * Licencse:  see RTEMS License.
 */

#ifndef RTEMS_VIRT_LAYER_BSP_H
#define RTEMS_VIRT_LAYER_BSP_H

/**
 * \brief Initializes the console.
 *
 */
int
virt_consoleInit(void);

/**
 * \brief Reads a character from the console.
 *
 * Currently not supported by POK. -- phi 06/14/2013
 */
char
virt_charRead(void);

/**
 * \brief Writes a character to the console.
 */
void
virt_charWrite(char* c);

/**
 * \brief Initializes the clock.
 */
int
virt_clockInit(void);

/**
 * \brief Reads the clock.
 */
int
virt_clockRead(void);

/**
 * \brief Handles faults delivered by the host system.
 *
 * POK will forward occuring faults to this handler.
 */
int
virt_faultHandler(void);



#endif /* RTEMS_VIRT_LAYER_BSP_H */
