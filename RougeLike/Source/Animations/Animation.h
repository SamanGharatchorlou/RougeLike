#pragma once

class Texture;


class Animation
{
	friend class Animator;

public:
	enum Action
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

	struct Data
	{
		Data(Texture* animTexture, VectorF tileDim, int frames, Action animAction) :
			texture(animTexture), tileDimentions(tileDim), frameCount(frames), action(animAction) { }

		Texture* texture;
		VectorF tileDimentions;
		int frameCount;
		Action action;
	};

public:
	Animation(const Data& data);

	void reset();

	void render(RectF rect, SDL_RendererFlip flip) const;
	void render(RectF rect, SDL_RendererFlip flip, Uint8 alpha) const;
	void render(RectF rect, SDL_RendererFlip flip, RenderColour colourMod) const;
	void render(RectF rect, double rotation, VectorF aboutPoint) const;

	void nextFrame();
	int currentFrame() const;
	

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


Animation::Action stringToAction(const BasicString& action);