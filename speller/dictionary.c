// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <cs50.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int word_counter = 0;

// TODO: Choose number of buckets in hash table
// value for first version
const unsigned int N = 676;

// value for second version (slower)
//const unsigned int N = 100000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    //hash the word --> returns index
    int hash_value = hash(word);

    //access hash table at that index using cursor
    node *cursor = table[hash_value];

    //traverse the linked list at that index and look for word with strcasecmp
    while (cursor != NULL)
    {
        int n = strcasecmp(word, cursor->word);

        if (n == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    //return toupper(word[0]) - 'A'; //original function!


    // first version, considering the first to characters of each word
    int m = 0;

    if (sizeof(word) == 1)
    {
        m = (toupper(word[0]) - 'A') * sqrt(N);
    }

    else if (strcasecmp(&word[1], "'") == 0)
    {
        m = (toupper(word[0]) - 'A') * sqrt(N);
    }

    else
    {
        m = (toupper(word[0]) - 'A') * sqrt(N) + (toupper(word[1]) - 'A');
    }

    return m % N;

    //second version, using the sum of each character in a word (slower)
    //int sum = 0;
    //for (int i = 0; i < sizeof(word); i++)
    //{
    //    sum = sum + (toupper(word[i]) - 'A');
    //}

    //return sum % N;

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // --> open dictionary file: use fopen, check if return value is NULL! (if so, return false)
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // --> read strings from file one at a time: fscanf(file, "%s", word) for every word in dictionary. fscanf returns EOF if end of file. file = file pointer (result from calling fopen on dictionary!). word = character-array, where the word is stored.

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        // --> create new node for each word: allocate memory, check for NULL, copy word into node using strcpy
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Not enough memory.\n");
            return false;
        }
        strcpy(n->word, word);
        n->next = NULL;

        // --> Hash the word to obtain a hash value: call the hash-function inside the load-function to determine at which index in the hashtable to place the node
        int hash_value = hash(word);

        // --> insert node into hash table at that location: add node to linked list. First: set pointer of new word to second element in linked list. second: set pointer of first element in linked list to new node.
        n->next = table[hash_value];
        table[hash_value] = n;

        word_counter++;
    }

    // Close file
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (word_counter == 0)
    {
        return 0;
    }

    else
    {
        //printf("%i\n", word_counter);
        return word_counter;
    }

}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    node *cursor;
    node *tmp;

    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}
