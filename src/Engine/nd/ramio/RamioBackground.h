//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


#pragma once
#include "oktypes.h"
class Logger;


#include "../../map/Map.h"


class RamioBackground : public Map
{
public:

	static Logger log;


	RamioBackground(sp<Engine> g);


	int RAMIO_screen_x = 0;
	int RAMIO_screen_y = 0;

	int RAMIO_tv_rolling = 0;

	int RAMIO_scrollingplayingfield_map_x = 0;
	int RAMIO_scrollingplayingfield_map_y = 0;
	int RAMIO_playing_field_map_scroll_x = 0;


	int RAMIO_map_x = 0;
	int RAMIO_offset_x = 0;


	int RAMIO_map_width = 0;


	int RAMIO_tileset_size = 0;

	int dir = 0;

	bool fadeInOutPulseToggle = false;


	void RAMIO_shakeplayingfield();


	void init();


	void update();
};

