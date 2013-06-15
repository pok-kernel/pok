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
 * Created by peter on Fri Nov  6 21:20:14 2009 
 */

/**
 * @file
 * @author Fabien Chouteau
 * @brief  Define all constant values for a SPARC bsp.
 */

#ifndef __POK_SPARC_LEON3_CONF_H_
# define __POK_SPARC_LEON3_CONF_H_

# define SPARC_RAM_ADDR 0x40000000 /**< RAM base adress */

# define SPARC_PROC_FREQ 50000000U /**< Processor frequency (in Hz) */

# define WINDOWS_NBR 8 /**< Number of register windows */

# define ASI_MMU_BYPASS 0x1c /* not sparc v8 compliant */

# define SPARC_PAGE_SIZE (256 * 1024) /**< Page size (256 Kbytes) */
# define SPARC_PARTITION_SIZE SPARC_PAGE_SIZE /**< Maximum partition size */

# define SPARC_PARTITION_BASE_VADDR 0x0 /**< Partition virtual base adress. Should always be 0x0 */

#endif /* ! __POK_SPARC_LEON3_CONF_H_ */
