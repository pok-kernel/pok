/* Author:    Philipp Eppelt - philipp.eppelt@mailbox.tu-dresden.de
 *
 * COPYRIGHT (c) 2013 Philipp Eppelt.
 *
 * Purpose:   CPU part of the virtualization layer.
 * Licencse:  see RTEMS License.
 */


#ifndef RTEMS_VIRT_LAYER_CPU_H
#define RTEMS_VIRT_LAYER_CPU_H

#ifndef ASM

/* Interrupts */

/**
 * \brief Requests an interrupt vector.
 */
__attribute__((fastcall)) int
_CPU_Virtual_Irq_request( int vector );

/**
 * \brief Releases an interrupt vector.
 */
__attribute__((fastcall)) void
_CPU_Virtual_Irq_detach( int vector );

/**
 * \brief Enables interrupt delivery or sets it to the specified level.
 * 
 * Use this function in collaboration with _CPU_Virtual_Interrupts_disable and
 * use its returned _level as argument to this function to assure proper
 * behaviour.
 */
__attribute__((fastcall)) void
_CPU_Virtual_Interrupts_enable( int _level );

/**
 * \brief Disables interrupt delivery and returns the previous level.
 *
 */
__attribute__((fastcall)) void
_CPU_Virtual_Interrupts_disable( int _level );

/**
 * \brief Enables and directly disables interrupt delivery.
 */
__attribute__((fastcall)) void 
_CPU_Virtual_Interrupts_flash( int _level );

/**
 * \brief Requests the current interrupt level.
 */
__attribute__((fastcall)) int
_CPU_Virtual_Interrupts_get_level( int _level );



/* Idle Thread */

/**
 * \brief Lowest priority thread, doing nothing, never returns;
 */

__attribute__((fastcall)) void
_CPU_Virtual_idle_thread( void );


/* Error handling */

/**
 * \brief Handlers execution errors.
 */
__attribute__((fastcall)) void
_CPU_Virtual_exec_stop_error( int _error );

#endif /* ASM */

#endif /* RTEMS_VIRT_LAYER_CPU_H */
