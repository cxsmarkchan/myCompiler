#pragma once

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <cstring>
using namespace std;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 8

typedef enum {
  ENDFILE, ERROR,
  IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE, // reserved words
  ID, NUM, // multi-character tokens
  ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI // operations
} TokenType;

extern FILE* source; // source code file
extern FILE* listing; // listing output file
extern FILE* code; // TM code file

extern int lineno; // source line number for listing

#endif
