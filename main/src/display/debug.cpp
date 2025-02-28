#include "debug.h"

void Debug::init()
{
    Serial.begin(115200);
}

void Debug::print(const char *msg)
{
    Serial.print(msg);
}

void Debug::println(const char *msg)
{
    Serial.println(msg);
}

void Debug::print(const String &msg)
{
    Serial.print(msg);
}

void Debug::println(const String &msg)
{
    Serial.println(msg);
}

void Debug::print(const int val)
{
    Serial.print(val);
}

void Debug::println(const int val)
{
    Serial.println(val);
}

void Debug::print(const Printable &val)
{
    Serial.print(val);
}

void Debug::println(const Printable &val)
{
    Serial.println(val);
}