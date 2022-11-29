#include "index.h"

//***********************
/* TRIE GEN. FUNCTION*/
//***********************

int trieGeneration(char * URL)
{

  //printf("Entering trieGen\n");

  char currWord[maxWordLen];
  memset(currWord, 0, maxWordLen);//clear word for reuse

  int * totalTerms = malloc(sizeof(int));

  *totalTerms = 0;

  if(totalTerms == NULL)
  {
    printf("Failed to allocate memory for terms\n");
    return 0;
  }

  //This is a requirement - 1
  struct trieNode *root; 

  //printf("Reached IndexPage\n");

  root = indexPage(URL, totalTerms);

  //printf("Reached trieContents\n"); 

  //printf("%s \n", URL);
  //printf("Total Words on WebPage: %d \n", *totalTerms);

  //printTrieContents(root, currWord, strlen(currWord)); //"Call a function the prints out the counts of all the words in the trie" - 2 requirements
  freeTrieMemory(root); //"Call a function to destroy the trie" - 3 requirements

  free(totalTerms);

  return 1;
}

//***********************
/* INDEX PAGE FUNCTION*/
//***********************

struct trieNode* indexPage(const char* url, int * TotalTerms)
{
  //printf("Inside Index\n");

  char *buffer = malloc(sizeof(char) * bufferLim);//allocate buffer memory

  //printf("Buffer acquired\n");

  int bytesRead = getText(url, buffer, bufferLim);//get text from web page

  //printf("Text acquired\n");

  char currWord[maxWordLen]; //I don't expect any words to be longer than 200 characters

  memset(currWord, 0, maxWordLen);

  printf("%s \n", url);//display URL of indexed Site

  //base of our trieNode, initialized
  struct trieNode* newTreeBase = malloc(sizeof(struct trieNode));

  if(newTreeBase == NULL)
  {
    printf("trieNode treeBase allocation failed\n");
    return 0;
  }

  newTreeBase->character = '0';
  newTreeBase->occurrences = -1;

  //printf("treeBase built\n");

  //set children to NULL for base Node
  int i;
  for(i = 0; i < ALPHA_LEN; i++)
  {
    newTreeBase->pointers[i] = NULL;
  }

  //printf("%s", buffer); Testing purposes
  
  int count = 0; //length of our current Word
  for(i = 0; i < bytesRead; i++)
  {

    char currLetter = tolower(buffer[i]);//convert letter

    if( (int)currLetter >= 'a' && (int)currLetter <= 'z' ) //add any lowercase letters to current word
    {
      currWord[count] = currLetter;
      count++;
    }

    if(count == 200 && currWord[0] != 0)//we don't want to surpass 200 letters in length, this seems reasonable since there are no english words longer in length than 200
    {
      printf("\t%s\n", currWord);//display word

      currWord[199] = '\0';//end word

      addWordOccurrence(currWord, count, newTreeBase);//call AddWord...
      (*TotalTerms)++;

      memset(currWord, '\0', 200);//clear word for reuse

      count = 0;//reset count
      
    }

    if( (currLetter < 'a' || currLetter > 'z') && currWord[0] != 0)//only neanderthals use numerous OR statements -Levi (who totally didn't use multiple OR statements)
    {
  
      currWord[count+1] = '\0';//end word

      printf("\t%s\n", currWord); //display word  

      addWordOccurrence(currWord, count, newTreeBase); //call addWord...
      (*TotalTerms)++;

      memset(currWord, '\0', 200);//clear word for reuse

      count = 0;//reset count
      
    } 
    
  }

  free(buffer);
  return newTreeBase;//return trie to main

}//END INDEX

//***********************
/* ADD OCCURRENCE FUNCTION*/
//***********************

int addWordOccurrence(const char* word, const int wordLength, struct trieNode* root)
{

  if(root == NULL || wordLength == 0)//if our root contains nothing, we quit
  {
    return 1;
  }

  //we get the current letter for the sequence of our alphabet
  int currLetter = (int)(word[0]  - 'a');   

  if(root->pointers[currLetter] != NULL )//check if pointer to next node is empty
  {
    //check if that same pointer has a character equal to last letter of word and check word's next letter
    if(word[1] == '\0' && root->pointers[currLetter]->character == word[0])
    {
      root->pointers[currLetter]->occurrences++;//increment, since we reached last letter
    }
    addWordOccurrence(word+1, wordLength - 1, root->pointers[currLetter]);//next recurse
  }
  else if(root->pointers[currLetter] == NULL)//we want to check if we have a node allocated for our current letter, if not then we create the new node
  {

    //create new node
    struct trieNode* newNode = malloc(sizeof(struct trieNode));
    newNode->character = word[0];
    newNode->occurrences = 0;

    int i;
    for(i = 0; i < ALPHA_LEN; i++)
    {
      newNode->pointers[i] = NULL;
    }

    root->pointers[currLetter] = newNode;    

    //this will check for the end of the word
    if(wordLength == 1)
    {
      
      root->pointers[currLetter]->occurrences++;
      return 3; //I put this here just to avoid performing an extra call
    }

    //we increment our word pointer and pass other variables to our recursive call
    addWordOccurrence(word+1, wordLength - 1, root->pointers[currLetter]);

  } 

  return 2;
}//END addWordOccurrence


//***********************
/* PRINT TRIE FUNCTION*/
//***********************

void printTrieContents(struct trieNode * root, char currWord[], int currWordLen)
{
    //check root for info, if NULL; quit
    if(root == NULL)
    {
      return;
    }

    //if occurrence is greater than zero, print word
    if(root->occurrences > 0)
    {
      currWord[currWordLen] = root->character;
      currWord[currWordLen+1] = '\0';

      currWordLen++;

      printf("%s : %d \n", currWord, root->occurrences);
    }
    else if(root->character != '0')//avoids printing the root nodes content (since our root has to have the pointers to the first set of letters, but it cannot be a first letter itself)
    {
      currWord[currWordLen] = root->character;
      currWordLen++;
    }

    char chckPt[maxWordLen];//Use this to restore the word we had upto this point in the recursion tree.
     memset(chckPt, 0, maxWordLen);//clear word for reuse

    strncpy(chckPt, currWord, currWordLen);

    int i;
    //begin loop with recursive calls, this should be alphabetic automatically
    for(i = 0; i < ALPHA_LEN; i++)
    {
      if(root->pointers[i] != NULL)
      {
        strncpy(currWord, chckPt, currWordLen);
        printTrieContents(root->pointers[i], currWord, currWordLen);
      }
    }

    return;

}//END printTrieContents

//***********************
/* FREE MEMORY FUNCTION*/
//***********************

int freeTrieMemory(struct trieNode * root)
{
  //printf("Entering freeTrieMemory"); //Debug code

  //we check root for pointer stuff, if it is empty we just quit
  if(root == NULL)
  {
    return 0;
  }

  int ptrFlag = 0;        

  int i;
  //check each pointer for our root, if they are not NULL, we increase ptrFlag to indicate this
  for(i = 0; i < ALPHA_LEN; i++)
  {
    if(root->pointers[i] != NULL)
    {
      ptrFlag++;
      break;
    }
  }

  //we check if the pointer array is empty, if it is then we go ahead and free the node
  if(ptrFlag == 0)
  {
    free(root);
    return 0;
  }
  else//we proceed to recursively check each member of this node's pointer array for stuff we can free up.
  {
    int i;
    for(i = 0; i < ALPHA_LEN; i++)
    {
      if(root->pointers[i] != NULL)//we want to see if each position has something in it.
      {
        freeTrieMemory(root->pointers[i]);//recursive call
      }
      
    }

    free(root);//free memory

    return 1;
  }

}//END freeTrieMemory

//***********************
/* GET TEXT FUNCTION*/
//***********************

int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;

  int bytesRead;

  //printf("Approaching SNPRINTF\n");

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getText.py", srcAddr);

  //printf("%s\n", buffer);

  //printf("Opening buffer\n");

  pipe = popen(buffer, "r");

  //printf("checking Pipe \n");

  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  //printf("calculating bytes read\n");

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}

