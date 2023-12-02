#ifndef PRECEDENCE_H
#define PRECEDENCE_H
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "compiler.h"
#include "dll.h"

bool reduce_exp(char *r);

void pushLess(DLList *list, char c);

void pushEqual(DLList *list, char c);

void reduce(DLList *list);

#endif