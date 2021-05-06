#pragma once

#include "Utilities/ObjectPool.h"
#include "UI/Screens/Screen.h"

enum class ScreenType;

class ScreenPool : public ObjectPool<Screen, ScreenType>
{
public:
	Screen* screenRef(ScreenType type) const;

private:
	Screen* createNewObjects(ScreenType type, int count, int& outSize) const override;
};
