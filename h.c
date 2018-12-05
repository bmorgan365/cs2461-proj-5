#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(){
    char doc [3] = "D_";
    int i;

    for(i = 1; i <= 3; i++){
        doc[1] = i + 48;
        printf("Doc: %s", doc);
    }
}