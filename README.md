# Float-to-IEEE
Float-to-IEEE is a small program that allows you to take a number 
with Single Precision (a "float") or Double Precision (a "double")
and separates it into its Significand, Sign, Exponent and Mantissa 
components.

### Usage
```bash
./Float_to_IEEE
```
You may also run the program with command line arguments:
```bash
./Float_to_IEEE [Precision] [Number]
```
Where [Precision] can be the following values:

* float
* double

### Command-Line Example
```bash
./Float_to_IEEE float 4096.12
```
Entering the above will produce the following output:
```bash

------------------------------------------------------------------------------------------
Floating Point Information:
Initial Number: 4096.1201 (Single Precision)
Bit Representation (In Hexadecimal): 0x458000f6
Sign Value: 0 (Positive)
Exponent Value: 139
Fractional Value (In Hexadecimal): 0xf6
Floating-Point Classification: Normalized
Weighed Bias value : 12
So in M * 2^E form, the number  4096.1201 is represented as 

                            ---------------------------------
                            1.000029325485229492187500 * 2^12
                            ---------------------------------
When multiplied, the result is 4096.120117187500000000000000
------------------------------------------------------------------------------------------

```