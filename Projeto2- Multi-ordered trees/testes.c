#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv){
    char *a = "Alberto de Sousa";
    char *b= (char*)malloc(sizeof(char) * (2+1));
    strncpy(b,(a+0),2);
    printf("%d\n", strcmp(b,"M"));
    return 1;
}