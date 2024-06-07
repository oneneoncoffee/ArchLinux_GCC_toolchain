// Note: GCC to compile this program at the command,
// gcc square.c -o square -lm
// the -lm flag is necessary to link the math library.
//                                                    
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
    printf("USAGE: %s <NUMBER>\n", argv[0]);
    return 1;
    }
    char *endptr;
    double number1 = strtod(argv[1], &endptr);
    double limit = strtod(argv[1], &endptr);
    if (*endptr != '\0') {
    printf("Invalid number entered: %s\n", argv[1]);
    return 1;
    }
    if (number1 < 0) {
        printf("Cannot calculate the square root of a negative number.\n");
    } else {
        double squareRoot = sqrt(number1);
        printf("Square root of %.2lf is %.2lf\n", number1, squareRoot);
    }
    double number;
    printf("Enter a number: ");
    scanf("%lf", &number);

    if (number < 0) {
        printf("Cannot calculate the square root of a negative number.\n");
    } else {
        double squareRoot = sqrt(number);
        printf("Square root of %.2lf is %.2lf\n", number, squareRoot);
    }
    if (limit < 0) {
        printf("Cannot calculate square roots for a negative limit.\n");
        return 1;
    }

    printf("Square roots from 1 to %.2lf:\n", limit);

    for (double i = 1; i <= limit; i++) {
        double squareRoot = sqrt(i);
        //printf("Square root of %.2lf is %.2lf\n", i, squareRoot);
        printf("%.2lf ", squareRoot);
    }
    printf("\n");
return 0;
}
