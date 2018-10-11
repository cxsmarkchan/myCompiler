#include "globals.h"
#include "util.h"
#include "scan.h"

char tokenString[MAXTOKENLEN + 1];

typedef enum {
  START, INNUM, INASSIGN, INCOMMENT, INID, DONE
} StateType;

#define LINELEN 256
static char lineBuf[LINELEN + 1];
static int lineLen = 0;
static int linePos = 0;

char getNextChar(void) {
  while (linePos >= lineLen) {
    if (!fgets(lineBuf, LINELEN, source)) {
      linePos++;
      return EOF;
    }
    lineno++;
    lineLen = strlen(lineBuf);
    linePos = 0;
    fprintf(listing, "%4d: %s", lineno, lineBuf);
    if (lineLen > 0 && lineBuf[lineLen - 1] != '\n')
      fprintf(listing, "\n");
  }

  char cur = lineBuf[linePos];
  linePos++;
  return cur;
}

void ungetChar(void) {
  linePos--;
}

static TokenType checkReserved(const char* token) {
  if (strcmp(token, "if") == 0)
    return IF;
  if (strcmp(token, "then") == 0)
    return THEN;
  if (strcmp(token, "else") == 0)
    return ELSE;
  if (strcmp(token, "end") == 0)
    return END;
  if (strcmp(token, "repeat") == 0)
    return REPEAT;
  if (strcmp(token, "until") == 0)
    return UNTIL;
  if (strcmp(token, "read") == 0)
    return READ;
  if (strcmp(token, "write") == 0)
    return WRITE;
  return ID;
}

TokenType getToken(void) {
  TokenType currentToken = ENDFILE;
  StateType state = START;
  memset(tokenString, 0, sizeof(tokenString));
  int tokenPos = 0;

  char c = getNextChar();
  if (c == EOF)
    currentToken = ENDFILE;
  else {
    while (state != DONE) {
      if (state != INCOMMENT)
        tokenString[tokenPos++] = c;

      switch (state) {
      case START:
        if (c == ' ' || c == '\n') {
          state = START;
          tokenString[--tokenPos] = 0;
        } else if (c >= '0' && c <= '9') {
          currentToken = NUM;
          state = INNUM;
        } else if (c >= 'a' && c <= 'z') {
          currentToken = ID;
          state = INID;
        } else if (c == ':') {
          currentToken = ERROR;
          state = INASSIGN;
        } else if (c == '{') {
          currentToken = ENDFILE;
          state = INCOMMENT;
          tokenString[--tokenPos] = 0;
        } else if (c == '+') {
          currentToken = PLUS;
          state = DONE;
        } else if (c == '-') {
          currentToken = MINUS;
          state = DONE;
        } else if (c == '*') {
          currentToken = TIMES;
          state = DONE;
        } else if (c == '/') {
          currentToken = OVER;
          state = DONE;
        } else if (c == '=') {
          currentToken = EQ;
          state = DONE;
        } else if (c == '<') {
          currentToken = LT;
          state = DONE;
        } else if (c == '(') {
          currentToken = LPAREN;
          state = DONE;
        } else if (c == ')') {
          currentToken = RPAREN;
          state = DONE;
        } else if (c == ';') {
          currentToken = SEMI;
          state = DONE;
        } else {
          currentToken = ERROR;
          state = DONE;
        }
        break;
      case INCOMMENT:
        if (c == '}') {
          state = START;
        }
        break;
      case INNUM:
        if (!(c >= '0' && c <= '9')) {
          state = DONE;
          tokenString[--tokenPos] = 0;
          ungetChar();
        }
        break;
      case INID:
        if (!(c >= 'a' && c <= 'z')) {
          state = DONE;
          tokenString[--tokenPos] = 0;
          ungetChar();
        }
        break;
      case INASSIGN:
        if (c == '=') {
          currentToken = ASSIGN;
        } else {
          currentToken = ERROR;
          tokenString[--tokenPos] = 0;
          ungetChar();
        }
        state = DONE;
        break;
      default:
        fprintf(listing, "Unknown State: %d\n", state);
        break;
      }
      if (state != DONE) {
        c = getNextChar();
      }
    }
  }

  if (currentToken == ID) {
    currentToken = checkReserved(tokenString);
  }

  if (TraceScan) {
    fprintf(listing, "%8d: ", lineno);
    printToken(currentToken, tokenString);
  }

  return currentToken;
}