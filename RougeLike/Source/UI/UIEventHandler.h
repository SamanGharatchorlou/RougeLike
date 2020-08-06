#pragma once

#include "Events/Events.h"

class Screen;

class UIEventHandler
{
public:
	void handleEvent(Screen* activeScreen, EventData& data);

private:
	// Event handling functions
	void updateTextBox		(Screen* activeScreen, UpdateTextBoxEvent& eventData);
	void setUIbar			(Screen* activeScreen, SetUIBarEvent& eventData);
	void setHealth			(Screen* activeScreen, SetHealthBarEvent& eventData);
	void setArmor			(Screen* activeScreen, SetArmorBarEvent& eventData);
	void moveElement		(Screen* activeScreen, EditUIRectEvent& eventData);
	void setElementSize		(Screen* activeScreen, EditUIRectEvent& eventData);
	void setRect			(Screen* activeScreen, SetUIRectEvent& eventData);
	void setTextColour		(Screen* activeScreen, SetTextColourEvent& eventData);
	//void setMusicVolume(SetVolumeEvent& eventData);
	//void setGameVolume(SetVolumeEvent& eventData);
};