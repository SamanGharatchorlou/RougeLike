#pragma once


enum class RenderLayer
{
    Lowest,
    Floor,
    LowerTiles,
    Actors,
    UpperTiles,
    UI,
    Highest
};


struct RenderObject
{
    BasicString textureId;
    RectF rect;
};


// Global class to store all the render data
class RenderQueue
{
    // Allows texture class to access these private members
    friend class Texture;
    friend class RenderManager;
    
private:
    RenderQueue() : drawingQueue(&renderQueueA), fillingQueue(&renderQueueB), drawingQueueIsA(true), fillingComplete(false) { }
    
    static RenderQueue* Get()
    {
        static RenderQueue sInstance;
        return &sInstance;
    }

    void add(const RenderObject& object) { fillingQueue->push(object); }
    
    RenderObject getNext()
    {
        RenderObject object = drawingQueue->front();
        drawingQueue.pop();
        
        return object;
    }
    
    void fillingCompleted()
    {
        fillingComplete = true;
    }
    
    void canSwapQueues()
    {
        return drawingQueue.size() == 0 && fillingComplete;
    }
    
    void swapQueues()
    {
        if(drawingQueueIsA)
        {
            drawingQueue = &renderQueueB;
            fillingQueue = &renderQueueA;
            drawingQueueIsA = false;
        }
        else
        {
            drawingQueue = &renderQueueA;
            fillingQueue = &renderQueueB;
            drawingQueueIsA = true;
        }
        
        fillingComplete = false;
    }
   
    
private:
    // Each layer gets it own queue of objects
    //std::map<RenderLayer, Queue<RenderObject>> renderLayerQueues;
    
    bool drawingQueueIsA;
    bool fillingComplete;
    
    Queue<RenderObject>* drawingQueue;
    Queue<RenderObject>* fillingQueue;
    
    // Two queues so we can fill one while the other is being drawm/emptied, like a double buffer
    Queue<RenderObject> renderQueueA;
    Queue<RenderObject> renderQueueB;
}

