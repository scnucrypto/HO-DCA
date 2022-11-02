#include <stdio.h>
#include <string.h>

typedef unsigned char      u8;
typedef unsigned int       u32;
typedef unsigned long long u64;

extern u8 Ti[3532];

extern u8 op[2792138];
extern int operand1[2792138];
extern int operand2[2792138];
extern u8 Tmask1[2792138];
extern u8 Tmask2[2792138];

u8 memory[2792138];

int ptr_in[128]     = {3293,2975,2911,2536,2785,3248,3135,3120,2818,3115,2997,2598,3189,2857,3034,2957,3103,3397,2611,3158,2495,3461,2996,2727,2777,2875,2748,2900,3012,3062,2931,3085,2978,2647,3097,2878,3198,3210,3274,2991,3144,2880,2786,2655,2490,2762,2771,3032,2522,3014,3211,2609,2676,3096,3181,3312,2958,3230,2751,3084,3098,3139,3220,3030,2927,2610,3113,2701,2638,3195,2764,3053,2756,2798,2161,2568,2793,3315,3142,2865,2603,2604,2772,3000,3234,3269,3026,3040,3301,3140,2913,3100,3001,3227,2932,3086,2455,2544,3094,2759,3057,3061,2710,2939,2779,3051,2649,2813,2835,2850,3224,2640,3360,2972,3024,2933,3112,2743,3245,2942,2366,2963,3250,2877,2296,2423,3202,3215};
int ptr_output[142] = {2791733,2791662,2792032,2792096,2791942,2792115,2791727,2792083,2792137,2791337,2791792,2791790,2791957,2791696,2791952,2792113,2791936,2791715,2791840,2791528,2791934,2792042,2791853,2791859,2791843,2791652,2791817,2792095,2791741,2791904,2791690,2791969,2791731,2791766,2791929,2791961,2792044,2791355,2791980,2792072,2792008,2791604,2792136,2791672,2791403,2792036,2791752,2791620,2791703,2791644,2791625,2791805,2792046,2791869,2791756,2791810,2791740,2791865,2792061,2791994,2791968,2791814,2791698,2792114,2791606,2791864,2792060,2791920,2791611,2791857,2791908,2791999,2792109,2791719,2791959,2791974,2792012,2791618,2792017,2792124,2791759,2791512,2791995,2792079,2791542,2791932,2791945,2791667,2791967,2791717,2791476,2792123,2791984,2791596,2792135,2791538,2792078,2792094,2791955,2791960,2791978,2791800,2791823,2792074,2791616,2791475,2792086,2791901,2791958,2791534,2791481,2791701,2792048,2791847,2791822,2792059,2791357,2791803,2791405,2791951,2792005,2791474,2791640,2792122,2791762,2791623,2791839,2791911,2792016,2792134,2792068,2792043,2791406,2791601,2791891,2792110,2791639,2791917,2791966,2791836,2792007,2792056};

u8 p1[142];

void AES_128_encrypt(u8 *ct, u8 *pt, u8 p1_cache[142]);

// ============================================================================
// ============================================================================
// HODCA

#define TOTAL 767

extern u8 p1_cache[TOTAL][142];
extern char * pt[TOTAL];
extern char * exp_ct[TOTAL];
extern u8 ct[TOTAL][16];
extern u8 plaintext[TOTAL][16];

#define NB_TRACES 767
#define MAX_INDEX 2792138
#define MAX_TRACE_SIZE 300000

#define NB_SHARES_MIN 2
#define NB_SHARES_MAX 30

double scores[256][MAX_TRACE_SIZE];
double scores_max[256];
int index_max[256];

u64 acc[MAX_INDEX];
u64 traces[NB_TRACES][MAX_TRACE_SIZE];

int target_indexes[MAX_TRACE_SIZE];
int NB_TARGET_INDEXES;

u8 TRACES_GENERATED = 0;


#define OCC_CTR_MAX 100

u64 op_counter[2792138];

void count_operands()
{
  // count the number of co-operand of each gate
  for (int i=0; i<2792138; i++) {
    op_counter[i] = 0;
  }

  for (int i = 8351; i < 2792138; ++i) {
      if (op[i] == 1) {
	  op_counter[operand1[i]] ++;
	  op_counter[operand2[i]] ++;
	}
    }
}

void get_target_indexes()
{
  for(int i=0; i<MAX_INDEX; i++) 
  {
    if ((op_counter[i] >= NB_SHARES_MIN) && (op_counter[i] <= NB_SHARES_MAX)) 
    {
      target_indexes[NB_TARGET_INDEXES] = i;
      NB_TARGET_INDEXES ++;
    }
  }

  // printf("Nb target indexes: %d\n", NB_TARGET_INDEXES);

  if (NB_TARGET_INDEXES > MAX_TRACE_SIZE) 
  {
    printf("Error: Nb target indexes cannot be greater than max trace size!\n");
    exit(0);
  }

}

void generate_traces()
{
  // printf("\n> generating traces...\n");

  for (int t=0; t<TOTAL; t++) 
  {
    // if (t % (NB_TRACES/10) == 0) 
    // {
    //   printf("  %3d / %d\n", t, NB_TRACES);
    // }

    // generate acc trace

    for (int i = 0; i < 16; ++i) 
    {
      plaintext[t][i] = hex2char(pt[t]+2*i);
    }
    AES_128_encrypt(ct[t], plaintext[t], p1_cache[t]);

    // store acc trace
    for (int i=0; i<NB_TARGET_INDEXES; i++) 
    {
      int ind = target_indexes[i];
      traces[t][i] = acc[ind];
    }
  }
  // printf("\n");

  TRACES_GENERATED = 1;
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
static const u8 idM8[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

u8 Sfunction(u8 in, int Sbox_ind)
{
    int r, j;
    u8 x[8], s[8], ou = 0;
    u8 ANDin[64] = {0};
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
    //--
    ANDin[0] = y12;
    ANDin[1] = y15;
    //--
    t3 = y3 & y6;
    //--
    ANDin[2] = y3;
    ANDin[3] = y6;
    //--
    t4 = t3 ^ t2;
    t5 = y4 & x[7];
    //--
    ANDin[4] = y4;
    ANDin[5] = x[7];
    //--
    t6 = t5 ^ t2;
    t7 = y13 & y16;
    //--
    ANDin[6] = y13;
    ANDin[7] = y16;
    //--
    t8 = y5 & y1;
    //--
    ANDin[8] = y5;
    ANDin[9] = y1;
    //--
    t9 = t8 ^ t7;
    t10 = y2 & y7;
    //--
    ANDin[10] = y2;
    ANDin[11] = y7;
    //--
    t11 = t10 ^ t7;
    t12 = y9 & y11;
    //--
    ANDin[12] = y9;
    ANDin[13] = y11;
    //--
    t13 = y14 & y17;
    //--
    ANDin[14] = y14;
    ANDin[15] = y17;
    //--
    t14 = t13 ^ t12;
    t15 = y8 & y10;
    //--
    ANDin[16] = y8;
    ANDin[17] = y10;
    //--
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
    //--
    ANDin[18] = t21;
    ANDin[19] = t23;
    //--
    t27 = t24 ^ t26;
    t28 = t25 & t27;
    //--
    ANDin[20] = t25;
    ANDin[21] = t27;
    //--
    t29 = t28 ^ t22;
    t30 = t23 ^ t24;
    t31 = t22 ^ t26;
    t32 = t31 & t30;
    //--
    ANDin[22] = t31;
    ANDin[23] = t30;
    //--
    t33 = t32 ^ t24;
    t34 = t23 ^ t33;
    t35 = t27 ^ t33;
    t36 = t24 & t35;
    //--
    ANDin[24] = t24;
    ANDin[25] = t35;
    //--
    t37 = t36 ^ t34;
    t38 = t27 ^ t36;
    t39 = t29 & t38;
    //--
    ANDin[26] = t29;
    ANDin[27] = t38;
    //--
    t40 = t25 ^ t39;

    t41 = t40 ^ t37;
    t42 = t29 ^ t33;
    t43 = t29 ^ t40;
    t44 = t33 ^ t37;
    t45 = t42 ^ t41;
    z0 = t44 & y15;
    //--
    ANDin[28] = t44;
    ANDin[29] = y15;
    //--
    z1 = t37 & y6;
    //--
    ANDin[30] = t37;
    ANDin[31] = y6;
    //--
    z2 = t33 & x[7];
    //--
    ANDin[32] = t33;
    ANDin[33] = x[7];
    //--
    z3 = t43 & y16;
    //--
    ANDin[34] = t43;
    ANDin[35] = y16;
    //--
    z4 = t40 & y1;
    //--
    ANDin[36] = t40;
    ANDin[37] = y1;
    //--
    z5 = t29 & y7;
    //--
    ANDin[38] = t29;
    ANDin[39] = y7;
    //--
    z6 = t42 & y11;
    //--
    ANDin[40] = t42;
    ANDin[41] = y11;
    //--
    z7 = t45 & y17;
    //--
    ANDin[42] = t45;
    ANDin[43] = y17;
    //--
    z8 = t41 & y10;
    //--
    ANDin[44] = t41;
    ANDin[45] = y10;
    //--
    z9 = t44 & y12;
    //--
    ANDin[46] = t44;
    ANDin[47] = y12;
    //--
    z10 = t37 & y3;
    //--
    ANDin[48] = t37;
    ANDin[49] = y3;
    //--
    z11 = t33 & y4;
    //--
    ANDin[50] = t33;
    ANDin[51] = y4;
    //--
    z12 = t43 & y13;
    //--
    ANDin[52] = t43;
    ANDin[53] = y13;
    //--
    z13 = t40 & y5;
    //--
    ANDin[54] = t40;
    ANDin[55] = y5;
    //--
    z14 = t29 & y2;
    //--
    ANDin[56] = t29;
    ANDin[57] = y2;
    //--
    z15 = t42 & y9;
    //--
    ANDin[58] = t42;
    ANDin[59] = y9;
    //--
    z16 = t45 & y14;
    //--
    ANDin[60] = t45;
    ANDin[61] = y14;
    //--
    z17 = t41 & y8;
    //--
    ANDin[62] = t41;
    ANDin[63] = y8;
    //--

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

    if(ANDin[Sbox_ind]) ou ^= idM8[7];
    return ou;
}
u64 prediction(u8* pt, int byte_ind, int bit_ind, u8 k, int Sbox_ind)
{
  // return (u8) ((AES_Sbox[pt[byte_ind]^k] >> bit_ind) & 1);
  return (u8) ((Sfunction(pt[byte_ind]^k, Sbox_ind) >> bit_ind) & 1);
}
u8 rkey[16] = {0xa5, 0x2e, 0xb3, 0x40, 0xc2, 0xaf, 0x72, 0xd3, 0xb7, 0x6b, 0xc8, 0x67, 0xd0, 0x64, 0xe3, 0x63};
int attack(int byte_ind, int bit_ind, int Sbox_ind)
{
  if (!TRACES_GENERATED) 
  {
    count_operands();
    get_target_indexes();
    generate_traces();
  }

  // init scores to 0
  for (int i=0; i<NB_TARGET_INDEXES; i++) 
  {
    for (int k=0; k<256; k++) 
    {
      scores[k][i] = 0;
    }
  }

  // update scores

  // printf("> attack ...\n");
  for (int t=0; t< NB_TRACES; t++) 
  {
    // if (t % (NB_TRACES/10) == 0) 
    // {
    //   printf("  %3d / %d\n",t,NB_TRACES);
    // }

    u8 current_plaintext[16];
    for (int i=0; i<16; i++) 
    {
      current_plaintext[i] = plaintext[t][i];
    }

    // update scores
    for (int k=0; k<256; k++) 
    {
      u64 pred = prediction(current_plaintext, byte_ind, bit_ind, k, Sbox_ind);

      //for (int i = 0; i < NB_TARGET_INDEXES; i++)
      for (int i=77000; i<102000; i++) 
      // for (int i=0; i<77000; i++)
      {
	      scores[k][i] += pred * traces[t][i] + (1 - pred) * (1 - traces[t][i]);
      }
    }
  }
  // printf("\n");

  // normalize scores
  //for (int i=0; i<NB_TARGET_INDEXES; i++)
  for (int i=77000; i<102000; i++)
  // for (int i=0; i<77000; i++)  
  {
    for (int k=0; k<256; k++) 
    {
      scores[k][i] = fabs(scores[k][i] - ((double)NB_TRACES)/2) / NB_TRACES;
    }
  }

  // compute max normalized scores
  for (int k=0; k<256; k++) 
  {
    double max = 0;

    //for (int i=0; i<NB_TARGET_INDEXES; i++)
    for (int i=77000; i<102000; i++)
    // for (int i=0; i<77000; i++)
    {
      if (scores[k][i] > max) 
      {
        max = scores[k][i];
        index_max[k] = i;
      }
    }

    scores_max[k] = max;
  }

  // print results

  double max = 0;
  u8 k_max;
  int ind_max;

  for (int k=0; k<256; k++)
  {
    if (scores_max[k] > max)
    {
      max = scores_max[k];
      k_max = k;
      ind_max = index_max[k];
    }
  }

  double second_max = 0;

  for (int k=0; k<256; k++)
  {
    if (scores_max[k] < max)
    {
      if (scores_max[k] > second_max)
      {
        second_max = scores_max[k];
      }
    }
  }

  int nbs = op_counter[target_indexes[ind_max]];

  // printf("(byte: %02d | bit: %d | index: %d | nb shares: %d)  %02x =>  %lf  /  %lf\n", byte_ind, bit_ind, ind_max, nbs, k_max, max, second_max);
  printf("(byte: %02d | bit: %d | index: %d | nb shares: %d)  %02x =>  %lf  /  %lf\n", byte_ind, bit_ind, ind_max, nbs, k_max, max, second_max);
  // FILE *fpWrite = fopen("improved_HO-DCA.txt", "a");
  // fprintf(fpWrite, "(byte: %02d | bit: %d | Sbox_index: %d | index: %d | nb shares: %d)  %02x =>  %lf  /  %lf\n", byte_ind, bit_ind, Sbox_ind, ind_max, nbs, k_max, max, second_max);
  // fclose(fpWrite);
  // if(rkey[byte_ind] == k_max) return 1;
  // else return 0;
}

// ============================================================================
// ============================================================================



void the_while_loop() 
{

  // HODCA: init accumulators
  for (int i = 0; i < MAX_INDEX; ++i) 
  {
    acc[i] = 0;
  }

  Tmask1[2792137] = 1;

  for (int i = 0; i < 128; ++i) 
  {
    memory[i] = Ti[ptr_in[i]];
  }

  for (int i = 128; i < 136; ++i) 
  {
    memory[i] = 0;
  }


  for (int i = 136; i < 2792138; ++i) 
  {
    if (op[i] == 0) 
    {
      memory[i] = memory[operand1[i]] ^ memory[operand2[i]];
    } 
    else 
    { //if (op[i] == 1)
      u8 mask1 = Tmask1[i];
      u8 mask2 = Tmask2[i];
      memory[i] = (memory[operand1[i]] ^ mask1) & (memory[operand2[i]] ^ mask2);

      // data-dependency trace: accumulating the value of co-operand of AND gate
      acc[operand1[i]] ^= memory[operand2[i]];
      acc[operand2[i]] ^= memory[operand1[i]];
    }
  }

  for (int i = 0; i < 142; ++i) 
  {
    p1[i] ^= memory[ptr_output[i]];
  }
}

extern u8 end_op[117958];
extern int end_operand1[117958];
extern int end_operand2[117958];
extern u8 end_Tmask1[117958];
extern u8 end_Tmask2[117958];
u8 end_mem[117958];

int end_constants_idx[257] = {113803, 113808, 113819, 114069, 114082, 114097, 114100, 114103, 114105, 114107, 114109, 114115, 114117, 114291, 114312, 114324, 114325, 114330, 114335, 114345, 114348, 114423, 114425, 114428, 114431, 114444, 114447, 114451, 114455, 114462, 114608, 114626, 114628, 114632, 114635, 114638, 114641, 114646, 114648, 114651, 114653, 114655, 114657, 114664, 114793, 114798, 114800, 114802, 114804, 114807, 114809, 114811, 114975, 114982, 114992, 114997, 115003, 115008, 115011, 115015, 115018, 115218, 115220, 115313, 115314, 115318, 115320, 115323, 115326, 115329, 115333, 115335, 115336, 115338, 115340, 115342, 115623, 115650, 115652, 115717, 115879, 115882, 115908, 115911, 115912, 115917, 115922, 115927, 115931, 115936, 115941, 115947, 115950, 116014, 116117, 116121, 116128, 116132, 116136, 116142, 116144, 116148, 116150, 116154, 116156, 116160, 116162, 116163, 116165, 116168, 116170, 116173, 116175, 116177, 116233, 116235, 116258, 116295, 116303, 116328, 116337, 116345, 116347, 116352, 116354, 116359, 116361, 116362, 116364, 116366, 116368, 116370, 116372, 116398, 116400, 116403, 116406, 116419, 116423, 116428, 116433, 116449, 116451, 116457, 116463, 116468, 116481, 116484, 116488, 116492, 116495, 116498, 116499, 116500, 116501, 116502, 116503, 116504, 116523, 116527, 116532, 116536, 116538, 116540, 116543, 116545, 116547, 116549, 116552, 116558, 116565, 116567, 116570, 116572, 116575, 116577, 116579, 116581, 116584, 116588, 116630, 116633, 116634, 116635, 116637, 116639, 116641, 116644, 116651, 116654, 116661, 116664, 116669, 116672, 116676, 116678, 116703, 116704, 116705, 116706, 116707, 116708, 116718, 116721, 116726, 116731, 116734, 116737, 116878, 116888, 116897, 116901, 116902, 116910, 116912, 116914, 116966, 116976, 116984, 116987, 116993, 116998, 116999, 117005, 117599, 117611, 117613, 117615, 117617, 117626, 117629, 117631, 117633, 117636, 117638, 117642, 117648, 117651, 117657, 117660, 117780, 117790, 117792, 117796, 117798, 117799, 117809, 117813, 117916, 117926, 117935, 117939, 117943, 117948, 117949, 117955, 117958};
u8  end_constants_val[257] = {0,1,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,1,0,0,0,1,1,0,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,1,0,0,1,0,1,0,1,0,1,0,1,1,0,0,1,1,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,0,0,1,1,1,0,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,1,0,1,0,0,1,1,1,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,1,1,0};

void the_loop_in_the_end() {
  for (int i = 142; i < 117958; ++i) {
    if (end_op[i] == 0) {
      end_mem[i] = end_mem[end_operand1[i]] ^ end_mem[end_operand2[i]];
    } else {
      end_mem[i] = (end_mem[end_operand1[i]] ^ end_Tmask1[i]) & (end_mem[end_operand2[i]] ^ end_Tmask2[i]);
    }
  }
}


void AES_128_encrypt(u8 *ct, u8 *pt, u8 p1_cache[142]) {
  memcpy(p1, p1_cache, 142);
  memset(Ti, 0, 3252);
  memset(memory, 0, 2792138);
  memset(end_mem, 0, 117958);
  // set memory to be zero

  // the Boolean circuit in the beginning
  // from 128 plaintext bit to 3531 values in table Ti
  start_boolean_circuit(pt);

  // the while loop
  // input:  128 bit from Ti + magic number
  // output: 142 bit (table p1) by paritying
  the_while_loop();


  // xor value from table Ti onto table p1
  end_boolean_circuit();

  // the loop before finish
  // input: 142 bits (table p1)
  for (int i = 0; i < 142; i++ ) {
    end_mem[i] = p1[i];
  }
  the_loop_in_the_end();

  // recover ciphertext bits
  memset(ct, 0, 16);
  ct[ 0] ^= end_mem[116396] << 5;
  ct[ 0] ^= end_mem[116477] << 7;
  ct[ 0] ^= end_mem[116478] << 6;
  ct[ 0] ^= end_mem[116516] << 0;
  ct[ 0] ^= end_mem[116623] << 1;
  ct[ 0] ^= end_mem[116625] << 4;
  ct[ 0] ^= end_mem[116701] << 2;
  ct[ 0] ^= end_mem[116702] << 3;
  ct[ 1] ^= end_mem[116649] << 5;
  ct[ 1] ^= end_mem[116656] << 6;
  ct[ 1] ^= end_mem[116710] << 0;
  ct[ 1] ^= end_mem[116712] << 7;
  ct[ 1] ^= end_mem[116752] << 1;
  ct[ 1] ^= end_mem[116754] << 4;
  ct[ 1] ^= end_mem[116778] << 2;
  ct[ 1] ^= end_mem[116779] << 3;
  ct[ 2] ^= end_mem[115324] << 5;
  ct[ 2] ^= end_mem[115332] << 6;
  ct[ 2] ^= end_mem[115654] << 0;
  ct[ 2] ^= end_mem[115656] << 7;
  ct[ 2] ^= end_mem[115659] << 4;
  ct[ 2] ^= end_mem[115954] << 1;
  ct[ 2] ^= end_mem[116183] << 2;
  ct[ 2] ^= end_mem[116184] << 3;
  ct[ 3] ^= end_mem[117918] << 5;
  ct[ 3] ^= end_mem[117928] << 0;
  ct[ 3] ^= end_mem[117937] << 7;
  ct[ 3] ^= end_mem[117941] << 2;
  ct[ 3] ^= end_mem[117945] << 1;
  ct[ 3] ^= end_mem[117951] << 4;
  ct[ 3] ^= end_mem[117953] << 6;
  ct[ 3] ^= end_mem[117957] << 3;
  ct[ 4] ^= end_mem[116732] << 5;
  ct[ 4] ^= end_mem[116750] << 7;
  ct[ 4] ^= end_mem[116769] << 0;
  ct[ 4] ^= end_mem[116774] << 6;
  ct[ 4] ^= end_mem[116789] << 1;
  ct[ 4] ^= end_mem[116790] << 4;
  ct[ 4] ^= end_mem[116798] << 2;
  ct[ 4] ^= end_mem[116799] << 3;
  ct[ 5] ^= end_mem[115646] << 5;
  ct[ 5] ^= end_mem[115946] << 0;
  ct[ 5] ^= end_mem[115948] << 7;
  ct[ 5] ^= end_mem[115952] << 6;
  ct[ 5] ^= end_mem[116180] << 1;
  ct[ 5] ^= end_mem[116182] << 4;
  ct[ 5] ^= end_mem[116374] << 2;
  ct[ 5] ^= end_mem[116375] << 3;
  ct[ 6] ^= end_mem[117622] << 5;
  ct[ 6] ^= end_mem[117637] << 0;
  ct[ 6] ^= end_mem[117639] << 7;
  ct[ 6] ^= end_mem[117641] << 6;
  ct[ 6] ^= end_mem[117653] << 1;
  ct[ 6] ^= end_mem[117655] << 4;
  ct[ 6] ^= end_mem[117663] << 2;
  ct[ 6] ^= end_mem[117664] << 3;
  ct[ 7] ^= end_mem[116493] << 5;
  ct[ 7] ^= end_mem[116592] << 6;
  ct[ 7] ^= end_mem[116605] << 0;
  ct[ 7] ^= end_mem[116607] << 7;
  ct[ 7] ^= end_mem[116680] << 4;
  ct[ 7] ^= end_mem[116690] << 1;
  ct[ 7] ^= end_mem[116743] << 2;
  ct[ 7] ^= end_mem[116744] << 3;
  ct[ 8] ^= end_mem[116968] << 5;
  ct[ 8] ^= end_mem[116978] << 0;
  ct[ 8] ^= end_mem[116988] << 7;
  ct[ 8] ^= end_mem[116990] << 1;
  ct[ 8] ^= end_mem[116995] << 2;
  ct[ 8] ^= end_mem[117001] << 4;
  ct[ 8] ^= end_mem[117003] << 6;
  ct[ 8] ^= end_mem[117007] << 3;
  ct[ 9] ^= end_mem[117644] << 5;
  ct[ 9] ^= end_mem[117656] << 0;
  ct[ 9] ^= end_mem[117658] << 7;
  ct[ 9] ^= end_mem[117661] << 6;
  ct[ 9] ^= end_mem[117666] << 1;
  ct[ 9] ^= end_mem[117668] << 4;
  ct[ 9] ^= end_mem[117669] << 2;
  ct[ 9] ^= end_mem[117670] << 3;
  ct[10] ^= end_mem[116479] << 5;
  ct[10] ^= end_mem[116589] << 6;
  ct[10] ^= end_mem[116593] << 0;
  ct[10] ^= end_mem[116595] << 7;
  ct[10] ^= end_mem[116682] << 1;
  ct[10] ^= end_mem[116684] << 4;
  ct[10] ^= end_mem[116739] << 2;
  ct[10] ^= end_mem[116740] << 3;
  ct[11] ^= end_mem[116716] << 5;
  ct[11] ^= end_mem[116755] << 0;
  ct[11] ^= end_mem[116757] << 7;
  ct[11] ^= end_mem[116760] << 6;
  ct[11] ^= end_mem[116781] << 1;
  ct[11] ^= end_mem[116783] << 4;
  ct[11] ^= end_mem[116794] << 2;
  ct[11] ^= end_mem[116795] << 3;
  ct[12] ^= end_mem[117782] << 5;
  ct[12] ^= end_mem[117800] << 7;
  ct[12] ^= end_mem[117802] << 6;
  ct[12] ^= end_mem[117805] << 4;
  ct[12] ^= end_mem[117806] << 3;
  ct[12] ^= end_mem[117807] << 0;
  ct[12] ^= end_mem[117811] << 2;
  ct[12] ^= end_mem[117815] << 1;
  ct[13] ^= end_mem[116486] << 5;
  ct[13] ^= end_mem[116590] << 6;
  ct[13] ^= end_mem[116599] << 0;
  ct[13] ^= end_mem[116601] << 7;
  ct[13] ^= end_mem[116686] << 1;
  ct[13] ^= end_mem[116687] << 4;
  ct[13] ^= end_mem[116741] << 2;
  ct[13] ^= end_mem[116742] << 3;
  ct[14] ^= end_mem[116724] << 5;
  ct[14] ^= end_mem[116762] << 0;
  ct[14] ^= end_mem[116764] << 7;
  ct[14] ^= end_mem[116766] << 6;
  ct[14] ^= end_mem[116785] << 1;
  ct[14] ^= end_mem[116786] << 4;
  ct[14] ^= end_mem[116796] << 2;
  ct[14] ^= end_mem[116797] << 3;
  ct[15] ^= end_mem[116880] << 5;
  ct[15] ^= end_mem[116890] << 0;
  ct[15] ^= end_mem[116903] << 7;
  ct[15] ^= end_mem[116906] << 1;
  ct[15] ^= end_mem[116907] << 2;
  ct[15] ^= end_mem[116915] << 4;
  ct[15] ^= end_mem[116917] << 3;
  ct[15] ^= end_mem[116919] << 6;

  return;
}
