#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define PROGRAM_LENGTH 1024
#define MAX_VARIABLE_COUNT 128
#define MAX_VARIABLE_NAME 64

typedef enum {
    op_nop,
    op_exit,
    op_int,
    op_varname,
    op_number,
    opCount
} ops;

int keywordCount = 5;
int nonKwordsCount = 2;
char* keywords[] = {
    "nop",
    "exit",
    "int",
    "varname",
    "number"
};

typedef struct{
    int row;
    int col;
    ops opnum;
    int value;
} token;

bool isWhiteSpace(char c){
    return c == ' ' || c == '\t' || c == '\n' ? true : false;
}

void lex(char* codeFileName, token program[], char* varNames[], int* varNamesCount){
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
                program[programCounter++] = (token){line, prevCol - wordIndex, op_number, strtol(word, NULL, 10)};
            } else {
                bool wordIsKeyword = false;
                for (int i = 0; i < opCount - nonKwordsCount; i++){
                    if (!strcmp(word, keywords[i])){
                        program[programCounter++] = (token){line, prevCol - wordIndex, i, 0};
                        wordIsKeyword = true;
                        break;
                    }
                }
                if (!wordIsKeyword){
                    strcpy(varNames[*varNamesCount], word);
                    program[programCounter++] = (token){line, prevCol - wordIndex, op_varname, (*varNamesCount)++};
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
            program[programCounter++] = (token){line, prevCol - wordIndex, op_number, strtol(word, NULL, 10)};
        } else {
            for (int i = 0; i < opCount - nonKwordsCount; i++){
                if (!strcmp(word, keywords[i])){
                    program[programCounter++] = (token){line, prevCol - wordIndex, i, 0};
                } else {
                        strcpy(varNames[*varNamesCount], word);
                        program[programCounter++] = (token){line, prevCol - wordIndex, op_varname, (*varNamesCount)++};
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

void printProgram(FILE* asmOutFile, token* program, int* variables){
    int variableIndex = 0;
    for (int i = 0; i < PROGRAM_LENGTH; i++){
        if (!program[i].row){
            return;
        }
        switch (program[i].opnum){
            case op_nop:
                fprintf(asmOutFile, "\tmov rax, rax\t\t\t\t; nop\n");
                break;
            case op_exit:
                assert(i < PROGRAM_LENGTH - 1 && program[i + 1].opnum == op_number && "exit keyword must be followed by exit code type(int)");
                fprintf(asmOutFile, "\tmov rax, 60\t\t\t\t; exit\n");
                fprintf(asmOutFile, "\tmov rdi, %d\t\t\t\t; |\n", program[++i].value);
                fprintf(asmOutFile, "\tsyscall\t\t\t\t; |\n");
                break;
            case op_int:
                assert(i < PROGRAM_LENGTH - 2 && program[i + 1].opnum == op_varname && program[i + 2].opnum == op_number && "int takes 2 parameters a name and a valuue");
                variables[variableIndex++] = program[i + 2].value;
                i += 2;
            case op_varname:
                break;
            default:
                break;
                fprintf(stderr, "unexpected token: ln%d:col%d\t(%d)%s\t%d\n",
                    program[i].row, program[i].col, program[i].opnum, keywords[program[i].opnum], program[i].value);
                assert(0 && "Not recognised operation");
                break;
        }
    }
}

void printVariables(FILE* asmOutFile, int* variables, char* varNames[]){
    fprintf(asmOutFile, "segment readable writable\n");
    for (int i = 0; i < MAX_VARIABLE_COUNT; i++){
        if (!varNames[i][0]){
            return;
        }
        fprintf(asmOutFile, "var%d dq %d\t\t\t\t; %s\n", i, variables[i], varNames[i]);
    }
}

typedef struct {
    bool assembly;
    bool debug;
    bool tokens;
    bool help;
} flagList;

int main(int argc, char* argv[]){
    if (argc <= 1){
        system("cat help.txt");
        assert(0 && "Not enough arguments");
    }
    flagList flags = {0};
    bool inputGiven = false;
    char* codeFileName;
    char outputFileName[64] = " out.elf";
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
                case 'h':
                    flags.help = true;
                    break;
                default:
                    assert(0 && "Unknown option");
                    break;
            }
        } else {
            if (inputGiven){
                strcpy(outputFileName, " ");
                strcat(outputFileName, argv[i]);
            } else {
                codeFileName = argv[i];
                inputGiven = true;
            }
        }
    }
    if (flags.help){
        system("cat help.txt");
        return 0;
    }

    static token program[PROGRAM_LENGTH];
    static int variables[MAX_VARIABLE_COUNT];
    static char* varNames[MAX_VARIABLE_COUNT];
    for (int i = 0; i < MAX_VARIABLE_COUNT; i++){
        varNames[i] = (char*)malloc(MAX_VARIABLE_NAME * sizeof(char));
    }
    int varNamesCount = 0;
    
    lex(codeFileName, program, varNames, &varNamesCount);

    if (flags.tokens){
        writeTokens(program);
    }

    char* asmOutFileName = "temp.asm";
    FILE* asmOutFile = fopen(asmOutFileName, "w");
    printAssemblyHeader(asmOutFile);
    printProgram(asmOutFile, program, variables);
    printVariables(asmOutFile, variables, varNames);
    fclose(asmOutFile);

    char command[256];
    strcpy(command, "fasm ");
    if (flags.debug){
        assert(0 && "debug flag is broken");
        strcpy(command, "-s ");
        strcpy(command, " out.symbol");
    }
    strcat(command, asmOutFileName);
    strcat(command, outputFileName);
    system(command);

    strcpy(command, "chmod +x");
    strcat(command, outputFileName);
    system(command);

    if (!flags.assembly){
        strcpy(command, "rm temp.asm");
        system(command);
    }
    
    return 0;
}