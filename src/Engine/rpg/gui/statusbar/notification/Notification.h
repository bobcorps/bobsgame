//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------

#pragma once
#include "oktypes.h"
class Logger;



#include "../buttons/StatusBarButton.h"

class Notification : public StatusBarButton
{
public:

	static Logger log;

	shared_ptr<Caption> caption = nullptr;

	string notificationString;

	bool fadeIn = false;
	bool fadeOut = false;

	float progress = 0;
	float lastProgress = 0;
	int progressBarFrame = 0;
	bool hasProgressBar = true;
	int progressTicks = 0;
	int lastProgressTicks = 0;

	float alpha = 0.0f;

	float scrollX = 0;
	bool scrolling = false;

	Notification(shared_ptr<BGClientEngine> g, const string& s);

	virtual void update() override;

	virtual void render(int layer) override;

	shared_ptr<Notification> fadeOutAndDelete();
};

