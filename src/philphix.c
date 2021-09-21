/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
#define WORD_LENGTH 60
#ifndef _PHILPHIX_UNITTEST
#define WORD_LENGTH 60



char *toLower(char *str, size_t len)
{
    char *str_l = calloc(len+1, sizeof(char));

    for (size_t i = 0; i < len; ++i)
    {
        str_l[i] = tolower((unsigned char)str[i]);
    }
    return str_l;
}

char *toLowerExceptFirst(char *str, size_t len)
{
    char *str_l = calloc(len+1, sizeof(char));

    for (size_t i = 0; i < len; ++i)
    {
        str_l[i] = tolower((unsigned char)str[i]);
    }
    str_l[0] = str[0];
    return str_l;
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}
#endif /* _PHILPHIX_UNITTEST */

/* Task 3 */void readDictionary(char *dictName)
{
    FILE * file ;
    file =  fopen(dictName, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File failed to open!\n");
        exit(61);
    }

    char ch;
    ch = fgetc(file);
    char *word = (char *)malloc(sizeof(char) * WORD_LENGTH);
    char *key = (char *)malloc(sizeof(char) * WORD_LENGTH);
    int thisOne = 1;
    int thatOne = 0;
    int currentWordLengthCap = WORD_LENGTH;
    while(ch != EOF)
    {
        if(ch == ' ' | ch == '\t' | ch == '\n')
        {
            ch = fgetc(file);
            continue;
        }

        int wordIndex = 0;
        while( (ch != EOF) & (ch != '\t') & (ch != '\n') & (ch != ' ')  )
        {
            //if to large, realloc.
            if(wordIndex+1 > currentWordLengthCap)
            {
                word = realloc(word, sizeof(char) * currentWordLengthCap*2);
                currentWordLengthCap = currentWordLengthCap*2;
            }
            word[wordIndex] = ch;
            ch = fgetc(file);
            wordIndex++;
        }

        if(thisOne)
        {
            //give key more memoery since it could be larger.
            key = realloc(key, sizeof(char) * currentWordLengthCap);
            strncpy(key, word, strlen(word));
            thatOne = 1;
            thisOne = 0;
        }
        else if(thatOne)
        {
            //another friggen key
            char *anotherDamnkey = (char *)malloc(sizeof(char) * strlen(key)+1);
            char *theString = (char *)malloc(sizeof(char) * strlen(word)+1);
            strcpy(theString, word);
            strcpy(anotherDamnkey, key);

            insertData(dictionary, key,theString);
            //reset the key memeory
            remove(key);
            thatOne = 0;
            thisOne = 1;
            currentWordLengthCap = WORD_LENGTH;
            key = (char *)malloc(sizeof(char) * WORD_LENGTH);
        }
        wordIndex = 0;
        //reset the memeory
        remove(word);
        word = (char *)malloc(sizeof(char) * WORD_LENGTH);
        ch = fgetc(file);

    }
}


/* Task 4 */

void processInput()
{
    char *word = (char *)malloc(sizeof(char) * WORD_LENGTH);
    char c;
    c = getchar();
    int wordIndex = 0;
    int currentLargestWord = WORD_LENGTH;
    while (c != EOF)
    {
        //if we encounter a space or a new line we assume we
        //got to end of line without printing or finding a new thing.
        if(wordIndex+1> currentLargestWord )
        {
            word = realloc(word, sizeof(char) * currentLargestWord*2);
            currentLargestWord = currentLargestWord*2;
        }

        if(c== ' ' | c == '\n'  | !isalnum(c) )
        {
            printf("%s",word);
            printf("%c", c);
            //clear the word.
            remove(word);
            word = (char *)malloc(sizeof(char) * WORD_LENGTH);
            //get next char
            c = getchar();
            wordIndex = 0;
            continue;
        }

        //add current character to word as its not empty or new line
        word[wordIndex]= c;
        //if we find the word is in the dictionary we change it
        //check for exact
        c = getchar();
        //test if this is a large number.
        if(isdigit(c) & isdigit(word[wordIndex]))
        {
            wordIndex++;
            continue;
        }
        //test if this is exact.
        if( ((char *)findData(dictionary, word)))
        {
            printf("%s", (char *)findData(dictionary, word));
            //remove word
            remove(word);
            word = (char *)malloc(sizeof(char) * WORD_LENGTH);
            wordIndex = 0;
            continue;
        }
        //test if this is lower case.
        if( (char *)findData(dictionary, toLowerExceptFirst(word, strlen(word))) )
        {
            printf("%s", (char *)findData(dictionary, toLowerExceptFirst(word, strlen(word))));

            remove(word);
            word = (char *)malloc(sizeof(char) * WORD_LENGTH);
            wordIndex = 0;
            continue;
        }
        //test if this is all lower.
        if( ((char *)findData(dictionary, toLower(word, strlen(word))))     )
        {
            printf("%s", (char *)findData(dictionary, toLower(word, strlen(word))));
            remove(word);
            word = (char *)malloc(sizeof(char) * WORD_LENGTH);
            wordIndex = 0;
            continue;
        }
        wordIndex++;
    }
        printf("%s",word);
        remove(word);
}


