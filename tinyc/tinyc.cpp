#define _CRT_SECURE_NO_WARNINGS
/************************************/
/* File: tinyc.cpp                  */
/* Main program of TINY compiler    */
/************************************/

#include "globals.h"

#define NO_PARSE FALSE
#define NO_ANALYZE TRUE
#define NO_CODE TRUE

#include "util.h"

#include "scan.h"
#if !NO_PARSE
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif

// global variables
int lineno = 0;
FILE* source;
FILE* listing;
FILE* code;

int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;

int Error = FALSE;

int main(int argc, char* argv[]) {
  TreeNode* syntaxTree;
  char pgm[20]; // source code name
  if (argc != 2) {
    fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    exit(1);
  }

  strcpy(pgm, argv[1]);
  if (strchr(pgm, '.') == NULL)
    strcat(pgm, ".tny");

  source = fopen(pgm, "r");
  if (source == NULL) {
    fprintf(stderr, "File %s not found\n", pgm);
    exit(1);
  }

  listing = stdout;
  fprintf(listing, "\n TINY COMPILATION: %s\n", pgm);

#if NO_PARSE
  while (getToken() != ENDFILE);
#else
  syntaxTree = parse();
  if (TraceParse) {
    fprintf(listing, "\nSyntax tree:\n");
    printTree(syntaxTree);
  }
#endif

  return 0;
}

