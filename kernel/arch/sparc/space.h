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

#ifndef __POK_SPARC_SPACE_H__
#define __POK_SPARC_SPACE_H__

#include <types.h>

/**
 * @{
 * @name PTD/PTE ET field
 * (cf SPARC V8 Manual, page 247)
*/
#define MM_ET_INVALID 0x0 /**< Invalid */
#define MM_ET_PTD     0x1 /**< Page Table Descriptor */
#define MM_ET_PTE     0x2 /**< Page Table Entry */
/** @} */

/**
 * @{
 * @name PTE ACC field
 * Acces permisions. (cf SPARC V8 Manual, page 248)
*/
#define MM_ACC_R      (0x0 << 2) /**< All Read only */
#define MM_ACC_RW     (0x1 << 2) /**< All Read Write */
#define MM_ACC_RE     (0x2 << 2) /**< All Read Execute */
#define MM_ACC_RWE    (0x3 << 2) /**< All Read Write Execute */
#define MM_ACC_E      (0x4 << 2) /**< All Execute only */
#define MM_ACC_R_S_RW (0x5 << 2) /**< User Read only, Supervisor Read Write */

#define MM_ACC_S_RE   (0x6 << 2) /**< Supervisor Read Write Execute */
#define MM_ACC_S_RWE  (0x7 << 2) /**< Supervisor Read Execute */
/** @} */

/**
 * @{
 * @name PTE misc fields
 * (cf SPARC V8 Manual, page 248)
*/
#define MM_CACHEABLE  (1 << 7)
#define MM_MODIFIED   (1 << 6)
#define MM_REFERENCED (1 << 5)
/** @} */

/**
 * @{
 * @name MMU levels utils
*/
#define MM_LVL1_ENTRIES_NBR 256  /**< Number of entries in 1st level table */
#define MM_LVL1_PAGE_SIZE (64 * 64 * 4 * 1024) /**< 16 MegaBytes */
/**
 * @def mm_index1(addr)
 * Compute the index in 1st level table for the given adress.
 */
#define mm_index1(addr) (((addr) >> 24) & 0xFF)

#define MM_LVL2_ENTRIES_NBR 64  /**< Number of entries in 2nd level table */
#define MM_LVL2_PAGE_SIZE (64 * 4 * 1024) /**< 256 KiloBytes */
/**
 * @def mm_index2(addr)
 * Compute the index in 2nd level table for the given adress.
 */
#define mm_index2(addr) (((addr) >> 18) & 0x3F)

#define MM_LVL3_ENTRIES_NBR 64 /**< Number of entries in 3rd level table */
#define MM_LVL3_PAGE_SIZE (4 * 1024) /**< 4 KiloBytes */
/**
 * @def mm_index3(addr)
 * Compute the index in 3rd level table for the given adress.
 */
#define mm_index3(addr) (((addr) >> 12) & 0x3F)
/** @} */


/**
 * @{
 * @name MMU ASI and registers
*/
#define ASI_M_MMUREGS    0x19 /* not sparc v8 compliant */
#define MMU_CTRL_REG     0x00000000
#define MMU_CTXTBL_PTR   0x00000100
#define MMU_CTX_REG      0x00000200
#define MMU_FAULT_STATUS 0x00000300
#define MMU_FAULT_ADDR   0x00000400
/** @} */

#define LEON_CTX_NBR 256 /**< Maximum number of contexts */

typedef uint32_t pte;
typedef uint32_t ptd;

void pok_arch_space_init (void);

#endif /* !__POK_PPC_SPACE_H__ */

