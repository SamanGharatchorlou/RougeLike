#pragma once

#include "Graphics/Tileset.h"
#include "Animation.h"


class Animator
{
public:
	Animator();

	void setFrameTime(float frameTime) { mFrameTime = frameTime; }
	void addAnimation(const Animation::Data& data);

	void slowUpdate(float dt);
	void render(RectF rect, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	void render(RectF rect, SDL_RendererFlip flip, Uint8 alpha) const;
	void render(RectF rect, SDL_RendererFlip flip, RenderColour colourMod) const;
	void render(RectF rect, double rotation, VectorF aboutPoint) const;

	void reset();
	void clear();

	const Animation* getAnimation(Animation::Action action) const;
	float animationTime(Animation::Action action) const;

	const Animation* activeAimation() const { return &mAnimations[mActiveIndex]; }
	Animation* activeAimation() { return &mAnimations[mActiveIndex]; }

	Animation::Action currentAction() const { return mAnimations[mActiveIndex].mState; }

	int animationCount() const { return mAnimations.size(); }

	void selectAnimation(Animation::Action state);
	void startAnimation(Animation::Action state);

	void setSpeedFactor(float speed) { speedFactor = speed; }

	void start() { timer.start(); }
	void pause() { timer.pause(); }
	void stop();
	bool isRunning() const { return !timer.isPaused() && timer.isStarted(); }

	int loops() const { return mAnimations[mActiveIndex].mLoops; }

	VectorF frameSize() const { return mAnimations[mActiveIndex].mTileDimentions; }
	int frameCount() const { return mAnimations[mActiveIndex].mFrameCount; }
	int currentFrame() const { return mAnimations[mActiveIndex].currentFrame(); }
	float frameTime() const { return mFrameTime; }

	bool isLastFrame() const { return mAnimations[mActiveIndex].currentFrame() == mAnimations[mActiveIndex].mFrameCount - 1; }


private:
	int mActiveIndex;
	std::vector<Animation> mAnimations;

	float speedFactor;
	float mFrameTime;
	TimerF timer;
};
