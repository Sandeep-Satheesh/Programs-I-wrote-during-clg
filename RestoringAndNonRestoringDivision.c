#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

/*This is a program to display to the user, the step-by-step process of how restoring and non-restoring division is performed.
It is quite inefficient in the sense that it uses strings to represent binary numbers, but it made the job of displaying them easier. :P
The user will be asked to choose whether he wants to divide two numbers using the restoring (R) or the non-restoring (N) method at the start of the program.

The program will then call the appropriate functions to perform the selected operation.

Assumptions:
    1. Both the dividend and the divisor are positive integers.
    2. The divisor is not equal to 0.
*/

//This is a generic routine to terminate the program "gracefully" with an error message that there was a fatal runtime error.
void fatalError() {
	printf("\nA FATAL ERROR HAS OCCURRED! STOPPING PROGRAM...");
	exit(0);
}

//Function to reverse a given string. Used to reverse the bit string obtained as output by performing repeated division by 2 in the convertToBinArray() function.
void rev(char s[]) {
	int i, t, x = strlen(s);
	//Swap the elements on either half-side of the string (left side -> right side, and vice-versa.
	for (i = 0; i < x / 2; i++) {
		t = s[i];
		s[i] = s[x - i - 1];
		s[x - i - 1] = t;
	}
}

//Function to convert an integer into a bit string of 1s and 0s.
// x => The integer to be converted into binary.
//sgnFlag => A flag value depicting whether we want to explicitly set the MSB of the bit-string to 0 or 1 (In the algorithms, a 1 in the MSB means we're dealing with a -ve number).
char *convertToBinArray(int x, int sgnFlag) {
	char *bin = (char *)malloc(sizeof(char) * 10);
	int t = 0;
	if (!bin) fatalError(); //Terminate the program as there was a memory allocation failure.
	while (x) { //While we still don't reach x = 0...
		bin[t++] = '0' + (x % 2); //Perform x%2 and convert it to its ASCII value by adding the value of '0' (in ASCII) to it, so that it becomes a readable string.
		x /= 2; //Performing repeated division by 2.
	}
	if (sgnFlag) bin[t++] = '0'; //Append a 0 to the end of the bit string, if required. This, when reversed by the rev() function, will appear as the MSB of the final converted bit string.
	bin[t] = 0; //Add a trailing '\0' (ASCII Code 0) to make it a null-terminated string.

	rev(bin);

	return bin; //Return ptr to the converted final string.
}

//Function to add two bit strings s1 and s2, and store the result in s1.
//Assumption: s1 is a larger string than s2.
void add(char s1[], char s2[]) {
	int carry = 0, i, j, l1 = strlen(s1), l2 = strlen(s2), a, b;
    /*Logic:
        Perform addition from the end of both the strings, working our way to the MSB (just like how we perform normal addition).
        Call the two bits we're currently adding, as x and y.
        Two main cases arise:
            1. When we don't have a carry-bit to be added:
                a. If either x or y is zero (or both are zero), result is a OR b.
                b. Else, both are 1. Then, set result = 0 and carry = 1. (1 + 1 = 10 in binary).
            2. When we have a carry-bit:
                a. When both are zero, set result = 1, carry = 0.
                b. Else if either is zero, set result = 0, and do not alter carry (carry is propagated to next step of addition).
                c. If both are 1, set result = 1 and propagate the carry.
        After the smaller number's MSB has been reached, we just have to add the carry (if any) with the remaining bits of the larger number until we reach its MSB.
    */
	for (j = l2 - 1, i = l1 - 1; i >= 0 && j >= 0; i--, j--) {
		a = s1[i];
		b = s2[j];
		if (carry == 0) {
			if (a == '0' || b == '0')
				s1[i] = a + b - '0'; //'0' is subtracted since it is added twice into the string (a = bit + ASCII '0', b = bit + ASCII '0').
			else {
				s1[i] = '0';
				carry = 1;
			}
		}
		else {
			if (a == '0' && b == '0') {
				s1[i] = '1';
				carry = 0;
			}
			else if (a == '0' || b == '0')
				s1[i] = '0';
			else
				s1[i] = '1';
		}
	}
	while (carry != 0 && i >= 0) {
		if (s1[i] == '0') {
			s1[i] = '1';
			carry = 0;
		}
		else
			s1[i] = '0';
		i--;
	}
}

//Function to "normalize" a bit string by extending the sign bit of the bit-string n places more.
//Creates a new string, and destroys the older one.
void normalize(char **bin, int n) {
	if (!n)
		return;
	int x, t = strlen(*bin);
	char sgn = **bin, *p = *bin;
	char *temp = (char *)malloc(sizeof(char)*(t+n+2)); //2bytes extra for tolerance :P

	if (!temp) fatalError();

	for (x = 0; x < n; x++)
		temp[x] = sgn; //sign extension to n bits.
	for (x = n; x < t+n; x++)
		temp[x] = p[x-n]; //copy the rest of the bits of the string into the resultant string
    temp[t+n] = 0;
	free(*bin);
	*bin = temp;
}

//Function to find the two's complement of the input string (returns the answer as a string) by inverting the bits and adding 1.
char *TwosComp(char s[]) {
	int t = strlen(s), i;
	char *comp = (char *)malloc(sizeof(char) * (1+t));
	if (!comp)
		fatalError();

    //Inverting the bits...
	for (i = 0; i < t; i++) {
		if (s[i] == '0')
			comp[i] = '1';
		else
			comp[i] = '0';
	}
	//Null-terminating the string.
	comp[i] = 0;
	//Adding 1 to obtain 2's complement...
	add(comp, "1");
	return comp;
}

//Left-shift two bit-strings together (specifically the Q and the A bit strings).
//The shift is normal, except that the MSB of s2 would get shifted to the LSB of s1. The LSB of s2 will have a '_', indicating that the value at that place would be determined after the current step of division in progress.
void shiftLeft(char s1[], char s2[]) {
	int i = 0;
	for (i = 0; i < strlen(s1) - 1; i++)
		s1[i] = s1[i + 1];
	s1[strlen(s1) - 1] = s2[0];
	for (i = 0; i < strlen(s2) - 1; i++)
		s2[i] = s2[1 + i];
	s2[strlen(s2) - 1] = '_';
}

//Main logic function for the Non-Restoring division algorithm.
void nonRestoring() {
	int q, m, i, t;
	char *Q, *M, *TwosM;

	printf("Enter dividend: ");
	scanf("%d", &q);

	printf("Enter divisor: ");
	scanf("%d", &m);

	//Checking if input is positive...
    if (q < m || m <= 0 || q < 0) {
		printf("Invalid i/p. Cannot perform division. Terminating program...");
		return;
	}

    if (q >= 1024 || m >= 1024) {
        printf("\n\nWARNING: The output may be distorted for numbers higher than 1024, but final answer should not be affected.");
        system("pause");
    }
	Q = convertToBinArray(q, 0); //Dividend is Q (at the end it will contain the quotient)
	M = convertToBinArray(m, 1);//Divisor is M

	TwosM = TwosComp(M); //Calculate the two's comp of M

	if (!TwosM)
		fatalError();

	normalize(&TwosM, abs(strlen(Q) - strlen(M)) + 1); //normalize the two's complement string to be 1 more than the larger of Q and M.

	char *A = (char *)malloc(strlen(Q) * sizeof(char) + 2); //A is the accumulator register string (at the end it will contain the remainder)
	if (!A)
		fatalError();

    //Initializing A with all 0s.
	for (i = 0; i < strlen(Q) / sizeof(char) + 1; i++)
		A[i] = '0';
	A[strlen(Q) / sizeof(char) + 1] = 0;

	//Printing some relevant information and then the column names.
	printf("\nGiven dividend (Q): %d => %s\nGiven divisor (M): %d => %s\nFinalized sign-extended 2's complement of M: %s\n\nProceeding with division process...", q, Q, m, M, TwosM);

	printf("\n\nStep\t      A\t\t    Q\t     Desc");

	//Making the output neat...
	printf("\n");
		for (t = 0; t < 46; t++)
			printf("=");
		printf("\n\n");

    /*Algorithm of non-restoring division:
        The following is repeated as many times as there are bits in M (divisor):
        1. If MSB(A) = 0 (i.e. A is +ve), left shift and subtract M from A (add TwosM to A).
            a. If result is +ve, set LSB(Q) = 1.
            b. Else, set LSB(Q) = 0.
        2. Else, left shift and add M to A.
            a. If result is +ve, set LSB(Q) = 1.
            b. Else, set LSB(Q) = 0.
        If the current step is the last step, add M and make Q +ve (we can't have a negative quotient).
    */
	for (i = 0; i < strlen(Q); i++) {
		printf(" %d\t%12s\t%9s", i + 1, A, Q);

		if (A[0] == '0') {
			shiftLeft(A, Q);
			printf("\n\n\t%12s\t%9s   Shift Left", A, Q);
			add(A, TwosM);
			printf("\n\t+%11s\t%22s", TwosM, " Add 2's(M)");
			printf("\n\t-------------");
			if (A[0] == '0') {
				Q[strlen(Q) - 1] = '1';
				printf("\n\t%12s\t%9s    Result +ve", A, Q);
			}
			else {
				Q[strlen(Q) - 1] = '0';
				printf("\n\t%12s\t%9s   Result -ve", A, Q);
				if (i == strlen(Q) - 1) {
					printf("\n\t+%8s\t%22s", M, " A must be +ve");
					printf("\n\t-------------");
					add(A, M);
					printf("\n\t%12s\t%8s   A corrected", A, Q);
				}
			}
		}
		else {
			shiftLeft(A, Q);
			printf("\n\n\t%12s\t%9s   Shift Left", A, Q);
			add(A, M);
			printf("\n\t+%11s\t%22s",M, " Add M");
			printf("\n\t-------------");
			if (A[0] == '0') {
				Q[strlen(Q) - 1] = '1';
				printf("\n\t%12s\t%9s    Result +ve", A, Q);
			}
			else {
				Q[strlen(Q) - 1] = '0';
				printf("\n\t%12s\t%9s    Result -ve", A, Q);
				if (i == strlen(Q) - 1) {
					printf("\n\t+%11s\t%22s", M, " A must be +ve");
					printf("\n\t-------------");
					add(A, M);
					printf("\n\t%12s\t%8s   A corrected", A, Q);
				}
			}
		}
		printf("\n");
		for (t = 0; t < 46; t++)
			printf("=");
		printf("\n\n");
	}
	printf("\n\nQuotient: %s, Remainder: %s.", Q, A);
	free(Q);
	free(M);
	free(TwosM);
}

//Similar story for restoring algorithm. We left shift and subtract M from A. If result is -ve, restore A to its initial value by adding M back to it. Otherwise just continue.
void Restoring() {
	int q, m, i, t;
	char *Q, *M, *TwosM;

	printf("Enter dividend: ");
	scanf("%d", &q);

	printf("Enter divisor: ");
	scanf("%d", &m);

	if (q < m || m <= 0 || q < 0) {
		printf("Invalid i/p. Cannot perform division. Terminating program...");
		return;
	}
    if (q >= 1024 || m >= 1024) {
        printf("\n\nWARNING: The output may be distorted for numbers higher than 1024, but final answer should not be affected.");
        system("pause");
    }
	Q = convertToBinArray(q, 0);
	M = convertToBinArray(m, 1);

	TwosM = TwosComp(M);

	if (!TwosM)
		fatalError();

	normalize(&TwosM, abs(strlen(Q) - strlen(M)) + 1);
	char *A = (char *)malloc(strlen(Q) * sizeof(char) + 2);
	if (!A)
		fatalError();
	for (i = 0; i < strlen(Q) / sizeof(char) + 1; i++)
		A[i] = '0';
	A[strlen(Q) / sizeof(char) + 1] = 0;

	printf("\nGiven dividend (Q): %d => %s\nGiven divisor (M): %d => %s\nFinalized sign-extended 2's complement of M: %s\n\nProceeding with division process...", q, Q, m, M, TwosM);

	printf("\n\nStep\t      A\t\t    Q\t     Desc\n");
		for (t = 0; t < 46; t++)
			printf("=");
		printf("\n\n");
	for (i = 0; i < strlen(Q); i++) {
		printf("\n%d\t%11s\t%8s", i + 1, A, Q);
		shiftLeft(A, Q);
		printf("\n\n\t%11s\t%8s   Shift Left", A, Q);
		add(A, TwosM);
		printf("\n\t+%10s\t%21s", TwosM, "Add 2's(M)");
		printf("\n\t-------------");
		if (A[0] == '0') {
			Q[strlen(Q) - 1] = '1';
			printf("\n\t%11s\t%8s   Result +ve", A, Q);
		}
		else {
			Q[strlen(Q) - 1] = '0';
			printf("\n\t%11s\t%8s   Result -ve", A, Q);
			add(A, M);
			printf("\n\t+%10s", M);
			printf("\n\t-------------");
			printf("\n\t%11s\t%8s   Restored", A, Q);
		}
		printf("\n");
		for (t = 0; t < 46; t++)
			printf("=");
		printf("\n\n");
	}
	printf("\n\nQuotient: %s, Remainder: %s.", Q, A);
	free(Q);
	free(M);
	free(TwosM);
}

//Finally...We come to the main() function. Nothing much here, just decide whether we want to perform restoring or non-restoring division, and call the appropriate functions.
int main() {
	printf("\nRestoring or Non-Restoring Division? (Hit R or N): ");
	char ch;
	scanf(" %c", &ch);
	if (tolower(ch) == 'n')
		nonRestoring();
	else if (tolower(ch) == 'r')
		Restoring();
	else
		printf("\n\nInvalid Option! Exiting...");
	printf("\n\nProgram Session terminated successfully.");
	return 0;
}
