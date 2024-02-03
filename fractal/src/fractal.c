#include "complexlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define HEADER 54
#define RGB 3
#define MAX_ROOTS 10

void cprint(complex n) {
    if (n.imag >= 0) {
        printf("%.2f+%.2fi ", n.real, n.imag);
    } else {
        printf("%.2f%.2fi ", n.real, n.imag);
    }
}

void add_if_new(complex solution, complex solutions[], int * found) {
    int add = 1;
    for (int i=0; i<*found; i++) {
        if (ccompare(solution, solutions[i])) {
            add=0;
        }
    }
    if (add) {
        solutions[*found] = solution;
        (*found)++;
    }

}

int index_of(complex solution, complex solutions[], int len) {
    for (int pos=0; pos<len; pos++) {
        if (ccompare(solution, solutions[pos])) return pos;
    }
    return MAX_ROOTS;
}

void set_size(char* ptr, long int byte, uint size) {
    uint temp = size;
    int times;
    int mlt=1;

    int digits = 0;

    do {
        digits++;
        mlt*=256;
    } while(temp/=256);
    
    /* We break the initial size in powers of 16^2=256 and we convert that to 2-bit hexadecimal values 
        with the help of the bitwise (&) operator, which we store in out output data pointer*/
    mlt/=256;
    while (digits) {
        times=size/mlt;
        ptr[byte+digits-1] = 0xff & times;
        size-=times*mlt;
        mlt/=256;
        digits--;
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
    
    int iterations = ((max_real-min_real)/step+1) * ((max_imag-min_imag)/step+1);
    complex * all_solutions = malloc(sizeof(complex) * iterations);
    if (all_solutions == NULL) {
        return 1;
    }
    complex sep_solutions[deg];
    int found=0;

    int * loops_arr = calloc(iterations, sizeof(int));
    if (loops_arr == NULL) {
        return 1;
    }
    int counter=0;


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
                loops_arr[counter] = 1e-6;
            } else if (loops < 0) {
                printf("nan ");
                loops_arr[counter] = 1e-6;
            } else {
                cprint(zn_1);
                loops_arr[counter] = loops;
                add_if_new(zn_1, sep_solutions, &found);
                all_solutions[counter] = zn_1;
            }
            counter++;
            // Inner step increase (imaginary part += step)
            tmp += step;
        }
        printf("\n");
        // Outer step increase (real part += step)
        min_real += step;
    }


    // Create the bmp file
    if (bmp_filename) {
        int width = sqrt(counter);
        int height = width;


        int padding = (4 - (RGB*width % 4))%4;
        int filesize = HEADER + (RGB*width+padding)*height;
        int imagesize = filesize - HEADER;

        // Creating bmp file simple header
        FILE* bmp_file = fopen(bmp_filename, "w");
        char * bmp_array = calloc(filesize, 1);
        bmp_array[0] = 'B';
        bmp_array[1] = 'M';
        set_size(bmp_array, 2, filesize);
        bmp_array[10]=0x36;
        bmp_array[14]=0x28;
        set_size(bmp_array, 18, width);
        set_size(bmp_array, 22, height);
        bmp_array[26]=0x01; // color planes=1
        bmp_array[28]=0x18; // 24-depth pixels
        set_size(bmp_array, 34, imagesize);

        fwrite(bmp_array, 1, HEADER, bmp_file);

        // Creating bmp file pixel array

        // define colors for each separate solution (up to 10)
        int colors[MAX_ROOTS][RGB] = {{0, 0, 255}, {0, 255, 0}, {0, 255, 255}, {255, 0, 0}, {255, 255, 0}, {255, 0, 255}, {64, 0, 128}, {0, 128, 64}, {128, 64, 0}, {0, 0, 0}};

        for (int h=0; h<height; h++) {
            for (int w=0; w<width; w++) {
                // This generates image based on separate solutions and iterations taken to reach each solution
                for (int r=0; r<RGB; r++) {
                    fputc(85*log10(loops_arr[width*h+w])*colors[index_of(all_solutions[width*h+w], sep_solutions, deg)][r], bmp_file);
                } // 85 comes from 255/3. 255 is the largest r, g or b value of a color and 3 is the max of log10(loops_arr[i]) since loops <= 1000
            }

            // Place padding bytes inbetween pixels
            for (int p=0; p<padding; p++) {
                fputc(0, bmp_file);
            }
        }
    }


    return 0;
}