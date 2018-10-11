#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; // current token

// function prototypes
static TreeNode* stmt_sequence(void);
static TreeNode* statement(void);
static TreeNode* if_stmt(void);
static TreeNode* repeat_stmt(void);
static TreeNode* assign_stmt(void);
static TreeNode* read_stmt(void);
static TreeNode* write_stmt(void);
static TreeNode* exp(void);
static TreeNode* simple_exp(void);
static TreeNode* term(void);
static TreeNode* factor(void);

static void syntaxError(const char* message) {
  fprintf(listing, "\n>>> ");
  fprintf(listing, "Syntax error at line %d: %s", lineno, message);
  Error = TRUE;
}

static void match(TokenType expected) {
  if (token == expected)
    token = getToken();
  else {
    syntaxError("unexpected token -> ");
    printToken(token, tokenString);
    fprintf(listing, "      ");
  }
}

TreeNode* parse(void) {
  TreeNode* t = NULL;
  token = getToken();
  t = stmt_sequence();
  if (token != ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}

////////////////////////////////////
TreeNode* stmt_sequence(void) {
  TreeNode* t = statement(); // first statement
  TreeNode* p = t; // current statement
  while ((token != ENDFILE) && (token != END) && (token != ELSE) && (token != UNTIL)) {
    TreeNode* q; // new statement
    match(SEMI);
    q = statement();
    if (q != NULL) {
      if (t == NULL)
        t = p = q;
      else {
        p->sibling = q;
        p = q;
      }
    }
  }
  return t;
}

TreeNode* statement(void) {
  TreeNode* t = NULL;
  switch (token) {
  case IF:
    t = if_stmt();
    break;
  case REPEAT:
    t = repeat_stmt();
    break;
  case ID:
    t = assign_stmt();
    break;
  case READ:
    t = read_stmt();
    break;
  case WRITE:
    t = write_stmt();
    break;
  default:
    syntaxError("unexpected token -> ");
    printToken(token, tokenString);
    token = getToken();
    break;
  }
  return t;
}

TreeNode* if_stmt(void) {
  TreeNode* t = newStmtNode(IfK);

  match(IF);

  t->child[0] = exp();

  match(THEN);

  t->child[1] = stmt_sequence();

  if (token == ELSE) {
    match(ELSE);
    t->child[2] = stmt_sequence();
  }

  match(END);
  return t;
}

TreeNode* repeat_stmt(void) {
  TreeNode* t = newStmtNode(RepeatK);
  match(REPEAT);
  t->child[0] = stmt_sequence();
  match(UNTIL);
  t->child[1] = exp();
  return t;
}

TreeNode* assign_stmt(void) {
  TreeNode* t = newStmtNode(AssignK);
  t->attr.name = tokenString;
  match(ID);
  match(ASSIGN);
  t->child[0] = exp();
  return t;
}

TreeNode* read_stmt(void) {
  TreeNode* t = newStmtNode(ReadK);
  match(READ);
  if (token == ID)
    t->attr.name = tokenString;
  match(ID);
  return t;
}

TreeNode* write_stmt(void) {
  TreeNode* t = newStmtNode(WriteK);
  match(WRITE);
  t->child[0] = exp();
  return t;
}

TreeNode* exp(void) {
  TreeNode* t = simple_exp();
  if ((token == LT) || (token == EQ)) {
    TreeNode* p = newExpNode(OpK);
    p->child[0] = t;
    p->attr.op = token;
    t = p;
    match(token);
    t->child[1] = simple_exp();
  }
  return t;
}

TreeNode* simple_exp(void) {
  TreeNode* t = term();
  while (token == PLUS || token == MINUS) {
    TreeNode* p = newExpNode(OpK);
    p->child[0] = t;
    p->attr.op = token;
    t = p;
    match(token);
    t->child[1] = term();
  }
  return t;
}

TreeNode* term(void) {
  TreeNode* t = factor();
  while (token == TIMES || token == OVER) {
    TreeNode* p = newExpNode(OpK);
    p->child[0] = t;
    p->attr.op = token;
    t = p;
    match(token);
    t->child[1] = factor();
  }
  return t;
}

TreeNode* factor(void) {
  TreeNode* t = NULL;
  switch (token) {
  case LPAREN:
    match(LPAREN);
    t = exp();
    match(RPAREN);
    break;
  case NUM:
    t = newExpNode(ConstK);
    t->attr.val = atoi(tokenString);
    match(NUM);
    break;
  case ID:
    t = newExpNode(IdK);
    t->attr.name = tokenString;
    match(ID);
    break;
  default:
    syntaxError("unexpected token -> ");
    printToken(token, tokenString);
    token = getToken();
    break;
  }
  return t;
}