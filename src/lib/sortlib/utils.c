#include <stdlib.h>
#include <string.h>
#include "sortlib.h"

void* access(void * vector, int i, int size) {
    char* t = (char*) vector; 
    t += size * i;

    return (void*) t;
}

void swap(void* a, void* b, int size) {
    
    char buffer[size]; 
  
    memcpy(buffer, a, size); 
    memcpy(a, b, size); 
    memcpy(b, buffer, size); 
}

void set(void* vector, int i, int size, void* value) {    
    char *val = (char*) value;
    char *vec = (char*) vector;
    vec += size * i;
    for(int j = 0; j < size; j++){
        vec[j] = val[j];
    }
}

void* get(void* vector, int i, int size) {
    char *vec = (char*) vector;
    vec += size * i;
 
    char *val = (char *)malloc(size);

    for(int j = 0; j < size; j++) {
        val[j] = vec[j];
    }

    return (void*) val;
}
