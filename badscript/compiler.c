#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

// TODO token position not working corectly
void lex(char* codeFile, token program[]){
    FILE* f = fopen(codeFile, "r");
    char c;
    char word[32];
    int col = 0;
    int row = 0;
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
            if (wordIsNumber){
                program[programCounter++] = (token){row, col - wordIndex, keywordCount, strtol(word, NULL, 10)};
            } else {
                for (int i = 0; i < keywordCount - nonKwordsCount; i++){
                    if (!strcmp(word, keywords[i])){
                        program[programCounter++] = (token){row, col - wordIndex, i, 0};
                    }
                }
            }
            wordIndex = 0;
            wordIsNumber = true;
        }
    }
    if (wordIndex){
        if (wordIsNumber){
            program[programCounter++] = (token){row, col - wordIndex, keywordCount, strtol(word, NULL, 10)};
        } else {
            for (int i = 0; i < keywordCount - nonKwordsCount; i++){
                if (!strcmp(word, keywords[i])){
                    program[programCounter++] = (token){row, col - wordIndex, i, 0};
                }
            }
        }
        wordIndex = 0;
    }
    fclose(f);
}
// TODO END

int main(int argc, char* argv[]){
    if (argc <= 1){
        printf("Not enough arguments!\n");
        return -1;
    }
    static token program[1024];
    
    char* codeFile = argv[1];
    lex(codeFile, program);
    
    return 0;
}