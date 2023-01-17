/************************************************
*
* Author: Bryce McWhirter
* Assignment: Program 7 & 8 (cache.h)
* Class: Intro to Operating Systems
*
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>

#define CHAR_LIMIT_PATH 1024
#define MAX_SIZE_CACHE 10

/*
* Defines an entry within the cache. Every entry
* describes a file with its respected parameters. 
*
* path - the full path of the file
* mode - the mode of the file 
* uid - the user id of the file
* size - size of the file in bytes
* next - pointer to the next entry within the cache. 
*
*/
struct cacheEntry{
    char path[CHAR_LIMIT_PATH];
    mode_t mode;
    uid_t uid;
    off_t size;
    struct cacheEntry *next;
};


/**
* Adds a file into the cache. 
*
* @param head - Reference to the head of the cache
* @param path - the path of the file to add
* @param mode - the mode of the file
* @param uid  - the user id of the file
* @param size - the size of the file
*
*/
void addToCache(struct cacheEntry** head, char *path, mode_t mode, uid_t uid, off_t size);



/**
* Removes a file from the cache
*
* @param head - Reference to the head of the cache
* @param path - the path of the file to remove
*
*/
void removeFromCache(struct cacheEntry** head, char *path);



/**
* Removes the oldest file in the cache. Method Utilizes
* First in First out
*
* @param head - Reference to the head of the cache
*
*/
void removeOldestFile(struct cacheEntry **head);



/**
* Returns the specific attributes of a file identified by a path
*
* @param head - Reference to the head of the cache
* @param path - the path of the file 
* @param mode - the mode of the file
* @param uid  - the user id of the file
* @param size - the size of the file
*
*/
void getFileAttrFromCache(struct cacheEntry** head, const char *path, mode_t *mode, uid_t *uid, off_t *size);


/**
* Identifies if a specific file is on the cache. 
*
* @param head - Reference to the head of the cache
* @param path - the path of the file to find
*/
int pathOnCache(struct cacheEntry *head, const char *path);



/**
* Prints the cache starting from the head
*
* @param head - Reference to the head of the cache
*
*/
void printCache(struct cacheEntry *head);



/**
* Counts the number of parameters on the cache
*
* @param head - Reference to the head of the cache
*
* @return the size of the cache
*/
int countCache(struct cacheEntry *head);



/**
* Updates a file size 
*
* @param head - Reference to the head of the cache
* @param path - the path of the file 
* @param s    - the new size of the file
*
*/
void updateSizeOfFileOnCache(struct cacheEntry *head, char *path, off_t s);



/**
* Returns the specific attributes of a file identified by a path
*
* @param head - Reference to the head of the cache
* @param path - the path of the file 
* @param mode - the new mode of the file
*
*/
void updateModeOfFileOnCache(struct cacheEntry *head, char *path, mode_t m);



