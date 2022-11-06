﻿#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once



Logger BobColor::log = Logger("Color");
bool BobColor::colorsInitialized = false;


ArrayList<shared_ptr<BobColor>> BobColor::colors;

shared_ptr<BobColor> BobColor::clear = make_shared<BobColor>(0, 0, 0, 0);
shared_ptr<BobColor> BobColor::transparent = clear;
shared_ptr<BobColor> BobColor::black = make_shared<BobColor>(0, 0, 0);
shared_ptr<BobColor> BobColor::darkerGray = make_shared<BobColor>(0.2f * 255, 0.2f * 255, 0.2f * 255);
shared_ptr<BobColor> BobColor::darkGray = make_shared<BobColor>(0.35f * 255, 0.35f * 255, 0.35f * 255);
shared_ptr<BobColor> BobColor::gray = make_shared<BobColor>(0.5f * 255, 0.5f * 255, 0.5f * 255);
shared_ptr<BobColor> BobColor::lightGray = make_shared<BobColor>(0.65f * 255, 0.65f * 255, 0.65f * 255);
shared_ptr<BobColor> BobColor::lighterGray = make_shared<BobColor>(0.8f * 255, 0.8f * 255, 0.8f * 255);
shared_ptr<BobColor> BobColor::white = make_shared<BobColor>(1.0f*255, 1.0f * 255, 1.0f * 255);

shared_ptr<BobColor> BobColor::cyan = make_shared<BobColor>(0, 255, 255);
shared_ptr<BobColor> BobColor::lightCyan = make_shared<BobColor>(0, 255, 255); //cyan->lighter();
shared_ptr<BobColor> BobColor::lighterCyan = make_shared<BobColor>(0, 255, 255); //cyan->lighter();
shared_ptr<BobColor> BobColor::darkCyan = make_shared<BobColor>(0, 255, 255); //cyan->darker();
shared_ptr<BobColor> BobColor::darkerCyan = make_shared<BobColor>(0, 255, 255); //cyan->darker()->darker();

shared_ptr<BobColor> BobColor::magenta = make_shared<BobColor>(255, 0, 127);
shared_ptr<BobColor> BobColor::lightMagenta = make_shared<BobColor>(255, 0, 127); //magenta->lighter();
shared_ptr<BobColor> BobColor::lighterMagenta = make_shared<BobColor>(255, 0, 127); //magenta->lighter();
shared_ptr<BobColor> BobColor::darkMagenta = make_shared<BobColor>(255, 0, 127); //magenta->darker();
shared_ptr<BobColor> BobColor::darkerMagenta = make_shared<BobColor>(255, 0, 127); //magenta->darker()->darker();

shared_ptr<BobColor> BobColor::yellow = make_shared<BobColor>(255, 255, 0);
shared_ptr<BobColor> BobColor::lightYellow = make_shared<BobColor>(255, 255, 0); //yellow->lighter();
shared_ptr<BobColor> BobColor::lighterYellow = make_shared<BobColor>(255, 255, 0); //yellow->lighter();
shared_ptr<BobColor> BobColor::darkYellow = make_shared<BobColor>(255, 255, 0); //yellow->darker();
shared_ptr<BobColor> BobColor::darkerYellow = make_shared<BobColor>(255, 255, 0); //yellow->darker()->darker();

shared_ptr<BobColor> BobColor::orange = make_shared<BobColor>(255, 140, 0);
shared_ptr<BobColor> BobColor::lightOrange = make_shared<BobColor>(255, 140, 0); //orange->lighter();
shared_ptr<BobColor> BobColor::lighterOrange = make_shared<BobColor>(255, 140, 0); //orange->lighter();
shared_ptr<BobColor> BobColor::darkOrange = make_shared<BobColor>(255, 140, 0); //orange->darker();
shared_ptr<BobColor> BobColor::darkerOrange = make_shared<BobColor>(255, 140, 0); //orange->darker()->darker();

shared_ptr<BobColor> BobColor::red = make_shared<BobColor>(255, 0, 0);
shared_ptr<BobColor> BobColor::lightRed = make_shared<BobColor>(255, 0, 0); //red->lighter();
shared_ptr<BobColor> BobColor::lighterRed = make_shared<BobColor>(255, 0, 0); //red->lighter();
shared_ptr<BobColor> BobColor::darkRed = make_shared<BobColor>(255, 0, 0); //red->darker();
shared_ptr<BobColor> BobColor::darkerRed = make_shared<BobColor>(255, 0, 0); //red->darker()->darker();

shared_ptr<BobColor> BobColor::pink = make_shared<BobColor>(255, 0, 255);
shared_ptr<BobColor> BobColor::lightPink = make_shared<BobColor>(255, 0, 255); //pink->lighter();
shared_ptr<BobColor> BobColor::lighterPink = make_shared<BobColor>(255, 0, 255); //pink->lighter();
shared_ptr<BobColor> BobColor::darkPink = make_shared<BobColor>(255, 0, 255); //pink->darker();
shared_ptr<BobColor> BobColor::darkerPink = make_shared<BobColor>(255, 0, 255); //pink->darker()->darker();

shared_ptr<BobColor> BobColor::purple = make_shared<BobColor>(127, 0, 255);
shared_ptr<BobColor> BobColor::lightPurple = make_shared<BobColor>(127, 0, 255); //purple->lighter();
shared_ptr<BobColor> BobColor::lighterPurple = make_shared<BobColor>(127, 0, 255); //purple->lighter();
shared_ptr<BobColor> BobColor::darkPurple = make_shared<BobColor>(127, 0, 255); //purple->darker();
shared_ptr<BobColor> BobColor::darkerPurple = make_shared<BobColor>(127, 0, 255); //purple->darker()->darker();


shared_ptr<BobColor> BobColor::blue = make_shared<BobColor>(0, 0, 255);
shared_ptr<BobColor> BobColor::lighterBlue = make_shared<BobColor>(0, 0, 255); //blue->lighter();
shared_ptr<BobColor> BobColor::lightBlue = make_shared<BobColor>(0, 0, 255); //blue->lighter();
shared_ptr<BobColor> BobColor::darkBlue = make_shared<BobColor>(0, 0, 255); //blue->darker();
shared_ptr<BobColor> BobColor::darkerBlue = make_shared<BobColor>(0, 0, 255); //blue->darker()->darker();

shared_ptr<BobColor> BobColor::green = make_shared<BobColor>(0, 255, 0);
shared_ptr<BobColor> BobColor::lighterGreen = make_shared<BobColor>(0, 255, 0); //green->lighter()->lighter();
shared_ptr<BobColor> BobColor::lightGreen = make_shared<BobColor>(0, 255, 0); //green->lighter();
shared_ptr<BobColor> BobColor::darkGreen = make_shared<BobColor>(0, 255, 0); //green->darker();
shared_ptr<BobColor> BobColor::darkerGreen = make_shared<BobColor>(0, 255, 0); //green->darker()->darker();

shared_ptr<BobColor> BobColor::aqua = make_shared<BobColor>(0, 150, 255);
shared_ptr<BobColor> BobColor::lighterAqua = make_shared<BobColor>(0, 150, 255); //aqua->lighter()->lighter();
shared_ptr<BobColor> BobColor::lightAqua = make_shared<BobColor>(0, 150, 255); //aqua->lighter();
shared_ptr<BobColor> BobColor::darkAqua = make_shared<BobColor>(0, 150, 255); //aqua->darker();
shared_ptr<BobColor> BobColor::darkerAqua = make_shared<BobColor>(0, 150, 255); //aqua->darker()->darker();

shared_ptr<BobColor> BobColor::turquoise = make_shared<BobColor>(0, 255, 150);
shared_ptr<BobColor> BobColor::lighterTurquoise = make_shared<BobColor>(0, 255, 150); //turquoise->lighter()->lighter();
shared_ptr<BobColor> BobColor::lightTurquoise = make_shared<BobColor>(0, 255, 150); //turquoise->lighter();
shared_ptr<BobColor> BobColor::darkTurquoise = make_shared<BobColor>(0, 255, 150); //turquoise->darker();
shared_ptr<BobColor> BobColor::darkerTurquoise = make_shared<BobColor>(0, 255, 150); //turquoise->darker()->darker();

shared_ptr<BobColor> BobColor::olive = make_shared<BobColor>(64, 72, 0);
shared_ptr<BobColor> BobColor::darkerOlive = make_shared<BobColor>(64, 72, 0);
shared_ptr<BobColor> BobColor::darkOlive = make_shared<BobColor>(64, 72, 0);
shared_ptr<BobColor> BobColor::lighterOlive = make_shared<BobColor>(64, 72, 0);
shared_ptr<BobColor> BobColor::lightOlive = make_shared<BobColor>(64, 72, 0);

shared_ptr<BobColor> BobColor::burgandy = make_shared<BobColor>(220, 70, 0);
shared_ptr<BobColor> BobColor::darkerBurgandy = make_shared<BobColor>(220, 70, 0);
shared_ptr<BobColor> BobColor::darkBurgandy = make_shared<BobColor>(220, 70, 0);
shared_ptr<BobColor> BobColor::lighterBurgandy = make_shared<BobColor>(220, 70, 0);
shared_ptr<BobColor> BobColor::lightBurgandy = make_shared<BobColor>(220, 70, 0);


shared_ptr<BobColor> BobColor::wine = make_shared<BobColor>(200, 80, 0);



//==========================================================================================================================
int HARDWARE_RGB(int r, int g, int b)
{//==========================================================================================================================
	return ((1 << 15) + (r) + ((g) << 5) + ((b) << 10));
}


//==========================================================================================================================
void RGBToHSL(float r, float g, float b, float* h, float* s, float* l)
{//==========================================================================================================================


	//float fmin = min(min(r, g), b);    //Min. value of RGB
	//float fmax = max(max(r, g), b);    //Max. value of RGB
	float fmin = r;
	if (g < fmin)fmin = g;
	if (b < fmin)fmin = b;

	float fmax = r;
	if (g > fmax)fmax = g;
	if (b > fmax)fmax = b;

	float delta = fmax - fmin; //Delta RGB value

	*l = (fmax + fmin) / 2.0f; // Luminance

	if (delta == 0.0f) //This is a gray, no chroma...
	{
		*h = 0.0f; // Hue
		*s = 0.0f; // Saturation
	}
	else //Chromatic data...
	{
		if (*l < 0.5f)
			*s = delta / (fmax + fmin); // Saturation
		else
			*s = delta / (2.0f - fmax - fmin); // Saturation

		float deltaR = (((fmax - r) / 6.0f) + (delta / 2.0f)) / delta;
		float deltaG = (((fmax - g) / 6.0f) + (delta / 2.0f)) / delta;
		float deltaB = (((fmax - b) / 6.0f) + (delta / 2.0f)) / delta;

		if (r == fmax)
			*h = deltaB - deltaG; // Hue
		else if (g == fmax)
			*h = (1.0f / 3.0f) + deltaR - deltaB; // Hue
		else if (b == fmax)
			*h = (2.0f / 3.0f) + deltaG - deltaR; // Hue

		if (*h < 0.0f)
			*h += 1.0f; // Hue
		else if (*h > 1.0f)
			*h -= 1.0f; // Hue
	}
}

//==========================================================================================================================
float HueToRGB(float f1, float f2, float hue)
{//==========================================================================================================================
	if (hue < 0.0f)
		hue += 1.0f;
	else if (hue > 1.0f)
		hue -= 1.0f;
	float res;
	if ((6.0f * hue) < 1.0f)
		res = f1 + (f2 - f1) * 6.0f * hue;
	else if ((2.0f * hue) < 1.0f)
		res = f2;
	else if ((3.0f * hue) < 2.0f)
		res = f1 + (f2 - f1) * ((2.0f / 3.0f) - hue) * 6.0f;
	else
		res = f1;
	return res;
}

//==========================================================================================================================
void HSLToRGB(float h, float s, float l, float* r, float* g, float* b)
{//==========================================================================================================================


	if (s == 0.0f)
	{
		*r = l;
		*g = l;
		*b = l;
	}
	else
	{
		float f2;

		if (l < 0.5f)
			f2 = l * (1.0f + s);
		else
			f2 = (l + s) - (s * l);

		float f1 = 2.0f * l - f2;

		*r = HueToRGB(f1, f2, h + (1.0f / 3.0f));
		*g = HueToRGB(f1, f2, h);
		*b = HueToRGB(f1, f2, h - (1.0f / 3.0f));
	}
}

//float adjust_gamma = 1.0f;
//float adjust_brightness = 1.0f;
//float adjust_lightness = 1.0f;

//==========================================================================================================================
void adjust_color(int* pr, int* pg, int* pb)
{//==========================================================================================================================

	int r, g, b;
	r = *pr;
	g = *pg;
	b = *pb;

	//if its not black
	if (r != b || r != g || (r == b && r == g && r > 16))
	{
		float fr, fg, fb, fh, fs, fl;
		fr = (float)((float)r / 256.0f);
		fg = (float)((float)g / 256.0f);
		fb = (float)((float)b / 256.0f);


		//gamma
		//fr = pow(fr,0.8f);
		//fg = pow(fg,0.8f);
		//fb = pow(fb,0.8f);

		//increase blue hue unless color is gray
		if (r != b || r != g)
			fb = fb + (5.0f / 256.0f);

		//increase lightness
		RGBToHSL(fr, fg, fb, &fh, &fs, &fl);
		fl += (10.0f / 256.0f);
		fs += (5.0f / 256.0f);
		if (fl > 1.0f)fl = 1.0f;
		if (fs > 1.0f)fs = 1.0f;
		HSLToRGB(fh, fs, fl, &fr, &fg, &fb);

		//brighten
		//fr*=1.1f;
		//fg*=1.1f;
		//fb*=1.1f;


		r = (int)(fr * 256.0f);
		g = (int)(fg * 256.0f);
		b = (int)(fb * 256.0f);

		if (r > 255)r = 255;
		if (g > 255)g = 255;
		if (b > 255)b = 255;
	}

	*pr = r;
	*pb = b;
	*pg = g;

	/*
	vec3 hslcolor = RGBToHSL(color);
	//hslcolor.x+=(3.0f/255.0f);
	hslcolor.z+=(5.0f/255.0f);
	color = HSLToRGB(hslcolor);


	color = ContrastSaturationBrightness(color,1.0,1.0,1.1);

	*/
}


//==========================================================================================================================
void HARDWARE_create_brightened_palette(int* pal, int* newpal, int bright)//HARDWARE_CreatePalBright
{//==========================================================================================================================

	int x = 0;
	for (x = 0; x < 256; x++)
	{
		//bgr->rgb->hsl->lighten->rgb->bgr
		/*

		int bgr=pal[x];

		//bgr to r g b

		int r=((((bgr%32))*8));
		int g=((((bgr/32)%32))*8);
		int b=(((bgr/1024))*8);


		//r g b to h s l

		float hue, saturation, brightness;

		int cmax = (r > g) ? r : g;
		if (b > cmax) cmax = b;
		int cmin = (r < g) ? r : g;
		if (b < cmin) cmin = b;

		brightness = ((float) cmax) / 255.0f;
		if (cmax != 0)
		saturation = ((float) (cmax - cmin)) / ((float) cmax);
		else
		saturation = 0;
		if (saturation == 0)
		hue = 0;
		else {
		float redc = ((float) (cmax - r)) / ((float) (cmax - cmin));
		float greenc = ((float) (cmax - g)) / ((float) (cmax - cmin));
		float bluec = ((float) (cmax - b)) / ((float) (cmax - cmin));
		if (r == cmax)
		hue = bluec - greenc;
		else if (g == cmax)
		hue = 2.0f + redc - bluec;
		else
		hue = 4.0f + greenc - redc;
		hue = hue / 6.0f;
		if (hue < 0)
		hue = hue + 1.0f;
		}


		//h s l lighten

		brightness=brightness*((((float)bright)+31.0f)/31.0f);
		if(brightness<0.0f)brightness=0.0f;
		//if(brightness>1.0f)brightness=1.0f;

		//h s l to r g b



		if (saturation == 0)
		{
		r = g = b = (int) (brightness * 255.0f + 0.5f);
		}
		else
		{
		float h = (hue - (float)floor(hue)) * 6.0f;
		float f = h - (float)floor(h);
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));
		switch ((int) h)
		{
		case 0:
		r = (int) (brightness * 255.0f + 0.5f);
		g = (int) (t * 255.0f + 0.5f);
		b = (int) (p * 255.0f + 0.5f);
		break;
		case 1:
		r = (int) (q * 255.0f + 0.5f);
		g = (int) (brightness * 255.0f + 0.5f);
		b = (int) (p * 255.0f + 0.5f);
		break;
		case 2:
		r = (int) (p * 255.0f + 0.5f);
		g = (int) (brightness * 255.0f + 0.5f);
		b = (int) (t * 255.0f + 0.5f);
		break;
		case 3:
		r = (int) (p * 255.0f + 0.5f);
		g = (int) (q * 255.0f + 0.5f);
		b = (int) (brightness * 255.0f + 0.5f);
		break;
		case 4:
		r = (int) (t * 255.0f + 0.5f);
		g = (int) (p * 255.0f + 0.5f);
		b = (int) (brightness * 255.0f + 0.5f);
		break;
		case 5:
		r = (int) (brightness * 255.0f + 0.5f);
		g = (int) (p * 255.0f + 0.5f);
		b = (int) (q * 255.0f + 0.5f);
		break;
		}
		}

		//r g b to bgr
		if(r>255)r=255;
		if(g>255)g=255;
		if(b>255)b=255;

		int newbgr =((b/8)*1024)+((g/8)*32)+((r/8));
		*/

		//plain old rgb lightening

		int r, g, b;

		r = pal[x] & 31;
		r += bright;
		if (r < 0) r = 0;
		if (r > 31) r = 31;
		g = (pal[x] >> 5) & 31;
		g += bright;
		if (g < 0) g = 0;
		if (g > 31) g = 31;
		b = (pal[x] >> 10) & 31;
		b += bright;
		if (b < 0) b = 0;
		if (b > 31) b = 31;
		newpal[x] = HARDWARE_RGB(r, g, b);
	}
}


//=========================================================================================================================
void BobColor::initPresetColors()
{//=========================================================================================================================
	if (colorsInitialized)return;

	colorsInitialized = true;
	BobColor c;

	lightCyan->lighter();
	lighterCyan->lighter();
	lighterCyan->lighter();
	darkCyan->darker();
	darkerCyan->darker();
	darkerCyan->darker();


	lightMagenta->lighter();
	lighterMagenta->lighter();
	lighterMagenta->lighter();
	darkMagenta->darker();
	darkerMagenta->darker();
	darkerMagenta->darker();


	lightYellow->lighter();
	lighterYellow->lighter();
	lighterYellow->lighter();
	darkYellow->darker();
	darkerYellow->darker();
	darkerYellow->darker();


	lightOrange->lighter();
	darkOrange->darker();
	darkerOrange->darker();
	darkerOrange->darker();


	lightRed->lighter();
	lighterRed->lighter();
	lighterRed->lighter();
	darkRed->darker();
	darkerRed->darker();
	darkerRed->darker();


	lightPink->lighter();
	lighterPink->lighter();
	lighterPink->lighter();
	darkPink->darker();
	darkerPink->darker();
	darkerPink->darker();


	lightPurple->lighter();
	lighterPurple->lighter();
	lighterPurple->lighter();
	darkPurple->darker();
	darkerPurple->darker();
	darkerPurple->darker();


	lightBlue->lighter();
	lighterBlue->lighter();
	lighterBlue->lighter();
	darkBlue->darker();
	darkerBlue->darker();
	darkerBlue->darker();


	lighterGreen->lighter();
	lighterGreen->lighter();
	lightGreen->lighter();
	darkGreen->darker();
	darkerGreen->darker();
	darkerGreen->darker();


	lighterAqua->lighter();
	lighterAqua->lighter();
	lightAqua->lighter();
	darkAqua->darker();
	darkerAqua->darker();
	darkerAqua->darker();


	lighterTurquoise->lighter();
	lighterTurquoise->lighter();
	lightTurquoise->lighter();
	darkTurquoise->darker();
	darkerTurquoise->darker();
	darkerTurquoise->darker();

	lighterOlive->lighter();
	lighterOlive->lighter();
	lightOlive->lighter();
	darkOlive->darker();
	darkerOlive->darker();
	darkerOlive->darker();

	lighterBurgandy->lighter();
	lighterBurgandy->lighter();
	lightBurgandy->lighter();
	darkBurgandy->darker();
	darkerBurgandy->darker();
	darkerBurgandy->darker();

	clear->name="clear";
	black->name="black";
	darkerGray->name="darkerGray";
	darkGray->name="darkGray";
	gray->name="gray";
	lightGray->name="lightGray";
	lighterGray->name="lighterGray";
	white->name="white";

	cyan->name="cyan";
	lightCyan->name="lightCyan";
	lighterCyan->name="lighterCyan";
	darkCyan->name="darkCyan";
	darkerCyan->name="darkerCyan";

	magenta->name="magenta";
	lightMagenta->name="lightMagenta";
	lighterMagenta->name="lighterMagenta";
	darkMagenta->name="darkMagenta";
	darkerMagenta->name="darkerMagenta";

	yellow->name="yellow";
	lightYellow->name="lightYellow";
	lighterYellow->name="lighterYellow";
	darkYellow->name="darkYellow";
	darkerYellow->name="darkerYellow";

	orange->name="orange";
	lightOrange->name="lightOrange";
	lighterOrange->name="lighterOrange";
	darkOrange->name="darkOrange";
	darkerOrange->name="darkerOrange";

	red->name="red";
	lightRed->name="lightRed";
	lighterRed->name="lighterRed";
	darkRed->name="darkRed";
	darkerRed->name="darkerRed";

	pink->name="pink";
	lightPink->name="lightPink";
	lighterPink->name="lighterPink";
	darkPink->name="darkPink";
	darkerPink->name="darkerPink";

	purple->name="purple";
	lightPurple->name="lightPurple";
	lighterPurple->name="lighterPurple";
	darkPurple->name="darkPurple";
	darkerPurple->name="darkerPurple";


	blue->name="blue";
	lighterBlue->name="lighterBlue";
	lightBlue->name="lightBlue";
	darkBlue->name="darkBlue";
	darkerBlue->name="darkerBlue";

	green->name="green";
	lighterGreen->name="lighterGreen";
	lightGreen->name="lightGreen";
	darkGreen->name="darkGreen";
	darkerGreen->name="darkerGreen";

	aqua->name="aqua";
	lighterAqua->name="lighterAqua";
	lightAqua->name="lightAqua";
	darkAqua->name="darkAqua";
	darkerAqua->name="darkerAqua";

	turquoise->name="turquoise";
	lighterTurquoise->name="lighterTurquoise";
	lightTurquoise->name="lightTurquoise";
	darkTurquoise->name="darkTurquoise";
	darkerTurquoise->name="darkerTurquoise";

	olive->name="olive";
	lighterOlive->name = "lighterOlive";
	lightOlive->name = "lightOlive";
	darkOlive->name = "darkOlive";
	darkerOlive->name = "darkerOlive";

	burgandy->name="burgandy";
	lighterBurgandy->name = "lighterBurgandy";
	lightBurgandy->name = "lightBurgandy";
	darkBurgandy->name = "darkBurgandy";
	darkerBurgandy->name = "darkerBurgandy";

	wine->name="wine";


	colors.add(clear);
	colors.add(transparent);
	colors.add(black);
	colors.add(darkerGray);
	colors.add(darkGray);
	colors.add(gray);
	colors.add(lightGray);
	colors.add(lighterGray);
	colors.add(white);
	colors.add(cyan);
	colors.add(lightCyan);
	colors.add(lighterCyan);
	colors.add(darkCyan);
	colors.add(darkerCyan);
	colors.add(magenta);
	colors.add(lightMagenta);
	colors.add(lighterMagenta);
	colors.add(darkMagenta);
	colors.add(darkerMagenta);
	colors.add(yellow);
	colors.add(lightYellow);
	colors.add(lighterYellow);
	colors.add(darkYellow);
	colors.add(darkerYellow);
	colors.add(orange);
	colors.add(lightOrange);
	colors.add(lighterOrange);
	colors.add(darkOrange);
	colors.add(darkerOrange);
	colors.add(red);
	colors.add(lightRed);
	colors.add(lighterRed);
	colors.add(darkRed);
	colors.add(darkerRed);
	colors.add(pink);
	colors.add(lightPink);
	colors.add(lighterPink);
	colors.add(darkPink);
	colors.add(darkerPink);
	colors.add(purple);
	colors.add(darkPurple);
	colors.add(darkerPurple);
	colors.add(lightPurple);
	colors.add(lighterPurple);
	colors.add(blue);
	colors.add(lightBlue);
	colors.add(lighterBlue);
	colors.add(darkBlue);
	colors.add(darkerBlue);
	colors.add(green);
	colors.add(lighterGreen);
	colors.add(lightGreen);
	colors.add(darkGreen);
	colors.add(darkerGreen);
	colors.add(aqua);
	colors.add(lighterAqua);
	colors.add(lightAqua);
	colors.add(darkAqua);
	colors.add(darkerAqua);
	colors.add(turquoise);
	colors.add(lighterTurquoise);
	colors.add(lightTurquoise);
	colors.add(darkTurquoise);
	colors.add(darkerTurquoise);
	colors.add(olive);
	colors.add(lighterOlive);
	colors.add(lightOlive);
	colors.add(darkOlive);
	colors.add(darkerOlive);
	colors.add(burgandy);
	colors.add(lighterBurgandy);
	colors.add(lightBurgandy);
	colors.add(darkBurgandy);
	colors.add(darkerBurgandy);
	colors.add(wine);

}


//==========================================================================================================================
shared_ptr<BobColor> BobColor::getColorByName(string name)
{//==========================================================================================================================

	if (name == "" || name == "empty")return nullptr;
	for(int i=0;i<colors.size();i++)
	{
		shared_ptr<BobColor >c = colors.get(i);

		if (c->name == name)return c;

	}
	return nullptr;


}


//=========================================================================================================================
BobColor::BobColor()
{//=========================================================================================================================
	name = "empty";

//	boost::uuids::random_generator generator;
//	uuid = to_string(generator());

}
////=========================================================================================================================
//BobColor::BobColor(shared_ptr<BobColor> c)
//{//=========================================================================================================================
//	if (c != nullptr)
//	{
//		name = c->name;
//		r = c->ri();
//		g = c->gi();
//		b = c->bi();
//		a = c->ai();
//	}
//	else
//		log.error("Color c was null!");
//}
//
////=========================================================================================================================
//BobColor::BobColor(const BobColor &c)
//{//=========================================================================================================================
//
////	boost::uuids::random_generator generator;
////	uuid = to_string(generator());
//
//		name = c.name;
//		r = c.r;
//		g = c.g;
//		b = c.b;
//		a = c.a;
//
//}
//=========================================================================================================================
BobColor::BobColor(BobColor c, u8 a)
{//=========================================================================================================================
//
//	boost::uuids::random_generator generator;
//	uuid = to_string(generator());

	this->name = c.name;
	this->r = c.ri();
	this->g = c.gi();
	this->b = c.bi();
	this->a = a;

}


////=========================================================================================================================
//BobColor::BobColor(BobColor c, float af)
//{//=========================================================================================================================
////
////	boost::uuids::random_generator generator;
////	uuid = to_string(generator());
//
//	name = c.name;
//	r = c.ri();
//	g = c.gi();
//	b = c.bi();
//	a = (int)(af * 255.0f);
//
//}
//
////=========================================================================================================================
//BobColor::BobColor(BobColor c, float af)
//{//=========================================================================================================================
//	if (c != nullptr)
//	{
//		name = c.name;
//		r = c.ri();
//		g = c.gi();
//		b = c.bi();
//		a = (int)(af * 255.0f);
//	}
//	else
//		log.error("Color c was null!");
//}
////=========================================================================================================================
//BobColor::BobColor(float rf, float gf, float bf)
//{//=========================================================================================================================
//
////	boost::uuids::random_generator generator;
////	uuid = to_string(generator());
//
//	r = (int)(rf * 255.0f);
//	g = (int)(gf * 255.0f);
//	b = (int)(bf * 255.0f);
//	a = 255;
//}
//
//
////=========================================================================================================================
//BobColor::BobColor(float rf, float gf, float bf, float af)
//{//=========================================================================================================================
//
////	boost::uuids::random_generator generator;
////	uuid = to_string(generator());
//
//	r = (int)(rf * 255.0f);
//	g = (int)(gf * 255.0f);
//	b = (int)(bf * 255.0f);
//	a = (int)(af * 255.0f);
//}


//=========================================================================================================================
BobColor::BobColor(u8 r, u8 g, u8 b)
{//=========================================================================================================================

//	boost::uuids::random_generator generator;
//	uuid = to_string(generator());

	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255;
}


//=========================================================================================================================
BobColor::BobColor(u8 r, u8 g, u8 b, u8 a)
{//=========================================================================================================================
//
//	boost::uuids::random_generator generator;
//	uuid = to_string(generator());

	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}


//=========================================================================================================================
BobColor::BobColor(int rgb)
{//=========================================================================================================================

//	boost::uuids::random_generator generator;
//	uuid = to_string(generator());


	u8 r = (rgb >> 24) & 0xFF;
	u8 g = (rgb >> 16) & 0xFF;
	u8 b = (rgb >> 8) & 0xFF;
	u8 a = (rgb >> 0) & 0xFF;

	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}


//=========================================================================================================================
BobColor::BobColor(float hf, float sf, float bf, float af, bool hsbOverload)
{//=========================================================================================================================

//	boost::uuids::random_generator generator;
//	uuid = to_string(generator());

	int rgb = HSBtoRGB(hf, sf, bf);
	u8 r = (rgb >> 16) & 0xFF;
	u8 g = (rgb >> 8) & 0xFF;
	u8 b = (rgb >> 0) & 0xFF;
	u8 a = (u8)(af * 255.0f);

	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}


//=========================================================================================================================
float BobColor::rf()
{//=========================================================================================================================
	return (float)(r / 255.0f);
}


//=========================================================================================================================
float BobColor::gf()
{//=========================================================================================================================
	return (float)(g / 255.0f);
}


//=========================================================================================================================
float BobColor::bf()
{//=========================================================================================================================
	return (float)(b / 255.0f);
}


//=========================================================================================================================
float BobColor::af()
{//=========================================================================================================================
	return (float)(a / 255.0f);
}


//=========================================================================================================================
u8 BobColor::ri()
{//=========================================================================================================================
	return r;
}


//=========================================================================================================================
u8 BobColor::gi()
{//=========================================================================================================================
	return g;
}


//=========================================================================================================================
u8 BobColor::bi()
{//=========================================================================================================================
	return b;
}


//=========================================================================================================================
u8 BobColor::ai()
{//=========================================================================================================================
	return a;
}


//=========================================================================================================================
float* BobColor::RGBtoHSB(u8 r, u8 g, u8 b)
{//=========================================================================================================================
	float hue, saturation, brightness;


	int cmax = (r > g) ? r : g;
	if (b > cmax)
	{
		cmax = b;
	}
	int cmin = (r < g) ? r : g;
	if (b < cmin)
	{
		cmin = b;
	}

	brightness = ((float)(cmax)) / 255.0f;
	if (cmax != 0)
	{
		saturation = ((float)(cmax - cmin)) / ((float)(cmax));
	}
	else
	{
		saturation = 0;
	}
	if (saturation == 0)
	{
		hue = 0;
	}
	else
	{
		float redc = ((float)(cmax - r)) / ((float)(cmax - cmin));
		float greenc = ((float)(cmax - g)) / ((float)(cmax - cmin));
		float bluec = ((float)(cmax - b)) / ((float)(cmax - cmin));
		if (r == cmax)
		{
			hue = bluec - greenc;
		}
		else if (g == cmax)
		{
			hue = 2.0f + redc - bluec;
		}
		else
		{
			hue = 4.0f + greenc - redc;
		}
		hue = hue / 6.0f;
		if (hue < 0)
		{
			hue = hue + 1.0f;
		}
	}

	float* hsbvals = new float[3];
	hsbvals[0] = hue;
	hsbvals[1] = saturation;
	hsbvals[2] = brightness;
	return hsbvals;
}


//=========================================================================================================================
int BobColor::HSBtoRGB(float hue, float saturation, float brightness)
{//=========================================================================================================================
	int r = 0, g = 0, b = 0;
	if (saturation == 0)
	{
		r = g = b = (int)(brightness * 255.0f + 0.5f);
	}
	else
	{
		float h = (hue - (float)(floor(hue))) * 6.0f;
		float f = h - (float)(floor(h));
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));
		switch ((int)(h))
		{
		case 0:
			r = (int)(brightness * 255.0f + 0.5f);
			g = (int)(t * 255.0f + 0.5f);
			b = (int)(p * 255.0f + 0.5f);
			break;
		case 1:
			r = (int)(q * 255.0f + 0.5f);
			g = (int)(brightness * 255.0f + 0.5f);
			b = (int)(p * 255.0f + 0.5f);
			break;
		case 2:
			r = (int)(p * 255.0f + 0.5f);
			g = (int)(brightness * 255.0f + 0.5f);
			b = (int)(t * 255.0f + 0.5f);
			break;
		case 3:
			r = (int)(p * 255.0f + 0.5f);
			g = (int)(q * 255.0f + 0.5f);
			b = (int)(brightness * 255.0f + 0.5f);
			break;
		case 4:
			r = (int)(t * 255.0f + 0.5f);
			g = (int)(p * 255.0f + 0.5f);
			b = (int)(brightness * 255.0f + 0.5f);
			break;
		case 5:
			r = (int)(brightness * 255.0f + 0.5f);
			g = (int)(p * 255.0f + 0.5f);
			b = (int)(q * 255.0f + 0.5f);
			break;
		}
	}

	unsigned int rs = static_cast<unsigned int>(r << 16);
	unsigned int gs = static_cast<unsigned int>(g << 8);
	unsigned int bs = static_cast<unsigned int>(b << 0);

	return (int)(0xff000000 | rs | gs | bs);
}


//=========================================================================================================================
void BobColor::darker()
{//=========================================================================================================================
	darker(0.4f);
}


//=========================================================================================================================
void BobColor::darker(float scale)
{//=========================================================================================================================
	scale = 1 - scale;

	float* hsbvals = RGBtoHSB(ri(), gi(), bi());


	//if already darkest increase saturation
	if (hsbvals[2] == 0.0f)
	{
		hsbvals[1] *= 1.0f / scale;
		if (hsbvals[1] > 1.0f)
		{
			hsbvals[1] = 1.0f;
		}
	}

	hsbvals[2] *= scale;
	if (hsbvals[2] < 0.0f)
	{
		hsbvals[2] = 0.0f;
	}

	int rgb = HSBtoRGB(hsbvals[0], hsbvals[1], hsbvals[2]);
	u8 ri = (rgb >> 16) & 0xFF;
	u8 gi = (rgb >> 8) & 0xFF;
	u8 bi = (rgb >> 0) & 0xFF;

	r = ri;
	g = gi;
	b = bi;
}


//=========================================================================================================================
void BobColor::lighter()
{//=========================================================================================================================

	//if(lighter!=null)return lighter;

	lighter(0.4f);
}


//=========================================================================================================================
void BobColor::lighter(float scale)
{//=========================================================================================================================

	if (scale < 1.0f)
	{
		scale += 1.0f;
	}


	float* hsbvals = RGBtoHSB(ri(), gi(), bi());

	//if already lightest, decrease saturation instead
	if (hsbvals[2] == 1.0f)
	{
		hsbvals[1] *= 1.0f / scale;
		if (hsbvals[1] < 0.0f)
		{
			hsbvals[1] = 0.0f;
		}
	}

	hsbvals[2] *= scale;
	if (hsbvals[2] > 1.0f)
	{
		hsbvals[2] = 1.0f;
	}

	int rgb = HSBtoRGB(hsbvals[0], hsbvals[1], hsbvals[2]);
	u8 ri = (rgb >> 16) & 0xFF;
	u8 gi = (rgb >> 8) & 0xFF;
	u8 bi = (rgb >> 0) & 0xFF;

	r = ri;
	g = gi;
	b = bi;
}

/*
void Color::multiply(shared_ptr<Color >c)
{
   return make_shared<Color>(rf() * c->rf(), gf() * c->gf(), bf() * c->bf(), af() * c->af());
}

void Color::addToCopy(shared_ptr<Color >c)
{
   return make_shared<Color>(rf() + c->rf(), gf() + c->gf(), bf() + c->bf(), af() + c->af());
}

void Color::scaleCopy(float value)
{
   return make_shared<Color>(rf() * value, gf() * value, bf() * value, af() * value);
}
*/


//int BobColor::rgbatoABGRInt(u8 r, u8 g, u8 b, u8 a)
//{ //==========================================================================================================================
//	return ((a << 15) + ((b) << 10) + ((g) << 5) + (r));
//}

//int Color::HexToDec(const string &hexst)
//{ //===============================================================================================
//   return int::valueOf(hexst, 16);
//}

int BobColor::DecToRed(int dec)
{ //===============================================================================================
	return (dec % 32) * 8;
}

int BobColor::DecToGreen(int dec)
{ //===============================================================================================
	return ((dec / 32) % 32) * 8;
}

int BobColor::DecToBlue(int dec)
{ //===============================================================================================
	return (dec / 1024) * 8;
}

//==========================================================================================================================
int BobColor::getRGBA()
{//==========================================================================================================================
	return (r << 24) + (g << 16) + (b << 8) + a;
}

int BobColor::getRGBA(u8 r, u8 g, u8 b, u8 a)
{
	return (r << 24) + (g << 16) + (b << 8) + a;
}
