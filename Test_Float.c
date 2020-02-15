//--------------------------------------------------------------------------------------------------
// Created by Ulysses Carlos on 10/19/19 using Ulysses-H270-HD3.
//
// Separated Testing from Float_to_IEEE.h
//--------------------------------------------------------------------------------------------------

#include "Float_to_IEEE.h"
const int max_length = 100;
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

void compute_with_value(char *string, double value) {
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
    puts ("You can omit the [PRECISION] to evaluate a number with double precision.");


}

bool is_valid_number(char *string){
    int dot_count = 0;

    unsigned length = strlen(string);
    if (length < 2)
        Error(1);
    // Remove any newline/control characters from the string
    // if there are any
    if (iscntrl(string[length - 1]))
        string[length - 1] = '\0';

    if (string[0] == '-' || string[0] == '+') // Sign
        string++;

    for (char *p = string; *p; p++) {
        if (*(p) == '.') {
            dot_count++;
            if (dot_count > 1)
                return false;
        }
        else if (!isdigit(*p))
            return false;
    }
    return true;
}

void test_valid_number(char *string){
    bool check = is_valid_number(string);
    if (!check){
        fprintf(stderr, "Error: ");
        fputs(string, stderr);
        fprintf(stderr, " is not a valid number.\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    // User passed only ./Float_to_IEEE as argument
    // Consider a argc 2 case, default to double
    if (argc == 1)
        compute();
    else if (argc == 2){
        char *precision = "double";
        test_valid_number(argv[1]);

        char *c_pointer = NULL;
        double value = strtod(argv[1], &c_pointer);

        compute_with_value(precision, value);

    }
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
            fprintf(stderr, "Error: \"");
            fputs(precision, stderr);
            fprintf(stderr,"\" is a invalid precision option.\n");
            exit(EXIT_FAILURE);
        }


        char *temp;
        test_valid_number(argv[2]);

        double value = strtod(argv[2], &temp);

        compute_with_value(precision, value);
        free(precision);
    }

    return 0;
}


