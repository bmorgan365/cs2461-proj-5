#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int main(){
    int docs = 3;
    int df = 2;
    double ret = log((double)docs / df) / log(10);

    printf("log(%lf) = %lf", (double) docs / df, ret);
    return 0;
}