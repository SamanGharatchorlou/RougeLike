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

Action stringToAction(const BasicString& action);


struct AnimationData
{
	AnimationData(Texture* animTexture, VectorF tileDim, int frames, Action animAction) :
		texture(animTexture), tileDimentions(tileDim), frameCount(frames), action(animAction) { }

	Texture* texture;
	VectorF tileDimentions;
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
		Animation(const AnimationData& data);

		void reset();

		void render(RectF rect, SDL_RendererFlip flip) const;
		void render(RectF rect, SDL_RendererFlip flip, Uint8 alpha) const;
		void render(RectF rect, SDL_RendererFlip flip, RenderColour colourMod) const;
		void render(RectF rect, double rotation, VectorF aboutPoint) const;

		Texture* texture() const { return mTexture; }

		void nextFrame();
		int currentFrame() const;
		int frameCount() const { return mFrameCount; }


	private:
		RectF subTileRect(RectF rect) const;
		RectF autoSize(RectF rect) const;

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
	void addAnimation(const AnimationData& data);

	void slowUpdate(float dt);
	void render(RectF rect, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	void render(RectF rect, SDL_RendererFlip flip, Uint8 alpha) const;
	void render(RectF rect, SDL_RendererFlip flip, RenderColour colourMod) const;
	void render(RectF rect, double rotation, VectorF aboutPoint) const;

	void reset();
	void clear();

	Texture* texture() const { return mAnimations[mActiveIndex].texture(); }

	bool hasAnimations() const { return (bool)mAnimations.size(); }
	void selectAnimation(Action state);
	void startAnimation(Action state);

	void setSpeedFactor(float speed) { speedFactor = speed; }

	void start() { timer.start(); }
	void pause() { timer.pause(); }
	void stop();
	bool isRunning() const { return !timer.isPaused() && timer.isStarted(); }

	int loops() const { return mAnimations[mActiveIndex].mLoops; }
	VectorF frameSize() const { return mAnimations[mActiveIndex].mTileDimentions; }
	int frameCount() const { return mAnimations[mActiveIndex].frameCount(); }
	int currentFrame() const { return mAnimations[mActiveIndex].currentFrame(); }
	float frameTime() const { return mFrameTime; }
	int animationCount() const { return mAnimations.size(); }

	const Animation* activeAimation() const { return &mAnimations[mActiveIndex]; }
	Animation* activeAimation() { return &mAnimations[mActiveIndex]; }

private:

	int mActiveIndex;
	std::vector<Animation> mAnimations;

	float speedFactor;
	float mFrameTime;
	Timer<float> timer;
};
