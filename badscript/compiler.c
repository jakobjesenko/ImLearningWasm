#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PROGRAM_LENGTH 1024

typedef enum{
    op_exit
} ops;

int keywordCount = 3;
int nonKwordsCount = 1;
char* keywords[] = {
    "nop",
    "exit",
    "number" // number not keyword count lowerd by 1
};

typedef struct{
    int row;
    int col;
    int opnum;
    int value;
} token;

bool isWhiteSpace(char c){
    return c == ' ' || c == '\t' || c == '\n' ? true : false;
}

void lex(char* codeFile, token program[]){
    FILE* f = fopen(codeFile, "r");
    char c;
    char prevC;
    char word[32];
    int col = 1;
    int prevCol = 1;
    int row = 1;
    int wordIndex = 0;
    int programCounter = 0;
    bool wordIsNumber = true;

    while ((c = fgetc(f)) != EOF){
        switch (c){
        case ' ':
            break;
        case '\t':
            col += 3;
            break;
        case '\n':
            row++;
            col = 0;
            break;
        default:
            word[wordIndex++] = c;
            word[wordIndex] = 0;
            wordIsNumber = c < '0' || c > '9' ? false : wordIsNumber;
            break;
        }
        col++;
        if (isWhiteSpace(c) && wordIndex){
            int line = prevC == '\n' ? row - 1 : row;
            if (wordIsNumber){
                program[programCounter++] = (token){line, prevCol - wordIndex, keywordCount - 1, strtol(word, NULL, 10)};
            } else {
                for (int i = 0; i < keywordCount - nonKwordsCount; i++){
                    if (!strcmp(word, keywords[i])){
                        program[programCounter++] = (token){line, prevCol - wordIndex, i, 0};
                    }
                }
            }
            wordIndex = 0;
            wordIsNumber = true;
        }
        prevC = c;
        prevCol = col;
    }
    if (wordIndex){
        int line = prevC == '\n' ? row - 1 : row;
        if (wordIsNumber){
            program[programCounter++] = (token){line, prevCol - wordIndex, keywordCount - 1, strtol(word, NULL, 10)};
        } else {
            for (int i = 0; i < keywordCount - nonKwordsCount; i++){
                if (!strcmp(word, keywords[i])){
                    program[programCounter++] = (token){line, prevCol - wordIndex, i, 0};
                }
            }
        }
        wordIndex = 0;
    }
    fclose(f);
}

void tokensToString(token* program){
    token t;
    for (int i = 0; i < PROGRAM_LENGTH; i++){
        t = program[i];
        if (!t.row){
            return;
        }
        fprintf(stdout, "ln%d:col%d\t(%d)%s\t%d\n", t.row, t.col, t.opnum, keywords[t.opnum], t.value);
    }
}

int main(int argc, char* argv[]){
    if (argc <= 1){
        printf("Not enough arguments!\n");
        return -1;
    }
    static token program[PROGRAM_LENGTH];
    
    char* codeFile = argv[1];
    lex(codeFile, program);

    tokensToString(program);
    
    return 0;
}