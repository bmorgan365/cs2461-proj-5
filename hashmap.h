#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
        char* word;
        char* document_id;
        int termFrequency;
        struct llnode* next;
};

struct hashmap {
        struct llnode** map;
        int num_buckets;
        int num_elements;
};

struct hashmap* hm_create(int num_buckets);
int hm_get(struct hashmap* hm, char* word, char* document_id);
void hm_put(struct hashmap* hm, char* word, char* document_id, int termFrequency);
void hm_remove(struct hashmap* hm, char* word, char* document_id);
void hm_destroy(struct hashmap* hm);
int hash(struct hashmap* hm, char* word);
void rank(float** tf_idf_vals, int numTerms, int numDocs);
void stop_word(struct hashmap* hm, int numDocs);
void read_query(struct hashmap* hm, char* query, int numDocs);
void search(struct hashmap* hm, char* word, float* tf_idf_vals_word, int numDocs);
#endif
