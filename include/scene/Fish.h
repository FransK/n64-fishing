#pragma once

#include <libdragon.h>

class Fish
{
private:
public:
    static int GetNewTimer() { return rand() % 3 + 2; } // 2 - 4 second timer
};