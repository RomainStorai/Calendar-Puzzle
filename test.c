
#include <stdint.h>
#include <stdio.h>
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

void edit(int test[5])
{
    test[0] = 5;
}

int main()
{
    uint8_t nb = 1;

    nb = nb << 8;

    printf("nb= "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(nb));

    int te[5] = {0};

    printf("Before %d\n", te[0]);

    edit(te);

    printf("After %d\n", te[0]);
}
