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
 */

#ifndef __POK_SPARC_LEON3_TIMER_H__
# define __POK_SPARC_LEON3_TIMER_H__

# define TIMER_CTRL_EN (1 << 0) /**< Enable  */
# define TIMER_CTRL_RS (1 << 1) /**< Restart */
# define TIMER_CTRL_LD (1 << 2) /**< Load */
# define TIMER_CTRL_IE (1 << 3) /**< Interrupt enable */
# define TIMER_CTRL_IP (1 << 4) /**< Interrupt Pending */
# define TIMER_CTRL_CH (1 << 5) /**< Chain */
# define TIMER_CTRL_DH (1 << 6) /**< Debug Halt */

# define TIMER_SCALER_OFFSET      0x00 /**< Scaler value register offset */
# define TIMER_SCAL_RELOAD_OFFSET 0x04 /**< Scaler reload register offset */

# define TIMER_CNT_VAL_OFFSET     0x10 /**< Counter value register offset */
# define TIMER_RELOAD_OFFSET      0x14 /**< Counter reload register offset */
# define TIMER_CTRL_OFFSET        0x18 /**< Control register offset */

# define TIMER_IRQ 0x8U

# define TIMER1 0x80000300 /**< first Leon3 TIMER IO adress */

#endif /* __POK_SPARC_LEON3_TIMER_H__ */
