#include <stdio.h>
#include <iostream>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>   // stat
#include <stdbool.h>
#include <string.h>

#include <papi.h>


//#define DEBUG

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


int is_avail(int EventCode) {
    return PAPI_query_event(EventCode) == PAPI_OK;
}

int main(int argc, char *argv[]) {
    
    #ifdef DEBUG
    
    cout << "PAPI_FP_OPS: " << (PAPI_query_event(PAPI_FP_OPS) != PAPI_OK) << endl;
    cout << "PAPI_L1_TCM: " << (PAPI_query_event(PAPI_L1_TCM) != PAPI_OK) << endl;



    #endif

    int length = atoi(argv[1]);
    char * method = argv[2];
    Events events;
    SortMethod sorthmethod = mergesort;
    
    const PAPI_hw_info_t *hwinfo;
    hwinfo = PAPI_get_hardware_info();

    string s;

    #ifdef DEBUG
    printf("Selecting Method\n");
    #endif

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

    #ifdef DEBUG
    printf("Events\n");
    #endif

    string events_header = "Length,Elapsed Time";
    int events_number = 0;

    events.setNumberOfEvents(5);

    int status;    

    if(status = is_avail(PAPI_L1_TCM)) {
        events_number += 1;
        events_header += ",L1 Cache miss";
        events.addEvents(PAPI_L1_TCM);
    }

    #ifdef DEBUG
    cout << "PAPI_L1_TCM: " << status << endl;
    #endif

    if(status = is_avail(PAPI_L2_TCM)) {
        events_number += 1;
        events_header += ",L2 Cache miss";
        events.addEvents(PAPI_L2_TCM);
    }

    #ifdef DEBUG
    cout << "PAPI_L2_TCM: " << status << endl;
    #endif

    if(is_avail(PAPI_L3_TCM)) {
        events_number += 1;
        events_header += ",L3 Cache miss";
        events.addEvents(PAPI_L3_TCM);
    }
    
    #ifdef DEBUG
    cout << "PAPI_L3_TCM: " << status << endl;
    #endif

    if(is_avail(PAPI_TOT_INS)) {
        events_number += 1;
        events_header += ",Num of instructions";
        events.addEvents(PAPI_TOT_INS);
    }

    #ifdef DEBUG
    cout << "PAPI_TOT_INS: " << status << endl;
    #endif
    
    if(is_avail(PAPI_TOT_CYC)) {
        events_number += 1;
        events_header += ",Total cycles";
        events.addEvents(PAPI_TOT_CYC);
    }
    
    #ifdef DEBUG
    cout << "PAPI_TOT_CYC: " << status << endl;
    #endif

    #ifdef DEBUG
    cout << "PAPI_FNV_INS: " << is_avail(PAPI_FNV_INS) << endl;
    #endif

    events_header += "\n";
    events.start();

    #ifdef DEBUG
    cout << "Total: " << events_number << " Event List: " << events_header << endl;
    #endif

    #ifdef DEBUG
    printf("Stopwatch\n");
    #endif
    
    Stopwatch stopwatch;
    FREQUENCY(stopwatch);
    START_STOPWATCH(stopwatch);

    #ifdef DEBUG
    printf("Sort\n");
    #endif


    sorthmethod(vector, length, sizeof(vector[0]), compare_u_int64_t);

    STOP_STOPWATCH(stopwatch);
    events.stop();
    
    #ifdef DEBUG
    printf("File\n");
    #endif

    FILE* file;
    if(file_exists((char *)s.c_str()) == 0 ) {
        file = fopen(s.c_str(), "a");
        fprintf(file, events_header.c_str());
    }
    else {
        file = fopen(s.c_str(), "a");
    }
    
    string values = "";

    values += std::to_string(length) + ",";
    values += std::to_string(stopwatch.mElapsedTime) + ",";

    for(int i = 0; i < events_number; i++)
        values += std::to_string(events.getEventbyIndex(i)) + ",";
    values.erase(values.end() - 1);
    values += "\n";
    
    #ifdef DEBUG
        cout << "Values: " << values << endl;
    #endif

    fprintf(file, values.c_str());


    free(vector);
    fclose(file);

    return 0;
}

