#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int main(){
    char* word = "computer";
    int wordSum = 0;
    int resetCharPtr = 0;
    while(*word != '\0'){
        wordSum += *word;
        resetCharPtr++;
        word++;
    }
    word -= resetCharPtr;
    printf("Bucket %d",(wordSum % 5));
    return 0;
}