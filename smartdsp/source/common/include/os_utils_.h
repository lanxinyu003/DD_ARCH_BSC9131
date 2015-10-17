/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2011/11/08 09:53:40 $
 $Id: os_utils_.h,v 1.3 2011/11/08 09:53:40 b17975 Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_utils_.h,v $
 $Revision: 1.3 $
******************************************************************************/

/******************************************************************************
 
 @File          os_utils_.h

 @Description   OS Utilities header file.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_UTILS_H_
#define __OS_UTILS_H_

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define __MAX(A,B)              (((A)>(B)) ? (A) : (B))
#define __MIN(A,B)              (((A)<(B)) ? (A) : (B))
#define __ARRAY_SIZE(ARRAY)     (sizeof(ARRAY) / sizeof(ARRAY[0]))
#define __INT_CEIL(NOM,DEN)     (((((NOM)/(DEN)) * (DEN)) == (NOM)) ? \
                                    ((NOM)/(DEN)) : (1 + ((NOM)/(DEN))))

#define __INT_FLOOR(NOM,DEN)    ((NOM)/(DEN))

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* The following util package handles an Array Of Entries (EOA), each entry hanlding
 * resources indicated by a few bits. For all macros:
 * NUM_ENTRIES      - number of resources to manage
 * BIT_PER_ENTRY    - how many bits we need per resource
 * VECTOR_SIZE      - the size in bits of each cell in array.
 *
 * Note: all macros are dependant on that integer calculations always round down
 *       in case of a division with a remainder
 * Note: BIT_PER_ENTRY may not be higher than VECTOR_SIZE
 *
 * __EOA_NUM_ENTRIES()  claculates the size of the array by perforimg:
 *                          ceil((NUM_ENTRIES * BIT_PER_ENTRY) / VECTOR_SIZE)
 * __EOA_ARRAY_INDEX()  calculates the index in the array that manages a given entry by performing
 *                          floor((ENTRY * BIT_PER_ENTRY) / VECTOR_SIZE)
 * __EOA_ENTRY_OFFSET() calculates the offset of the bits managing an entry in __ARRAY_ENTRY() by performing
 *                          BIT_PER_ENTRY * (ENTRY - (__EOA_ARRAY_INDEX() * floor(VECTOR_SIZE / BIT_PER_ENTRY)))
 *
 * For example if we have to calculate the size of an array for mnaging 16
 * entries; each entry requiring 3 bits to manage:
 * __ENTRIES_REQIRED(16, 3, 32) should return 2 with the following distribution:
 * In order to find the array index managing entry #14:
 * __ARRAY_ENTRY(14, 32, 3) will return 1
 * In order to find the offset of bits managing entry #14:
 * __ENTRY_OFFSET(14, 32, 3) will return 12
 *
 *               30       27       24       21       18       15       12        9        6        3        0
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * Entry #0 |XXXXX|    9   |    8   |    7   |    6   |    5   |    4   |    3   |    2   |    1   |    0   |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * Entry #1 |XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|   15   |   14   |   13   |   12   |   11   |   10   |
 *          +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 *
 */

/* claculates the size of the array of entries */
#define __EOA_NUM_ENTRIES(NUM_ENTRIES, BIT_PER_ENTRY, VECTOR_SIZE) \
            __INT_CEIL((NUM_ENTRIES * BIT_PER_ENTRY), VECTOR_SIZE)

/* calculates the index in an array for a given entry */
#define __EOA_ARRAY_INDEX(ENTRY, VECTOR_SIZE, BIT_PER_ENTRY) \
            __INT_FLOOR((ENTRY * BIT_PER_ENTRY), VECTOR_SIZE)

/* calculates the offset in an index of an array for a given entry */
#define __EOA_ENTRY_OFFSET(ENTRY, VECTOR_SIZE, BIT_PER_ENTRY) \
    BIT_PER_ENTRY * (ENTRY - (__ARRAY_ENTRY(ENTRY, VECTOR_SIZE, BIT_PER_ENTRY) * (VECTOR_SIZE / BIT_PER_ENTRY)))



#endif // __OS_UTILS_H_



