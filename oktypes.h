#pragma once

//#define _USING_V110_SDK71_ 1

//#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS 1
//#endif
#define _CRT_SECURE_NO_WARNINGS 1




#include "SDL_platform.h"


#ifdef __WINDOWS__
#include "vcruntime.h"
#endif

//#define GOOGLE_ARRAYSIZE(a) ((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

///-----------------------------
///C libs
///-----------------------------
//#include <stdlib.h>
//#include <math.h>
//#include <time.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <string.h>
//
//#include <stdarg.h>
//#include <assert.h>
//#include <setjmp.h>
//#include <sys/stat.h>
//#include <errno.h>
//#include <fcntl.h>
//#include <malloc.h>



///-----------------------------
///C++ libs
///-----------------------------
//#include <iostream>
//#include <iomanip>
#include <string>
//#include <vector>
//#include <deque>
//#include <algorithm>
//#include <unordered_map>
//#include <utility>
//#include <limits>
//#include <random>
//#include <fstream>
//#include <memory>
//#include <cmath>





//glu is outdated and i dont have it anyway
#ifndef GLEW_NO_GLU
#define GLEW_NO_GLU
#endif
//because im including glew.c instead of linking to the lib/dll
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include "GL/glew.h"

#ifdef __WINDOWS__
#include "GL/wglew.h"
#endif
#ifdef __LINUX__
#include "GL/glxew.h"
#endif





//#define __WINDOWS__




//#define USE_SOLOUD
#define USE_SDL_MIXER

///-----------------------------
///SDL libs
///-----------------------------
#include "SDL.h"

//disable GL extensions through SDL, use glew instead
#define NO_SDL_GLEXT
#include "SDL_opengl.h"

#define BYTE_RED 2
#define BYTE_GREEN 1
#define BYTE_BLUE 0

#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_net.h"

//#include "SDL2_rotozoom.h"
//#include "SDL2_framerate.h"
//#include "SDL2_gfxPrimitives.h"
//#include "SDL2_imageFilter.h"

//#include "SDL_stbimage.h"
//#include "stb_image.h"
//#include "stb_image_resize.h"
//#include "stb_image_write.h"
//#include "stb_truetype.h"

//#include "DG_misc.h"
//#include "imgui.h"

#pragma warning (push) 
#pragma warning( disable : 4244 ) 
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>


#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#pragma warning (pop) 


using namespace std;

typedef unsigned char u8;


class ByteArray
{
	u8* bytes = nullptr;
	unsigned int len = 0;
public:

	ByteArray(u8* bytes, unsigned int len)
	{
		this->bytes = bytes;
		this->len = len;
	}

	ByteArray(unsigned int len)
	{
		this->bytes = (u8*)calloc(len, 1);// new u8[len];
		this->len = len;
	}

	~ByteArray()
	{
		free(bytes);
		//delete[] bytes;
	}

	u8* data()
	{
		return bytes;
	}

	long long size()
	{
		return len;
	}
};

class IntArray
{
	unsigned int* ints = nullptr;
	unsigned int len = 0;
public:

	IntArray(unsigned int* ints, unsigned int len)
	{
		this->ints = ints;
		this->len = len;
	}
	IntArray(unsigned int len)
	{
		this->ints = (unsigned int*)calloc(len*4, 4);// new int[len];
		this->len = len;
	}
	~IntArray()
	{
		free(ints);
		//delete[] ints;
	}
	unsigned int* data()
	{
		return ints;
	}

	long long size()
	{
		return len;
	}
};


typedef struct
{
	int image_w;
	int image_h;
	int texture_w;
	int texture_h;
	GLuint texture_id;
} texture_STRUCT;


typedef struct
{
	string text;
	float x;
	float y;
	int width;
	//SDL_Color color;
} DEBUG_overlay_STRUCT;



struct GFX
{
	string FileName;// [128];
	int* indexed_gfx_data;
	int data_size_x;
	int data_size_y;

	int texture_size_x;
	int texture_size_y;

	int IndexInCachedTextureIDArray;

	bool shadow; //this is used to decide whether the texture gfx will be generated with a shadow or not

	bool hq2x; //this is needed because sprites can have their own scaling, and hq2x textures are all drawn at 0.5x by default and need the hq2x palette

	bool kid;//set on load based on size, used because kids/adults/cars need a larger texture for shadows, since the kid already fills a 32*64 texture, it creates a 32*128 one instead

	bool adult;

	bool car;
	bool bike;//car or bike will generate random alternate colors from the texture by messing with color channels. when creating a car or bike sprite, set the frame accordingly to choose a color

	bool generic;//this should somehow generate random color sets of clothing from the greyscale people. not sure how yet, especially with hq2x, i would need to output with outfit colors and then manually adjust the hq2x palette.

	int content_size_x;
	int content_size_y;
}; //this replaces gfx data

   //const GFX GFX_INIT = {"hello",NULL,0,0,-1,0};


struct SPRITE
{
	GLuint texture_id;

	GFX* gfx;

	int alpha;
	int layer;
	float scale;
	float screen_x;
	float screen_y;

	//bool draw_full_texture;

	int draw_size_x;
	int draw_size_y;//this is used to determine whether the shadow is clipped or not


					//cached or volatile? probably dont need this


					//PLAYER_gfx_slot_alpha need alpha, but it should just be per sprite, shouldnt it?

					//car
					//kid
					//bike

					//GFX struct? //if i store the gfx in here, i wont need to send in the GFX for every update_gfx_slot, i can just use NULL!


					//frames
					//layer/priority

					//zoom/getScale
					//screenx
					//screeny
					//alpha?
}; //this replaces sprite id and gfx slot, the only reason for having this is so i can index them in an array and z-order them, and then draw them in order during render


enum class RenderOrder
{

		
		GROUND,
		ABOVE,

		ABOVE_TOP, //over overlayer, underneath lights

				   //sprites over top
				   //captions
				   //overlay under lights
				   //stadium screen


				   //lights

				   //should have birds here?

				   SPRITE_DEBUG_OUTLINES,
				   SPRITE_DEBUG_INFO,
				   OVER_TEXT,
				   OVER_GUI,
				   CONSOLE,
	


};

static RenderOrder RenderOrderValueOf(string s)
{
	if (s == "GROUND")return RenderOrder::GROUND;
	if (s == "ABOVE")return RenderOrder::ABOVE;
	if (s == "ABOVE_TOP")return RenderOrder::ABOVE_TOP;
	if (s == "SPRITE_DEBUG_OUTLINES")return RenderOrder::SPRITE_DEBUG_OUTLINES;
	if (s == "SPRITE_DEBUG_INFO")return RenderOrder::SPRITE_DEBUG_INFO;
	if (s == "OVER_TEXT")return RenderOrder::OVER_TEXT;
	if (s == "OVER_GUI")return RenderOrder::OVER_GUI;
	if (s == "CONSOLE")return RenderOrder::CONSOLE;
	return RenderOrder::GROUND;
}


class Info
{
public:
	string label = "";
	string tip = "";
	Info(string label, string tip)
	{
		this->label = label;
		this->tip = tip;
	}
};

   //#include "soloud.h"
   //#include "soloud_wav.h"
   //#include "soloud_modplug.h"
   //#include "soloud_wavstream.h"


   //#include "SDL_config_lib.h"

   //extern "C"
   //{
   //#include "../lib/ini/iniparser.h"
   //#include "../lib/ini/textfile.h"
   //}

   //-----------------------------
   //defines
   //-----------------------------
#define GL_TEXTURE_CROP_RECT_OES 0



