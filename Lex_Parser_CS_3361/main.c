/* Assignment 3 Matthew Uriegas, R11565427 
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>



#define KEYWORD 0
#define LETTER 1
#define DIGIT 2
#define UNKNOWN 99

#define INT_LIT 10
#define IDENT 11
#define LESSER_OP 12
#define GREATER_OP 13
#define EQUAL_OP 14
#define NEQUAL_OP 15
#define LEQUAL_OP 16
#define GEQUAL_OP 17
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define SEMICOLON 27
#define KEY_READ 28
#define KEY_WRITE 29
#define KEY_WHILE 30
#define KEY_DO 31
#define KEY_OD 32

/* Global Variable */
int nextToken;

/* Local Variables */
static int charClass;
static char lexeme [100];
static char keywords [5][6] = {"read", "write", "while", "do", "od"};
static char operators [4][3] = {":=", "<>", "<=", ">="};
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();
static int lex();

/* main driver */
// int main(int argc, char* argv[]) 
// {
//     printf("DanC Analyzer :: R11565427\n");
//     for(int i=1; i <= argc; i++){ // i=1, assuming files arguments are right after the executable
//         char *fin = argv[i]; //filename
//         /* Open the input data file and process its contents */
//         if ((in_fp = fopen(fin, "r")) == NULL) {
//             printf("ERROR - cannot open %s\n", fin);
//             break;
//         } else{
//             getChar();
//             do{
//                   lex();
//             }while (nextToken != EOF);
//             break;
//         }
//     }
//     return 0;
// }


/* lookup - a function to lookup operators and parentheses and return the 
 * token */
static int lookup(char ch) {
    //Switch statement which looks for operators
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case '=':
            addChar();
            nextToken = EQUAL_OP;
            break;
        // Less than case which check for NEQUAL and LEQUAL cases
        case '<':
            addChar();
            getChar();
            if(nextChar == '>'){
              addChar();
              nextToken = NEQUAL_OP;
            }  
            else if(nextChar == '='){
              addChar();
              return nextToken = LEQUAL_OP;
            }
            else{
              nextToken = LESSER_OP;
            }
            break;
        // Greater than case which checks for GEQUAL case
        case '>':
            addChar();
            getChar();
            if(nextChar == '='){
              addChar();
              nextToken = GEQUAL_OP;
            }
            else{
              nextToken = GREATER_OP;
            }
            break;
        // Checks for ASSIGN_OP case
        case ':':
            addChar();
            getChar();
            if(nextChar == '='){
              addChar();
              nextToken = ASSIGN_OP;
            }
            break;
        case ';':
          addChar();
          nextToken = SEMICOLON;
          break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse strings */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            //For loop below checks for keywords
            for(int i = 0; i<5;i++){
                if(strcmp(lexeme, keywords[i]) == 0){
                    if(strcmp(lexeme,"read")==0){//each strcmp checks for specific keywords
                        nextToken = KEY_READ;
                        // printf("%s   KEY_READ\n", lexeme);
                        return nextToken;
                    }
                    else if(strcmp(lexeme,"write")==0){
                        nextToken = KEY_WRITE;
                        // printf("%s   KEY_WRITE\n", lexeme);
                        return nextToken;
                    }
                    else if(strcmp(lexeme,"while")==0){
                        nextToken = KEY_WHILE;
                        // printf("%s  KEY_WHILE\n", lexeme);
                        return nextToken;
                    }
                    else if(strcmp(lexeme,"do")==0){
                        nextToken = KEY_DO;
                        // printf("%s     KEY_DO\n", lexeme);
                        return nextToken;
                    }
                    else{
                      nextToken = KEY_OD;
                      // printf("%s     KEY_OD\n", lexeme);
                        return nextToken;
                    }
                    break;
                }
            }
            nextToken = IDENT;
            break;

        // Parse integers 
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        // Parentheses and operators
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        // EOF 
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } // End of switch
    //Switch statement used to format the print statements 
    switch(nextToken){
      case INT_LIT:
        // printf("%s      INT_LIT\n", lexeme);
        break;
      case IDENT:
        // printf("%s      IDENT\n", lexeme);
        break;
      case LESSER_OP:
        // printf("%s      LESSER_OP\n", lexeme);
        break;
      case GREATER_OP:
        // printf("%s      GREATER_OP\n", lexeme);
        break;
      case EQUAL_OP:
        // printf("%s      EQUAL_OP\n", lexeme);
        break;
      case NEQUAL_OP:
        // printf("%s     NEQUAL_OP\n", lexeme);
        break;  
      case LEQUAL_OP:
        // printf("%s     LEQUAL_OP\n", lexeme);
        break;
      case GEQUAL_OP:
        // printf("%s     GEQUAL_OP\n", lexeme);
        break;
      case ASSIGN_OP:
        // printf("%s     ASSIGN_OP\n", lexeme);
        break;
      case ADD_OP:
        // printf("%s      ADD_OP\n", lexeme);
        break;
      case SUB_OP:
        // printf("%s      SUB_OP\n", lexeme);
        break;
      case MULT_OP:
        // printf("%s      MULT_OP\n", lexeme);
        break;
      case DIV_OP:
        // printf("%s      DIV_OP\n", lexeme);
        break;
      case LEFT_PAREN:
        // printf("%s      LEFT_PAREN\n", lexeme);
        break;
      case RIGHT_PAREN:
        // printf("%s      RIGHT_PAREN\n", lexeme);
        break;
      case SEMICOLON:
        // printf("%s      SEMICOLON\n", lexeme);
        break;
      case UNKNOWN:
        // printf("%s      UNKNOWN\n", lexeme);
        break;        
    }
    return nextToken;
}
