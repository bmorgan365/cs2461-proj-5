#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

void printHash(struct hashmap* h);

int main(void){
    struct hashmap* hash = hm_create(5);
    int dBaseCh;                    // used for strcmp of current word to database
    int dBaseVals [15];             // values for each row (1 -10), to keep count of words
    int i;                          // for-loop incrementation
    int index;                      // index of word when storing from file
    char cont = 'n';                // continuation of accepting user input
    char curChar;                   // current character being read from file
    char curWord [15];              // char[] to containing current word being read, "letters between spaces"
    char docID [3];                 // char[] containing document ID
    char filename [10];              // char[] to store filename from user input
    char** wordDbase;               // "string" array with encountered words, 10 words, 15 characters each
    FILE *document;                 // FILE* to access files for data input

    for(i = 0; i < 15; i++){                                        // initialize "num word" values to zero
        dBaseVals[i] = 0;
    }                          
    wordDbase = (char**) malloc(sizeof(char*) * 15);                // initialize wordDbase to null for each of 15 words (15 chars each)
    for(i = 0; i < 15; i++){
        wordDbase[i] = (char*) malloc(sizeof(char) * 15);
        strcpy(wordDbase[i], "");
    }
    
    do{
        printf("Enter the filename to parse: ");
        scanf("%s", filename);

        for(i = 0; i < 2; i++){                                         // doc id is first 2 characters of filename + string terminator
            docID[i] = filename[i];
        }
        docID[2] = '\0';                                                // string terminator
        document = fopen(filename, "r");                                // if file doesn't exist, still open
        index = 0;                                                      // used for first character of word
        do{                                                             // while current character is not EOF     
            curChar = fgetc(document);                                  //get next character      
            if(curChar != ' ' && curChar != EOF){                         
                curWord[index] = curChar;
                index++;
            }else{
                curWord[index] = '\0';                    
                for(i = 0; i < 15; i++){
                    dBaseCh = strcmp(curWord, wordDbase[i]);            // use strcmp to determine whether or not current word exists in database
                    if(dBaseCh == 0){                                   
                        dBaseVals[i]++;                                 // if it exists, increment the respective index in the int[] keeping track
                        hm_put(hash, wordDbase[i], docID, dBaseVals[i]);    //puts current word, document id, num occurrences into hashmap
                        break;
                    }else if(strcmp(wordDbase[i], "") == 0){
                        strcpy(wordDbase[i], curWord);
                        dBaseVals[i]++;
                        hm_put(hash, wordDbase[i], docID, dBaseVals[i]);    //puts current word, document id, num occurrences into hashmap
                        break;
                    }else{
                        continue;
                    }
                }
                index = 0;
            }
                        
        }while(curChar != EOF);
        fclose(document);                               //close file once done manipulation
        
        printf("Parse another document?\nEnter 'y' for yes, any other character to quit. . . ");
        scanf(" %c", &cont);
    }while(cont == 'y' || cont == 'Y');
    printHash(hash);
    printf("hm_get(hash, \"computer\", \"D1\") = %d\n", hm_get(hash, "computer", "D1"));
    printf("hm_get(hash, \"homework\", \"D3\") = %d\n", hm_get(hash, "homework", "D3"));
    hm_remove(hash, "computer", "D1");
    hm_destroy(hash);                                 //deallocate hashmap once done with client
    printf("Hashmap deleted. Exiting. . .\n");
    return 0; 
}
/*
    Used to print hashMap values
*/
void printHash(struct hashmap* h){
    int i;
    printf("Number of Elements: %d\n", h->num_elements);
    for(i = 0; i < h->num_buckets; i++){
        printf("Bucket %d:\n", i);
        if(h->map[i] == NULL){
            printf("Empty\n");
        }else{
            struct llnode* entry = h->map[i];
            while(entry != NULL){
            printf("Word: %s | DocID: %s | Occurrences: %d\n", entry->word, entry->document_id, entry->termFrequency);
            entry = entry->next;
            }
        }
    }
}