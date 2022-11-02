#include "wbaes.h"

u8 randbit()
{
    return cus_random() & 0x1;
}
shares *randshares()
{
    int i;
    shares *s = (shares *)malloc(sizeof(shares));
    s->a[0] = cus_random() & 0x1;
    s->a[1] = cus_random() & 0x1;
    for(i = 1; i <= N; i++)
    {
        s->b[i] = cus_random() & 0x1;
    }
    return s;
}
//////////

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

shares *attack_evalxor(shares *x, shares *y, u8 ptx)
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

int trace[256][SAMNUM] = {0};
int index = 0;
shares *attack_evaland(shares *x, shares *y, u8 ptx)
{
    int i, j, t;
    u8 ra[2][N + 1], rb[N + 1][N + 1], u = 0;
    shares *s = (shares *)malloc(sizeof(shares));
    u8 x_cooperands[2 + N] = {0};
    u8 y_cooperands[2 + N] = {0};
    x = refresh(x);
    y = refresh(y);
    for(i = 0; i <= 1; i++)
    {
        s->a[i] = x->a[i] & y->a[(i + 1) % 2];
        //--
        x_cooperands[i] ^= y->a[(i + 1) % 2];
        y_cooperands[(i + 1) % 2] ^= x->a[i];
        //--
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
                //--
                x_cooperands[0] ^= y->b[j];                                       //x->a[0]
                x_cooperands[1] ^= ((x->a[0] & y->b[j]) ^ (ra[0][j] & y->a[0]));  //x->a[1]
                x_cooperands[1 + j] ^= y->a[0];            //x->b[j]

                y_cooperands[0] ^= ra[0][j] ^ x->b[j];                            //y->a[0]
                y_cooperands[1] ^= ((y->a[0] & x->b[j]) ^ (ra[1][j] & x->a[0]));  //y->a[1]
                y_cooperands[1 + j] ^= x->a[0];            //y->b[j]
                //--
            }
            else
            {
                rb[i][j] = randbit();
                rb[j][i] = rb[i][j] ^ (x->b[i] & y->b[j]) ^ (x->b[j] & y->b[i]);
                //--
                x_cooperands[1 + i] ^= y->b[j];  //x->b[i]
                x_cooperands[1 + j] ^= y->b[i];  //x->b[j]

                y_cooperands[1 + j] ^= x->b[i];  //y->b[j]
                y_cooperands[1 + i] ^= x->b[j];  //y->b[i]
                //--
            }
        }
    }
    for(i = 1; i <= N; i++)
    {
        s->b[i] = x->b[i] & y->b[i];
        //--
        x_cooperands[1 + i] ^= y->b[i];  //x->b[i]
        y_cooperands[1 + i] ^= x->b[i];  //y->b[i]
        //--
        for(j = 0; j <= N; j++)
        {
            if(j != i) s->b[i] ^= rb[i][j];
        }
    }
    for(i = 0; i < 2 + N; i++)
    {
        trace[ptx][index++] = x_cooperands[i];
        trace[ptx][index++] = y_cooperands[i];
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

void attack_bs_wbsbox(shares *x[8], u8 ptx)
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

    y14 = attack_evalxor(x[3], x[5], ptx);
    y13 = attack_evalxor(x[0], x[6], ptx);
    y9 = attack_evalxor(x[0], x[3], ptx);
    y8 = attack_evalxor(x[0], x[5], ptx);
    t0 = attack_evalxor(x[1], x[2], ptx);
    y1 = attack_evalxor(t0, x[7], ptx);
    y4 = attack_evalxor(y1, x[3], ptx);
    y12 = attack_evalxor(y13, y14, ptx);
    y2 = attack_evalxor(y1, x[0], ptx);
    y5 = attack_evalxor(y1, x[6], ptx);
    y3 = attack_evalxor(y5, y8, ptx);
    t1 = attack_evalxor(x[4], y12, ptx);
    y15 = attack_evalxor(t1, x[5], ptx);
    y20 = attack_evalxor(t1, x[1], ptx);
    y6 = attack_evalxor(y15, x[7], ptx);
    y10 = attack_evalxor(y15, t0, ptx);
    y11 = attack_evalxor(y20, y9, ptx);
    y7 = attack_evalxor(x[7], y11, ptx);
    y17 = attack_evalxor(y10, y11, ptx);
    y19 = attack_evalxor(y10, y8, ptx);
    y16 = attack_evalxor(t0, y11, ptx);
    y21 = attack_evalxor(y13, y16, ptx);
    y18 = attack_evalxor(x[0], y16, ptx);

    t2 = attack_evaland(y12, y15, ptx);
    t3 = attack_evaland(y3, y6, ptx);
    t4 = attack_evalxor(t3, t2, ptx);
    t5 = attack_evaland(y4, x[7], ptx);
    t6 = attack_evalxor(t5, t2, ptx);
    t7 = attack_evaland(y13, y16, ptx);
    t8 = attack_evaland(y5, y1, ptx);
    t9 = attack_evalxor(t8, t7, ptx);
    t10 = attack_evaland(y2, y7, ptx);
    t11 = attack_evalxor(t10, t7, ptx);
    t12 = attack_evaland(y9, y11, ptx);
    t13 = attack_evaland(y14, y17, ptx);
    t14 = attack_evalxor(t13, t12, ptx);
    t15 = attack_evaland(y8, y10, ptx);
    t16 = attack_evalxor(t15, t12, ptx);
    t17 = attack_evalxor(t4, t14, ptx);
    t18 = attack_evalxor(t6,  t16, ptx);
    t19 = attack_evalxor(t9, t14, ptx);
    t20 = attack_evalxor(t11,  t16, ptx);
    t21 = attack_evalxor(t17, y20, ptx);
    t22 = attack_evalxor(t18, y19, ptx);
    t23 = attack_evalxor(t19, y21, ptx);
    t24 = attack_evalxor(t20, y18, ptx);

    t25 = attack_evalxor(t21, t22, ptx);
    t26 = attack_evaland(t21, t23, ptx);
    t27 = attack_evalxor(t24, t26, ptx);
    t28 = attack_evaland(t25, t27, ptx);
    t29 = attack_evalxor(t28, t22, ptx);
    t30 = attack_evalxor(t23, t24, ptx);
    t31 = attack_evalxor(t22, t26, ptx);
    t32 = attack_evaland(t31, t30, ptx);
    t33 = attack_evalxor(t32, t24, ptx);
    t34 = attack_evalxor(t23, t33, ptx);
    t35 = attack_evalxor(t27, t33, ptx);
    t36 = attack_evaland(t24, t35, ptx);
    t37 = attack_evalxor(t36, t34, ptx);
    t38 = attack_evalxor(t27, t36, ptx);
    t39 = attack_evaland(t29, t38, ptx);
    t40 = attack_evalxor(t25, t39, ptx);

    t41 = attack_evalxor(t40, t37, ptx);
    t42 = attack_evalxor(t29, t33, ptx);
    t43 = attack_evalxor(t29, t40, ptx);
    t44 = attack_evalxor(t33, t37, ptx);
    t45 = attack_evalxor(t42, t41, ptx);
    z0 = attack_evaland(t44, y15, ptx);
    z1 = attack_evaland(t37, y6, ptx);
    z2 = attack_evaland(t33, x[7], ptx);
    z3 = attack_evaland(t43, y16, ptx);
    z4 = attack_evaland(t40, y1, ptx);
    z5 = attack_evaland(t29, y7, ptx);
    z6 = attack_evaland(t42, y11, ptx);
    z7 = attack_evaland(t45, y17, ptx);
    z8 = attack_evaland(t41, y10, ptx);
    z9 = attack_evaland(t44, y12, ptx);
    z10 = attack_evaland(t37, y3, ptx);
    z11 = attack_evaland(t33, y4, ptx);
    z12 = attack_evaland(t43, y13, ptx);
    z13 = attack_evaland(t40, y5, ptx);
    z14 = attack_evaland(t29, y2, ptx);
    z15 = attack_evaland(t42, y9, ptx);
    z16 = attack_evaland(t45, y14, ptx);
    z17 = attack_evaland(t41, y8, ptx);

    t46 = attack_evalxor(z15, z16, ptx);
    t47 = attack_evalxor(z10, z11, ptx);
    t48 = attack_evalxor(z5, z13, ptx);
    t49 = attack_evalxor(z9, z10, ptx);
    t50 = attack_evalxor(z2, z12, ptx);
    t51 = attack_evalxor(z2, z5, ptx);
    t52 = attack_evalxor(z7, z8, ptx);
    t53 = attack_evalxor(z0, z3, ptx);
    t54 = attack_evalxor(z6, z7, ptx);
    t55 = attack_evalxor(z16, z17, ptx);
    t56 = attack_evalxor(z12, t48, ptx);
    t57 = attack_evalxor(t50, t53, ptx);
    t58 = attack_evalxor(z4, t46, ptx);
    t59 = attack_evalxor(z3, t54, ptx);
    t60 = attack_evalxor(t46, t57, ptx);
    t61 = attack_evalxor(z14, t57, ptx);
    t62 = attack_evalxor(t52, t58, ptx);
    t63 = attack_evalxor(t49, t58, ptx);
    t64 = attack_evalxor(z4, t59, ptx);
    t65 = attack_evalxor(t61, t62, ptx);
    t66 = attack_evalxor(z1, t63, ptx);
    x[0] = attack_evalxor(t59, t63, ptx);
    x[6] = evalnot(attack_evalxor(t56, t62, ptx));
    x[7] = evalnot(attack_evalxor(t48, t60, ptx));
    t67 = attack_evalxor(t64, t65, ptx);
    x[3] = attack_evalxor(t53, t66, ptx);
    x[4] = attack_evalxor(t51, t66, ptx);
    x[5] = attack_evalxor(t47, t65, ptx);
    x[1] = evalnot(attack_evalxor(t64, x[3], ptx));
    x[2] = evalnot(attack_evalxor(t55, t67, ptx));
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
    for(i = 0; i < 11; i++)
    {
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
void AddRoundKey(shares *x[128], shares *k[128])
{
    int i;
    for(i = 0; i < 128; i++)
    {
        x[i] = evalxor(x[i], k[i]);
    }
}
void attack_AddRoundKey(shares *x[128], shares *k[128], int byteth, u8 ptx)
{
    int i;
    for(i = byteth * 8; i < byteth * 8 + 8; i++)
    {
        x[i] = attack_evalxor(x[i], k[i], ptx);
    }
}
void SubBytes(shares *x[128])
{
    int i;
    for(i = 0; i < 16; i++)
    {
        bs_wbsbox(x + i * 8);
    }
}
void attack_SubBytes(shares *x[128], int byteth, u8 ptx)
{
    int i;
    for(i = 0; i < 16; i++)
    {
        attack_bs_wbsbox(x + i * 8, ptx);
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
    AddRoundKey(y, k[0]);
    for(r = 1; r < 10; r++)
    {
        SubBytes(y);
        ShiftRows(y);
        MixColumns(y);
        AddRoundKey(y, k[r]);
    }
    SubBytes(y);
    ShiftRows(y);
    AddRoundKey(y, k[10]);
}

const u8 AES_Sbox[256] = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

u8 Sfunction(u8 in)
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

    // for(j = 0; j < 8; j++)
    // {
    //     if(s[j]) ou ^= idM8[j];
    // }
    
    if(t29) ou ^= idM8[7];
    return ou;
}

u8 key[16] = {0xa5, 0x2e, 0xb3, 0x40, 0xc2, 0xaf, 0x72, 0xd3, 0xb7, 0x6b, 0xc8, 0x67, 0xd0, 0x64, 0xe3, 0x63};

void attack_wbaes(int byteth)
{
    int i, j;
    u8 in[16] = {0};
    u8 ou[16] = {0};
    shares *k[11][128];
    genenkey(key, k);
    shares *x[128];
    shares *y[128];
    memset(trace, 0, sizeof(trace));
    for(i = 0; i < 256; i++)
    {
        index = 0;
        in[byteth] = i;
        encode_ptx(in, x);
        memmove(y, x, sizeof(shares*) * 128);
        AddRoundKey(y, k[0]);
        SubBytes(y);
        ShiftRows(y);
        MixColumns(y);
        AddRoundKey(y, k[1]);
        attack_SubBytes(y, byteth, i);
        // printf("ptx: %02x, samples: %d\n", i, index);
    }
    i = 0;
    for(i = 0; i < 8; i++)
    {
        CDCA(byteth, i);
    }
}
double score[256][SAMNUM] = {0.0};
void CDCA(int byteth, int bit)
{
    int i, j, k;
    memset(score, 0, sizeof(score));
    double kmax[256] = {0.0};
    int f = 0, g = 0;
    for(k = 0; k < 256; k++) // key
    {
        for(j = 0; j < index; j++) // column
        {
            int Nf0 = 0, Nf1 = 0, Ng0 = 0, Ng1 = 0, N00 = 0, N01 = 0, N10 = 0, N11 = 0;
            for(i = 0; i < 256; i++) // plaintext
            {
                f = trace[i][j];
                if(f) Nf1++;
                else Nf0++;

                g = (AES_Sbox[i ^ k] >> (7 - bit)) & 1;
                // g = Sfunction(i ^ k) & 1;
                if(g) Ng1++;
                else Ng0++;
                
                if(f == 1 && g == 1) N11++;
                else if(f == 0 & g == 0) N00++;
                else if(f == 1 & g == 0) N10++;
                else N01++;
            }
            score[k][j] = abs((N11 * N00 - N10 * N01)) / sqrt(Nf1 * Nf0 * Ng1 * Ng0);
        }
    }
    for(k = 0; k < 256; k++)
    {
        double max = 0.0;
        for(j = 0; j < index; j++)
        {
            if(score[k][j] > max) max = score[k][j];
        }
        kmax[k] = max;
    }
    double key_guess_value = 0.0;
    int key_guess;
    for(k = 0; k < 256; k++)
    {
        if(kmax[k] > key_guess_value)
        {
            key_guess_value = kmax[k];
            key_guess = k;
        }
    }
    printf("byte: %d | bit: %d | key guess: %02x | Cor: %f\n", byteth, bit, key_guess, key_guess_value);
    // FILE *fpWrite = fopen("DDHO-DCA_1st_round_t29_scores.txt", "a");
    // for(k = 0; k < 256; k++)
    // {
    //     for(j = 0; j < index; j++)
    //     {
    //         fprintf(fpWrite, "%f ", score[k][j]);
    //     }
    //     fprintf(fpWrite, "\n");
    // }
    // FILE *fpWrite = fopen("DDHO-DCA_1st_round.txt", "a");
    // fprintf(fpWrite, "byte: %d | bit: %d | key guess: %02x | Cor: %f\n", byteth, bit, key_guess, key_guess_value);
    // fclose(fpWrite);
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