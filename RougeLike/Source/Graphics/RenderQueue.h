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
    
private:
    static RenderQueue* Get()
    {
        static RenderQueue sInstance;
        return &sInstance;
    }

    void add(const RenderObject& object) { renderQueue.push(object); }
   
    
private:
    // Each layer gets it own queue of objects
    //std::map<RenderLayer, Queue<RenderObject>> renderLayerQueues;
    
    Queue<RenderObject> renderQueue;
}

