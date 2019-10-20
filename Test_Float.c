//--------------------------------------------------------------------------------------------------
// Created by Ulysses Carlos on 10/19/19 using Ulysses-H270-HD3.
//
// Separated Testing from Float_to_IEEE.h
//--------------------------------------------------------------------------------------------------

#include "Float_to_IEE.h"
const int max_length = 100;

void test_edit(void){
    struct float_number *fn = calloc(1, sizeof(struct float_number));
    char string[max_length];
    strncpy(string, "FLOAT", sizeof("FLOAT"));

    printf("\nTesting Num edit with 260.21(Float)\n");
    Edit_Floating_Number(fn, string, 260.21);
    Print_Float_Number(fn);
    strncpy(string, "DOUBLE", strlen("DOUBLE"));
    printf("\nTesting Num edit with 260.21(Double)\n");
    Edit_Floating_Number(fn, string, 260.21);
    Print_Float_Number(fn);

    free(fn);
}

void compute(void){
    struct float_number *fn = calloc(1, sizeof(struct float_number));

    Initialize_Floating_Number(fn);
    Create_Bit_Representation(fn);
    Separate_Floating_Number(fn);
    Generate_Significand_Number(fn);
    Print_Float_Number(fn);
    Additional_Info();
    free(fn);

}

void compute_with_value(char *string, int length, double value){
    struct float_number *fn = calloc(1, sizeof(struct float_number));

    char temp[max_length];
    strncpy(temp, string, strlen(string));
    Edit_Floating_Number(fn, temp, value);
    Print_Float_Number(fn);
    Additional_Info();

    free(fn);

}

void command_instructions(void){
    puts("Usage: ./Float_to_IEEE [PRECISION] [Number]");
    puts("[PRECISION] may be one of the following options:");
    puts("float -- Single Precision");
    puts("double -- Double Precision");


}

int main(int argc, char *argv[]){
    // User passed only ./Float_to_IEEE as argument
    if (argc == 1)
        compute();
    else{
        // Usage: ./Float_to_IEEE, float/double [Number]
        // First make sure that argc == 3, if not exit.
        if (argc != 3){
            command_instructions();
            exit(EXIT_FAILURE);
        }
        char *precision = calloc(max_length, sizeof(char));
        int precision_len  = sizeof(argv[1]);
        strncpy(precision, argv[1], precision_len);

        bool check = (!strncmp(precision, "double", precision_len)
                    || !strncmp(precision, "float", precision_len));

        if (!check){
            printf("Error: \"");
            fputs(precision, stdout);
            printf("\" is a invalid precision option.\n");
            exit(EXIT_FAILURE);
        }

        char *temp;
        double value = strtod(argv[2], &temp);

        compute_with_value(precision, precision_len, value);
    }

    return 0;
}


