#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODE_LENGTH 1024

char operators[] = {
    '+',
    '*',
    '-',
    '/',
    '%',
    '(',
    ')'
};

typedef struct{
    int row;
    int col;
    char name;
    int value;
} token;

void readInput(char* code){
    char c;
    for (int i = 0; i < CODE_LENGTH && (c = getchar()) != EOF; i++){
        code[i] = c;
    }
}

int lexer(char* code, token* programm, int tokenCount){
    char wordBuffer[32];
    int bufferIndex = 0;
    int chrc = 0;
    int col = 0;
    int row = 0;

    while (code[chrc]){
        switch (code[chrc]){
            case '\n':
                col = 0;
                row++;
                break;
            case ' ':
                break;
            case '\t':
                col += 3;
                break;
            case '+':
                programm[tokenCount] = (token){row, col, '+', 0};
                tokenCount++;
                break;
            case '-':
                programm[tokenCount] = (token){row, col, '-', 0};
                tokenCount++;
                break;
            case '*':
                programm[tokenCount] = (token){row, col, '*', 0};
                tokenCount++;
                break;
            case '/':
                programm[tokenCount] = (token){row, col, '/', 0};
                tokenCount++;
                break;
            case '%':
                programm[tokenCount] = (token){row, col, '%', 0};
                tokenCount++;
                break;
            case '(':
                programm[tokenCount] = (token){row, col, '(', 0};
                tokenCount++;
                break;
            case ')':
                programm[tokenCount] = (token){row, col, ')', 0};
                tokenCount++;
                break;
            default:
                wordBuffer[bufferIndex] = code[chrc];
                bufferIndex++;
                if (chrc < CODE_LENGTH - 1 && (code[chrc + 1] == ' ' || code[chrc + 1] == '\n' || code[chrc + 1] == '\t' || code[chrc + 1] == 0)){
                    wordBuffer[bufferIndex] = 0;
                    programm[tokenCount] = (token){row, col, 'n', (int)strtol(wordBuffer, NULL, 10)};
                    tokenCount++;
                    bufferIndex = 0;
                }
                break;
        }
        col++;
        chrc++;
    }
    return tokenCount;
}

int main(){
    static char code[CODE_LENGTH];
    static token programm[256];
    int tokenCount = 0;

    readInput(code);

    tokenCount = lexer(code, programm, tokenCount);
    
    return 0;
}