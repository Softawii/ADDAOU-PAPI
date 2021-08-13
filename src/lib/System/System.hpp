#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_
#include <papi.h>
#include <time.h>
#include <sys/time.h>
#include <string>
#define CACHE_LINE 64
using namespace std;

class System{
public:
    System();
    ~System();
protected:
    const PAPI_hw_info_t *hwinfo;
};


class Cycles:public System{
public:
    Cycles();
    ~Cycles();
    void start(void);
    void stop(void);
    long long getCycles(void);
private:
    long long elapsedcycles,
              startcycles,
              stopcycles;
};


class Events:public System{
public:
    Events();
    ~Events();
    void setNumberOfEvents(int);
    void addEvents(int);

    void clearCache(size_t);
    void start(void);
    void stop(void);
    long long getEventbyIndex(int);
    int getnumberofEvents(void);


private:
    void clear(void);

    long long *values;

    int EventSet;

    int *events,
        numberOfEvents,
        index;

    size_t bytes;
};


#endif
