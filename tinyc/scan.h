#pragma once

#ifndef _SCAN_H_
#define _SCAN_H_

#include "util.h"

#define MAXTOKENLEN 40
extern char tokenString[MAXTOKENLEN + 1];

TokenType getToken(void);

#endif