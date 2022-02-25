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
    ')',
    'h', // push name: 'n'
    'l', // pull
    '='  // print
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
            case '=':
                programm[tokenCount] = (token){row, col, '=', 0};
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

// operations definiton

void push(int n, int* execStack, int* stackPointer){
    execStack[(*stackPointer)++] = n;
}

int pull(int* execStack, int* stackPointer){
    return execStack[--(*stackPointer)];
}

void add(int* execStack, int* stackPointer){
    int b = pull(execStack, stackPointer);
    int a = pull(execStack, stackPointer);
    push(a + b, execStack, stackPointer);
}

void sub(int* execStack, int* stackPointer){
    int b = pull(execStack, stackPointer);
    int a = pull(execStack, stackPointer);
    push(a - b, execStack, stackPointer);
}

void mul(int* execStack, int* stackPointer){
    int b = pull(execStack, stackPointer);
    int a = pull(execStack, stackPointer);
    push(a * b, execStack, stackPointer);
}

void dev(int* execStack, int* stackPointer){
    int b = pull(execStack, stackPointer);
    int a = pull(execStack, stackPointer);
    push(a / b, execStack, stackPointer);
}

void mod(int* execStack, int* stackPointer){
    int b = pull(execStack, stackPointer);
    int a = pull(execStack, stackPointer);
    push(a % b, execStack, stackPointer);
}

void print(int* execStack, int* stackPointer){
    int a = pull(execStack, stackPointer);
    printf("%d\n", a);
}

// execute code

int execute(int* execStack, token* programm){
    int stackPointer = 0;
    int done = 0;

    for (int opNum = 0; opNum < PROGRAM_LENGTH && !done; opNum++){
        token op = programm[opNum];
        switch (op.name){
            case 'n':
                push(op.value, execStack, &stackPointer);
                break;
            assert(stackPointer >= 2);
            case '+':
                add(execStack, &stackPointer);
                break;
            case '-':
                sub(execStack, &stackPointer);
                break;
            case '*':
                mul(execStack, &stackPointer);
                break;
            case '/':
                dev(execStack, &stackPointer);
                break;
            case '%':
                mod(execStack, &stackPointer);
                break;
            case '=':
                print(execStack, &stackPointer);
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
    
    return 0;
}