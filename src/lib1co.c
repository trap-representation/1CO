#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <emscripten.h>

#include "errors.h"
#include "config.h"

enum block {
  BLOCK_OPERAND1,
  BLOCK_OPERAND2,
  BLOCK_SWAP,
  BLOCK_SIGN,
  BLOCK_ZERO,
  BLOCK_JUMP,
  BLOCK_STORE,
  BLOCK_LOAD,
  BLOCK_NULL,
  BLOCK_IMPL1,
  BLOCK_IMPL2
};

EMSCRIPTEN_KEEPALIVE enum error eval(char *code, size_t ssize, int *tape, int *blocks) {
  size_t bp = 0;

  for (size_t i = 0; i < 11; i++) {
    blocks[i] = 0;
  }

  uintmax_t cycles = 0;

  for (size_t i = 0; cycles < MAX_CYCLES && i < ssize; i++) {
    cycles++;

    switch(code[i]) {
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
    case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
      if (bp == BLOCK_OPERAND1) {
	if (!blocks[BLOCK_SIGN]) {
	  blocks[BLOCK_OPERAND1]++;
	}
	else {
	  blocks[BLOCK_OPERAND1]++;
	}
      }
      else if (bp == BLOCK_OPERAND2) {
	if (!blocks[BLOCK_SIGN]) {
	  blocks[BLOCK_OPERAND2]++;
	}
	else {
	  blocks[BLOCK_OPERAND2]++;
	}
      }
      else if (bp == BLOCK_SWAP) {
	int t = blocks[BLOCK_OPERAND1];
	blocks[BLOCK_OPERAND1] = blocks[BLOCK_OPERAND2];
	blocks[BLOCK_OPERAND2] = t;
      }
      else if (bp == BLOCK_SIGN) {
	if (!blocks[BLOCK_SIGN]) {
	  blocks[BLOCK_SIGN] = 1;
	}
	else {
	  blocks[BLOCK_SIGN] = 0;
	}
      }
      else if (bp == BLOCK_ZERO) {
	  blocks[BLOCK_OPERAND1] = 0;
      }
      else if (bp == BLOCK_JUMP) {
	if (blocks[BLOCK_OPERAND2]) {
	  if (!blocks[BLOCK_SIGN]) {
	    i += blocks[BLOCK_OPERAND1] - 1;
	  }
	  else {
	    i -= blocks[BLOCK_OPERAND1] - 1;
	  }
	}
      }
      else if (bp == BLOCK_STORE) {
	tape[blocks[BLOCK_OPERAND2]] = blocks[BLOCK_OPERAND1];
      }
      else if (bp == BLOCK_LOAD) {
	blocks[BLOCK_OPERAND1] = tape[blocks[BLOCK_OPERAND2]];
      }
      else if (bp == BLOCK_IMPL1 || bp == BLOCK_IMPL2) {
	continue;
      }
      break;

    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
    case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
      if (bp == 10) {
	bp = 0;
      }
      else {
	bp++;
      }
      break;
    }
  }

  return ERR_SUCCESS;
}
