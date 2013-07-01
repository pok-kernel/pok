/* Date:      06/19/2013
 * Author:    Philipp Eppelt
 * Purpose:   CPU part of the virtualization layer.
 * Licencse:  see RTEMS License.
 */


#ifndef RTEMS_VIRT_LAYER_CPU_H
#define RTEMS_VIRT_LAYER_CPU_H

#ifndef ASM

/* Startup functions */

int 
virt_getWorkSpaceArea( void );



/* Interrupts */
int
virt_requestIrq( int vector );

void
virt_detachIrq( int vector );

void
virt_enableInterrupts( int _level );

void
virt_disableInterrupts( int _level );

void 
virt_flashInterrupts( int _level );

void
virt_openInterrupts( void );

void
virt_closeInterrupts( void );

int
virt_getInterruptLevel( int _level );



/* Idle Thread */

/**
 * @brief Lowest priority thread, doing nothing, never returns;
 */

void
virt_idleThread( void );


/* Error handling */

void
virt_execStopError( int _error );

#endif /* ASM */

#endif /* RTEMS_VIRT_LAYER_CPU_H */
