#include "wbaes.h"
int main()
{
    unsigned char IN[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    unsigned char OUT[16];
    unsigned char OOUT[16];
    u8 key[16] = {0x25, 0x2e, 0xb3, 0x40, 0xc2, 0xaf, 0x72, 0xd3, 0xb7, 0x6b, 0xc8, 0x67, 0xd0, 0x64, 0xe3, 0x63};
    shuffling_aes(IN, key, OUT);
    printstate(OUT);
    
    aes_128_encrypt(IN, key, OOUT);
    printstate(OOUT);
}