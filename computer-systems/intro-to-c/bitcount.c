#include <assert.h>
#include <stdio.h>

int bitcount(int n)
{
    int sum = 0;

    for (int i = sizeof(n); i >= 0; i--)
    {

        if ((n >> i) & 1)
            sum++;
    }

    return sum;
}

int main()
{
    assert(bitcount(0) == 0);
    assert(bitcount(1) == 1);
    assert(bitcount(3) == 2);
    assert(bitcount(8) == 1);
    // harder case:
    // assert(bitcount(0xffffffff) == 32);
    printf("OK\n");
}
