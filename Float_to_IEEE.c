
/* ----------------------------------------------------------------------------------------------------
 *
 *  Created by Ulysses Carlos on 3/13/19 (Earlier versions were created the weekend before)
 *
 *  Program Amendments:
 *      Various adjustments made before 07/21/19
 *      07/21/19 -- Introduced commenting to make program easier to read
 *
 *  Program Usage:
 *      This program converts a floating-point number (whether single or double precision), and converts
 *      it to scientific notation (i.e M * 2^N). Additional information is shown such as
 *      Bit Representation, Sign Value, Exponent Value, Fractional Value, Classification and Bias value.
 *      Additional information is displayed to show Structure size.
 *
 *  To Do:
 *      I plan to incorporate this program as part of a GUI application written in C++.
 *
 *  Note to the Reader:
 *      This program is meant to be used in learning how a Floating-Point Number is represented
 *      in a computer. The Bit-masks have been tailored to isolate various parts of a Floating-point
 *      number, so please don't mess with them.
 *
 *      If you don't know how this program works, try to learn how to take decimal numbers and convert
 *      them to their floating-point equivalent(in binary first). This program such makes that process
 *      easier. As an reference, try this: https://en.wikipedia.org/wiki/Floating-point_arithmetic
 *
 *      As an example, try to take a number (say 12.25), and convert it by hand first by converting
 *      it to binary(using Fractional Binary numbers), and then convert the Fractional number into
 *      its Floating-Point version by finding its bias, etc.
 *
 * ----------------------------------------------------------------------------------------------------
 */

#include "common_header.h"

#include <math.h>
#include <stdbool.h>

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

#define CHAR_LINE ('-')
// Typedefs:
typedef unsigned char *byte_pointer;

// Struct definition
typedef struct float_number{
	unsigned long long int sign_val;
	unsigned long long int exponent_val;
	unsigned long long int fractional;
	unsigned long long int byte_rep;
	int weighed_bias;
	bool isDouble;

    //Union to separate float and double:
	union datatype_value{
		double double_value;
		float float_value;
	}value;
    long double signficand_val;

    /* Normalization Status
     * Status 0: Normalized Floating value (Exponent not 0 or 255(2047) )
     * Status 1: Denormalized Floating value (Exponent 0)
     * Status 2: Special_Case (Exponent 255(2047) )
     * Status 3: Byte Representation has been created, but Floating Point Number
     *           has not been seperated
     * Status 4: Initial Value
     * Status 5-7: NOT USED
     */
    struct{
        unsigned int value : 3;
    }Normalization_Status;

}Float_Number;

// Structure Functions Prototypes::
void Initialize_Floating_Number(struct float_number *fn);
void Create_Bit_Representation(struct float_number *fn);
void Separate_Floating_Number(struct float_number *fn);
void Print_Float_Number(struct float_number *fn);
void Generate_Sigificand_Number(struct float_number *fn);
void Print_Error_Message(unsigned int error_code);
void Print_Instructions(void);
void Center_Line(int start, int end);


// Helper Functions
int Test_Little_Endian(void);
void Reverse_Bit_Representation(char *string, int string_length);


int Test_Little_Endian(void){
    // Test whether machine is little or big endian:
    int val = 1;
    char *test = (char *) &val;

    return *test ? 1 : 0;

}

void Reverse_Bit_Representation(char *string, int string_length){
    // Little-Endian Machines have to have their bit representations
    // reversed in order to produce correct output.
    int limit = string_length / 2;
    int end_point = string_length;
    char temp1, temp2;
    for (int i = 0; i < limit; i += 2){
        end_point -= 2;

        temp1 = string[i];
        temp2 = string[i + 1];
        string[i] = string[end_point];
        string[i + 1] = string[end_point + 1];
        string[end_point] = temp1;
        string[end_point + 1] = temp2;

    }
}



void Print_Instructions(void){
	printf("This Program evaluates a Float (32-bit Floating-point) "
        "or a Double (64-bit Floating-point) value.\n");
	printf("If you plan on using a Float number, Please input 1. "
        "If you plan on using a Double number, "
        "Please input 2.\n");
	printf("If you don't know which to choose, input 2.\n");

}

#define WINDOW_SIZE (90)
void print_dash_line(void){
    for (int i = 0; i < WINDOW_SIZE; i++)
        putchar(CHAR_LINE);
    printf("\n");
};

void print_partial_line(int start, int size){
    Center_Line(0, start);
    if ((start + size) > WINDOW_SIZE) {
        Print_Error_Message(5);
        exit(EXIT_FAILURE);
    }
    int loop_end = (start + size);
    for (int i = start; i < loop_end; i++)
        putchar(CHAR_LINE);
    printf("\n");

}

void Print_Error_Message(unsigned int error_code){
    printf("Error: ");
    switch(error_code){
        case 2:
            printf("Cannot use a Special-Case "
                   "Floating-Point Number for this situation.\n");
            break;
        case 3:
            printf("The Floating-Point Case "
                   "for this number has not been generated.\n");
            break;

        case 4:
            printf("The Bit-level Representation "
                   "has yet to be created for this Floating-Point Number.\n");
            break;
        case 5:
            printf("Partial line is larger than window length (%d)\n", WINDOW_SIZE);
            break;
        default:
            puts("An Unknown error has occurred. Exiting.");
            break;
    }
}

//--------------------------------------------------------------------------------
//
// Structure Functions
//
//--------------------------------------------------------------------------------

void Initialize_Floating_Number(struct float_number *fn){
	float float_num;
	double double_num;
	int test;
    Print_Instructions();

	scanf("%d", &test);

	if (!(test == 1 || test == 2)){
	    system("clear");
		printf("Invalid Input. This program will now close.");
		sleep(1);
		exit(EXIT_FAILURE);
	}

	puts("Now please enter a number:");
	if (test == 1){
		scanf("%f", &float_num);
		fn->value.float_value = float_num;
		fn->isDouble = false;
	}
	else {
        scanf("%lf", &double_num);
        fn->value.double_value = double_num;
        fn->isDouble = true;
    };

	fn->Normalization_Status.value = 4;
    system("clear");
}

void Create_Bit_Representation(struct float_number *fn){

    int string_size = (fn->isDouble) ? DOUBLE_SIZE : FLOAT_SIZE;
    char *string = calloc(string_size, sizeof(char));
    byte_pointer float_address = (byte_pointer) &fn->value;

    char *endpoint;
    int i;
    char *sp = string;
    int loop_end = (fn->isDouble) ? DOUBLE_LEN : FLOAT_LEN;
    for (i = 0; i < loop_end; i++){
        sprintf((sp), "%.2x", float_address[i]);
        sp += 2;
    }

    int test_endian = Test_Little_Endian();

    if (test_endian) // Only necessary on machines that use little endian
        Reverse_Bit_Representation(string, (int)strlen(string));

    // Now read into fn:
    fn->byte_rep = (unsigned long long int) strtoll(string, &endpoint, 16);
    // Change Status:
    fn->Normalization_Status.value = 3;
    free(string);
}


void Separate_Floating_Number(struct float_number *fn){
    // Can only be used after Create_Bit_Representation()
    if (fn->Normalization_Status.value > 3){
        Print_Error_Message(4);
        return;
    }
    // Difference Masks are used depending on float/double
    // and are right-shifted depending on the length of the data type
    if (fn->isDouble){
        fn->sign_val = (fn->byte_rep & DOUBLE_SIGN_MASK) >> (DOUBLE_EXP_LEN + DOUBLE_FRAC_LEN);
        fn->exponent_val = (fn->byte_rep & DOUBLE_EXP_MASK) >> (DOUBLE_FRAC_LEN);
        fn->fractional = (fn->byte_rep & DOUBLE_FRAC_MASK);
    }
    else {
        // Convert to unsigned int to make sure the bit operations work for Float:
        unsigned int bit_rep = (unsigned int)fn->byte_rep;
        // Get Sign value:

        fn->sign_val = (bit_rep & FLOAT_SIGN_MASK) >> (FLOAT_EXP_LEN + FLOAT_FRAC_LEN);
        // Get Exponent Value:
        fn->exponent_val = (bit_rep & FLOAT_EXP_MASK) >> (FLOAT_FRAC_LEN);
        // Get Fraction Value
        fn->fractional = (bit_rep & FLOAT_FRAC_MASK);
    }

    //Generate Normalization Value:
    if (fn->isDouble)
        fn->Normalization_Status.value = (fn->exponent_val == DOUBLE_MAX_EXPONENT_VAL) ? 2 :
                                         (fn->exponent_val == 0) ? 1 : 0;
    else
        fn->Normalization_Status.value = (fn->exponent_val == FLOAT_MAX_EXPONENT_VAL) ? 2:
                                         (fn->exponent_val == 0) ? 1 : 0;

    //Generate Bias for Normalized and Denormalized values:
    if (fn->Normalization_Status.value != 2){
        int temp_bias = (fn -> isDouble) ? DOUBLE_EXPONENT_BIAS : FLOAT_EXPONENT_BIAS;
        fn->weighed_bias = (!fn->Normalization_Status.value) ? ((int) fn->exponent_val - temp_bias)
                : (1 - temp_bias);

/* Same as above:

        if (fn->isDouble)
           fn->weighed_bias = (!fn->Normalization_Status.value) ?
                   ((int) fn->exponent_val - DOUBLE_EXPONENT_BIAS)
                   : (1 - DOUBLE_EXPONENT_BIAS);
        else
            fn->weighed_bias = (!fn->Normalization_Status.value) ?
                    ((int)fn->exponent_val - FLOAT_EXPONENT_BIAS)
                    : (1 - FLOAT_EXPONENT_BIAS);
*/

    }

}

void Generate_Sigificand_Number(struct float_number *fn){
    if (fn->Normalization_Status.value >= 3){
        Print_Error_Message(3);
        return;
    }

    fn->signficand_val = (fn->isDouble) ? significandl(fn->value.double_value) :
            significandl(fn->value.float_value);
}

void Center_Line(int start, int end){
    for (int i = 0; i < (end - start) / 2; i++)
        printf(" "); // Adds white space until the center is reached
}

void Print_Float_Number(struct float_number *fn){
    if (fn->Normalization_Status.value >= 3){ // Considers case 3, 4, and 5, 6, 7(Not used)
        Print_Error_Message(3);
        return;
    }

    print_dash_line();
    if (fn->isDouble)
        printf("Initial Number: %.4f (Double Precision)\n", fn->value.double_value);
    else
        printf("Initial Number: %.4f (Single Precision)\n", fn->value.float_value);


    printf("Bit Representation (In Hexadecimal): 0x%llx\n", fn->byte_rep);
    printf("Sign Value: %llu", fn->sign_val);
    printf(!fn->sign_val? " (Positive)\n" : " (Negative)\n");
    printf("Exponent Value: %llu\n", fn->exponent_val);
    printf("Fractional Value (In Hexadecimal): 0x%llx\n", fn->fractional);

    printf("Floating-Point Classification: ");
    if (fn->Normalization_Status.value == 2)
        printf("Special Case (Exponent field is all ones)\n");
    else if (fn->Normalization_Status.value == 1)
        printf("Denormalized(Exponent field is all zeros)\n");
    else
        printf("Normalized\n");

    if (fn->Normalization_Status.value != 2){
        printf("Weighed Bias value : %d\n", fn->weighed_bias);
        printf("So in M * 2^E form, the number ");

        if (fn->isDouble)
            printf(" %.4f is represented as ", fn->value.double_value);
        else
            printf(" %.4f is represented as ", fn->value.float_value);
        printf("\n\n");

        // Center number:
        char *mantissa_string = calloc(WINDOW_SIZE / 2, sizeof(char));
        char *exponent_string = calloc(WINDOW_SIZE / 2, sizeof(char));
        sprintf(mantissa_string, "%.24Lf", fn->signficand_val);
        sprintf(exponent_string, "%d", fn->weighed_bias);
        int exponent_space = 5; // Accounts for 5-digit exponent values(i.e 2^0 - 2^99999), adjust if necessary
        int center_val = (strlen(mantissa_string) + exponent_space + strlen(exponent_string));

        print_partial_line(WINDOW_SIZE - (center_val), center_val);
        Center_Line(0, WINDOW_SIZE - (center_val));

        printf("%s", mantissa_string);
        printf(" * 2^%s\n", exponent_string);
        print_partial_line(WINDOW_SIZE - center_val, center_val);
        puts("");
    }
    
    printf("When multiplied, the result is %.24Lf\n", fn->signficand_val * pow(2, fn->weighed_bias));
    print_dash_line();

}

void Additional_Info(void){
    puts("Additional Information:");
    printf("Structure size: %d bytes\n", (int)sizeof(struct float_number));
    print_dash_line();
}

void compute(void){
    struct float_number *fn = calloc(1, sizeof(struct float_number));
    Initialize_Floating_Number(fn);
    Create_Bit_Representation(fn);
    Separate_Floating_Number(fn);
    Generate_Sigificand_Number(fn);
    Print_Float_Number(fn);
    Additional_Info();
    free(fn);


}	


int main(void){
    compute();
    return 0;
}

