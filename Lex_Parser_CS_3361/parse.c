/* Assignment 4 Matthew Uriegas, R11565427 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "main.c"

static void error();

void stmt();
void expr();
void exprP();
void term();
void termP();
void factor();
void V();
void C();

// extern int nextToken;

int main(int argc, char* argv[]){
    printf("DanC Parser :: R11565427\n");
    for(int i=1; i <= argc; i++){ // i=1, assuming files arguments are right after the executable
        char *fin = argv[i]; //filename
        /* Open the input data file and process its contents */
        if(argc == 1){
          printf("\nERROR - no file provied.\n");
          return 2;
        }
        if (argc == 2) {
          if((in_fp = fopen(fin, "r")) == NULL){
            printf("\nERROR - file %s does not exist.\n", fin);
            return 3;
          }
          else{
            getChar();
            do{
              stmt();
              if(nextToken != EOF){
                printf("\nERROR - unexpected token\n");
                exit(1);
              }
            }while (nextToken != EOF);
            break;
          }
        }
        
    }
    printf("\nSyntax Validated\n");
    return 0;
}
/* stmtC
 * Parses strings in the language
 */
void stmtC(){
    // lex();
    if(nextToken == SEMICOLON){
      // printf("this is a semicolon\n");
      stmt();
    }
    else{
      return;
    }
}

/* stmt
 * Parses strings in the language
 */
void stmt(){
    lex();
    if(nextToken == IDENT){ // Checks for V := E in language
      lex();
      if(nextToken == ASSIGN_OP){
        lex();
        expr();
      }
      stmtC();
    }
    else if(nextToken == KEY_READ || nextToken == KEY_WRITE){ // Checks for read or write function
      lex();
      if(nextToken == LEFT_PAREN){
        lex();
        // printf("this is V\n");
        V();
        lex();
      }
      stmtC();
    }
    else if(nextToken == KEY_WHILE){ // checks for a while loop
      lex();
      C();
      if(nextToken == KEY_DO){
        // printf("this is do\n");
        stmt();
        if(nextToken == KEY_OD){
          lex();
          stmtC();
        }
        else{
          return;
        }
      }
    }
}

/* C
 * Parses strings in the language generated by the rule:
 * C -> E < E | ... | E >= E
 */
void C(){
    expr();
    if(nextToken == LESSER_OP || nextToken == GREATER_OP || nextToken == EQUAL_OP || nextToken == NEQUAL_OP || nextToken == LEQUAL_OP || nextToken == GEQUAL_OP){
      // printf("This is while\n");
      lex();
      expr();
    }
    else{
      printf("\nERROR - unexpected token\n");
      exit(1);
    }
}

/* expr
 * Parses strings in the language generated by the rule:
 * E -> T (+ | -) T
 */
void expr() 
{
    /* Parse the first term */
    term();

    /* As long as the next token is + or -, get
    the next token and parse the next term */
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
} /* End of function expr */

/* term
 * Parses strings in the language generated by the rule:
 * T -> F (* | /) F
 */
void term() 
{
    /* Parse the first factor */
    factor();

    /* As long as the next token is * or /, get the
    next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
            lex();
            factor();
    }
} /* End of function term */

/* factor
 * Parses strings in the language generated by the rule:
 * <factor> -> id | int_constant | ( <expr )
 * */
void factor() 
{
    /* Determine which RHS */
    if (nextToken == IDENT || nextToken == INT_LIT) {
        lex(); /* Get the next token */
    } else {
        /* If the RHS is (<expr>), call lex to pass over the 
        left parenthesis, call expr, and check for the right 
        parenthesis */
        if (nextToken == LEFT_PAREN) {
            lex(); 
            expr();

            if (nextToken == RIGHT_PAREN) {
                lex(); 
            } else { 
                error();
            }
        } /* End of if (nextToken == ... */
        /* It was not an id, an integer literal, or a left parenthesis */
        else 
        { 
            error(); 
        }
    } /* End of else */
} /* End of function factor */

void V(){
    if(nextToken == IDENT)
      lex();
}

static void error() {
    printf("\nERROR - unexpected token\n");
    exit(1);
}
