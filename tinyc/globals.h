#pragma once

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <cstring>
#include <string>
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

// Syntax Tree
typedef enum {StmtK, ExpK} NodeKind;
typedef enum {IfK, RepeatK, AssignK, ReadK, WriteK} StmtKind;
typedef enum {OpK, ConstK, IdK} ExpKind;
typedef enum {Void, Integer, Boolean} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode {
  treeNode* child[MAXCHILDREN];
  treeNode* sibling;
  int lineno;
  NodeKind nodeKind;
  struct { StmtKind stmt; ExpKind exp;} kind;
  struct {
    TokenType op;
    int val;
    string name;
  } attr;
  ExpType type;
} TreeNode;

extern int EchoSource; // whether to echo the listing file with line numbers during parse

extern int TraceScan; // whether to print token information
extern int TraceParse;
extern int TraceAnalyze;
extern int TraceCode;

extern int Error;

#endif
