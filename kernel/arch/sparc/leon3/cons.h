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

#ifndef __POK_SPARC_LEON3_CONS_H__
#define __POK_SPARC_LEON3_CONS_H__

#define UART_STATUS_DR   0x00000001 /**< Data Ready */
#define UART_STATUS_TSE  0x00000002 /**< TX Send Register Empty */
#define UART_STATUS_THE  0x00000004 /**< TX Hold Register Empty */
#define UART_STATUS_BR   0x00000008 /**< Break Error */
#define UART_STATUS_OE   0x00000010 /**< RX Overrun Error */
#define UART_STATUS_PE   0x00000020 /**< RX Parity Error */
#define UART_STATUS_FE   0x00000040 /**< RX Framing Error */
#define UART_STATUS_ERR  0x00000078 /**< Error Mask */

#define UART_CTRL_RE     0x00000001 /**< Receiver enable */
#define UART_CTRL_TE     0x00000002 /**< Transmitter enable */
#define UART_CTRL_RI     0x00000004 /**< Receiver interrupt enable */
#define UART_CTRL_TI     0x00000008 /**< Transmitter interrupt enable */
#define UART_CTRL_PS     0x00000010 /**< Parity select */
#define UART_CTRL_PE     0x00000020 /**< Parity enable */
#define UART_CTRL_FL     0x00000040 /**< Flow control enable */
#define UART_CTRL_LB     0x00000080 /**< Loop Back enable */

#define UART_DATA_OFFSET   0x0 /**< Data register offset */
#define UART_STAT_OFFSET   0x4 /**< Stat register offset */
#define UART_CTRL_OFFSET   0x8 /**< Control register offset */
#define UART_SCALER_OFFSET 0xc /**< Scaler register offset */

#define  UART1 0x80000100 /**< First Leon3 UART IO adress */

int         pok_cons_init(void);

#endif /* ! __POK_SPARC_LEON3_CONS_H__ */
