#pragma once

#include "RenderQueue.h"

// Move this to render queue class?
class RenderableObject
{
private:
    // Could include a RenderLayer parameter
    void addToRenderQueue(Texture* texture, const RectF& rect) const
    {
        RenderObject object(texture, rect);
        RenderQueue* RenderQueue::Get()->add(object);
    }
};
