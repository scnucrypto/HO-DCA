#ifndef GENTABLES_H
#define GENTABLES_H

#include "aes.h"
#include "WBMatrix/WBMatrix.h"
#define SlotNum 3
static const u8 RandKey[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
u8 bs_sbox(u8 in);
void evaluation(u8 input[16], int round, int index, u8 output[SlotNum]);
u8 output_selection(u8 input[SlotNum]);
void randnum_init(u8 input[16]);
void sbox_init(u8 key[16]);
u8 randnum();
void printstate(unsigned char * in);
void shuffling_aes(u8 input[16], u8 key[16], u8 output[16]);

#endif // GENTABLES_H