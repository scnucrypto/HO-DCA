#ifndef WBAES_H
#define WBAES_H

#include "aes.h"
#include "random.h"
#include <math.h>

typedef unsigned char u8;
typedef unsigned int u32;

#define N 5
#define SAMNUM 7200
// #define SAMNUM 500

typedef struct shares
{
    u8 a[2], b[N + 1];
}shares;

static const u8 idM8[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

u8 randbit();
shares *randshares();
shares *encode(u8 x);
u8 decode(shares *s);
shares *refresh(shares *x);
shares *evalxor(shares *x, shares *y);
shares *evaland(shares *x, shares *y);
shares *evalnot(shares *x);

void bs_wbsbox(shares *x[8]);
void bs_wbmc(shares *x[32]);
void bs_wbsr(shares *x[128]);

void AddRoundKey(shares *x[128], shares *k[128]);
void ShiftRows(shares *x[128]);
void MixColumns(shares *x[128]);
void SubBytes(shares *x[128]);

void genenkey(u8 key[16], shares *k[11][128]);
void encode_ptx(u8 in[16], shares *x[128]);
void wbaes(shares *x[128], shares *k[11][128], shares *y[128]);
void decode_ctx(shares *x[128], u8 out[16]);

void attack_wbaes(int byteth);
void attack_AddRoundKey(shares *x[128], shares *k[128], int byteth, u8 ptx);
void attack_SubBytes(shares *x[128], int byteth, u8 ptx);
void attack_bs_wbsbox(shares *x[8], u8 ptx);
shares *attack_evalxor(shares *x, shares *y, u8 ptx);
shares *attack_evaland(shares *x, shares *y, u8 ptx);
void DCA(int byteth);
void CDCA(int byteth, int bit);
u8 Sfunction(u8 in);
#endif