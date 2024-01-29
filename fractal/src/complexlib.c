#include <math.h>

typedef struct {
    double real;
    double imag;
} complex;


complex cadd(complex n1, complex n2) {
    return (complex){n1.real+n2.real, n1.imag+n2.imag};
}

complex csub(complex n1, complex n2) {
    return (complex){n1.real-n2.real, n1.imag-n2.imag};
}

complex cmul(complex n1, complex n2) {
    return (complex){n1.real*n2.real-n1.imag*n2.imag, n1.real*n2.imag+n2.real*n1.imag};
}

complex cdiv(complex n1, complex n2) {
    double div = n2.real*n2.real+n2.imag*n2.imag;
    return (complex){(n1.real*n2.real+n1.imag*n2.imag)/div, (n1.imag*n2.real-n1.real*n2.imag)/div};
}

complex crmul(complex n1, double n2) {
    return (complex){n1.real * n2, n1.imag * n2};
}

complex cpower(complex n1, int p) {
    if (p==0) {
        return (complex){1, 0};
    }
    complex res = n1;
    while (--p) {
        res = cmul(res, n1);
    }
    return res;
}

double cabsol(complex n) {
    return sqrt(n.real * n.real + n.imag * n.imag);
}


