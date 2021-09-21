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
#ifndef _PHILPHIX_UNITTEST
#define WORD_LENGTH 1000



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

int main(int argc, char **argv) {
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

/* Task 3 */
void readDictionary(char *dictName)
{
    FILE * file ;
    file =  fopen(dictName, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File failed to open!\n");
        exit(61);
    }
    char line[WORD_LENGTH];
    char *delimiter = " \t";
    while(fgets(line, WORD_LENGTH, file) != NULL)
    {
        char *key = (char *)malloc(sizeof(char) * WORD_LENGTH);
        char *string = (char *)malloc(sizeof(char) * WORD_LENGTH);

        key =strcpy(key,strtok(line, delimiter));
        string = strcpy(string,strtok(NULL, delimiter));

        for(char *temp = string; (*temp) != '\0';  temp++)
        {
            if(*temp == '\n')
            {
                *temp = '\0';
                break;
            }
        }
        insertData(dictionary, key, string);
    }
}

/* Task 4 */
void processInput()
{
    char word[WORD_LENGTH];
    char c;
    c = getchar();
    int wordIndex = 0;
    memset(word,0,WORD_LENGTH);
    while (c != EOF)
    {
        //if we encounter a space or a new line we assume we
        //got to end of line without printing or finding a new thing.
        if(c== ' ' | c == '\n'  | !isalnum(c) | (wordIndex>WORD_LENGTH-2)  )
        {
            printf("%s",word);
            printf("%c", c);
            memset(word,0,WORD_LENGTH);
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
            memset(word,0,WORD_LENGTH);
            wordIndex = 0;
            continue;
        }
        //test if this is lower case.
        if( (char *)findData(dictionary, toLowerExceptFirst(word, strlen(word))) )
        {
            printf("%s", (char *)findData(dictionary, toLowerExceptFirst(word, strlen(word))));

            memset(word,0,WORD_LENGTH);
            wordIndex = 0;
            continue;
        }
        //test if this is all lower.
        if( ((char *)findData(dictionary, toLower(word, strlen(word))))     )
        {
            printf("%s", (char *)findData(dictionary, toLower(word, strlen(word))));
            memset(word,0,WORD_LENGTH);
            wordIndex = 0;
            continue;
        }


        wordIndex++;
    }

        printf("%s",word);
        memset(word,0,WORD_LENGTH);


}











/*

  char c;
    char word[60];
    int i = 0;
    char *sic = " [sic]";
    int t1, t2, t3;
    while (c = getchar())
    {

        if (isalpha(c)) {
            word[i] = c;
            i++;
        } else {
            word[i] = '\0';
            if (word[0] != '\0') {
                fprintf(stdout, "%s", word);
                char tmp[60];
                strcpy(tmp, word);
                t1 = t2 = t3 = 0;
                if (findData(dictionary, tmp) != NULL) {
                    t1 = 1;
                }

                int j = 0;
                for (j = 1; tmp[j] != '\0'; j++) {
                    tmp[j] = tolower(tmp[j]);
                }
                if (findData(dictionary, tmp) != NULL) {
                    t2 = 1;
                }


                tmp[0] = tolower(tmp[0]);
                if (findData(dictionary, tmp) != NULL) {
                    t3 = 1;
                }

                if (!t1 && !t2 && !t3) {
                    fprintf(stdout, "%s", sic);
                }
            }

            i = 0;

            if (c != EOF) {
                putchar(c);
            } else {
                break;
            }
        }
    }



 *
 */