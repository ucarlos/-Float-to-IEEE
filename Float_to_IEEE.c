
/* ----------------------------------------------------------------------------------------------------
 *
 *  Created by Ulysses Carlos on 3/13/19 (Earlier versions were created the weekend before at a
 *  friend's house.)
 *
 *  Program Amendments:
 *      Various adjustments made before 07/21/19
 *      07/21/19 -- Introduced commenting to make program easier to read
 *      09/14/19 -- Replaced the Bit-field Normalization_Status with enum
 *                  Changed Print_Floating_Number to display Special Case Floating-Point Numbers.
 *                  Changed unsigned long long ints to uint64 for portability
 *     10/19/19  -- Added Edit_Floating_Number to change value, moved macros, structure and function definitions
 *                  to Float_to_IEE.h
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
 *      The idea behind using a byte pointer came from Randall Bryant's Computer Systems (3rd Edition)
 *      so, I give credit to him.
 *
 *  Additional Information:
 *      Bitmasks used:
 *          #define FLOAT_SIGN_MASK (0x80000000)
 *          #define FLOAT_EXP_MASK (0x7f800000)
 *          #define FLOAT_FRAC_MASK (0x007fffff)
 *          #define DOUBLE_SIGN_MASK (0x8000000000000000)
 *          #define DOUBLE_EXP_MASK (0x7ff0000000000000)
 *          #define DOUBLE_FRAC_MASK (0x000fffffffffffff)
 *
 * ----------------------------------------------------------------------------------------------------
 */

#include "Float_to_IEE.h"


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


void String_to_Lower(char *string){
    char temp;
    for (char *p = string; (*p); p++) {
        temp = tolower(*p);
        *p = temp;
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


void print_dash_line(void){
    for (int i = 0; i < WINDOW_SIZE; i++)
        putchar(CHAR_LINE);
    printf("\n");
};

void print_partial_line(int start, int size){
    Center_Line(0, start);
    if ((start + size) > WINDOW_SIZE) {
        Error(5);
    }

    int loop_end = (start + size);
    for (int i = start; i < loop_end; i++)
        putchar(CHAR_LINE);
    printf("\n");

}


void Error(unsigned int error_code){
    Print_Error_Message(error_code);
    exit(EXIT_FAILURE);
}

void Print_Error_Message(unsigned int error_code){
    printf("Error: ");
    switch(error_code){
        case 1:
            printf("Invalid Input.");
            break;
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
        case 6:
            printf("Edit_Floating_Number must have a string of \"double\" or \"float\" "
                   "as a parameter.\n");
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
		puts("Invalid Input. This program will now close.");
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

	fn->norm_status = Initialized;
    system("clear");
}

void Create_Bit_Representation(struct float_number *fn){
    // There should be a test here to prevent an uninitialized struct here
    // But I can't think up of one right now.
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
    fn->byte_rep = (uint64_t) strtoll(string, &endpoint, 16);
    // Change Status:
    fn->norm_status = Not_Separated;
    free(string);
}


void Separate_Floating_Number(struct float_number *fn){
    // Can only be used after Create_Bit_Representation()
    if (fn->norm_status < Not_Separated){
        Error(4);
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
    uint64_t test_exponent_val = (fn->isDouble) ? DOUBLE_MAX_EXPONENT_VAL : FLOAT_MAX_EXPONENT_VAL;
    fn->float_status = (fn->exponent_val == test_exponent_val) ? Special_Case :
            (!fn->exponent_val)? Denormalized : Normalized;


    //Generate Bias for Normalized and Denormalized values:
    if (fn->float_status != Special_Case){
        int temp_bias = (fn -> isDouble) ? DOUBLE_EXPONENT_BIAS : FLOAT_EXPONENT_BIAS;
        fn->weighed_bias = (!fn->float_status) ? ((int) fn->exponent_val - temp_bias)
                : (1 - temp_bias);


    }
    fn->norm_status = Separated;

}

void Generate_Significand_Number(struct float_number *fn){
    if (fn->norm_status != Separated){
        Error(3);
    }

    fn->signficand_val = (fn->isDouble) ? significandl(fn->value.double_value) :
            significandl(fn->value.float_value);
}

void Center_Line(int start, int end){
    for (int i = 0; i < (end - start) / 2; i++)
        printf(" "); // Adds white space until the center is reached
}

void Print_General_Float_Info(struct float_number *fn){
    if (fn->isDouble)
        printf("Initial Number: %.4f (Double Precision)\n", fn->value.double_value);
    else
        printf("Initial Number: %.4f (Single Precision)\n", fn->value.float_value);


    printf("Bit Representation (In Hexadecimal): 0x%" PRIx64 "\n", fn->byte_rep);
    printf("Sign Value: %" PRIu64, fn->sign_val);
    printf(!(fn->sign_val)? " (Positive)\n" : " (Negative)\n");
    printf("Exponent Value: %" PRIu64 "\n", fn->exponent_val);
    printf("Fractional Value (In Hexadecimal): 0x%" PRIx64 "\n", fn->fractional);

    printf("Floating-Point Classification: ");

    if (fn->float_status == Special_Case)
        printf("Special Case (Exponent field is all ones)\n");
    else if (fn->float_status == Denormalized)
        printf("Denormalized(Exponent field is all zeros)\n");
    else
        printf("Normalized\n");

}

void Print_Float_Number(struct float_number *fn){
    if (fn->norm_status != Separated){
        Error(3);
    }

    print_dash_line();
    Print_General_Float_Info(fn);
    // For Normalized and Denormalized Values:
    if (fn->float_status != Special_Case){
        printf("Weighed Bias value : %d\n", fn->weighed_bias);
        printf("So in M * 2^E form, the number ");

        if (fn->isDouble)
            printf(" %.4f is represented as ", fn->value.double_value);
        else
            printf(" %.4f is represented as ", fn->value.float_value);
        printf("\n\n");
        // Center number:
	    Center_Float_Number(fn);
    	printf("When multiplied, the result is %.24Lf\n", fn->signficand_val * pow(2, fn->weighed_bias));
    }
    else{ 
	    printf("Special Case Value: ");
	    if (!(fn->fractional))
	        printf(!(fn->sign_val)? "Positive Infinity\n" : "Negative Infinity\n");
	    else
	        printf("Not A Number (NaN)\n");
    }

    print_dash_line();

}

void Center_Float_Number(struct float_number *fn){
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

void Edit_Floating_Number(struct float_number *fn, char string[], double new_value){
    // First, make string lowercase.
    String_to_Lower(string);
    int length  = strlen(string);

    // If String is not double or float, exit.
    if (!(!strncmp(string, "double", length) || !strncmp(string, "float", length))){
        Error(6);
    }

    if (!(strncmp(string, "double", length))) {
        fn->isDouble = true;
        fn->value.double_value = new_value;
    }
    else{
        fn->isDouble = false;
        fn->value.float_value = (float)new_value;
    }

    //Regardless of value, recreate bit number.
    Create_Bit_Representation(fn);
    Separate_Floating_Number(fn);
    Generate_Significand_Number(fn);


}

void Additional_Info(void){
    puts("Additional Information:");
    printf("Structure size: %d bytes\n", (int)sizeof(struct float_number));
    print_dash_line();
}


