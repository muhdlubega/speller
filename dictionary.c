// Implements a dictionary's functionality

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// An arbritarily high number of buckets in hash table larger than word count chosen
const unsigned int N = 999999;

// Hash table
node *table[N];

int count = 0;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Open dictionary and return false if null
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Error: Could not open %s\n", dictionary);
        return false;
    }

    //Initiate word array
    char wordplus[LENGTH + 1];

    //Scan strings from dictionary while end of file is not reached
    while (fscanf(dict, "%s", wordplus) != EOF)
    {
        //Allocate memory to store node and return false if null
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        //Copy word into character array using strcpy
        strcpy(n -> word, wordplus);

        //Hash word to obtain a hash value
        int hashword = hash(wordplus);

        //Insert node into hash table and increase word count
        n -> next = table[hashword];
        table[hashword] = n;
        count++;
    }

    //Close dictionary to avoid memory leak
    fclose(dict);
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //Add the ASCII values of the characters in the word together as long as '\0' is not reached
    //Use tolower to ensure spell-checking is case-insensitive
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash += tolower(word[i]);
    }
    //Value of modulus N is taken to ensure it is in the appropriate range
    return (hash % N);
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    //Return the number of words in the dictionary using the word count
    return count;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //Create a copy of word to use the hash function on
    int s = strlen(word);
    char copy[LENGTH + 1];

    //Ensure copy is case-insensitive to avoid spell checking errors
    for (int i = 0; i < s; i++)
    {
        copy[i] = tolower(word[i]);
    }

    //End string at null
    copy[s] = '\0';

    //Hash copy to obtain its hash value and access linked list at that index in the hash table
    int hashcopy = hash(copy);
    node *n = table[hashcopy];

    //Search dictionary for word using strcasecmp, which returns true when word is found
    while (n != NULL)
    {
        if (strcasecmp(copy, n -> word) == 0)
        {
            return true;
        }
        //Else advance to next word if word not found
        n = n -> next;
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        //Make cursor to point at the hash table
        node *n = table[i];

        //Make temporary variable and free the subject of this variable while moving through the linked list
        while (n != NULL)
        {
            node *tmp = n;
            n = n -> next;
            free(tmp);
        }

        //Return true if all nodes in the linked list are freed
        if (n == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}