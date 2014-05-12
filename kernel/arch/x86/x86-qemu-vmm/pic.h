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


#ifndef __POK_X86_PIC_H__
#define __POK_X86_PIC_H__

#define PIC_MASTER_BASE 0x20
#define PIC_SLAVE_BASE 0xa0

#define PIC_MASTER_ICW1 0x11
#define PIC_MASTER_ICW2 0x20
#define PIC_MASTER_ICW3 0x04
#define PIC_MASTER_ICW4 0x01

#define PIC_SLAVE_ICW1 0x11
#define PIC_SLAVE_ICW2 0x28
#define PIC_SLAVE_ICW3 0x02
#define PIC_SLAVE_ICW4 0x01

int		pok_pic_init();
int		pok_pic_mask(uint8_t irq);
int		pok_pic_unmask(uint8_t irq);
void		pok_pic_eoi(uint8_t irq);

#endif /* __POK_X86_PIC_H_ */
