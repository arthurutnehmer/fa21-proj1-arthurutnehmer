#include "hashtable.h"

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),int (*equalFunction)(void *, void *))
{
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  if (NULL == newTable)
  {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newTable->size = size;
  newTable->buckets = malloc(sizeof(struct HashBucketEntry *) * size);
  if (NULL == newTable->buckets)
  {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  for (i = 0; i < size; i++)
  {
    newTable->buckets[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/* Task 1.2 */
void insertData(HashTable *table, void *key, void *data)
{

    unsigned int bucketIndex = (table->hashFunction(key)) % (table->size);

    //what will the hash be stored as.
    struct HashBucketEntry *newEntry = (struct HashBucketEntry *) malloc(sizeof(struct HashBucketEntry));
    newEntry->data = data;
    newEntry->next = NULL;
    newEntry->key = key;
    table->buckets[bucketIndex] = newEntry;


}

/* Task 1.3 */
void *findData(HashTable *table, void *key)
{
  unsigned int bucketIndex = (table->hashFunction(key)) % table->size;
  struct HashBucketEntry *currentNode = table->buckets[bucketIndex];

  while( (currentNode != NULL) )
  {
      if(table->equalFunction(key, currentNode->key) != 0)
      {
          return currentNode->data;
      }

      currentNode = currentNode->next;
  }

    return NULL;

}

/* Task 2.1 */
unsigned int stringHash(void *s)
{
    char *string = (char *) s;
    unsigned int hash = 10000;
    int c;
    while ((c = *string++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/* Task 2.2 */
int stringEquals(void *s1, void *s2)
{
    if (strcmp((char *) s1, (char *) s2) == 0)
    {
        return 1;
    } else
    {
        return 0;
    }

}