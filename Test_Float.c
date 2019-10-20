//--------------------------------------------------------------------------------------------------
// Created by Ulysses Carlos on 10/19/19 using Ulysses-H270-HD3.
//
// This is just a Driver Program; Test anything you want here.
//--------------------------------------------------------------------------------------------------

#include "Float_to_IEE.h"

void test_edit(void){
    struct float_number *fn = calloc(1, sizeof(struct float_number));
    int max_length = 10;
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

int main(void){
    compute();
    return 0;
}


