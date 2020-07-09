#pragma once

#include "Graphics/Tileset.h"

class Texture;

enum class Action
{
	None,
	Idle,
	Walk,
	Run,
	Attack,
	Alert,
	Hurt,
	Dead,

	Active,

	Count
};

Action stringToAction(const std::string& action);


struct AnimationData
{
	Texture* texture = nullptr;
	Vector2D<int> tileDimentions;
	int frameCount = 0;
	Action action = Action::None;
};


class Animator
{
public:
	class Animation
	{
		friend Animator;

	public:
		Animation(AnimationData& data);

		void reset();

		void render(RectF rect, SDL_RendererFlip flip) const;
		void render(RectF rect, SDL_RendererFlip flip, Uint8 alpha);

		Texture* texture() const { return mTexture; }

		void nextFrame();
		int currentFrame() const;
		int frameCount() const { return mFrameCount; }


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

	void setFrameTime(float frameTime) { mFrameTime = frameTime; }
	void addAnimation(AnimationData& data);

	void slowUpdate(float dt);
	void render(RectF rect, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	void render(RectF rect, SDL_RendererFlip flip, Uint8 alpha);

	void reset();
	void clear();

	Texture* texture() const { return mAnimations[mActiveIndex].texture(); }

	bool hasAnimations() const { return (bool)mAnimations.size(); }
	void selectAnimation(Action state);
	void startAnimation(Action state);
	Action currentAction() const { return mAnimations[mActiveIndex].mState; }

	void setSpeedFactor(float speed) { speedFactor = speed; }

	void start() { timer.start(); }
	void pause() { timer.pause(); }
	void stop() { timer.stop(); }
	bool isRunning() const { return !timer.isPaused() && timer.isStarted(); }

	int loops() const { return mAnimations[mActiveIndex].mLoops; }
	VectorF frameSize() const { return mAnimations[mActiveIndex].mTileDimentions; }
	int frameCount() const { return mAnimations[mActiveIndex].mFrameCount; }
	float frameTime() const { return mFrameTime; }
	int animationCount() const { return mAnimations.size(); }

	const Animation& animation() const { return mAnimations[mActiveIndex]; }


private:

	int mActiveIndex;
	std::vector<Animation> mAnimations;

	float speedFactor;
	float mFrameTime;
	Timer<float> timer;
};
