#pragma once

#ifndef _UTIL_H_
#define _UTIL_H_

#include "globals.h"

void printToken(TokenType, const char*);

// syntax tree
TreeNode* newStmtNode(StmtKind);
TreeNode* newExpNode(ExpKind);

void printTree(TreeNode*);

#endif
