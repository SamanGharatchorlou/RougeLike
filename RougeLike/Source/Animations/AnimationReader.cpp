#include "pch.h"
#include "AnimationReader.h"

#include "Graphics/TextureManager.h"


Animator AnimationReader::buildAnimator(XMLNode animationNode) const
{
	Animator animator;

	BasicString id = animationNode.child("ID").value();

	float frameTime = toFloat(animationNode.child("FrameTime").value());
	animator.setFrameTime(frameTime);

	XMLNode frameSizeNode = animationNode.child("FrameSize");

	StringMap attributes = animationNode.child("FrameSize").attributes();
	VectorF frameSize = attributes.getVector("x", "y");

	XMLNode animations = animationNode.child("Animations");
	XMLNode node = animations.child();

	const TextureManager* textures = TextureManager::Get();

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
