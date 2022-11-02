#include "wbaes.h"
#include <time.h>
int main()
{
    int byte_ind = 0;
    for(byte_ind = 0; byte_ind < 16; byte_ind++)
    {
        attack_wbaes(byte_ind);
    }
 
    return 0;
}