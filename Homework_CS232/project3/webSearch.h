#ifndef HEADERS
#define HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "index.h"
#endif

#ifndef WEBSRCH_H
#define WEBSRCH_H
#define MAX_ADDR_LENGTH 1000
#define MAX_URL_LEN 200

struct listNode{
  char addr[MAX_ADDR_LENGTH];

  struct listNode *next;
};

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr);

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr);

/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *pNode);

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode);

int getLink(const char* srcAddr, char* link, const int maxLinkLength);

#endif
