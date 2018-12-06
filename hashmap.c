#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**
 * Allocate a new HashMap with the specified number of buckets
 */
struct hashmap* hm_create(int num_buckets){
    int i;    
    // allocate memory for hashmap struct
    struct hashmap* hm = (struct hashmap*) malloc(sizeof(struct hashmap));
    hm->num_buckets = num_buckets;
    hm->num_elements = 0;
    // allocate memory for map(llnode**)
    hm->map = (struct llnode**) malloc(sizeof(struct llnode*) * num_buckets);
    // allocate memory for llnode elements of map and set next llnode to null
    for(i = 0; i < num_buckets; i++){
        hm->map[i] = NULL;
    }
    return hm;
}

/**
 * return the value associated with the key that is passed in within the HashMap that is passed in.
 * If the element is not found, return -1
 */
int hm_get(struct hashmap* hm, char* word, char* document_id){
    int bucket = hash(hm, word);
    int docSame, wordSame;
    struct llnode* trav;
   
    /*
        if bucket is null:
        1) no llnode exists
        2) "key" doesn't exist
        3) return -1
    */
    if(hm->map[bucket] == NULL){
        return -1;
    }

    /*
        traverse through bucket, comparing current node's key
        with parameter key, return value(termFrequency) if keys are equals
        else continue traversal
    */
    trav = hm->map[bucket];
    while(trav->next != NULL){
        docSame = strcmp(trav->document_id, document_id);
        wordSame = strcmp(trav->word, word);
        if(!docSame && !wordSame){
            return trav->termFrequency;
        }
        trav = trav->next;
    }
    
    /*
        At the last node of bucket, check if keys are equal
        return value(termFrequency) if equal
        otherwise return -1
    */
    docSame = strcmp(trav->document_id, document_id);
    wordSame = strcmp(trav->word, word);
    if(!docSame && !wordSame){
        return trav->termFrequency;
    }

    return -1;
}

/**
 * Put the key value pair into the HashMap that is passed in. If the word and document id
 * combination already exists within the HashMap, its value will be overwritten with the new one
 */
void hm_put(struct hashmap* hm, char* word, char* document_id, int termFrequency){
    int docSame, wordSame;                                  //stores value of strcmp                 
    int bucket = hash(hm, word);               //determines bucket
    char* d = malloc(strlen(document_id) + 1);            //copy "word" so it persists through mult. file entry
    strcpy(d, document_id);                               //copied "word"
    char* w = malloc(strlen(word) + 1);
    strcpy(w, word);
    struct llnode* trav;                                    //used for traversal
    struct llnode* add;                                     //new llnode with parameter attributes
    add = (struct llnode*) malloc(sizeof(struct llnode));   
    add->document_id = d;
    add->termFrequency = termFrequency;
    add->word = w;
    add->next = NULL;
    
    /*
        if bucket is null:
        1) no llnode exists
        2) "key" doesn't exist
        3) set head(hm->map[num_buckets]) equal to "add" node
        4) return
    */
    if(hm->map[bucket] == NULL){
        hm->map[bucket] = add;
        hm->num_elements++;
        return;
    }

    /*
        Traverse through list, if entry exists with same "key", 
        replace with parameter value
    */
    trav = hm->map[bucket];
    while(trav->next != NULL){
        docSame = strcmp(trav->document_id, add->document_id);
        wordSame = strcmp(trav->word, add->word);
        if(!docSame && !wordSame){
            trav->termFrequency++;
            return;
        }
        trav = trav->next;
    }
    /*
        no llnode in the bucket has the same "key" and at last node
        of list, check if has same "key", otherwise
        add "add" llnode to end, and increment count
    */
    docSame = strcmp(trav->document_id, add->document_id);
    wordSame = strcmp(trav->word, add->word);
    if(!docSame && !wordSame){
        trav->termFrequency++;
        return;
    }else{
        trav->next = add;
        hm->num_elements++;
    }
    // free(d);
    // free(w);
    // free(add);
}

/**
 * Remove the key value pair in the HashMap that is associated with the given key
 */
void hm_remove(struct hashmap* hm, char* word, char* document_id){
    int bucket = hash(hm, word);           //hash value
    int wordSame, docSame;                              //store values of strcmp
    struct llnode* prev;                                //used for traversal
    struct llnode* cur;

    /*
        if bucket is null:
        1) no llnode exists
        2) "key" doesn't exist
        3) return
    */
    if(hm->map[bucket] == NULL){
        return;
    }

    prev = NULL;
    cur = hm->map[bucket];
    while(cur->next != NULL){                   //} && docSame && wordSame){    //while "key" is not equal to parameter or cur->next != NULL, traverse
        docSame = strcmp(cur->document_id, document_id);
        wordSame = strcmp(cur->word, word);
        if(!docSame && !wordSame){                          //once traversed, check if the "keys" are the same, if so remove node
            if(prev == NULL){
                hm->map[bucket] = cur->next;
            }else{
                prev->next = cur->next;
            }
            hm->num_elements--;                             //decrement # elements
            return;
        }
        prev = cur;
        cur = cur->next;
    }

    docSame = strcmp(cur->document_id, document_id);
    wordSame = strcmp(cur->word, word);
    if(!docSame && !wordSame){
        prev->next = NULL;
        hm->num_elements--;
    }
}

/**
 * deallocate the HashMap that is passed in and all of its elements
 */
void hm_destroy(struct hashmap* hm){
    int i;
    //set next to null then deallocate memory for each bucket
    for(i = 0; i < hm->num_buckets; i++){
        struct llnode* temp;
        while(hm->map[i] != NULL){
            temp = hm->map[i];
            hm->map[i] = hm->map[i]->next;
            free(temp);
        }
    }
    //deallocate memory for map of (llnode*)s
    free(hm->map);
    //deallocate memory for hashmap* struct
    free(hm);
}

/**
 * Take the given word and document id and map them to a bucket in the HashMap
 * Sum the ASCII codes of all of the characters then modulo by the # of buckets
 */
int hash(struct hashmap* hm, char* word){
    int wordSum = 0;
    int resetCharPtr = 0;
    while(*word != '\0'){
        wordSum += *word;
        resetCharPtr++;
        word++;
    }
    word -= resetCharPtr;
    return (wordSum % hm->num_buckets);
}

void rank(double** tf_idf_vals, int numTerms, int numDocs){
    int i, j, df;
    double idf;
    double tf_idf;

    for(i = 0; i < numTerms; i++){                   // if term frequency isnt zero, increment document frequency
        df = 0;
        for(j = 0; j < numDocs; j++){
            if(tf_idf_vals[i][j]){
                df++;
            }
        }
        idf = log((double)numDocs/df) / log(10);
        printf("IDF: %.4lf\n", idf);
        for(j = 0; j < numDocs; j++){
            tf_idf_vals[i][j] = tf_idf_vals[i][j] * idf;
        }
    }    
    double* relevancies = (double*) malloc(sizeof(double) * numDocs);
    for(i = 0; i < numDocs; i++){
        relevancies[i] = 0;
    }
    for(i = 0; i < numTerms; i++){
        for(j = 0; j < numDocs; j++){
            relevancies[j] += tf_idf_vals[i][j];
        }
    }

    printf("Relevancies: \n");
    for(i = 0; i < numDocs; i++){
        printf("Index: %d Value: %.4lf\n", i, relevancies[i]);
    }
    printf("Stored Values: \n");
    for(i = 0; i < numTerms; i++){
        for(j = 0; j < numDocs; j++){
            printf("%.4lf ", tf_idf_vals[i][j]);
        }
        printf("\n");
    }
}

void stop_word(struct hashmap* hm, int numDocs){
    int i, j, df;
    double idf;
    struct llnode* trav;
    char doc [3] = "D_";

    for(i = 0; i < hm->num_buckets; i++){
        if(hm->map[i] == NULL){
            continue; 
        }
        trav = hm->map[i];
        while(trav != NULL){
            df = 0;
            printf("Word: %s\n", trav->word);
            for(j = 1; j <= numDocs; j++){
                doc[1] = j + 48;
                if(hm_get(hm, trav->word, doc) != -1){
                    df++;
                }
            }
            printf("DF: %d\n", df);
            if(!df){        //df == 0
                idf = log((double)numDocs/1.0+df);

            }else{
                idf = log((double)numDocs/df) / log(10);
            }
            printf("idf: %lf", idf);
            if(!idf){       //idf == 0
                for(j = 1; j <= numDocs; j++){
                    doc[1] = j + 48;
                    hm_remove(hm, trav->word, doc);
                }
            }
            trav = trav->next;
        }
    }
}

void read_query(struct hashmap* hm, char* query, int numDocs){
    int i;
    int numTerms = 10;                                             
    double **tf_idf_vals = (double**) malloc(sizeof(double*) * numTerms);
    for(i = 0; i < numTerms; i++){
        tf_idf_vals[i] = (double*) malloc(sizeof(double) * numDocs);
    }
    char* searchQ = malloc(strlen(query) + 1);
    strcpy(searchQ, query);
    char* c;
    c = strtok(searchQ, " ");
    i = 0;
    while(c != NULL){
        printf("%s\n", c);
        search(hm, c, tf_idf_vals[i], numDocs);
        c = strtok(NULL, " ");
        i++;
    }
    rank(tf_idf_vals, numTerms, numDocs);
    //free(searchQ);
}

void search(struct hashmap* hm, char* word, double* tf_idf_vals_word, int numDocs){
    int tf, i;
    char doc [3] = "D_";
    
    for(i = 0; i < numDocs; i++){                           // get termFrequency for word from each document
        doc[1] = i + 49;
        tf = hm_get(hm, word, doc);
        if(tf == -1){
            tf_idf_vals_word[i] = 0;                             // if doesn't exist, store 0 instead of -1 (from hm_get)
        }else{
            tf_idf_vals_word[i] = tf;                            // if exists store value
        }
    }
    for(i = 0; i < numDocs; i++){
        printf("Index: %d Value: %f\n", i, tf_idf_vals_word[i]);
    }
}
