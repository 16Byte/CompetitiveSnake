#include "Global.hpp"
#include "raylib.h"
#include "raymath.h"
#include <deque>

using namespace std;

bool Global::EventTriggered(double interval)
{
    double currentTime = GetTime();
    
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool Global::ElementInDeque(Vector2 element, const deque<Vector2>& dequeToCheck)
{
    for (unsigned int i = 0; i < dequeToCheck.size(); i++)
    {
        if (Vector2Equals(dequeToCheck[i], element))
            return true;
    }
    return false;
}