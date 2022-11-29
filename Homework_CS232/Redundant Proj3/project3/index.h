

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define ALPHA_LEN 26

/* TODO: structure definitions */

struct trieNode{
  int occurrences;
  char character;
  struct trieNode *pointers[ALPHA_LEN]; //we will need a pointer array with the capability to point to 26 memory locations, for each letter of the alphabet
}; 

/* NOTE: int return values can be used to indicate errors (typically non-zero)
   or success (typically zero return value) */

/* TODO: change this return type */
struct trieNode* indexPage(const char* url, int TotalTerms); //Need to change this to return a root node

int addWordOccurrence(const char* word, const int wordLength, struct trieNode* root, int TotalTerms); //added root into params
		       /* TODO: other parameters you need */
           //Is this to be recursive? we could include a current place and trie pointer

void printTrieContents(struct trieNode * root, char currWord[], int currWordLen, int TotalTerms);
//pass the trie root, we could make this recursive 

int freeTrieMemory(struct trieNode * root);
//we will only need to pass the trie root, this will be recursive

int getText(const char* srcAddr, char* buffer, const int bufSize);


int trieGeneration(char * URL)
{
  const int maxLen = 200;//here is our magic number

  char currWord[maxLen];

  int * totalTerms = 0;

  //This is a requirement - 1
  struct trieNode *root; 
  root = indexPage(URL, totalTerms); 

  printTrieContents(root, currWord, strlen(currWord), totalTerms); //"Call a function the prints out the counts of all the words in the trie" - 2 requirements
  freeTrieMemory(root); //"Call a function to destroy the trie" - 3 requirements
}

/* TODO: change this return type */
struct trieNode* indexPage(const char* url, int TotalTerms)
{

  char *buffer = malloc(sizeof(char*) * 300000);//allocate buffer memory

  int bytesRead = getText(url, buffer, 300000);//get text from web page

  char currWord[200]; //I don't expect any words to be longer than 200 characters

  printf("%s \n", url);//display URL of indexed Site

  //base of our trieNode, initialized
  struct trieNode* newTreeBase = malloc(sizeof(struct trieNode));
  newTreeBase->character = '0';
  newTreeBase->occurrences = -1;

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
      printf("\t %s \n", currWord);//display word

      currWord[199] = '\0';//end word

      addWordOccurrence(currWord, count, newTreeBase, TotalTerms);//call AddWord...

      memset(currWord, 0, 200);//clear word for reuse

      count = 0;//reset count
      
    }

    if( (currLetter < 'a' || currLetter > 'z') && currWord[0] != 0)//only neanderthals use numerous OR statements -Levi (who totally didn't use multiple OR statements)
    {
  
      currWord[count+1] = '\0';//end word

      printf("\t %s \n", currWord); //display word  

      addWordOccurrence(currWord, count, newTreeBase, TotalTerms); //call addWord...

      memset(currWord, 0, 200);//clear word for reuse

      count = 0;//reset count
      
    } 
    
  }

  free(buffer);
  return newTreeBase;//return trie to main

}

//we want to send our trieNode with this
int addWordOccurrence(const char* word, const int wordLength, struct trieNode* root, int TotalTerms)
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
      TotalTerms++;
    }
    addWordOccurrence(word+1, wordLength - 1, root->pointers[currLetter], TotalTerms);//next recurse
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
      TotalTerms++;
      return 3; //I put this here just to avoid performing an extra call
    }

    //we increment our word pointer and pass other variables to our recursive call
    addWordOccurrence(word+1, wordLength - 1, root->pointers[currLetter], TotalTerms);

  } 

  return 2;
}//END addWordOccurrence

void printTrieContents(struct trieNode * root, char currWord[], int currWordLen, int TotalTerms)
{

    //check root for info, if NULL; quit
    if(root == NULL)
    {
      return;
    }

    printf("Total Words on WebPage: %d \n", TotalTerms);

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

    char chckPt[200];//Use this to restore the word we had upto this point in the recursion tree.

    strncpy(chckPt, currWord, currWordLen);

    int i;
    //begin loop with recursive calls, this should be alphabetic 
    for(i = 0; i < ALPHA_LEN; i++)
    {
      if(root->pointers[i] != NULL)
      {
        strncpy(currWord, chckPt, currWordLen);
        printTrieContents(root->pointers[i], currWord, currWordLen, TotalTerms);
      }
    }

    return;

}//END printTrieContents


int freeTrieMemory(struct trieNode * root)
{
  //printf("Entering freeTrieMemory");

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

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}