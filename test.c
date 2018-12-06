#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashmap.h"

void printHash(struct hashmap* hm);
void training(struct hashmap* hm, int docs);

int main(void){
    int numBuckets = 0;
    int numDocs = 0;
    char choice;
    char query [100];
    
    printf("Please enter the number of buckets: ");
    scanf("%d", &numBuckets);
    printf("Please enter the number of documents(1 - 9).\nThey will be interpreted as D1.txt through Dn.txt, where n is the number of documents: ");
    scanf("%d", &numDocs);
    struct hashmap* hm = hm_create(numBuckets);
    training(hm, numDocs);
    printf("Welcome to El Searcho!\n");

    do{
        printf("(S) Search\n(X) Exit\n(Anything else) Exit\nPlease enter selection: ");
        scanf(" %c", &choice);
        if(toupper(choice) == 'S'){
            printf("Please enter your query: ");
            scanf(" %[^\n]s", query);
            read_query(hm, query, numDocs);
            printf("Search Completed!\n");
            //scanf(" %c", &choice);
        }
    }while(toupper(choice) == 'S');

    hm_destroy(hm);                                 //deallocate hashmap once done with client
    printf("Hashmap deleted. Exiting. . .\n");
    return 0; 
}

void training(struct hashmap* hm, int docs){
    int i;                          // for-loop incrementation
    int index;                      // index of word when storing from file
    char curChar;                   // current character being read from file
    char curWord [21];              // char[] to containing current word being read, "letters between spaces"
    char docID [3] = "D_";          // char[] containing document ID
    char filename [7] = "D_.txt";   // char[] to store filename from user input
    FILE *document;                 // FILE* to access files for data input

    for(i = 1; i <= docs; i++){
        docID[1] = i + 48;                              // sets document-id based on for-loop
        filename[1] = i + 48;                           // sets filename based on for-loop
        document = fopen(filename, "r");                // if file doesn't exist, still open
        index = 0;                                      // used for first character of word
        do{                                             // while current character is not EOF     
            curChar = fgetc(document);                  //get next character      
            if(curChar != ' ' && curChar != EOF){                         
                curWord[index] = curChar;
                index++;
            }else{
                curWord[index] = '\0';
                hm_put(hm, curWord, docID, 1);
                index = 0;
            }
                        
        }while(curChar != EOF);
        fclose(document);                               //close file once done manipulation
    }
    stop_word(hm, docs);
}
/*
    Used to print hashMap values
*/
void printHash(struct hashmap* hm){
    int i;
    printf("Number of Elements: %d\n", hm->num_elements);
    for(i = 0; i < hm->num_buckets; i++){
        printf("Bucket %d:\n", i);
        if(hm->map[i] == NULL){
            printf("Empty\n");
        }else{
            struct llnode* entry = hm->map[i];
            while(entry != NULL){
            printf("Word: %s | DocID: %s | Occurrences: %d\n", entry->word, entry->document_id, entry->termFrequency);
            entry = entry->next;
            }
        }
    }
}