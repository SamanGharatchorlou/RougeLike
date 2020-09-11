#pragma once

#include "Events/Events.h"

class ScreenController;
class Screen;

class UIEventHandler
{
public:
	void handleEvent(ScreenController* screenController, EventData& data);

private:
	void updateTextBox		(Screen* activeScreen, UpdateTextBoxEvent& eventData);
	void setUIbar			(Screen* activeScreen, SetUIBarEvent& eventData);
	void setTextColour		(Screen* activeScreen, SetTextColourEvent& eventData);
	void setSliderValue		(Screen* activeScreen, SetUISlider& eventData);


#if UI_EDITOR
	void moveElement(Screen* activeScreen, EditUIRectEvent& eventData);
	void setElementSize(Screen* activeScreen, EditUIRectEvent& eventData);
#endif
};