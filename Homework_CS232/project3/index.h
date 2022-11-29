#ifndef INDEX_H
#define INDEX_H

#ifndef HEADERS
#define HEADERS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#endif

#define ALPHA_LEN 26
#define bufferLim 300000
#define maxWordLen 200

/* TODO: structure definitions */

struct trieNode{
  int occurrences;
  char character;
  struct trieNode *pointers[ALPHA_LEN]; //we will need a pointer array with the capability to point to 26 memory locations, for each letter of the alphabet
}; 

/* NOTE: int return values can be used to indicate errors (typically non-zero)
   or success (typically zero return value) */

int trieGeneration(char * URL);

struct trieNode* indexPage(const char* url, int *TotalTerms); //Need to change this to return a root node

int addWordOccurrence(const char* word, const int wordLength, struct trieNode* root);

void printTrieContents(struct trieNode * root, char currWord[], int currWordLen);

int freeTrieMemory(struct trieNode * root);

int getText(const char* srcAddr, char* buffer, const int bufSize);

#endif
