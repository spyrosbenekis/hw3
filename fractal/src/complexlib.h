typedef struct {
    double real;
    double imag;
} complex;

complex cadd(complex n1, complex n2);
complex csub(complex n1, complex n2);
complex cmul(complex n1, complex n2);
complex cdiv(complex n1, complex n2);
complex crmul(complex n1, double n2);
complex cpower(complex n, int p);
double cabsol(complex n);
int ccompare(complex n1, complex n2);