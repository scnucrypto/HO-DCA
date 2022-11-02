#include "wbaes.h"
u8 mid_input[16];

void printstate(unsigned char * in)
{
    int i;
    for(i = 0; i < 16; i++)
    {
        printf("%.2X", in[i]);
    }
    printf("\n");
}
shares *randshares()
{
    int i;
    shares *s = (shares *)malloc(sizeof(shares));
    s->a[0] = randbit();
    s->a[1] = randbit();
    for(i = 1; i <= N; i++)
    {
        s->b[i] = randbit();
    }
    return s;
}
void shares_init1(shares **s)
{
    int i;
    s = (shares *)malloc(sizeof(shares));
    (*s)->a[0] = 0;
    (*s)->a[1] = 0;
    for(i = 1; i <= N; i++)
    {
        (*s)->b[i] = 0;
    }
}
shares *shares_init()
{
    int i;
    shares *s = (shares *)malloc(sizeof(shares));
    s->a[0] = 0;
    s->a[1] = 0;
    for(i = 1; i <= N; i++)
    {
        s->b[i] = 0;
    }
    return s;
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
u8 randbit()
{
    aes_128_encrypt(mid_input, RandKey, mid_input);
    return (mid_input[0] ^ mid_input[1] ^ mid_input[2] ^ mid_input[3] ^ mid_input[4] ^ mid_input[5] ^ mid_input[6] ^ mid_input[7] ^ mid_input[8] ^ mid_input[9] ^ mid_input[10] ^ mid_input[11] ^ mid_input[12] ^ mid_input[13] ^ mid_input[14] ^ mid_input[15]) & 0x1;
}
void evaluation(shares *input[128], shares *key[11][128], int round, int index, shares *output[8 * SlotNum])
{
    int main_location;
    shares *masks[8 * SlotNum];
    for(int i = 0; i < 8; i++)
    {
        masks[i] = shares_init();
        // shares_init1(&masks[i]);
    }
    for(int j = 0; j < 8; j++)
    {
        for(int i = 1; i < SlotNum; i++)
        {
            masks[i * 8 + j] = randshares();
            masks[j]->a[0] ^= masks[i * 8 + j]->a[0];
            masks[j]->a[1] ^= masks[i * 8 + j]->a[1];
            for(int m = 1; m <= N; m++)
            {
                masks[j]->b[m] ^= masks[i * 8 + j]->b[m];
            }
        }
    }
    // input shuffling
    for(int j = 0; j < 8; j++)
    {
        output[j] = input[index * 8 + j];
    }
    for(int i = 1; i < SlotNum; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            output[i * 8 + j] = randshares();
        }
    }
    main_location = randnum() % SlotNum;
    for(int i = 0; i < 8; i++)
    {
        shares *tmp = output[i];
        output[i] = output[(main_location * 8) + i];
        output[(main_location * 8) + i] = tmp;
    }
    // evaluation slots
    for(int i = 0; i < SlotNum; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            output[i * 8 + j] = evalxor(output[i * 8 + j], key[round][index * 8 + j]);
        }
        bs_wbsbox(output + i * 8);
        if(round == 9)
        {
            for(int j = 0; j < 8; j++)
            {
                output[i * 8 + j] = evalxor(output[i * 8 + j], key[round + 1][index * 8 + j]);
            }
        }
        if(i == main_location) 
        {
            for(int j = 0; j < 8; j++)
            {
                output[i * 8 + j]->a[0] ^= masks[i * 8 + j]->a[0];
                output[i * 8 + j]->a[1] ^= masks[i * 8 + j]->a[1];
                for(int m = 1; m <= N; m++)
                {
                    output[i * 8 + j]->b[m] ^= masks[i * 8 + j]->b[m];
                }
            }
        }
        else 
        {
            for(int j = 0; j < 8; j++)
            {
                output[i * 8 + j] = masks[i * 8 + j];
            }
        }
    }
}
void output_selection(shares *input[8 * SlotNum], shares *output[8])
{
    for(int i = 0; i < 8; i++)
    {
        output[i] = shares_init();
        // shares_init1(&output[i]);
        for(int j = 0; j < SlotNum; j++)
        {
            output[i]->a[0] ^= input[j * 8 + i]->a[0];
            output[i]->a[1] ^= input[j * 8 + i]->a[1];
            for(int m = 1; m <= N; m++)
            {
                output[i]->b[m] ^= input[j * 8 + i]->b[m];
            }
        }
    }
}
void masking_shuffling_aes(u8 input[16], u8 key[16], u8 output[16])
{
    randnum_init(input);
    shares *ptx[128];
    shares *k[11][128];
    shares *ctx[128];

    /// encode plaintexts
    encode_ptx(input, ptx);
    genenkey(key, k);
    /// encryption
    wbaes(ptx, k, ctx);
    ///// decode ciphertexts
    decode_ctx(ctx, output);
}                

////////// masking scheme
shares *encode(u8 x)
{
    int i;
    u8 ra[2], rb[N];
    shares *s = (shares *)malloc(sizeof(shares));
    for(i = 0; i <= 1; i++)
    {
        ra[i] = randbit();
        s->a[i] = ra[i];
    }
    s->b[N] = (ra[0] & ra[1]) ^ x;
    for(i = 1; i < N; i++)
    {
        rb[i] = randbit();
        s->b[i] = rb[i];
        s->b[N] ^= rb[i];
    }
    return s;
}

u8 decode(shares *s)
{
    int i;
    u8 temp = s->a[0] & s->a[1];
    for(i = 1; i <= N; i++)
    {
        temp ^= s->b[i];
    }
    return temp;
}

shares *refresh(shares *x)
{
    int i, j;
    shares *s = (shares *)malloc(sizeof(shares));
    u8 r[2], r0, temp, W, R;
    for(i = 0; i <= 1; i++)
    {
        r[i] = randbit();
        s->a[i] = x->a[i] ^ r[i];
    }
    for(i = 1; i <= N; i++)
    {
        s->b[i] = x->b[i];
    }
    for(i = 1; i <= N; i++)
    {    
        for(j = i + 1; j <= N; j++)
        {
            temp = randbit();
            s->b[i] ^= temp;
            s->b[j] ^= temp;
        }
    }
    r0 = randbit();
    W = (r[0] & (x->a[1] ^ r0)) ^ (r[1] & (x->a[0] ^ r0));
    R = ((r[0] ^ r0) & (r[1] ^ r0)) ^ r0;
    s->b[N] ^= W ^ R;
    return s;
}

shares *evalxor(shares *x, shares *y)
{
    int i;
    u8 U;
    shares *s = (shares *)malloc(sizeof(shares));
    x = refresh(x);
    y = refresh(y);
    for(i = 0; i <= 1; i++)
    {
        s->a[i] = x->a[i] ^ y->a[i];
    }
    for(i = 1; i < N; i++)
    {
        s->b[i] = x->b[i] ^ y->b[i];
    }
    U = (x->a[0] & y->a[1]) ^ (x->a[1] & y->a[0]);
    s->b[N] = x->b[N] ^ y->b[N] ^ U;
    return s;
}
shares *evaland(shares *x, shares *y)
{
    int i, j, t;
    u8 ra[2][N + 1], rb[N + 1][N + 1], u = 0;
    shares *s = (shares *)malloc(sizeof(shares));
    x = refresh(x);
    y = refresh(y);
    for(i = 0; i <= 1; i++)
    {
        s->a[i] = x->a[i] & y->a[(i + 1) % 2];
        for(j = 1; j <= N; j++)
        {
            ra[i][j] = randbit();
            s->a[i] ^= ra[i][j];
        }
    }
    for(t = 1; t <= N; t++)
    {
        u ^= ra[0][t];
    }
    
    for(i = 0; i <= N; i++)
    {
        for(j = i + 1; j <= N; j++)
        {
            if(i == 0) 
            {
                
                rb[j][0] = (x->a[1] & ((x->a[0] & y->b[j]) ^ (ra[0][j] & y->a[0]))) ^ (y->a[1] & ((y->a[0] & x->b[j]) ^ (ra[1][j] & x->a[0]))) ^ (ra[1][j] & u);
            }
            else
            {
                rb[i][j] = randbit();
                rb[j][i] = rb[i][j] ^ (x->b[i] & y->b[j]) ^ (x->b[j] & y->b[i]);
            }
        }
    }
    for(i = 1; i <= N; i++)
    {
        s->b[i] = x->b[i] & y->b[i];
        for(j = 0; j <= N; j++)
        {
            if(j != i) s->b[i] ^= rb[i][j];
        }
    }
    return s;
}
shares *evalnot(shares *x)
{
    int i;
    shares *s = (shares *)malloc(sizeof(shares));
    s->a[0] = x->a[0];
    s->a[1] = x->a[1];
    for(i = 1; i < N; i++)
    {
        s->b[i] = x->b[i];
    }
    s->b[N] = x->b[N] ^ 0x1;
    return s;
}
/////////// masking sbox
void bs_wbsbox(shares *x[8])
{
    shares
        *y1,*y2,*y3,*y4,*y5,*y6,*y7,*y8,
        *y9,*y10,*y11,*y12,*y13,*y14,*y15,*y16,
        *y17,*y18,*y19,*y20,*y21;

    shares
        *t0,*t1,*t2,*t3,*t4,*t5,*t6,*t7,*t8,
        *t9,*t10,*t11,*t12,*t13,*t14,*t15,
        *t16,*t17,*t18,*t19,*t20,*t21,*t22,
        *t23,*t24,*t25,*t26,*t27,*t28,*t29,
        *t30,*t31,*t32,*t33,*t34,*t35,*t36,
        *t37,*t38,*t39,*t40,*t41,*t42,*t43,
        *t44,*t45,*t46,*t47,*t48,*t49,*t50,
        *t51,*t52,*t53,*t54,*t55,*t56,*t57,
        *t58,*t59,*t60,*t61,*t62,*t63,*t64,
        *t65,*t66,*t67;

    shares
        *z0,*z1,*z2,*z3,*z4,*z5,*z6,*z7,*z8,
        *z9,*z10,*z11,*z12,*z13,*z14,
        *z15,*z16,*z17;

    y14 = evalxor(x[3], x[5]);
    y13 = evalxor(x[0], x[6]);
    y9 = evalxor(x[0], x[3]);
    y8 = evalxor(x[0], x[5]);
    t0 = evalxor(x[1], x[2]);
    y1 = evalxor(t0, x[7]);
    y4 = evalxor(y1, x[3]);
    y12 = evalxor(y13, y14);
    y2 = evalxor(y1, x[0]);
    y5 = evalxor(y1, x[6]);
    y3 = evalxor(y5, y8);
    t1 = evalxor(x[4], y12);
    y15 = evalxor(t1, x[5]);
    y20 = evalxor(t1, x[1]);
    y6 = evalxor(y15, x[7]);
    y10 = evalxor(y15, t0);
    y11 = evalxor(y20, y9);
    y7 = evalxor(x[7], y11);
    y17 = evalxor(y10, y11);
    y19 = evalxor(y10, y8);
    y16 = evalxor(t0, y11);
    y21 = evalxor(y13, y16);
    y18 = evalxor(x[0], y16);

    t2 = evaland(y12, y15);
    t3 = evaland(y3, y6);
    t4 = evalxor(t3, t2);
    t5 = evaland(y4, x[7]);
    t6 = evalxor(t5, t2);
    t7 = evaland(y13, y16);
    t8 = evaland(y5, y1);
    t9 = evalxor(t8, t7);
    t10 = evaland(y2, y7);
    t11 = evalxor(t10, t7);
    t12 = evaland(y9, y11);
    t13 = evaland(y14, y17);
    t14 = evalxor(t13, t12);
    t15 = evaland(y8, y10);
    t16 = evalxor(t15, t12);
    t17 = evalxor(t4, t14);
    t18 = evalxor(t6,  t16);
    t19 = evalxor(t9, t14);
    t20 = evalxor(t11,  t16);
    t21 = evalxor(t17, y20);
    t22 = evalxor(t18, y19);
    t23 = evalxor(t19, y21);
    t24 = evalxor(t20, y18);

    t25 = evalxor(t21, t22);
    t26 = evaland(t21, t23);
    t27 = evalxor(t24, t26);
    t28 = evaland(t25, t27);
    t29 = evalxor(t28, t22);
    t30 = evalxor(t23, t24);
    t31 = evalxor(t22, t26);
    t32 = evaland(t31, t30);
    t33 = evalxor(t32, t24);
    t34 = evalxor(t23, t33);
    t35 = evalxor(t27, t33);
    t36 = evaland(t24, t35);
    t37 = evalxor(t36, t34);
    t38 = evalxor(t27, t36);
    t39 = evaland(t29, t38);
    t40 = evalxor(t25, t39);

    t41 = evalxor(t40, t37);
    t42 = evalxor(t29, t33);
    t43 = evalxor(t29, t40);
    t44 = evalxor(t33, t37);
    t45 = evalxor(t42, t41);
    z0 = evaland(t44, y15);
    z1 = evaland(t37, y6);
    z2 = evaland(t33, x[7]);
    z3 = evaland(t43, y16);
    z4 = evaland(t40, y1);
    z5 = evaland(t29, y7);
    z6 = evaland(t42, y11);
    z7 = evaland(t45, y17);
    z8 = evaland(t41, y10);
    z9 = evaland(t44, y12);
    z10 = evaland(t37, y3);
    z11 = evaland(t33, y4);
    z12 = evaland(t43, y13);
    z13 = evaland(t40, y5);
    z14 = evaland(t29, y2);
    z15 = evaland(t42, y9);
    z16 = evaland(t45, y14);
    z17 = evaland(t41, y8);

    t46 = evalxor(z15, z16);
    t47 = evalxor(z10, z11);
    t48 = evalxor(z5, z13);
    t49 = evalxor(z9, z10);
    t50 = evalxor(z2, z12);
    t51 = evalxor(z2, z5);
    t52 = evalxor(z7, z8);
    t53 = evalxor(z0, z3);
    t54 = evalxor(z6, z7);
    t55 = evalxor(z16, z17);
    t56 = evalxor(z12, t48);
    t57 = evalxor(t50, t53);
    t58 = evalxor(z4, t46);
    t59 = evalxor(z3, t54);
    t60 = evalxor(t46, t57);
    t61 = evalxor(z14, t57);
    t62 = evalxor(t52, t58);
    t63 = evalxor(t49, t58);
    t64 = evalxor(z4, t59);
    t65 = evalxor(t61, t62);
    t66 = evalxor(z1, t63);
    x[0] = evalxor(t59, t63);
    x[6] = evalnot(evalxor(t56, t62));
    x[7] = evalnot(evalxor(t48, t60));
    t67 = evalxor(t64, t65);
    x[3] = evalxor(t53, t66);
    x[4] = evalxor(t51, t66);
    x[5] = evalxor(t47, t65);
    x[1] = evalnot(evalxor(t64, x[3]));
    x[2] = evalnot(evalxor(t55, t67));
}
//////////// masking mc
void bs_wbmc(shares *x[32])
{
    shares *s[32];
    shares *t0, *t1, *t2, *t3, *t4;
    t0 = evalxor(x[1], x[8]);
    t1 = evalxor(t0, x[9]);
    t2 = evalxor(t1, x[16]);
    s[0] = evalxor(t2, x[24]);

    t0 = evalxor(x[2], x[9]);
    t1 = evalxor(t0, x[10]);
    t2 = evalxor(t1, x[17]);
    s[1] = evalxor(t2, x[25]);

    t0 = evalxor(x[3], x[10]);
    t1 = evalxor(t0, x[11]);
    t2 = evalxor(t1, x[18]);
    s[2] = evalxor(t2, x[26]);

    t0 = evalxor(x[0], x[4]);
    t1 = evalxor(t0, x[8]);
    t2 = evalxor(t1, x[11]);
    t3 = evalxor(t2, x[12]);
    t4 = evalxor(t3, x[19]);
    s[3] = evalxor(t4, x[27]);

    t0 = evalxor(x[0], x[5]);
    t1 = evalxor(t0, x[8]);
    t2 = evalxor(t1, x[12]);
    t3 = evalxor(t2, x[13]);
    t4 = evalxor(t3, x[20]);
    s[4] = evalxor(t4, x[28]);

    t0 = evalxor(x[6], x[13]);
    t1 = evalxor(t0, x[14]);
    t2 = evalxor(t1, x[21]);
    s[5] = evalxor(t2, x[29]);

    t0 = evalxor(x[0], x[7]);
    t1 = evalxor(t0, x[8]);
    t2 = evalxor(t1, x[14]);
    t3 = evalxor(t2, x[15]);
    t4 = evalxor(t3, x[22]);
    s[6] = evalxor(t4, x[30]);

    t0 = evalxor(x[0], x[8]);
    t1 = evalxor(t0, x[15]);
    t2 = evalxor(t1, x[23]);
    s[7] = evalxor(t2, x[31]);

    t0 = evalxor(x[0], x[9]);
    t1 = evalxor(t0, x[16]);
    t2 = evalxor(t1, x[17]);
    s[8] = evalxor(t2, x[24]);

    t0 = evalxor(x[1], x[10]);
    t1 = evalxor(t0, x[17]);
    t2 = evalxor(t1, x[18]);
    s[9] = evalxor(t2, x[25]);

    t0 = evalxor(x[2], x[11]);
    t1 = evalxor(t0, x[18]);
    t2 = evalxor(t1, x[19]);
    s[10] = evalxor(t2, x[26]);

    t0 = evalxor(x[3], x[8]);
    t1 = evalxor(t0, x[12]);
    t2 = evalxor(t1, x[16]);
    t3 = evalxor(t2, x[19]);
    t4 = evalxor(t3, x[20]);
    s[11] = evalxor(t4, x[27]);

    t0 = evalxor(x[4], x[8]);
    t1 = evalxor(t0, x[13]);
    t2 = evalxor(t1, x[16]);
    t3 = evalxor(t2, x[20]);
    t4 = evalxor(t3, x[21]);
    s[12] = evalxor(t4, x[28]);

    t0 = evalxor(x[5], x[14]);
    t1 = evalxor(t0, x[21]);
    t2 = evalxor(t1, x[22]);
    s[13] = evalxor(t2, x[29]);

    t0 = evalxor(x[6], x[8]);
    t1 = evalxor(t0, x[15]);
    t2 = evalxor(t1, x[16]);
    t3 = evalxor(t2, x[22]);
    t4 = evalxor(t3, x[23]);
    s[14] = evalxor(t4, x[30]);

    t0 = evalxor(x[7], x[8]);
    t1 = evalxor(t0, x[16]);
    t2 = evalxor(t1, x[23]);
    s[15] = evalxor(t2, x[31]);

    t0 = evalxor(x[0], x[8]);
    t1 = evalxor(t0, x[17]);
    t2 = evalxor(t1, x[24]);
    s[16] = evalxor(t2, x[25]);

    t0 = evalxor(x[1], x[9]);
    t1 = evalxor(t0, x[18]);
    t2 = evalxor(t1, x[25]);
    s[17] = evalxor(t2, x[26]);

    t0 = evalxor(x[2], x[10]);
    t1 = evalxor(t0, x[19]);
    t2 = evalxor(t1, x[26]);
    s[18] = evalxor(t2, x[27]);

    t0 = evalxor(x[3], x[11]);
    t1 = evalxor(t0, x[16]);
    t2 = evalxor(t1, x[20]);
    t3 = evalxor(t2, x[24]);
    t4 = evalxor(t3, x[27]);
    s[19] = evalxor(t4, x[28]);

    t0 = evalxor(x[4], x[12]);
    t1 = evalxor(t0, x[16]);
    t2 = evalxor(t1, x[21]);
    t3 = evalxor(t2, x[24]);
    t4 = evalxor(t3, x[28]);
    s[20] = evalxor(t4, x[29]);

    t0 = evalxor(x[5], x[13]);
    t1 = evalxor(t0, x[22]);
    t2 = evalxor(t1, x[29]);
    s[21] = evalxor(t2, x[30]);

    t0 = evalxor(x[6], x[14]);
    t1 = evalxor(t0, x[16]);
    t2 = evalxor(t1, x[23]);
    t3 = evalxor(t2, x[24]);
    t4 = evalxor(t3, x[30]);
    s[22] = evalxor(t4, x[31]);

    t0 = evalxor(x[7], x[15]);
    t1 = evalxor(t0, x[16]);
    t2 = evalxor(t1, x[24]);
    s[23] = evalxor(t2, x[31]);

    t0 = evalxor(x[0], x[1]);
    t1 = evalxor(t0, x[8]);
    t2 = evalxor(t1, x[16]);
    s[24] = evalxor(t2, x[25]);

    t0 = evalxor(x[1], x[2]);
    t1 = evalxor(t0, x[9]);
    t2 = evalxor(t1, x[17]);
    s[25] = evalxor(t2, x[26]);

    t0 = evalxor(x[2], x[3]);
    t1 = evalxor(t0, x[10]);
    t2 = evalxor(t1, x[18]);
    s[26] = evalxor(t2, x[27]);

    t0 = evalxor(x[0], x[3]);
    t1 = evalxor(t0, x[4]);
    t2 = evalxor(t1, x[11]);
    t3 = evalxor(t2, x[19]);
    t4 = evalxor(t3, x[24]);
    s[27] = evalxor(t4, x[28]);

    t0 = evalxor(x[0], x[4]);
    t1 = evalxor(t0, x[5]);
    t2 = evalxor(t1, x[12]);
    t3 = evalxor(t2, x[20]);
    t4 = evalxor(t3, x[24]);
    s[28] = evalxor(t4, x[29]);

    t0 = evalxor(x[5], x[6]);
    t1 = evalxor(t0, x[13]);
    t2 = evalxor(t1, x[21]);
    s[29] = evalxor(t2, x[30]);

    t0 = evalxor(x[0], x[6]);
    t1 = evalxor(t0, x[7]);
    t2 = evalxor(t1, x[14]);
    t3 = evalxor(t2, x[22]);
    t4 = evalxor(t3, x[24]);
    s[30] = evalxor(t4, x[31]);

    t0 = evalxor(x[0], x[7]);
    t1 = evalxor(t0, x[15]);
    t2 = evalxor(t1, x[23]);
    s[31] = evalxor(t2, x[24]);

    memmove(x, s, sizeof(s));
}

void genenkey(u8 key[16], shares *k[11][128])
{
    u8 kk[176];
    int i, j, r;
    u8 temp;
    expandKey(key, kk);
    for(i = 0; i < 10; i++)
    {
        shiftRows(kk + 16 * i);
        for(j = 0; j < 16; j++)
        {
            temp = kk[i * 16 + j];
            for(r = 0; r < 8; r++)
            {
                if(temp & idM8[r]) k[i][j * 8 + r] = encode(1);
                else k[i][j * 8 + r] = encode(0);
            }
        }
    }
    i = 10;
    for(j = 0; j < 16; j++)
    {
        temp = kk[i * 16 + j];
        for(r = 0; r < 8; r++)
        {
            if(temp & idM8[r]) k[i][j * 8 + r] = encode(1);
            else k[i][j * 8 + r] = encode(0);
        }
    }
}
void encode_ptx(u8 in[16], shares *x[128])
{
    int j, r;
    u8 temp;
    for(j = 0; j < 16; j++)
    {
        temp = in[j];
        for(r = 0; r < 8; r++)
        {
            if(temp & idM8[r]) x[j * 8 + r] = encode(1);
            else x[j * 8 + r] = encode(0);
        }
    }
    u32 *t = (u32 *)in;
    InitRandom(*t ^ *(t + 1) ^ *(t + 2) ^ *(t + 3));
}

void decode_ctx(shares *x[128], u8 out[16])
{
    int j;
    memset(out, 0, sizeof(u8) * 16);
    for(j = 0; j < 128; j++)
    {
        if(decode(x[j])) out[j / 8] ^= idM8[j % 8];
    }
}
void ShiftRows(shares *x[128])
{
    bs_wbsr(x);
}
void MixColumns(shares *x[128])
{
    bs_wbmc(x);
    bs_wbmc(x + 32);
    bs_wbmc(x + 64);
    bs_wbmc(x + 96);
}

void wbaes(shares *x[128], shares *k[11][128], shares *y[128])
{
    int r;
    memmove(y, x, sizeof(shares*) * 128);
    shares *tmp[8 * SlotNum];
    for(r = 0; r < 9; r++)
    {
        ShiftRows(y);
        for(int j = 0; j < 16; j++)
        {
            evaluation(y, k, r, j, tmp);
            output_selection(tmp, y + j * 8);
        }
        MixColumns(y);
    }
    ShiftRows(y);
    for(int j = 0; j < 16; j++)
    {
        evaluation(y, k, r, j, tmp);
        output_selection(tmp, y + j * 8);
    }
}
//////////// masking shiftrows
void bs_wbsr(shares *x[128])
{
    shares *s[128];
    int i;
    for(i = 0; i < 8; i++)
    {
        s[i] = x[i];
        s[32 + i] = x[32 + i];
        s[64 + i] = x[64 + i];
        s[96 + i] = x[96 + i];

        s[8 + i] = x[40 + i];
        s[40 + i] = x[72 + i];
        s[72 + i] = x[104 + i];
        s[104 + i] = x[8 + i];

        s[16 + i] = x[80 + i];
        s[48 + i] = x[112 + i];
        s[80 + i] = x[16 + i];
        s[112 + i] = x[48 + i];

        s[24 + i] = x[120 + i];
        s[56 + i] = x[24 + i];
        s[88 + i] = x[56 + i];
        s[120 + i] = x[88 + i];
    }
    memmove(x, s, sizeof(s));
}