/*******************************************************************************
* COPYRIGHT BH
********************************************************************************
* FileName: os_basictypes.h
* Function: Basic data type.
* Author:   Andy
* Date:     2011/09/01
* History:
*
* Modify Date   Owner   BugID/CRID      Contents
* ------------------------------------------------------------------------------
* 2011/09/01    Andy                     Draft
*******************************************************************************/
#ifndef OS_BASICTYPES_H
#define OS_BASICTYPES_H

typedef  signed char                s8;
typedef  signed short               s16;
typedef  signed int                 s32;
typedef  unsigned char              u8;
typedef  unsigned short             u16;
typedef  unsigned int               u32;

typedef  volatile signed char       vs8;
typedef  volatile signed short      vs16;
typedef  volatile signed int        vs32;
typedef  volatile unsigned char     vu8;
typedef  volatile unsigned short    vu16;
typedef  volatile unsigned int      vu32;

typedef  vu8                        VBOOL;

#define  NULLPTR                    ((void *)0)
/********************* End of Protection For Header File **********************/
#endif
/*********************************End Of File *********************************/

