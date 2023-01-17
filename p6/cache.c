/************************************************
*
* Author: Bryce McWhirter
* Assignment: Program 7 & 8 (cache.c)
* Class: Intro to Operating Systems
*
************************************************/

#include "cache.h"

void removeOldestFile(struct cacheEntry **head){

    struct cacheEntry *i = *head;
    struct cacheEntry *prev;

    while(i->next != *head){
        prev = i;
        i = i->next;
    }

    prev->next = i->next;
    free(i);
    return;
}


void addToCache(struct cacheEntry** head, char *path, mode_t mode, uid_t uid, off_t size){

    // If the cache will break capacity on next entry
    // Remove Oldest File from the cache
    if(countCache(*head) + 1> MAX_SIZE_CACHE){
        removeOldestFile(head);
    }

    // Create a new node
    struct cacheEntry *newNode = (struct cacheEntry*)malloc(sizeof(struct cacheEntry));

    // Set Parameters
    strncpy(newNode->path, path, sizeof(newNode->path) - 1); 
    newNode->path[sizeof(newNode->path)] = '\0';
    newNode->mode = mode;
    newNode->uid = uid;
    newNode->size = size;
    newNode->next = *head;

    // If the cache isn't empty
    // add node to its new place
    if(*head != NULL){
        struct cacheEntry *i = *head;
        while(i->next != *head){
            i = i->next;
        }
        i->next = newNode;
    }
    else{
        // Set up first node
        newNode->next = newNode;
    }

    // update 
    *head = newNode;

};


void removeFromCache(struct cacheEntry** head, char *path){

    // Check if Empty
    if(*head == NULL){
        return;
    }

    struct cacheEntry *prev;
    struct cacheEntry *i = *head;

    // If This is the only value in the cache
    if((*head)->next == *head && strcmp(i->path, path) == 0){
        *head = NULL;
        free(i);
        return;
    }



    // Comparing on File Path Name
    while(strcmp(i->path, path) != 0){
        
        // Didn't find path
        if(i->next == *head){
            fprintf(stderr, "File Not Found on Cache: %s", path);
            return;
        }

        // Keep Searchings
        prev = i;
        i = i->next;
    }


   
    // If value to remove is head
    if(i == *head){
        prev = *head;
        while(prev->next != *head){
            prev = prev->next;
        }
        *head = i->next;
        prev->next = *head;
        free(i);
    }
    
    else if(i->next == *head && i == *head){
        prev->next = *head;
        free(i);
    }

    // Simply Remove the value if anywhere else. 
    else{
        prev->next = i->next;
        free(i);
    }
}



void getFileAttrFromCache(struct cacheEntry** head, const char *path, mode_t *mode, uid_t *uid, off_t *size){

    // Locate File and return attributes
    struct cacheEntry *i = *head;
    if(head != NULL){
        do{
            char *n = i->path;
            if(strcmp(n, path) == 0){
                *mode = i->mode;
                *uid = i->uid;
                *size = i->size;
                return;
            }
            i = i->next;
        }while(i != *head);
    }


}


int pathOnCache(struct cacheEntry *head, const char *path){
    // Locate File and return 1 (true) if found
    struct cacheEntry *i = head;
    if(head != NULL){
        do{
            char *n = i->path;
            if(strcmp(n, path) == 0){
                return 1;
            }
            i = i->next;
        }while(i != head);
    }
    return 0;
}



void printCache(struct cacheEntry *head){
    // Iterate through the list and print each parameter on each entry of
    // the cache
    printf("\nThe Cache\n");
    struct cacheEntry *i = head;
    if(head != NULL){
        do{
            printf("%s %zu %zu %zu\n", i->path, (size_t) i->mode, (size_t) i->uid, i->size);
            i = i->next;
        }while(i != head);
    }
    printf("\n");
}





int countCache(struct cacheEntry *head){

    // Iterate thorugh the list and count
    // until you circle back to head. 
    struct cacheEntry *i = head;

    //
    int count = 0;
    if(head != NULL){
        do{
            count += 1;
            i = i->next;
        }while(i != head);
    }
    return count;
}










void updateSizeOfFileOnCache(struct cacheEntry *head, char *path, off_t s){

    // Find the file on the cache and update the size
    struct cacheEntry *i = head;
    if(head != NULL){
        do{
            char *n = i->path;
            if(strcmp(n, path) == 0){
                i->size = s;
                return;
            }
            i = i->next;
        }while(i != head);
    }
    return;
}



void updateModeOfFileOnCache(struct cacheEntry *head, char *path, mode_t m){

    // Find the file on the cache and update the mode
    struct cacheEntry *i = head;
    if(head != NULL){
        do{
            char *n = i->path;
            if(strcmp(n, path) == 0){
                i->mode = m;
                return; 
            }
            i = i->next;
        }while(i != head);
    }
    return;
}

