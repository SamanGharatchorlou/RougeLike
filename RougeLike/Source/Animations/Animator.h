#pragma once

#include "Graphics/Tileset.h"

class Tile;
class Texture;
class XMLParser;



struct Animation
{
	// TODO: get better names
	Animation() : startingIndex(0), count(0), speed(0.0f) { }
	Animation(int start, int length, float theSpeed) :
		startingIndex(start), count(length), speed(theSpeed) { }

	int startingIndex;
	int count;
	float speed;
};


using Animations = std::unordered_map<std::string, Animation>;


class Animator
{
public:
	Animator() : animationIndex(0), loops(0) { }

	void init(Tileset spriteSheet, Animations animations);
	void slowUpdate(float dt);
	void fastUpdate(float dt) { }

	void start() { timer.start(); }
	void reset();
	void pause() { timer.pause(); }
	void stop() { timer.stop(); }

	void selectAnimation(std::string name);

	int loopCount() const { return loops; }

	const Tile* getSpriteTile();
	Texture* getSpriteTexture();

private:
	Tileset mSpriteSheet;
	Animations mAnimations;

	Animation mActiveAnimation;

	int animationIndex;

	int loops;

	Timer<float> timer;
};