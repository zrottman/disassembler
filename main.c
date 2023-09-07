#include <stdio.h>
#include <stdint.h>

typedef enum Template { A, B, C, D, A2, B2, C2, E2, A3, B3, E3, VACANT } Template;

uint8_t  read_instr_len(uint8_t instruction)
{
    uint8_t instr_mask = 0b11000000; // xc0
    return (instr_mask & instruction) >> 6;
}

uint8_t read_mode1(uint8_t instruction)
{
    uint8_t mode1_mask = 0b00111000; // x38
    return (mode1_mask & instruction) >> 3;
}

Template get_template(uint8_t instruction)
{
    switch (read_instr_len(instruction)) {
        case 0:
            return get_template_0(instruction);
        case 1:
            return get_template_1(instruction);
        case 2:
            return get_template_2(instruction);
        case 3:
            return get_template_3(instruction);
    }
}

Template get_template_0(uint8_t instruction)
{
    switch (read_mode1(instruction)) {
        case 0:
            return A;
        case 1:
            return B;
        case 2:
            return A;
        case 3:
            return B;
        case 4:
            return A;
        case 5:
            return A;
        case 6:
            return A;
        case 7:
            return B;
    }
}

Template get_template_1(uint8_t instruction)
{
    switch (read_mode1(instruction)) {
        case 0:
            return A;
        case 1:
            return C;
        case 2:
            return A;
        case 3:
            return B;
        case 4:
            return C;
        case 5:
            return VACANT;
        case 6:
            return A;
        case 7:
            return B;
    }
}

    

int main() {
    return 0;
}
