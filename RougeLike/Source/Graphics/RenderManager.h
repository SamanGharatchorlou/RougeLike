#pragma once

class Environment;
class ActorManager;
class UIManager;
class Collectables;
class Texture;

enum class RenderLayer
{
	Lowest,
	Floor,
	LowerTiles,
	Actors,
	Items,
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


class RenderManager
{
public:
	RenderManager();

	void Set(Environment* environment) { mEnvironment = environment; }
	void Set(ActorManager* actors) { mActors = actors; }
	void Set(UIManager* UI) { mUIManager = UI; }
	void Set(Collectables* collectables) { mCollectables = collectables; }

	void render();

	void AddRenderPacket(RenderPack renderPacket) { mRenderPackets.push_back(renderPacket); }


private:
	void renderPackets(RenderLayer layer);


private:
	Environment* mEnvironment;
	ActorManager* mActors;
	UIManager* mUIManager;
	Collectables* mCollectables;

	std::vector<RenderPack> mRenderPackets;
};
