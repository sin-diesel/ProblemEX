#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

unsigned as_uint(float f) {
    return *(unsigned*) &f;
}

float as_float(unsigned i) {

    return *(float*) &i;

}

void print_u_asbits(unsigned u) { // for representation of numbers in bits

    unsigned len = 0;
    unsigned num = u;
    int i = 0;

    while (num != 0) {
        num = num / 2;
        ++len;
    }
    num = u;

    char* bit_repr = (char*) calloc(len, sizeof(char));
    assert(bit_repr);

    for (i = 0; i < len; ++i) {
        bit_repr[len - i - 1] = num % 2;
        num = num / 2;
    }

    for (i = 0; i < len; ++i) {
        printf("%d", bit_repr[i]);
    }
    printf("\n");

    free(bit_repr);

}

float change_odd_bits(float f) {

    int const mantissa_size = 23;
    unsigned mask = UINT32_MAX;
    unsigned new_mask = UINT32_MAX;
    unsigned num_asbits = 0;
    int i = 0;

    for (i = 0; i < mantissa_size - 1; ++i) { // 111111 > 111111 > 111110 > 111101 > 111011 > 111010 ... (building a mask containing bits in positions
        if (i % 2 == 0) {
            mask = mask << 1u;
            mask = mask - 1u;
        } else {
            mask = mask << 1u;
            mask = mask + 1u;
        }
    }
    mask = mask - 1u;

    num_asbits = as_uint(f);
    printf("Initial num in binary representation:");
    print_u_asbits(num_asbits);

    new_mask = new_mask << 23u;  //
    new_mask = new_mask & num_asbits;// leaving bits which are not part of the mantissa unchanged
    num_asbits = num_asbits ^ mask;
    num_asbits = num_asbits & 0x7FFFFFu; // after that we now have to add the unchanged part and toggled part
    new_mask = new_mask + num_asbits;// res

    printf("Toggled num in binary representation:");
    print_u_asbits(new_mask);

    return as_float(new_mask);
}

int main() {

    int res = 0;
    float num = 0;
    float toggled = 0;

    printf("Enter num:\n");
    res = scanf("%f", &num) == 1;
    assert(res == 1);

    toggled = change_odd_bits(num);
    printf("Toggled num in float reptresentation: %.5f", toggled);

    return 0;
}