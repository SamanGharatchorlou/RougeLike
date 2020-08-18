#pragma once

#include "Events/Events.h"

class Screen;

class UIEventHandler
{
public:
	void handleEvent(Screen* activeScreen, EventData& data);

private:
	void updateTextBox		(Screen* activeScreen, UpdateTextBoxEvent& eventData);
	void setUIbar			(Screen* activeScreen, SetUIBarEvent& eventData);
	void setTextColour		(Screen* activeScreen, SetTextColourEvent& eventData);
	void setSliderValue		(Screen* activeScreen, SetUISlider& eventData);
	//void setMusicVolume(SetVolumeEvent& eventData);
	//void setGameVolume(SetVolumeEvent& eventData);

#if UI_EDITOR
	void moveElement(Screen* activeScreen, EditUIRectEvent& eventData);
	void setElementSize(Screen* activeScreen, EditUIRectEvent& eventData);
#endif
};