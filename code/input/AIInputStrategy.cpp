#include "AIInputStrategy.h"

InputState AIInputStrategy::update()
{
    return mAi->getInputState();
}