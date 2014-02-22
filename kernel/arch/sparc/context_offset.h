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
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */

/**
 * @file
 * @author Fabien Chouteau
 * @brief  Define registers offset in context stack.
 */

#ifndef __POK_SPARC_CONTEXT_OFFSET_H__
#define __POK_SPARC_CONTEXT_OFFSET_H__

/*
 * registers saved "over" the stack
 * st %xx, [%sp + OFFSET]
*/
# define L0_OFFSET  0x00
# define L1_OFFSET  0x04
# define L2_OFFSET  0x08
# define L3_OFFSET  0x0c
# define L4_OFFSET  0x10
# define L5_OFFSET  0x14
# define L6_OFFSET  0x18
# define L7_OFFSET  0x1c
# define I0_OFFSET  0x20
# define I1_OFFSET  0x24
# define I2_OFFSET  0x28
# define I3_OFFSET  0x2c
# define I4_OFFSET  0x30
# define I5_OFFSET  0x34
# define I6_OFFSET  0x38
# define I7_OFFSET  0x3c

/*
 * others registers are saved "under" the stack
 * st %xx, [%sp - OFFSET]
*/
# define G7_OFFSET    0x04
# define G6_OFFSET    0x08
# define G5_OFFSET    0x0c
# define G4_OFFSET    0x10
# define G3_OFFSET    0x14
# define G2_OFFSET    0x18
# define G1_OFFSET    0x1c

/*
 * next offsets start at -0x40 beacause there's "Inputs" registers from -0x20 to -0x3c
*/
# define WIM_OFFSET         0x40
# define PSR_OFFSET         0x44
# define Y_OFFSET           0x48
# define PC_OFFSET          0x4c
# define NPC_OFFSET         0x50
# define RESTORE_CNT_OFFSET 0x54

#endif /* !__POK_PPC_CONTEXT_OFFSET_H__ */
