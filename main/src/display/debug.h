#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

class Debug
{
public:
    void init();
    void print(const char *);
    void println(const char *);
    void print(const int);
    void println(const int);
    void print(const String &);
    void println(const String &);
    void print(const Printable &);
    void println(const Printable &);
};

#endif