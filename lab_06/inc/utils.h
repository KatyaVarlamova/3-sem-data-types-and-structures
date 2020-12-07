#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "balanced_tree.h"
#include "file.h"
#include "hash_table.h"
#include "tree.h"
#include "utils.h"

#define OK           0
#define MEMORY_ERROR 1
#define ERR          2

void statistics(char *filename);

#endif
