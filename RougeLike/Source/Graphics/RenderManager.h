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

	void AddRenderPacket(RenderPack renderPacket) { mRenderPackets.push_back(renderPacket); }

	void handleEvent(EventData& data) override;


private:
	void renderPackets(RenderLayer layer);


private:
	Environment* mEnvironment;
	UIManager* mUIManager;

	std::vector<RenderPack> mRenderPackets;
};
