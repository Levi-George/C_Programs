#include "webSearch.h"
#define true 1
#define false NULL
#define bufSize 1000

//***********************
/* MAIN FUNCTION*/
//***********************

int main(int argc, char ** argv)
{
  //printf("start main \n");
  
  FILE *uFile;

  if(argc < 2)
  {
    printf("WEBSEARCH USAGE: webSearch arg1(fileLocation) arg2(Hops) arg3(random seed)");
  }

  uFile = fopen(argv[1], "r");//Open our file

  if(uFile == NULL)//ensure file opened
  {
    printf("File Open Failed");
    return 0;
  }

  //printf("%s %s\n", argv[1], argv[2]);

  int MAX_N = atoi(argv[2]);//Is this our max number of hops?

  //set seed if it was provided
  if(argc == 4)
  {
    //printf("Random Seed Set");
    srand( atol(argv[3]));
  }

  int n = 0; //This tracks the current number of pages we have indexed and crawled

  //data allocation, initialization
  char * currURL = malloc(sizeof(char)* MAX_ADDR_LENGTH);
  char * newURL = malloc(sizeof(char)* MAX_ADDR_LENGTH);
  int *maxHops = malloc(sizeof(int)); 
  int numOfHops = 0, charsRead = 0;

  struct listNode* startList = NULL;
  
  printf("Indexing....\n");
  while((feof) && n < MAX_N)
  {
    //get info from file
    fscanf(uFile, "%s %d", currURL, maxHops, charsRead);

    //printf("MAX_N: %d", MAX_N);

    //printf("Address Retrieved: %s|\n", currURL);

    while(numOfHops < (*maxHops))
    {

      if(contains(startList, currURL) == 0 && startList != NULL)
      {
        //printf("URL: %s | not indexed \n", currURL);

        //url has not been indexed
        insertBack(startList, currURL);
        trieGeneration(currURL);
      }
      else if(contains(startList, currURL) == 1)
      {
        //printf("URL: %s | indexed \n", currURL);

        //url has been indexed
        numOfHops--;
      }
      else if(startList == NULL)
      {
        //printf("NULL StartList\n");

        startList = malloc(sizeof(struct listNode));
        startList->next = NULL;
        strncpy(startList->addr, currURL, MAX_ADDR_LENGTH);

        trieGeneration(currURL);
      }
    
      
      numOfHops++;
                  
      if(numOfHops < *maxHops && n < MAX_N)
      {
        int res = getLink(currURL, newURL, MAX_ADDR_LENGTH);

        //printf("Hopped Link: %s\n", newURL);

        if(!res)//this should execute when we have a dead link
        {
          break;
        }

        strncpy(currURL, newURL, MAX_ADDR_LENGTH);

      }
      else//if we have reached max hops or max urls crawled
      {
        break;
      }//END ELSE

    }//END WHILE TRUE

    n++;

  }//END WHILE feof

  destroyList(startList);
  free(currURL);
  free(newURL);
  free(maxHops);
  fclose(uFile);
  
}//END MAIN


//***********************
/* CONTAINS FUNCTION*/
//***********************

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr){
  // TODO: complete this

  //printf("\nEntering Contains:\n");
  //printf("%s", addr);

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
  
  //printf("\nContains: URLs compared\n");


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

  //printf("Checked for End of List\n");

  return breakout;
}//END Contains
    

//***********************
/* DESTROY LIST FUNCTION*/
//***********************

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

//***********************
/* CONTAINS FUNCTION*/
//***********************

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
  

  //printf("insertBack Complete\n");
  return;

}

//***********************
/* GETLINK FUNCTION*/
//
//***********************

int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  
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
}//END GETLINK FUNCTION