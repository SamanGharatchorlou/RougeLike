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

enum AnimationState
{
	None,
	Idle,
	Run,
	Count
};


struct AnimationData
{
	Texture* texture;
	VectorF tileDimentions;
	AnimationState state;
};


class Animator2
{
public:
	class Animation
	{
	public:
		Animation(AnimationData& data);

		void render(RectF rect);

		AnimationState state() const { return mState; }

		void nextFrame();

	private:
		Texture* mTexture;
		AnimationState mState;

		VectorF mTileDimentions;
		
		Index mIndex;
		int mCount;
	};


public:
	Animator2();

	void addAnimation(AnimationData& data);
	void slowUpdate(float dt);
	void render(RectF rect);

	void clear();
	bool hasAnimations() const { return (bool)mAnimations.size(); }

	void start() { timer.start(); }
	void pause() { timer.pause(); }
	void stop() { timer.stop(); }

	void selectAnimation(AnimationState state);

	int animationCount() { return mAnimations[mActiveAnimation].count; }

	void setSpeedFactor(float speed) { speedFactor = speed; }


private:

	std::vector<Animation> mAnimations;

	int mActiveIndex;

	float speedFactor;

	Timer<float> timer;
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