#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cache.h"

int main(){

    struct cacheEntry *cache = NULL;


    
    addToCache(&cache, "f", 511, 100, 0);
    addToCache(&cache, "g", 511, 100, 0);
    addToCache(&cache, "h", 511, 100, 0);
    addToCache(&cache, "i", 511, 100, 0);
    addToCache(&cache, "j", 511, 100, 0);
    addToCache(&cache, "k", 511, 100, 0);
    addToCache(&cache, "l", 511, 100, 0);
    addToCache(&cache, "m", 511, 100, 0);
    addToCache(&cache, "n", 511, 100, 0);
    addToCache(&cache, "o", 511, 100, 0);
    addToCache(&cache, "a", 511, 100, 0);
    addToCache(&cache, "b", 511, 100, 0);
    addToCache(&cache, "c", 511, 100, 0);
    addToCache(&cache, "d", 511, 100, 0);
    addToCache(&cache, "e", 511, 100, 0);
    printCache(cache);


    char *file = "a";
    if(pathOnCache(cache, file) == 1){
        printf("%s on cache\n", file);
    }

    updateModeOfFileOnCache(cache, file, 23);
    updateSizeOfFileOnCache(cache, file, 17);
    printCache(cache);

    
    int x = countCache(cache);
    printf("%d values on cache\n", x);

    const char *bFile = "b";
    mode_t bMode;
    uid_t bUid;
    off_t bSize;

    getFileAttrFromCache(&cache, bFile, &bMode, &bUid, &bSize);

    printf("%s: %zu %zu %zu\n", bFile, (size_t) bMode, (size_t) bUid, (size_t) bSize);





    removeFromCache(&cache, "a");
    printCache(cache);

    removeFromCache(&cache, "c");
    printCache(cache);

    removeFromCache(&cache, "d");
    printCache(cache);

    removeFromCache(&cache, "b");
    printCache(cache);

    removeFromCache(&cache, "e");
    printCache(cache);

    return 0;
}