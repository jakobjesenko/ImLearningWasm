#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

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

void lex(char* codeFileName, token program[]){
    FILE* f = fopen(codeFileName, "r");
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

void writeTokens(token* program){
    token t;
    for (int i = 0; i < PROGRAM_LENGTH; i++){
        t = program[i];
        if (!t.row){
            return;
        }
        fprintf(stdout, "ln%d:col%d\t(%d)%s\t%d\n", t.row, t.col, t.opnum, keywords[t.opnum], t.value);
    }
}

void printAssemblyHeader(FILE* asmOutFile){
    fprintf(asmOutFile, "format ELF64 executable 3\n");
    fprintf(asmOutFile, "segment readable executable\n");
    fprintf(asmOutFile, "entry start\n");
    fprintf(asmOutFile, "start:\n");
}

void printProgram(FILE* asmOutFile, token* program){
    for (int i = 0; i < PROGRAM_LENGTH; i++){
        if (!program[i].row){
            return;
        }
        switch (program[i].opnum){
            case 0:
                fprintf(asmOutFile, "\tmov rax, rax\n");
                break;
            case 1:
                assert(i < PROGRAM_LENGTH - 1 && program[i + 1].opnum == 2 && "exit keyword must be followed by exit code type(int)");
                fprintf(asmOutFile, "\tmov rax, 60\n");
                fprintf(asmOutFile, "\tmov rdi, %d\n", program[++i].value);
                fprintf(asmOutFile, "\tsyscall\n");
                break;
            default:
                assert(0 && "Not recognised operation");
                break;
        }
    }
}

typedef struct {
    bool assembly;
    bool debug;
    bool tokens;
} flagList;

int main(int argc, char* argv[]){
    if (argc <= 1){
        assert(0 && "Not enough arguments");
    }
    flagList flags = {0};
    char* codeFileName = argv[1];
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            switch (argv[i][1]){
                case 'a':
                    flags.assembly = true;
                    break;
                case 'd':
                    flags.debug = true;
                    break;
                case 't':
                    flags.tokens = true;
                    break;
                default:
                    assert(0 && "Unknown option");
                    break;
            }
        } else {
            codeFileName = argv[i];
        }
    }
    static token program[PROGRAM_LENGTH];
    
    lex(codeFileName, program);

    char* asmOutFileName = "temp.asm";
    FILE* asmOutFile = fopen(asmOutFileName, "w");
    printAssemblyHeader(asmOutFile);
    printProgram(asmOutFile, program);
    fclose(asmOutFile);

    if (flags.tokens){
        writeTokens(program);
    }

    char command[256];
    strcpy(command, "fasm ");
    if (flags.debug){
        assert(0 && "debug flag is broken");
        strcpy(command, "-s ");
        strcpy(command, " out.symbol");
    }
    strcat(command, asmOutFileName);
    strcat(command, " out.elf");
    system(command);

    strcpy(command, "chmod +x ");
    strcat(command, " out.elf");
    system(command);

    if (!flags.assembly){
        strcpy(command, "rm temp.asm");
        system(command);
    }
    
    return 0;
}