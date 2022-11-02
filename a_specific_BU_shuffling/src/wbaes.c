#include "wbaes.h"
u8 mid_input[16];
u8 sf_sbox[10][16][256];

void printstate(unsigned char * in)
{
    int i;
    for(i = 0; i < 16; i++)
    {
        printf("%.2X", in[i]);
    }
    printf("\n");
}

u8 bs_sbox(u8 in)
{
    int r, j;
    u8 x[8], s[8], ou = 0;
    for(r = 0; r < 8; r++)
    {
        if(in & idM8[r]) x[r] = 1;
        else x[r] = 0;
    }
    u8
        y1,y2,y3,y4,y5,y6,y7,y8,
        y9,y10,y11,y12,y13,y14,y15,y16,
        y17,y18,y19,y20,y21;

    u8
        t0,t1,t2,t3,t4,t5,t6,t7,t8,
        t9,t10,t11,t12,t13,t14,t15,
        t16,t17,t18,t19,t20,t21,t22,
        t23,t24,t25,t26,t27,t28,t29,
        t30,t31,t32,t33,t34,t35,t36,
        t37,t38,t39,t40,t41,t42,t43,
        t44,t45,t46,t47,t48,t49,t50,
        t51,t52,t53,t54,t55,t56,t57,
        t58,t59,t60,t61,t62,t63,t64,
        t65,t66,t67;

    u8
        z0,z1,z2,z3,z4,z5,z6,z7,z8,
        z9,z10,z11,z12,z13,z14,
        z15,z16,z17;

    y14 = x[3] ^ x[5];
    y13 = x[0] ^ x[6];
    y9 = x[0] ^ x[3];
    y8 = x[0] ^ x[5];
    t0 = x[1] ^ x[2];
    y1 = t0 ^ x[7];
    y4 = y1 ^ x[3];
    y12 = y13 ^ y14;
    y2 = y1 ^ x[0];
    y5 = y1 ^ x[6];
    y3 = y5 ^ y8;
    t1 = x[4] ^ y12;
    y15 = t1 ^ x[5];
    y20 = t1 ^ x[1];
    y6 = y15 ^ x[7];
    y10 = y15 ^ t0;
    y11 = y20 ^ y9;
    y7 = x[7] ^ y11;
    y17 = y10 ^ y11;
    y19 = y10 ^ y8;
    y16 = t0 ^ y11;
    y21 = y13 ^ y16;
    y18 = x[0] ^ y16;

    t2 = y12 & y15;
    t3 = y3 & y6;
    t4 = t3 ^ t2;
    t5 = y4 & x[7];
    t6 = t5 ^ t2;
    t7 = y13 & y16;
    t8 = y5 & y1;
    t9 = t8 ^ t7;
    t10 = y2 & y7;
    t11 = t10 ^ t7;
    t12 = y9 & y11;
    t13 = y14 & y17;
    t14 = t13 ^ t12;
    t15 = y8 & y10;
    t16 = t15 ^ t12;
    t17 = t4 ^ t14;
    t18 = t6 ^  t16;
    t19 = t9 ^ t14;
    t20 = t11 ^  t16;
    t21 = t17 ^ y20;
    t22 = t18 ^ y19;
    t23 = t19 ^ y21;
    t24 = t20 ^ y18;

    t25 = t21 ^ t22;
    t26 = t21 & t23;
    t27 = t24 ^ t26;
    t28 = t25 & t27;
    t29 = t28 ^ t22;
    t30 = t23 ^ t24;
    t31 = t22 ^ t26;
    t32 = t31 & t30;
    t33 = t32 ^ t24;
    t34 = t23 ^ t33;
    t35 = t27 ^ t33;
    t36 = t24 & t35;
    t37 = t36 ^ t34;
    t38 = t27 ^ t36;
    t39 = t29 & t38;
    t40 = t25 ^ t39;

    t41 = t40 ^ t37;
    t42 = t29 ^ t33;
    t43 = t29 ^ t40;
    t44 = t33 ^ t37;
    t45 = t42 ^ t41;
    z0 = t44 & y15;
    z1 = t37 & y6;
    z2 = t33 & x[7];
    z3 = t43 & y16;
    z4 = t40 & y1;
    z5 = t29 & y7;
    z6 = t42 & y11;
    z7 = t45 & y17;
    z8 = t41 & y10;
    z9 = t44 & y12;
    z10 = t37 & y3;
    z11 = t33 & y4;
    z12 = t43 & y13;
    z13 = t40 & y5;
    z14 = t29 & y2;
    z15 = t42 & y9;
    z16 = t45 & y14;
    z17 = t41 & y8;

    t46 = z15 ^ z16;
    t47 = z10 ^ z11;
    t48 = z5 ^ z13;
    t49 = z9 ^ z10;
    t50 = z2 ^ z12;
    t51 = z2 ^ z5;
    t52 = z7 ^ z8;
    t53 = z0 ^ z3;
    t54 = z6 ^ z7;
    t55 = z16 ^ z17;
    t56 = z12 ^ t48;
    t57 = t50 ^ t53;
    t58 = z4 ^ t46;
    t59 = z3 ^ t54;
    t60 = t46 ^ t57;
    t61 = z14 ^ t57;
    t62 = t52 ^ t58;
    t63 = t49 ^ t58;
    t64 = z4 ^ t59;
    t65 = t61 ^ t62;
    t66 = z1 ^ t63;
    s[0] = (t59 ^ t63) & 0x1;
    s[6] = (~ (t56 ^ t62)) & 0x1;
    s[7] = (~ (t48 ^ t60)) & 0x1;
    t67 = (t64 ^ t65) & 0x1;
    s[3] = (t53 ^ t66) & 0x1;
    s[4] = (t51 ^ t66) & 0x1;
    s[5] = (t47 ^ t65) & 0x1;
    s[1] = (~ (t64 ^ s[3])) & 0x1;
    s[2] = (~ (t55 ^ t67)) & 0x1;

    for(j = 0; j < 8; j++)
    {
        if(s[j]) ou ^= idM8[j];
    }
    
    return ou;
}
void randnum_init(u8 input[16])
{
    for(int i = 0; i < 16; i++)
    {
        mid_input[i] = input[i];
    }
}
u8 randnum()
{
    aes_128_encrypt(mid_input, RandKey, mid_input);
    return mid_input[0] ^ mid_input[1] ^ mid_input[2] ^ mid_input[3] ^ mid_input[4] ^ mid_input[5] ^ mid_input[6] ^ mid_input[7] ^ mid_input[8] ^ mid_input[9] ^ mid_input[10] ^ mid_input[11] ^ mid_input[12] ^ mid_input[13] ^ mid_input[14] ^ mid_input[15];
}
void evaluation(u8 input[16], int round, int index, u8 output[SlotNum])
{
    int main_location;
    u8 masks[SlotNum] = {0};
    for(int i = 1; i < SlotNum; i++)
    {
        masks[i] = randnum();
        masks[0] ^= masks[i];
    }
    // input shuffling
    output[0] = input[index];
    for(int i = 1; i < SlotNum; i++)
    {
        output[i] = randnum();
    }
    main_location = randnum() % SlotNum;
    u8 tmp = output[0];
    output[0] = output[main_location];
    output[main_location] = tmp;
    // evaluation slots
    for(int i = 0; i < SlotNum; i++)
    {
        output[i] = sf_sbox[round][index][output[i]];
        if(i == main_location) output[i] ^= masks[i];
        else output[i] = masks[i];
    }
}
u8 output_selection(u8 input[SlotNum])
{
    u8 output = 0;
    for(int i = 0; i < SlotNum; i++)
    {
        output ^= input[i];
    }
    return output;
}
void sbox_init(u8 key[16])
{
    u8 expandedKey[176];
    expandKey(key, expandedKey);
    for (int i = 0; i < 9; i++) 
    {
        shiftRows(expandedKey + 16 * i);
        for(int j = 0; j < 16; j++)
        {
            for(int m = 0; m < 256; m++)
            {
                sf_sbox[i][j][m] = SBox[m ^ expandedKey[16 * i + j]];
            }
        }
    }

    shiftRows(expandedKey + 16 * 9);
    for(int j = 0; j < 16; j++)
    {
        for(int m = 0; m < 256; m++)
        {
            sf_sbox[9][j][m] = SBox[m ^ expandedKey[16 * 9 + j]] ^ expandedKey[16 * 10 + j];
        }
    }
}
void shuffling_aes(u8 input[16], u8 key[16], u8 output[16])
{
    randnum_init(input);
    sbox_init(key);
    u8 state[16];
    for(int i = 0; i < 16; i++)
    {
        state[i] = input[i];
    }
    for (int r = 0; r < 9; r++) 
    {
        shiftRows(state); 
        for(int j = 0; j < 16; j++)
        {
            u8 tmp[SlotNum];
            evaluation(state, r, j, tmp);
            state[j] = output_selection(tmp);
        }
        mixColumns(state);        
    }
    shiftRows(state); 
    for(int j = 0; j < 16; j++)
    {
        u8 tmp[SlotNum];
        evaluation(state, 9, j, tmp);
        output[j] = output_selection(tmp);
    }
}                
