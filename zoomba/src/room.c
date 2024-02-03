#include <stdlib.h>
#include <stdio.h>

#define RGB 3
#define H 54

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

int main() {
    int width;
    if (1!=scanf("%d", &width)) {
        return 1;
    }

    int zx, zy, tx, ty;
    if (4!=scanf("%d %d %d %d", &zx, &zy, &tx, &ty)) {
        return 1;
    }

    fprintf(stderr, "Zoomba X=%d, Zoomba Y=%d, Target X=%d, Target Y=%d\n", zx, zy, tx, ty);

    int squares[width*width];
    for (int i=0; i<width*width; i++) {
        scanf("%d", &squares[i]);
    }

    int padding = (4-RGB*width%4)%4;

    int filesize = H+(width+padding*RGB)*width;
    int imagesize = filesize-H;
    char data[H];
    for (int i=0; i<H; i++) {
        data[i]=0;
    }
    data[0] = 'B';
    data[1] = 'M';
    set_size(data, 2, filesize);
    data[10] = H;
    data[14] = H-14;
    set_size(data, 18, width);
    set_size(data, 22, width);
    data[26] = 1;
    data[28] = 24;
    set_size(data, 34, imagesize);

    fwrite(data, 1, H, stdout);

    for (int h=0; h<width; h++) {
        for (int w=0; w<width; w++) {
            for (int r=0; r<RGB; r++) {
                if (w==tx&&h==ty) {
                    putchar(r*127*(r-1));
                } else if (w==zx&&h==zy) {
                    putchar(r*255*(2-r));
                } 
                else {
                    putchar((squares[width*h+w])?0:255);
                }
            }
        }

        for (int p=0; p<padding; p++) {
            putchar(0);
        }
    }


    return 0;
}