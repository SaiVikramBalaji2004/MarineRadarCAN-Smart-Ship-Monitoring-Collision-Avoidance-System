#ifndef __HEADER_H_
#define __HEADER_H_

#define PORT0 0
#define PORT1 1
#define SET   1
#define CLEAR 0
typedef  unsigned int u32;
typedef  signed int s32;
typedef  unsigned char u8;
typedef  signed char s8;
typedef  unsigned short int u16;
typedef   signed short int s16;

typedef struct CAN2
{
	u32 id;
	u32 rtr;
	u32 dlc;
	u32 byteA;
	u32 byteB;
}CAN2_MSG;

void uart0_init(u32 baud);
void uart0_tx(u8 ch);
u8 uart0_rx(void);
void uart0_string(s8 *s);
void uart0_integer(s32 n);
#endif

