/* Author:    Philipp Eppelt - philipp.eppelt@mailbox.tu-dresden.de
 *
 * COPYRIGHT (c) 2013 Philipp Eppelt.
 *
 * Purpose:   BSP part of the virtualization layer.
 * Licencse:  see RTEMS License.
 */

#ifndef RTEMS_VIRT_LAYER_BSP_H
#define RTEMS_VIRT_LAYER_BSP_H

/**
 * \brief Initializes the console.
 *
 */
__attribute__((fastcall)) int
_BSP_Virtual_Console_init(void);

/**
 * \brief Reads a character from the console.
 */
__attribute__((fastcall)) char
_BSP_Virtual_Char_read(void);

/**
 * \brief Writes a character to the console.
 */
__attribute__((fastcall)) void
_BSP_Virtual_Char_write(char* c);

/**
 * \brief Initializes the clock.
 */
__attribute__((fastcall)) int
_BSP_Virtual_Clock_init(void);

/**
 * \brief Reads the clock.
 */
__attribute__((fastcall)) int
_BSP_Virtual_Clock_read(void);

/**
 * \brief Handles faults delivered by the host system.
 *
 * POK will forward occuring faults to this handler.
 */
__attribute__((fastcall)) int
_BSP_Virtual_faulthandler(void);

/* Startup functions 
 */

__attribute__((fastcall)) int 
_BSP_Virtual_getworkspacearea( void );

#endif /* RTEMS_VIRT_LAYER_BSP_H */
