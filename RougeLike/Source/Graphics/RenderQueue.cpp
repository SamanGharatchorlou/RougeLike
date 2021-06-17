#include "pch.h"
#include "RenderQueue.h"


RenderQueue* Get()
{
    static RenderQueue sInstance;
    return &sInstance;
}



void RenderQueue::add(const RenderObject& object)
{
    renderQueue.push(object);
}
