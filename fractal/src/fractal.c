#include "complexlib.h"
#include <stdio.h>

int main() {
    complex c1 = {5.1, 2.2};
    complex c2 = {-0.9, -4.61};
    complex resa = cadd(c1,c2);
    complex ress = csub(c1, c2);
    complex resm = cmul(c1, c2);
    complex resd = cdiv(c1, c2);
    
    printf("%.2f %.2fi\n", resa.real, resa.imag);
    printf("%.2f %.2fi\n", ress.real, ress.imag);
    printf("%.2f %.2fi\n", resm.real, resm.imag);
    printf("%.2f %.2fi\n", resd.real, resd.imag);

    return 0;
}