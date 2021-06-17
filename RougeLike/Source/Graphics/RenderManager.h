#pragma once

#include "Events/Observer.h"

class Environment;
class ActorManager;
class UIManager;
class Texture;

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


struct RenderPack
{
	RenderPack(Texture* tex, RectF box, RenderLayer renderLayer) : texture(tex), rect(box), layer(renderLayer) { }
	Texture* texture = nullptr;
	RectF rect;
	RenderLayer layer = RenderLayer::Highest;
};


class RenderManager : public Observer
{
public:
	RenderManager();

	void Set(Environment* environment) { mEnvironment = environment; }
	void Set(UIManager* UI) { mUIManager = UI; }

	void render();
    void fillRenderQueue();

	void AddRenderPacket(RenderPack renderPacket) { mRenderPackets.push_back(renderPacket); }

	void handleEvent(EventData& data) override;


private:
	void renderPackets(RenderLayer layer);


private:
	Environment* mEnvironment;
	UIManager* mUIManager;

	std::vector<RenderPack> mRenderPackets;
};


// Pass in render objects to be rendered by a thread
void render(Queue<RenderObject>& renderQueue);
