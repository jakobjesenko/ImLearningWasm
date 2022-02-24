#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CODE_LENGTH 1024
#define PROGRAM_LENGTH 256
#define STACK_SIZE 64

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
        assert(tokenCount < PROGRAM_LENGTH);
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
                if (chrc < CODE_LENGTH - 1 && (code[chrc + 1] < '0' || code[chrc + 1] > '9')){
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

int execute(int* execStack, token* programm){
    int stackPointer = 0;
    int done = 0;
    int a;
    int b;

    for (int opNum = 0; opNum < PROGRAM_LENGTH && !done; opNum++){
        token op = programm[opNum];
        switch (op.name){
            case 'n':
                execStack[stackPointer] = op.value;
                stackPointer++;
                break;
            assert(stackPointer >= 2);
            case '+':
                a = execStack[stackPointer - 2];
                b = execStack[stackPointer - 1];
                stackPointer--;
                execStack[stackPointer - 1] = a + b;
                break;
            case '-':
                a = execStack[stackPointer - 2];
                b = execStack[stackPointer - 1];
                stackPointer--;
                execStack[stackPointer - 1] = a - b;
                break;
            case '*':
                a = execStack[stackPointer - 2];
                b = execStack[stackPointer - 1];
                stackPointer--;
                execStack[stackPointer - 1] = a * b;
                break;
            case '/':
                a = execStack[stackPointer - 2];
                b = execStack[stackPointer - 1];
                stackPointer--;
                execStack[stackPointer - 1] = a / b;
                break;
            case '%':
                a = execStack[stackPointer - 2];
                b = execStack[stackPointer - 1];
                stackPointer--;
                execStack[stackPointer - 1] = a % b;
                break;
            default:
                done = 1;
                break;
        }
    }

    return stackPointer;
}

int main(){
    static char code[CODE_LENGTH];
    static token programm[PROGRAM_LENGTH];
    int tokenCount = 0;
    static int execStack[STACK_SIZE];
    int stackPointer = 0;

    readInput(code);

    tokenCount = lexer(code, programm, tokenCount);

    stackPointer = execute(execStack, programm);

    assert(stackPointer > 0);
    printf("%d\n", execStack[stackPointer - 1]);
    
    return 0;
}