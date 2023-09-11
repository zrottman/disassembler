#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

enum template { A, B, C, D, A2, B2, C2, E2, A3, B3, E3, VACANT, JUMP_TODO };

/* access IL bits */
uint8_t read_len(uint8_t *instruction)
{
    uint8_t instr_mask = 0b11000000; // xc0
    return (instr_mask & instruction[3]) >> 6;
}

/* access mode bits */
uint8_t read_mode1(uint8_t *instruction)
{
    uint8_t mode1_mask = 0b00111000; // x38
    return (mode1_mask & instruction[3]) >> 3;
}

/* access OP1 (?) bits */
uint8_t read_opcode_6(uint8_t *instruction)
{
    uint8_t mask_1 = 0b00000111;
    uint8_t mask_2 = 0b11100000;
    return ((instruction[3] & mask_1) << 3 | (instruction[2] & mask_2) >> 5);
}

/* access M bit */
uint8_t read_m_bit(uint8_t *instruction)
{
	uint8_t mask = 0b10000000;
	return (instruction[1] & mask) >> 7;
}

/* parse and output A template instruction */
void print_a(uint8_t *instruction)
{
    uint8_t mask;
    int output;

	printf("instruction length: %d\n", read_len(instruction));
	printf("mode: %d\n", read_mode1(instruction));
	printf("opcode: %d\n", read_opcode_6(instruction));

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
	printf("src register S: %d\n", output);

    mask = 0b11100000;
    output = instruction[0] & mask >> 5;
    printf("mask: %d\n", output);

    mask = 0b00011111;
	output = instruction[0] & mask;
	printf("source register T: %d\n", output);
}

/* parse and output B template instruction */
void print_b(uint8_t *instruction)
{
	uint8_t mask;
	int output;

	printf("instruction length: %d\n", read_len(instruction));
	printf("mode: %d\n", read_mode1(instruction));
	printf("opcode: %d\n", read_opcode_6(instruction));

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
	printf("src register S: %d\n", output);

	output = instruction[0];
	printf("immediate constant: %d\n", output);
}

/* parse and output C template instruction */
void print_c(uint8_t *instruction)
{
    uint8_t mask;
    int output;

    printf("instruction length: %d\n", read_len(instruction));
    printf("mode: %d\n", read_mode1(instruction));
    printf("opcode: %d\n", read_opcode_6(instruction));

    mask = 0b00011111;
    output = (instruction[2] & mask);
    printf("dest register: %d\n", output);

    output = instruction[1];
    printf("immediate constant 2: %d\n", output);

    output = instruction[0];
    printf("immediate constant 1: %d\n", output);
}

/* parse and output D template instruction */
void print_d(uint8_t *instruction)
{
    uint8_t mask;
    int output;

    printf("instruction length: %d\n", read_len(instruction));
    printf("mode: %d\n", read_mode1(instruction));

    mask = 0b00000111;
    output = instruction[3] & mask;
    printf("opcode: %d\n", output);

    output = instruction[2];
    output <<= 8;
    output |= instruction[1];
    output <<= 8;
    output |= instruction[0];
    
    printf("immediate constant 3: %d\n", output);
}

enum template get_template_0(uint8_t *instruction)
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
	       printf("error 0\n");
           exit(1);
    }
}

enum template get_template_1(uint8_t *instruction)
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
            return JUMP_TODO;
        case 7:
            if ((0 <= read_opcode_6(instruction)) && (read_opcode_6(instruction) < 16)) {
                return D;
            }
            return C;
        default:
	       printf("error 1\n");
            exit(1);
    }
}

enum template get_template_2(uint8_t *instruction)
{
    switch (read_mode1(instruction)) {
        case 0:
            switch (read_m_bit(instruction)) {
                case 0:
                    return E2;
                case 1:
                    return A2;
            }
        case 1:
            return A2;
        case 2:
            return E2;
        case 3:
            return A2;
        case 4:
            return A2;
        case 5:
            switch (read_opcode_6(instruction)) {
                case 3:
                    return A2;
                case 1:
                case 2:
                    return B2;
                case 4:
                case 5:
                case 7:
                    return C2;
            }
        case 6:
            return A2;
        case 7:
            return VACANT;
        default:
	        printf("error 2\n");
            exit(1);
    }
}

enum template get_template_3(uint8_t *instruction)
{
    switch (read_mode1(instruction)) {
        case 0:
            return E3;
        case 1:
            switch (read_opcode_6(instruction)) {
                case 0:
                    return A3;
                case 1:
                    return B3;
            }
        case 2:
            return E3;
        case 3:
            return A3;
        case 4:
            return VACANT;
        case 5:
            return VACANT;
        case 6:
            return VACANT;
        case 7:
            return VACANT;
        default:
	        printf("error 3\n");
            exit(1);
    }
}

enum template get_template(uint8_t *instruction)
{
    switch (read_len(instruction)) {
        case 0:
            return get_template_0(instruction);
        case 1:
            return get_template_1(instruction);
        case 2:
            return get_template_2(instruction);
        case 3:
            return get_template_3(instruction);
        default: // shouldn't ever happen
            return VACANT;
    }
}

int main(int argc, char **argv) {

    // validate command line input
    if (argc != 2) {
        printf("usage: disassembler <assembly_file>\n");
        return 1;
    }

    //int fd = open("memset.ob", O_RDONLY);
    int fd = open(argv[1], O_RDONLY);
    
    // open file
    if (fd < 0) {
	    printf("open failed\n");
	    exit(1);
    }

    // TODO: replace hard coded start and end points.
    // consider searching for 4 bytes set to 0 and then for 4 bytes not set to 0 and
    // start reading there until we see 0 x 4 bytes again?
    lseek(fd, 0x68, SEEK_SET); // why 0x68?
    size_t n = 4 * 17;
    uint8_t buf[n];

    if (read(fd, buf, n) != n) {
	    printf("read failed\n");
	    exit(1);
    }

    uint8_t *p = buf;

    while (p < buf + n) {
        for (int i=3; i>=0; --i) {
            printf("%x ", p[i]);
        }
        printf("\n");
        switch (get_template(p)) {
            case A:
                printf("template A\n");
                print_a(p);
                p += 4;
                break;
            case B:
                printf("template B\n");
                print_b(p);
                p += 4;
                break;
            case C:
                printf("template C\n");
                print_c(p);
                p += 4;
                break;
            case D:
                printf("template D\n");
                print_d(p);
                p += 4;
                break;
            case A2:
                printf("template A2\n");
                p += 8;
                break;
            case B2:
                printf("template B2\n");
                p += 8;
                break;
            case C2:
                printf("template C2\n");
                p += 8;
                break;
            case E2:
                printf("template E2\n");
                p += 8;
                break;
            case A3:
                printf("template A3\n");
                p += 12;
                break;
            case B3:
                printf("template B3\n");
                p += 12;
                break;
            case E3:
                printf("template E3\n");
                p += 12;
                break;
            case JUMP_TODO:
                printf("jump template 1.6A or 1.7B\n");
                p += 4;
                break;
            default:
                printf("error\n");
                exit(1);
        }
        printf("\n");
    }


    return 0;
}
