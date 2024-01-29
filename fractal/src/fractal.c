#include "complexlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void cprint(complex n) {
    if (n.imag >= 0) {
        printf("%.2f+%.2fi ", n.real, n.imag);
    } else {
        printf("%.2f%.2fi ", n.real, n.imag);
    }
}

int main(int argc, char * argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "Error: Program must be run as ./fractal filename [-g output.bmp]\n");
        return 1;
    }

    char * bmp_filename = NULL;
    if (argc == 4 && !strcmp(argv[2], "-g")) {
        bmp_filename = argv[3];
    }

    char * filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (file==NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 1;
    }

     int deg; // Polynomial degree
    if (1!=fscanf(file, "%d", &deg)) {
        return 1;
    }

    // Polynomial factors
    double * synt = malloc(sizeof(double) * (deg+1));
    if (synt == NULL) {
        fprintf(stderr, "Error: Malloc failed\n");
        return 1;
    }
    for (int i=0; i<deg+1; i++) {
        if (1!=fscanf(file, "%lf", &(synt[i]))) {
            return 1;
        }
    }

    // Polynomial solution range and step
    double min_real, min_imag, max_real, max_imag, step;
    if (5!=fscanf(file, "%lf %lf %lf %lf %lf", &min_real, &min_imag, &max_real, &max_imag, &step)) {
        return 1;
    }

    // Find out if more factors than needed are in input file = check if (deg = number of factors)
    double garbage;
    if (fscanf(file, "%lf", &garbage)!=EOF) {
        return 1;
    }

    double tmp;
    int loops;
    complex zn, zn_1;
    complex fzn, fdzn; // f(zn), f'(zn)
    
    /* Creating all iterations based on the range and step given
        using two loops that go through all combinations of complex numbers in the given range */
    while (min_real <= max_real) {
        tmp = min_imag;
        while (tmp <= max_imag) {
            loops=0;
            zn_1 = (complex){min_real, tmp}; // initial value of zn_1 (zn)
            do {
                // The new possible solution is the previously computed zn_1
                zn = zn_1;
                fdzn = (complex){0, 0};
                fzn = (complex){synt[0], 0};
                for (int i=1; i<deg+1; i++) {
                    // Compute f(zn) and its derivative f'(zn)
                    fzn = cadd(fzn, crmul(cpower(zn, i), synt[i]));
                    fdzn = cadd(fdzn, crmul(cpower(zn, i-1), i*synt[i]));
                }
                // Division by zero - must break out of the loop and print nan
                if (fdzn.real == 0 && fdzn.imag == 0) {
                    loops = -1;
                    break;
                }
                // Compute the next possible solution of our polynomial based on the Newton-Raphson method
                zn_1 = csub(zn, cdiv(fzn, fdzn));
                loops++;
            } while (cabsol(csub(zn_1, zn)) >= 1e-6 && loops <= 1000);

            if (loops > 1000) {
                printf("incomplete ");
            } else if (loops < 0) {
                printf("nan ");
            } else {
                cprint(zn_1);
            }
            // Inner step increase (imaginary part += step)
            tmp += step;
        }
        printf("\n");
        // Outer step increase (real part += step)
        min_real += step;
    }

    // Create the bmp file
    if (bmp_filename) {
        printf("Bmp yes\n");
    }


    return 0;
}