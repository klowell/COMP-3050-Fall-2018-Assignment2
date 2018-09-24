
/**********************************************************************************************************
File Name: Assignment2.c
Name: Kristopher Lowell
Date: 9/17/2018
Purpose: Accept two floating point numbers from the user and emulate the addition of the two floating point
numbers to then compare to the hardware's output.
Edits: 9/17/2018 - Editted and augmented "COMP3050_Assignment2_template.txt" to fulfill assignment details - KCL
**********************************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

typedef union float_32 {
	float   floating_value_in_32_bits;
	int     arg_32;
	struct  sign_exp_mantissa {
		unsigned  mantissa : 23;
		unsigned  exponent : 8;
		unsigned      sign : 1;
	} f_bits;
	struct single_bits {
		unsigned  b0 : 1;
		unsigned  b1 : 1;
		unsigned  b2 : 1;
		unsigned  b3 : 1;
		unsigned  b4 : 1;
		unsigned  b5 : 1;
		unsigned  b6 : 1;
		unsigned  b7 : 1;
		unsigned  b8 : 1;
		unsigned  b9 : 1;
		unsigned  b10 : 1;
		unsigned  b11 : 1;
		unsigned  b12 : 1;
		unsigned  b13 : 1;
		unsigned  b14 : 1;
		unsigned  b15 : 1;
		unsigned  b16 : 1;
		unsigned  b17 : 1;
		unsigned  b18 : 1;
		unsigned  b19 : 1;
		unsigned  b20 : 1;
		unsigned  b21 : 1;
		unsigned  b22 : 1;
		unsigned  b23 : 1;
		unsigned  b24 : 1;
		unsigned  b25 : 1;
		unsigned  b26 : 1;
		unsigned  b27 : 1;
		unsigned  b28 : 1;
		unsigned  b29 : 1;
		unsigned  b30 : 1;
		unsigned  b31 : 1;
	}bit;
} FLOAT_UN;

void print_bits(FLOAT_UN, char *text);		// Prints the floating point number in an easy to read format (in binary)
void print_helper(int helper);				// Prints an helper integer variable in an easy to read format (in binary)

void clear_keyboard_buffer(void);			// Clears the input buffer in the case the user mistakenly typed incorrectly

int main(int argc, char *argv[])
{
	FLOAT_UN float_32_s1, float_32_s2, float_32_rslt;

	float hardware_result;										// Hardware result variable
	int	mant_s1, mant_s2, mant_res, exp_s1, exp_s2, exp_res;	// Helper variables
	int	shift_count;											// How much to shift the lower value

	printf("Please enter two positive floating point values each with:\n");		// Prompt user to enter two positive floating point numbers
	printf("- no more than 6 significant digits\n");
	printf("- a value between + 10**37 and 10 **-37\n");

	printf("\n");												// Prompt user to enter first floating point number
	printf("Enter Float 1: ");
	scanf("%g", &float_32_s1.floating_value_in_32_bits);		// Accept first floating point number from user
	clear_keyboard_buffer();

	printf("Enter Float 2: ");									// Prompt user to enter second floating point number
	scanf("%g", &float_32_s2.floating_value_in_32_bits);		// Accept second floating point number from user
	clear_keyboard_buffer();

	hardware_result = float_32_s1.floating_value_in_32_bits + float_32_s2.floating_value_in_32_bits;	// Calculate the hardware's addition result

	printf("\n");
	print_bits(float_32_s1, "Original pattern of Float 1: ");
	print_bits(float_32_s2, "Original pattern of Float 2: ");

	printf("\n");
	printf("COPY MANTISSA AND EXPONENT BITS INTO HELPER INT VARIABLES:\n");

	mant_s1 = float_32_s1.f_bits.mantissa;						// Copy bits from entered floating point numbers into helper integer variables
	mant_s2 = float_32_s2.f_bits.mantissa;
	exp_s1 = float_32_s1.f_bits.exponent;
	exp_s2 = float_32_s2.f_bits.exponent;

	printf("Float 1:\n");
	printf("Exponent Bits: ");
	print_helper(exp_s1);
	printf("Mantissa Bits: ");
	print_helper(mant_s1);

	printf("Float 2:\n");
	printf("Exponent Bits: ");
	print_helper(exp_s2);
	printf("Mantissa Bits: ");
	print_helper(mant_s2);

	printf("\n");
	printf("SLAM HIDDEN BIT ONTO HELPER INT VARIABLES:\n");		// Slamming hidden bit onto helpers
																// Or-ing the 24th bit will add the hidden bit if the exponent is not zero
	if (exp_s1 > 0 && exp_s1 < 255)					// 255 = 1111 1111
		mant_s1 = mant_s1 | 8388608;				// 8388608 = 0000 0000 1000 0000 0000 0000 0000 00000

	printf("Float 1 Mantissa Bits: ");				
	print_helper(mant_s1);

	if (exp_s2 > 0 && exp_s2 < 255)					// 255 = 1111 1111
		mant_s2 = mant_s2 | 8388608;				// 8388608 = 0000 0000 1000 0000 0000 0000 0000 00000

	printf("Float 2 Mantissa Bits: ");
	print_helper(mant_s2);

	printf("\n");
	printf("CALCULATE HOW MUCH TO SHIFT LOWER VALUES:\n");
	shift_count = exp_s1 - exp_s2;					// Calculating how much to shift the lower value using the exponent values

	if (shift_count < 0)							// If this number ends up negative, then the first float must have a lower exponent value
	{												// Thus, the first float must be shifted and the result adopts the second's exponent value
		shift_count = 0 - shift_count;

		printf("Float 1 must be shifted %d bits.\n", shift_count);
		printf("Result adopts Float 2's exponent.\n");

		for (int i = 0; i < shift_count; i++)		// Ensuring that 0's are being shifted into the helper integer.
		{
			mant_s1 = mant_s1 >> 1;
			mant_s1 = mant_s1 & 16777215;			// 16777215 = 0000 0000 1111 1111 1111 1111 1111 1111
		}

		exp_res = exp_s2;
		float_32_rslt.f_bits.exponent = exp_s2;
	}
	else											// If this number ends up negative, then the first float must have a lower exponent value
	{												// Thus, the second float must be shifted and the result adopts the first's exponent value
		printf("Float 2 must be shifted %d bits.\n", shift_count);
		printf("Result adopts Float 1's exponent.\n");

		for (int i = 0; i < shift_count; i++)		// Ensuring that 0's are being shifted into the helper integer.
		{
			mant_s2 = mant_s2 >> 1;
			mant_s2 = mant_s2 & 16777215;			// 16777215 = 0000 0000 1111 1111 1111 1111 1111 1111
		}

		exp_res = exp_s1;
		float_32_rslt.f_bits.exponent = exp_s1;
	}

	printf("\n");
	printf("SHIFT MANTISSA BITS OF LOWER VALUE:\n");

	printf("Float 1 Mantissa Bits: ");
	print_helper(mant_s1);

	printf("Float 2 Mantissa Bits: ");
	print_helper(mant_s2);

	printf("\n");
	printf("INTEGER ADD MANTISSA HELPERS:\n");

	mant_res = mant_s1 + mant_s2;					// Simple integer addition can be used to calculate the result's mantissa bits
													// since only positive numbers are accepted for this assignment.
	printf("Result Exponent Bits: ");
	print_helper(exp_res);

	printf("Result Mantissa Bits: ");
	print_helper(mant_res);

	printf("\n");									
	printf("CHECK AND ACCOUNT FOR OVERFLOW IN MANTISSA BITS:\n");
	while (mant_res > 16777216)						// 16777216 = 0000 0001 0000 0000 0000 0000 0000 0000
	{
		mant_res = mant_res >> 1;					// However, if the mantissa bits overflowed into the 25th bit or higher,
		float_32_rslt.f_bits.exponent++;			// then shift the mantissa bits by one and increment the exponent by one
		exp_res++;									// until no more overflow has occured

		if (float_32_rslt.f_bits.exponent == 255)	// 255 = 1111 11111
			mant_res = 0;							// In the case of a max exponent and not create a "NaN" value, set mantissa to zero
	}												// to label result as "inf"

	printf("Result Exponent Bits: ");
	print_helper(exp_res);

	printf("Result Mantissa Bits: ");
	print_helper(mant_res);

	printf("\n");
	printf("REMOVE HIDDEN BIT:\n");
													// And-ing with the number with all 1's in 1st to 23rd bit remove any bits 24th to 32nd
	mant_res = mant_res & 8388607;					// 8388607 = 0000 0000 0111 1111 1111 1111 1111 1111

	printf("Result Exponent Bits: ");
	print_helper(exp_res);

	printf("Result Mantissa Bits: ");
	print_helper(mant_res);

	float_32_rslt.f_bits.mantissa = mant_res;
	float_32_rslt.f_bits.sign = 0;

	printf("\n");
	print_bits(float_32_rslt, "Bit pattern of result: ");

	printf("\n");									// Print the value calculated by emulation process, as well as the hardware's result
	printf("EMULATED FLOATING RESULT FROM PRINTF ==>>> %g\n", float_32_rslt.floating_value_in_32_bits);
	printf("HARDWARE FLOATING RESULT FROM PRINTF ==>>> %g\n", hardware_result);

	return 0;
}

void print_bits(FLOAT_UN float_32, char *text)
{
	char bit_string[41];							// Create a string for printing bits contained in a 32-bit floating point number

	for (int i = 0; i < 41; i++)					// Fill the string with spaces
		bit_string[i] = ' ';

	bit_string[40] = '\0';							// End string with null character

	bit_string[0] = float_32.bit.b31 ? '1' : '0';	// 1 Sign bit

	bit_string[2] = float_32.bit.b30 ? '1' : '0';	// 8 Exponent bits
	bit_string[3] = float_32.bit.b29 ? '1' : '0';
	bit_string[4] = float_32.bit.b28 ? '1' : '0';
	bit_string[5] = float_32.bit.b27 ? '1' : '0';

	bit_string[7] = float_32.bit.b26 ? '1' : '0';
	bit_string[8] = float_32.bit.b25 ? '1' : '0';
	bit_string[9] = float_32.bit.b24 ? '1' : '0';
	bit_string[10] = float_32.bit.b23 ? '1' : '0';

	bit_string[12] = float_32.bit.b22 ? '1' : '0';	// 23 Mantissa bits
	bit_string[13] = float_32.bit.b21 ? '1' : '0';
	bit_string[14] = float_32.bit.b20 ? '1' : '0';

	bit_string[16] = float_32.bit.b19 ? '1' : '0';
	bit_string[17] = float_32.bit.b18 ? '1' : '0';
	bit_string[18] = float_32.bit.b17 ? '1' : '0';
	bit_string[19] = float_32.bit.b16 ? '1' : '0';

	bit_string[21] = float_32.bit.b15 ? '1' : '0';
	bit_string[22] = float_32.bit.b14 ? '1' : '0';
	bit_string[23] = float_32.bit.b13 ? '1' : '0';
	bit_string[24] = float_32.bit.b12 ? '1' : '0';

	bit_string[26] = float_32.bit.b11 ? '1' : '0';
	bit_string[27] = float_32.bit.b10 ? '1' : '0';
	bit_string[28] = float_32.bit.b9 ? '1' : '0';
	bit_string[29] = float_32.bit.b8 ? '1' : '0';

	bit_string[31] = float_32.bit.b7 ? '1' : '0';
	bit_string[32] = float_32.bit.b6 ? '1' : '0';
	bit_string[33] = float_32.bit.b5 ? '1' : '0';
	bit_string[34] = float_32.bit.b4 ? '1' : '0';

	bit_string[36] = float_32.bit.b3 ? '1' : '0';
	bit_string[37] = float_32.bit.b2 ? '1' : '0';
	bit_string[38] = float_32.bit.b1 ? '1' : '0';
	bit_string[39] = float_32.bit.b0 ? '1' : '0';

	printf("%23s  %s\n", text, bit_string);			// Print the text given to the function as well as the floating point number
}

void print_helper(int helper)
{
	int temp;							// Variable for holding current first bit in integer value
	char bit_string[40];				// Create a string for printing bits contained in a 32-bit integer

	for (int i = 0; i < 39; i++)		// Fill the string with spaces
		bit_string[i] = ' ';

	bit_string[39] = '\0';				// End the string with a null character

	for (int i = 7; i >= 0; i--)		
	{
		for (int j = 3; j >= 0; j--)	// Start filling string at end for proper printing format
		{
			temp = helper & 1 ? '1' : '0';	// Set temp variable with current first bit
			bit_string[(5 * i) + j] = temp;	// Insert this character value into the string
			helper >>= 1;					// Shift the integer by one to obtain new first bit
		}
	}
	printf("%s\n", bit_string);			// Print the integer in binary in an easier format to read
}

void clear_keyboard_buffer(void)
{
	char ch = ' ';						// Clears buffer scanning input line until it finds a new line character
	while (ch != '\n')
		scanf("%c", &ch);
}