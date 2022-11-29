#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <index.h>


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


int main(int argc, char ** argv)
{
    FILE *uFile;

    uFile = fopen(argv[1], "r");//We don't know the format yet

    int MAX_N = argv[2];//Is this our max number of hops?
    int n = 0; //This tracks the current number of pages we have index and crawl

    struct listNode *listStart;
    
    char * currURL;
    char * newURL;
    int maxHops = 0, numOfHops = 0;

    while(fscanf("%s %d", currURL, maxHops) != EOF && n < MAX_N) //read file while file is not EOF and n < MAX N
    {

        while(1)
        {
            if(contains(listStart, currURL) == 0)//maybe a function that accepts a list of URLs and currUrl
            {
                insertBack(listStart, currURL);
                trieGeneration(currURL);
            }

            numOfHops++;

            if(numOfHops <= maxHops && n < MAX_N)
            {
              //get new link, place it as the Current URL
                getLink(currURL, newURL, MAX_ADDR_LENGTH);
                strncpy(currURL, newURL, MAX_ADDR_LENGTH);

                //check if the new curr URL is a dead end
                if(getLink(currURL, newURL, MAX_ADDR_LENGTH) == 0)
                {
                    break;
                }//END IF
            }
            else
            {
                break;
            }//END ELSE IF


        }//END WHILE

        destroyList(listStart);

    }//END WHILE
    

}//END MAIN


/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr){
  // TODO: complete this

  //if list is empty, we return without performing function
  if(pNode == NULL)
  {
    return 0;
  }

  //if strings are the same, we return one
  if(strcmp(addr, pNode->addr) == 0)
  {
    return 1;
  }
  
  //tracks value across multiple calls, this was tricky to figure out
  //it took me a second to realize that they would all default to zero when
  //my final return statement was just zero. The first call on the stack
  //would return zero, even if any subsequent call had returned 1
  int breakout = 0;

  //if list does not end at this node, we continue recursively
  if(pNode->next != NULL)
  {
    breakout = contains(pNode->next, addr);
  }

  return breakout;
}//END Contains
    
void destroyList(struct listNode *pNode){
  // TODO: complete this

  //if this is the last node in the list, we clear memory and quit
  if(pNode->next == NULL)
  {
    free(pNode);
    return;
  }

  //get the node after the current
  struct listNode *nextNode = pNode->next;

  //free memory
  free(pNode);

  //continue recursively down list
  destroyList(nextNode);

  return;
}

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr){
  // TODO: complete this

  //if list is empty, we return
  if(pNode == NULL)
  {
    return;
  }

  //if list does not end here, we allocate and integrate a listNode pointer into our List
  if(pNode->next == NULL)
  {
    struct listNode *nextNode = malloc(sizeof(struct listNode));//allocates memory

    //populates new Node with address and pointer to next node
    strncpy(nextNode->addr, addr, MAX_ADDR_LENGTH);//copies string
    nextNode->next = NULL;

    pNode->next = nextNode;//inserts Node
  }
  else
  {
    insertBack(pNode->next, addr);//if our node is not at the end, then we continue down the list
  }
  
  return;

}

int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
		    break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    
    /* get rid of the newline */
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		    *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}