/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

//root node of trie
node root;

unsigned int wordstotal;

bool freemem(node* nod);


bool check(const char* word)
{
    node* pointer = &root;
    unsigned int index;
    unsigned int wordpos = 0;
    while(word[wordpos]!= '\0')
    {
        //printf("%c", word[wordpos]);
        if (isalpha(word[wordpos]))
        {
            index = (int) tolower(word[wordpos]) - 'a';
            //If bucket doesnt exist, exit return false word does not exist
            if(pointer->child[index]==NULL)
            {
                return false;
            }
            pointer = pointer->child[index];
        }
        else if(word[wordpos]=='\'')//Apostrophe
        {
            if(pointer->child[26]==NULL)
            {
                return false;
            }
            else
            {
                pointer = pointer->child[26];
            }
        }
        wordpos++;//Move through the words
    }
    if(pointer->is_word)
    {
        return true;
    }//Once the end of the word is reached, is there a marker present.
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    unsigned int index;
    FILE* dict = fopen(dictionary, "r");
    if (dict==NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    node *pointer = &root;
    for(char c = fgetc(dict) ; ;c = fgetc(dict))
    {
        if (isalpha(c))
        {
            index = (int) c - 'a';
            
            if (pointer->child[index]==NULL)//Does not exist
            {
                pointer->child[index] = (node*) malloc(sizeof(node));
            }
                pointer = pointer->child[index];//moves down the word letter by letter
        }
        else
        {
            if (c=='\n')//newline ascii
            {
                if(!pointer->is_word)//Don't count duplicate words
                {
                    pointer->is_word = true;
                    wordstotal++;
                }
                pointer = &root;
            }  
            else if (c=='\'')//apostrophe gets put into last bucket = 26
            {
                //printf("letter from text is /'");
                if (pointer->child[26]==NULL)
                {
                    pointer->child[26] = (node*) malloc(sizeof(node));
                }
                pointer = pointer->child[26];
            }
            if (c==EOF)//Break at end of file.
            {
                //printf("Break\n");
                break;   
            }   
        }
    }
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    //printf("%i", wordstotal);
    return wordstotal;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
//    freemem(root);
    for (int x = 0 ; x < 27 ; x++) //Root node not declared via malloc
    {
        if (root.child[x]!=NULL)
        {
            freemem(root.child[x]);//calls the freemem function for all nodes
                                  //that have trees or leaves
        }
    }

    return true; //exit success
}


bool freemem(node* nod)
{
    for(int i = 0 ; i < 27 ; i++)
    {
        if(nod->child[i] != NULL)
        {
            freemem(nod->child[i]); //Recursively frees all nodes
            //free(nod->child[i]);
        }
        
    }
    if (nod!=NULL)
    {
        free(nod);
    }
    return true; //exit success
}