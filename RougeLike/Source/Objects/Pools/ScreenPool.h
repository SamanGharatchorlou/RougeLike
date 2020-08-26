#pragma once

#include "Utilities/ObjectPool.h"
#include "UI/Screens/Screen.h"

enum class ScreenType;

// TODO: overrite get new object function so it doesnt create new screens?
class ScreenPool : public ObjectPool<Screen, ScreenType>
{
public:
	Screen* screenRef(ScreenType type) const;

private:
	Screen* createNewObject(ScreenType type) const override;
};
