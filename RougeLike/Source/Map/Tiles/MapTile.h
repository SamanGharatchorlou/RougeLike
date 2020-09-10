#pragma once

#include "Animations/Animator.h"
#include "TileTypes.h"

class Texture;


class BasicTile
{
public:
	BasicTile() : mRect(-1) { }
	BasicTile(RectF rect) : mRect(rect) { }

	RectF rect() const { return mRect; }
	RectF& rectRef() { return mRect; }
	void setRect(RectF rect) { mRect = rect; }

protected:
	RectF mRect;
};


// Add collisions
class PathTile : public BasicTile
{
public:
	PathTile() : mCollisionType(CollisionTile::None) { }
	PathTile(CollisionTile type, RectF rect) : mCollisionType(type), BasicTile(rect) { }

	// Collision type
	const CollisionTile collisionType() const { return mCollisionType; }
	virtual bool is(CollisionTile type) const;
	virtual bool has(CollisionTile type) const;

	virtual void set(CollisionTile type) { mCollisionType = type; }
	void add(CollisionTile type);
	void remove(CollisionTile type);


protected:
	CollisionTile mCollisionType;
};


// Add rendering and decorations
class MapTile : public PathTile
{
public:
	MapTile() : mRenderType(RenderTile::None), mDecorType(DecorType::None), mTexture(nullptr), mDeferredRendering(false) { }

	void slowUpdate(float dt);

	void setTexture(Texture* texture) { mTexture = texture; }

	// Render type
	const RenderTile renderType() const { return mRenderType; }
	bool has(RenderTile type) const;
	bool is(RenderTile type) const;
	void set(RenderTile type) { mRenderType = type; }

	void add(RenderTile type);
	void remove(RenderTile type);

	// Collision type
	void set(CollisionTile type) { PathTile::set(type); }
	bool is(CollisionTile type) const { return PathTile::is(type); }
	bool has(CollisionTile type) const { return PathTile::has(type); }

	// Decoration type
	const DecorType decorType() const { return mDecorType; }
	void set(DecorType type) { mDecorType = type; }
	void add(DecorType type);
	bool is(DecorType type) const;
	bool has(DecorType type) const;

	// Animations
	void addAnimation(Animator animation);
	Animator& animation(int index) { return mAnimations[index]; }

	// HACK: for animation tile populator grating trigger hack
	std::vector<Animator>& animations() { return mAnimations; }

	void render(RectF rect);

	void setDeferredRendering(bool deferr) { mDeferredRendering = deferr; }
	bool deferRender() const { return mDeferredRendering; }
	void deferredRender(RectF rect);

private:
	Texture* mTexture;
	std::vector<Animator> mAnimations;

	RenderTile mRenderType;
	DecorType mDecorType;

	bool mDeferredRendering;
};



template<typename T>
bool isValid(Index index, const Grid<T>& data)
{
	return (index.x >= 0 && index.x < data.xCount()) &&
		(index.y >= 0 && index.y < data.yCount());
}