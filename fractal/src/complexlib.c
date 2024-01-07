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
    // if (n2.real || n2.imag) {
        double div = n2.real*n2.real+n2.imag*n2.imag;
        return (complex){(n1.real*n2.real+n1.imag*n2.imag)/div, (n1.imag*n2.real-n1.real*n2.imag)/div};
    // }
}