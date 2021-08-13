#include "System.hpp"
#include <papi.h>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
using namespace std;



//#include <cstring>
System::System(){

   assert(PAPI_library_init(PAPI_VER_CURRENT) == PAPI_VER_CURRENT);
   hwinfo = PAPI_get_hardware_info();


}

System::~System(){

}


Cycles::Cycles():
System(),
elapsedcycles(0){}

Cycles::~Cycles(){}

void Cycles::start(void){ startcycles = PAPI_get_real_cyc();}

void Cycles::stop(void){
    stopcycles = PAPI_get_real_cyc();
    elapsedcycles = stopcycles - startcycles;
}

long long Cycles::getCycles(void){ return elapsedcycles;  }


Events::Events():
System(),
values(NULL),
EventSet(PAPI_NULL),
events(NULL),
numberOfEvents(0),
index(0){}

Events::~Events(){
    clear();
}

void Events::clear(void){
    if (events != NULL)
        free(events);
    if (values != NULL)
        free(values);

    events = NULL;
    values = NULL;
    numberOfEvents = 0;
    index = 0;

}
void Events::setNumberOfEvents(int s){
    clear();
    numberOfEvents = s;
    index = 0;
    posix_memalign((void**)&values, CACHE_LINE, numberOfEvents * sizeof(long long));
    posix_memalign((void**)&events, CACHE_LINE, numberOfEvents * sizeof(int));
    //bzero(values, numberOfEvents * sizeof(long long));
    //bzero(events, numberOfEvents * sizeof(int));

}

void Events::addEvents(int e){
    assert(index < numberOfEvents);
    events[index] = e;
    index++;
}

void Events::clearCache(size_t bytes){
  unsigned char *ptr = NULL;
  if (bytes == 0){
    cout << "Cache size not defined" << endl;
    return;
  }

  srand (time(NULL));
  posix_memalign((void**)&ptr, CACHE_LINE, bytes);
  for (size_t i = 0;  i < bytes; i++){
    ptr[i] = (unsigned char) rand() % 255;
  }

  for (int i = bytes - 1;  i > 0; i--){
    ptr[i] ^= (unsigned char) rand() % 255;
  }

  free(ptr);

}

void Events::start(void){


    assert(PAPI_create_eventset(&EventSet) == PAPI_OK);
    assert(PAPI_add_events(EventSet, events, numberOfEvents) == PAPI_OK);
    assert(PAPI_start(EventSet) == PAPI_OK);


}
void Events::stop(void){
    assert(PAPI_stop(EventSet, values) == PAPI_OK);
    assert(PAPI_cleanup_eventset(EventSet) == PAPI_OK);
    assert(PAPI_destroy_eventset(&EventSet) == PAPI_OK);
  }

long long Events::getEventbyIndex(int idx){
    assert(idx < numberOfEvents);
    return values[idx];
}

int Events::getnumberofEvents(void){
  return numberOfEvents;
}
