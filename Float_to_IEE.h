
#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H
//Include Standard Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>


// Size Definitions
#define FLOAT_SIZE (sizeof(float) * 8)
#define FLOAT_LEN (sizeof(float))
#define FLOAT_EXPONENT_BIAS (((1 << 7) - 1))
#define FLOAT_EXP_LEN (8)
#define FLOAT_FRAC_LEN (23)
#define FLOAT_MAX_EXPONENT_VAL ((1 << 8) - 1)

#define DOUBLE_SIZE (2 * FLOAT_SIZE)
#define DOUBLE_LEN (sizeof(double))
#define DOUBLE_EXPONENT_BIAS (((1 << 10) - 1))
#define DOUBLE_EXP_LEN (11)
#define DOUBLE_FRAC_LEN (52)
#define DOUBLE_MAX_EXPONENT_VAL ((1 << 11) - 1)

// Bit Masks:
#define FLOAT_SIGN_MASK (0x80000000)
#define FLOAT_EXP_MASK (0x7f800000)
#define FLOAT_FRAC_MASK (0x007fffff)
#define DOUBLE_SIGN_MASK (0x8000000000000000)
#define DOUBLE_EXP_MASK (0x7ff0000000000000)
#define DOUBLE_FRAC_MASK (0x000fffffffffffff)


// For Terminal Screen:
#define CHAR_LINE ('-')
#define WINDOW_SIZE (80)

// Typedefs:
typedef unsigned char *byte_pointer;


/*
 * Enum Status:
 * Normalized   Floating Value (Exponent not 0 or 255(2047 for Double))
 * Denormalized Floating Value (Exponent is 0)
 * Special Case Floating Value (Exponent is 255 or 2047))
 *
*/
enum Floating_Number_Status{
    Normalized = 0, Denormalized, Special_Case
};

/*
 * Normalization Status:
 * Initialized: Initial value
 * Not_Separated: Byte Representation has been created, but Number has not
 *                been separated.
 * Separated: Number is ready to be classified.
*/

enum Normalization_Status{
    Initialized = 0, Not_Separated, Separated
};

// Struct definition
struct float_number{
    uint64_t sign_val;
    uint64_t exponent_val;
    uint64_t fractional;
    uint64_t byte_rep;

    int32_t weighed_bias;
    bool isDouble;

    //Union to separate float and double:
    union datatype_value{
        double double_value;
        float float_value;
    }value;
    long double signficand_val;

    enum Floating_Number_Status float_status;
    enum Normalization_Status norm_status;
};

// Structure Functions Prototypes::
void Initialize_Floating_Number(struct float_number *fn);
void Create_Bit_Representation(struct float_number *fn);
void Separate_Floating_Number(struct float_number *fn);
void Print_Float_Number(struct float_number *fn);
void Generate_Significand_Number(struct float_number *fn);
void Error(unsigned int error_code);
void Print_Error_Message(unsigned int error_code);
void Print_Instructions(void);
void Additional_Info(void);
void Center_Line(int start, int end);
void Center_Float_Number(struct float_number *fn);
void Edit_Floating_Number(struct float_number *fn, char *string, double new_value);

// Helper Functions
int Test_Little_Endian(void);
void Reverse_Bit_Representation(char *string, int string_length);
void String_to_Lower(char *string);

// Line Drawing Functions:
void print_dash_line(void);
void print_partial_line(int start, unsigned int size);
bool is_valid_number(char *string);
void test_valid_number(char *string);

// Constants
inline static clear_buffer(void){
	while (getchar() != '\n')
        continue;
}
#endif
