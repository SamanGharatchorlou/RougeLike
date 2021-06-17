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



class RenderQueue
{
    friend class RenderableObject;
    
private:
    static RenderQueue* Get()

    void add(const RenderObject& object);
    
    // Each layer gets it own queue of objects
    //std::map<RenderLayer, Queue<RenderObject>> renderLayerQueues;
    
    Queue<RenderObject> renderQueue;
}
