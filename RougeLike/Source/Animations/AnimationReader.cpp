#include "pch.h"
#include "AnimationReader.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"


Animator AnimationReader::buildAnimator(XMLNode animationNode, const TextureManager* textures) const
{
	Animator animator;

	BasicString id = animationNode.child("ID").getString();

	float frameTime = animationNode.child("FrameTime").getFloat();
	animator.setFrameTime(frameTime);

	XMLNode frameSizeNode = animationNode.child("FrameSize");
	Vector2D<int> frameSize = getXYAttributes(frameSizeNode);

	XMLNode animations = animationNode.child("Animations");
	XMLNode node = animations.child();

	while (node)
	{
		BasicString fileName = id + "_" + node.name();
		Texture* texture = textures->getTexture(fileName, FileManager::Image_Animations);

		int frames = node.getInt();

		Action action = stringToAction(node.name());

		AnimationData data(texture, frameSize, frames, action);
		animator.addAnimation(data);

		node = node.next();
	}

	return animator;
}
