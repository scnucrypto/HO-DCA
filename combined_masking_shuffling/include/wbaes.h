#ifndef GENTABLES_H
#define GENTABLES_H

#include "aes.h"
#include "WBMatrix/WBMatrix.h"
#define SlotNum 3
#define N 5
typedef struct shares
{
    u8 a[2], b[N + 1];
}shares;

static const u8 RandKey[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
u8 bs_sbox(u8 in);
shares *randshares();
shares *shares_init();
void shares_init1(shares **s);
void evaluation(shares *input[128], shares *key[11][128], int round, int index, shares *output[8 * SlotNum]);
void output_selection(shares *input[8 * SlotNum], shares *output[8]);
void randnum_init(u8 input[16]);
u8 randnum();
void printstate(unsigned char * in);
void masking_shuffling_aes(u8 input[16], u8 key[16], u8 output[16]);

u8 randbit();
shares *encode(u8 x);
u8 decode(shares *s);
shares *refresh(shares *x);
shares *evalxor(shares *x, shares *y);
shares *evaland(shares *x, shares *y);
shares *evalnot(shares *x);

void bs_wbsbox(shares *x[8]);
void bs_wbmc(shares *x[32]);
void bs_wbsr(shares *x[128]);

void ShiftRows(shares *x[128]);
void MixColumns(shares *x[128]);

void genenkey(u8 key[16], shares *k[11][128]);
void encode_ptx(u8 in[16], shares *x[128]);
void wbaes(shares *x[128], shares *k[11][128], shares *y[128]);
void decode_ctx(shares *x[128], u8 out[16]);
#endif // GENTABLES_H