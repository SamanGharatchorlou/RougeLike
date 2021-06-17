#pragma once

#include "RenderQueue.h"

class RenderableObject
{
private:
    // Could include a RenderLayer parameter
    void addToRenderQueue(Texture* texture, RectF rect) const
    {
        RenderObject object(texture, rect);
        RenderQueue* RenderQueue::Get()->add(object);
    }
};
