/*
 * Types.h
 *
 *  Created on: 2020年4月29日
 *      Author: LiZhiqiang
 */

#ifndef THIRD_PARTY_W5500_INC_TYPES_H_
#define THIRD_PARTY_W5500_INC_TYPES_H_

#define	MAX_SOCK_NUM		8	/**< Maxmium number of socket  */

typedef char int8;

typedef volatile char vint8;

typedef unsigned char uint8;

typedef volatile unsigned char vuint8;

typedef int int16;

typedef unsigned short uint16;

typedef long int32;

typedef unsigned long uint32;

typedef uint8			u_char;		/**< 8-bit value */
typedef uint8 			SOCKET;
typedef uint16			u_short;	/**< 16-bit value */
typedef uint32			u_long;		/**< 32-bit value */

typedef union _un_l2cval
{
	u_long	lVal;
	u_char	cVal[4];
}un_l2cval;

typedef union _un_i2cval
{
	u_short	iVal;
	u_char	cVal[2];
}un_i2cval;

#endif /* THIRD_PARTY_W5500_INC_TYPES_H_ */
