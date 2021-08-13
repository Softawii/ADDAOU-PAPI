#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>   // stat
#include <stdbool.h>
#include <string.h>

#include "../lib/sortlib/sortlib.h"
#include "../lib/System/System.hpp"
#include "../lib/App.h"

static u_int64_t* gen(int length) {
    u_int64_t * vector;
    posix_memalign(reinterpret_cast <void**>(&vector), 64, length * sizeof(u_int64_t));

    for(int i = 0; i < length; i++) {
        vector[i] = length - i;
    }
    return vector;
}

int compare_u_int64_t(void * a, void * b) {
    int p1 = *((u_int64_t *) a);
    int p2 = *((u_int64_t *) b);

    if(p1 > p2)
        return 1;
    else if(p1 == p2)
        return 0;
    else
        return -1; 
}


bool file_exists (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

int main(int argc, char *argv[]) {
    int length = atoi(argv[1]);
    char * method = argv[2];
    Events events;
    SortMethod sorthmethod = mergesort;
    
    const PAPI_hw_info_t *hwinfo;
    hwinfo = PAPI_get_hardware_info();

    string s;

    if(strcmp(method, "quicksort") == 0){
        sorthmethod = quicksort;
        s = "../data/quicksort/" + (string) hwinfo->model_string + ".csv";
    } 
    else if(strcmp(method, "mergesort") == 0){
        sorthmethod = mergesort;
        s = "../data/mergesort/" + (string) hwinfo->model_string + ".csv";
    }
    else {
        printf("(ERRO) Método %s indefinido!\n", argv[2]);
        return -1;
    }

    u_int64_t *vector = gen(length);

    events.setNumberOfEvents(3);
    events.addEvents(PAPI_L1_TCM);
    events.addEvents(PAPI_L2_TCM);
    events.addEvents(PAPI_TOT_INS);
    events.start();
    Stopwatch stopwatch;
    FREQUENCY(stopwatch);
    START_STOPWATCH(stopwatch);

    sorthmethod(vector, length, sizeof(vector[0]), compare_u_int64_t);

    STOP_STOPWATCH(stopwatch);
    events.stop();
   
    FILE* file;
    if(file_exists((char *)s.c_str()) == 0 ) {
        file = fopen(s.c_str(), "a");
        fprintf(file, "Length,Elapsed Time,L1 Cache miss,L2 Cache miss,Num of instructions\n");
    }
    else {
        file = fopen(s.c_str(), "a");
    }
      
    fprintf(file, "%d,%.10lf,%lld,%lld,%lld\n", length, stopwatch.mElapsedTime, events.getEventbyIndex(0), events.getEventbyIndex(1), events.getEventbyIndex(2));


    free(vector);
    fclose(file);

    return 0;
}

