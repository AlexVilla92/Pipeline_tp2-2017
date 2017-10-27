#ifndef TP2_THREAD_H_INCLUDED
#define TP2_THREAD_H_INCLUDED

#include <iostream>
#include <thread>
#include <regex>
#define VACIO ""
#define TERMINE "E3C5D9D4C9D5C5"

class Thread {
private:
    std::thread thread;
 
public:
    Thread();
    void Start();
    void Join();
    virtual void run() = 0;
    virtual ~Thread();
};

#endif
