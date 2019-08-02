#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "elf_32.h"
#include "elf_64.h"

unsigned char elf_is_32_64(unsigned char *input){
	return input[4];
}