#include <stdio.h>
#include <string.h>

enum operations{
    num,
    plus,
    minus,
    times
};

int main(){
    static char programm[1024];
    static char code[1024];
    scanf("%s", code);
    strcpy(programm, code);
    printf("%s", programm);
    return 0;
}