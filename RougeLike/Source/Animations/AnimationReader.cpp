#include "pch.h"
#include "AnimationReader.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"


Animator AnimationReader::buildAnimator(XMLNode animationNode, const TextureManager* textures) const
{
	Animator animator;

	BasicString id = animationNode.child("ID").value();

	float frameTime = toFloat(animationNode.child("FrameTime").value());
	animator.setFrameTime(frameTime);

	XMLNode frameSizeNode = animationNode.child("FrameSize");

	StringMap attributes = animationNode.child("FrameSize").attributes();
	int x = attributes.getInt("x");
	int y = attributes.getInt("y");
	Vector2D<int> frameSize(x, y);

	XMLNode animations = animationNode.child("Animations");
	XMLNode node = animations.child();

	while (node)
	{
		BasicString fileName = id + "_" + node.name();
		Texture* texture = textures->getTexture(fileName, FileManager::Image_Animations);

		int frames = toInt(node.value());

		Action action = stringToAction(node.name());

		AnimationData data(texture, frameSize, frames, action);
		animator.addAnimation(data);

		node = node.next();
	}

	return animator;
}
