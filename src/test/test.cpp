#include <stdio.h>
#include <iostream>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>   // stat
#include <stdbool.h>
#include <string.h>
#include <array>

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
  
    int numberOfEvents = 0;
    std::array<int, 5> intEvents = {PAPI_L1_TCM, PAPI_L2_TCM, PAPI_L3_TCM, PAPI_TOT_INS, PAPI_TOT_CYC};
    string stringEvents[5] = {"L1 Cache miss", "L2 Cache miss", "L3 Cache miss", "Num of instructions", "Total cycles"};

    for (int i = 0; i < intEvents.size(); i++)
    {
        if(is_avail(intEvents[i]))
            numberOfEvents += 1;
    }
    
    events.setNumberOfEvents(numberOfEvents);

    int status;    

    for (int i = 0; i < intEvents.size(); i++)
    {
        if(status = is_avail(intEvents[i])){
            events_header += "," + stringEvents[i];
            events.addEvents(intEvents[i]);
        }
        #ifdef DEBUG
            cout << stringEvents[i] + ": " << status << endl;
        #endif
    }
    
    events_header += "\n";
    events.start();

    #ifdef DEBUG
    cout << "Total: " << numberOfEvents << " Event List: " << events_header << endl;
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

    for(int i = 0; i < numberOfEvents; i++)
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

