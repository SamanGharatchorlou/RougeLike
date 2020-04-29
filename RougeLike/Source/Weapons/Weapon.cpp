#include "pch.h"
#include "Weapon.h"

//
//const std::string* Weapon::consumeAudioEvent()
//{
//	//bool audioEvent = mHasAudio;
//	//mHasAudio = false;
//	//return audioEvent;
//
//	const std::string* audioEventLabel = mAudio;
//	mAudio = nullptr;
//
//	return audioEventLabel;
//}

WeaponAudioEvent Weapon::consumeAudioEvent()
{
	WeaponAudioEvent audioEvent(mAudioToPlay, mAudioToStop);

	mAudioToPlay = nullptr;
	mAudioToStop = nullptr;

	return audioEvent;
}