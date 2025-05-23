/**
 * This is the example lexical analyzer code from pages 173 - 177 of the textbook,
 * Sebesta, R. W. (2012). Concepts of Programming Languages.
 * Pearson, 10th edition.
 */

/* front.c - a lexical analyzer system for a language called dcooke. A .dc extension language. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;
char *tokens;
/* Local Variables */
static int charClass;
static char lexeme[100];
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
char *keywords[] = {
    "while", "write", "read", "do"
};

/* main driver */
int main(int argc, char* argv[])
{
    printf("DCooke Analyzer::R11569029 \n");
    /* Open the input data file and process its contents */
    if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR - cannot open front.in \n");
    } else {
        getChar();
        do {
            lex();
            //expr();
        } while (nextToken != EOF);
    }

    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the
 * token */
static int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            tokens = "LEFT_PAREN";
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            tokens = "RIGHT_PAREN";
            break;
        case '+':
            addChar();
            getChar();
            switch (nextChar) {
                case '+':
                    addChar();
                    nextToken = INC_OP;
                    tokens = "INC_OP";
                    break; // Add this break statement to exit the 'case '+'.
                default:
                    nextToken = ADD_OP;
                    tokens = "ADD_OP";
                    break;
            }
            break; // Add this break statement to exit the 'case '+' in the outer switch.
        case '-':
            addChar();
            getChar();
            switch (nextChar) {
                case '-':
                    addChar();
                    nextToken = DEC_OP;
                    tokens = "DEC_OP";
                    break; // Add this break statement to exit the 'case '-'.
                default:
                    nextToken = SUB_OP;
                    tokens = "SUB_OP";
                    break;
            }
            break; // Add this break statement to exit the 'case '-' in the outer switch.
        case '*':
            addChar();
            nextToken = MULT_OP;
            tokens = "MULT_OP";
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            tokens = "DIV_OP";
            break;
        case '=':
            addChar();
            getChar();
            switch (nextChar) {
                case '=':
                    addChar();
                    nextToken = EQUAL_OP;
                    tokens = "EQUAL_OP";
                    break; // Add this break statement to exit the 'case '=='.
                default:
                    nextToken = ASSIGN_OP;
                    tokens = "ASSIGN_OP";
                    break;
            }
            break; // Add this break statement to exit the 'case '=' in the outer switch.
        case '<':
            addChar();
            getChar();
            switch (nextChar) {
                case '=':
                    addChar();
                    nextToken = LEQUAL_OP;
                    tokens = "LEQUAL_OP";
                    break;
                default:
                    nextToken = LESSER_OP;
                    tokens = "LESSER_OP";
                    break;
            }
            break;
        case '>':
            addChar();
            getChar();
            switch (nextChar) {
                case '=':
                    addChar();
                    nextToken = GEQUAL_OP;
                    tokens = "GEQUAL_OP";
                    break;
                default:
                    nextToken = GREATER_OP;
                    tokens = "GREATER_OP";
                    break;
            }
            break;
        case '!':
            addChar();
            getChar();
            switch (nextChar) {
                case '=':
                    addChar();
                    nextToken = NEQUAL_OP;
                    tokens = "NEQUAL_OP";
                    break;
                default:
                    nextToken = UNKNOWN;
                    tokens = "UNKNOWN";
                    break;
            }
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            tokens = "SEMICOLON";
            break;
        case '{':
            addChar();
            nextToken = LEFT_CBRACE;
            tokens = "LEFT_CBRACE";
            break;
        case '}':
            addChar();
            nextToken = RIGHT_CBRACE;
            tokens = "RIGHT_CBRACE";
            break;
        case EOF:
            nextToken = EOF;
            return nextToken;
        default:
            addChar();
            nextToken = UNKNOWN;
            tokens = "UNKNOWN";
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = OPERATORS;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace
 * character */
static void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}
/*function for the keywords*/
int isKeyword(char *lexeme) {
    int numKeywords = sizeof(keywords) / sizeof(char *);
    int i;
    for (i = 0; i < numKeywords; i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            if (strcmp(lexeme, "while") == 0) {
                nextToken = KEY_WHILE;
                tokens = "KEY_WHILE";
                break;
            } else if (strcmp(lexeme, "write") == 0) {
                nextToken = KEY_WRITE;
                tokens = "KEY_WRITE";
                break;
            } else if (strcmp(lexeme, "read") == 0) {
                nextToken = KEY_READ;
                tokens = "KEY_READ";
                break;
            } else if (strcmp(lexeme, "do") == 0) {
                nextToken = KEY_DO;
                tokens = "KEY_DO";
                break;
            }
        } else {
            nextToken = IDENT;
            tokens = "IDENT";
        }
    }
    return 0;
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            isKeyword(lexeme);
            break;

            /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            tokens = "INT_LIT";
            break;

            /* Parentheses and operators */
        case OPERATORS:
            lookup(nextChar);
            getChar();
            break;

        case UNKNOWN:
            lookup(nextChar);

            /* EOF */
        case EOF:
            nextToken = EOF;
            exit(0);
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */

    printf(" %s \t %s\n", lexeme, tokens);
    return nextToken;
} /* End of function lex */
