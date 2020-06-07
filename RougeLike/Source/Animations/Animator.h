#pragma once

#include "Graphics/Tileset.h"

class Texture;


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
	Attack,
	Hurt,
	Dead,

	Active,

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


class Animator
{
public:
	class Animation
	{
		friend Animator;

	public:
		Animation(AnimationData& data);

		void render(RectF rect, SDL_RendererFlip flip) const;
		void render(RectF rect, SDL_RendererFlip flip, Uint8 alpha);

		Texture* texture() const { return mTexture; }

		void nextFrame();


	private:
		Texture* mTexture;
		Action mState;

		VectorF mTileDimentions;
		
		Index mIndex;
		int mFrameCount;

		int mLoops;
	};


public:
	Animator();

	void setFrameSpeed(float frameSpeed) { mFrameSpeed = frameSpeed; }
	void addAnimation(AnimationData& data);

	void slowUpdate(float dt);
	void render(RectF rect, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	void render(RectF rect, SDL_RendererFlip flip, Uint8 alpha);

	void clear();

	Texture* texture() const { return mAnimations[mActiveIndex].texture(); }

	bool hasAnimations() const { return (bool)mAnimations.size(); }
	void selectAnimation(Action state);
	void startAnimation(Action state);

	void setSpeedFactor(float speed) { speedFactor = speed; }

	void start() { timer.start(); }
	void pause() { timer.pause(); }
	void stop() { timer.stop(); }

	int loops() const { return mAnimations[mActiveIndex].mLoops; }
	VectorF frameSize() const { return mAnimations[mActiveIndex].mTileDimentions; }
	int count() const { return mAnimations.size(); }


private:

	int mActiveIndex;
	std::vector<Animation> mAnimations;

	float speedFactor;
	float mFrameSpeed;
	Timer<float> timer;

	const RectF* mRect;
};



/*
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
*/
