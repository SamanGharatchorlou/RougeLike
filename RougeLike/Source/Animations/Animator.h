#pragma once

#include "Graphics/Tileset.h"

class Tile;
class Texture;
class XMLParser;


struct AnimationInfo
{
	AnimationInfo() : startingIndex(0), count(0), speed(0.0f) { }
	AnimationInfo(int start, int length, float theSpeed) :
		startingIndex(start), count(length), speed(theSpeed) { }

	int startingIndex;
	int count;
	float speed;
};

enum class Action
{
	None,
	Idle,
	Run,
	Count
};

Action stringToAction(const std::string& action);


struct AnimationData
{
	Texture* texture;
	Vector2D<int> tileDimentions;
	int frameCount;
	Action action;
};


class Animator2
{
public:
	class Animation
	{
	public:
		Animation(AnimationData& data);

		void render(RectF rect, SDL_RendererFlip flip) const;

		Action state() const { return mState; }

		void nextFrame();

		VectorF size() const { return mTileDimentions; }

	private:
		Texture* mTexture;
		Action mState;

		VectorF mTileDimentions;
		
		Index mIndex;
		int mFrameCount;
	};


public:
	Animator2();

	void setFrameSpeed(float frameSpeed) { mFrameSpeed = frameSpeed; }
	void addAnimation(AnimationData& data);

	void slowUpdate(float dt);

	void render(RectF rect, SDL_RendererFlip flip) const;

	void clear();

	bool hasAnimations() const { return (bool)mAnimations.size(); }

	void selectAnimation(Action state);

	void setSpeedFactor(float speed) { speedFactor = speed; }

	void start() { timer.start(); }
	void pause() { timer.pause(); }
	void stop() { timer.stop(); }

	VectorF frameSize() const { return mAnimations[mActiveIndex].size(); }


private:

	std::vector<Animation> mAnimations;

	int mActiveIndex;

	float speedFactor;

	float mFrameSpeed;

	Timer<float> timer;

	const RectF* mRect;
};




// --- Animation 1 --- //

using Animations = std::unordered_map<std::string, AnimationInfo>;


class Animator
{
public:
	Animator() : mAnimationIndex(0), speedFactor(1.0f) { }

	void init(Tileset spriteSheet, Animations animations);
	void slowUpdate(float dt);
	void render(RectF rect);

	void clear();
	bool hasAnimations() const { return (bool)mAnimations.size(); }

	void start() { timer.start(); }
	void pause() { timer.pause(); }
	void stop() { timer.stop(); }

	void selectAnimation(const std::string& name);

	int animationIndex() const { return mAnimationIndex; }
	int animationCount() { return mAnimations[mActiveAnimation].count; }

	void setSpeedFactor(float speed) {
		printf("speed factor being set to %f\n", speed);

		speedFactor = speed;
	}

	Tile* getSpriteTile();
	Texture* getSpriteTexture();

private:
	Tileset mSpriteSheet;
	Animations mAnimations;

	std::string mActiveAnimation;

	float speedFactor;

	int mAnimationIndex;

	Timer<float> timer;
};