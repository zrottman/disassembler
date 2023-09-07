#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

typedef enum Template { A, B, C, D, A2, B2, C2, E2, A3, B3, E3, VACANT } Template;

uint8_t read_len(uint8_t *instruction)
{
    uint8_t instr_mask = 0b11000000; // xc0
    return (instr_mask & instruction[3]) >> 6;
}

uint8_t read_mode1(uint8_t *instruction)
{
    uint8_t mode1_mask = 0b00111000; // x38
    return (mode1_mask & instruction[3]) >> 3;
}

uint8_t read_operand_6(uint8_t *instruction)
{
    uint8_t mask_1 = 0b00000111;
    uint8_t mask_2 = 0b11100000;
    return ((instruction[3] & mask_1) << 3 | (instruction[2] & mask_2) >> 5);
}

void print_b(uint8_t *instruction)
{
	uint8_t mask;
	int output;

	printf("instruction length: %d\n", read_len(instruction));
	printf("mode: %d\n", read_mode1(instruction));
	printf("operand: %d\n", read_operand_6(instruction));

	mask = 0b00011111;
	output = (instruction[2] & mask);
	printf("dest register: %d\n", output);

	mask = 0b10000000;
	output = instruction[1] & mask >> 7;
	printf("M: %d\n", output);

	mask = 0b01100000;
	output = instruction[1] & mask >> 5;
	printf("operand type: %d\n", output);

	mask = 0b00011111;
	output = (instruction[1] & mask);
	printf("src register: %d\n", output);

	output = instruction[0];
	printf("immediate constant: %d\n", output);
}


Template get_template_0(uint8_t *instruction)
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
        default:
	       printf("error\n");
            exit(1);
    }
}

Template get_template_1(uint8_t *instruction)
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
        default:
	       printf("error\n");
            exit(1);
    }
}

    
Template get_template(uint8_t *instruction)
{
    switch (read_len(instruction)) {
        case 0:
            return get_template_0(instruction);
        case 1:
            return get_template_1(instruction);
//        case 2:
//            return get_template_2(instruction);
//        case 3:
//            return get_template_3(instruction);
        default:
	       printf("error\n");
            exit(1);
    }
}

int main() {
    uint8_t instruction[] = {
	    0b00000000,
	    0b00011111,
	    0b00011111,
	    0b01011000,
    };

    int fd = open("input.ob", O_RDONLY);
    if (fd < 0) {
	    printf("open failed\n");
	    exit(1);
    }
    lseek(fd, 0x68, SEEK_SET);
    size_t n = 4 * 17;
    uint8_t buf[n];
    if (read(fd, buf, n) != n) {
	    printf("read failed\n");
	    exit(1);
    }

    if (get_template(instruction) == B) {
	    printf("great success!\n");
    }
    return 0;
}
