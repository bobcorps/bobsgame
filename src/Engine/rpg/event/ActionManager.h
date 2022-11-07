//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


#pragma once
#include "oktypes.h"
class Logger;
#include "src/Utility/HashMap.h"
#include "src/Utility/ArrayList.h"
#include "src/Engine/EnginePart.h"

class Engine;
class Entity;
class Area;
class Caption;
class ScreenSprite;

class ActionManager : public EnginePart
{
public:

	static Logger log;


	shared_ptr<Caption> actionCaption = nullptr;


	shared_ptr<ScreenSprite> actionIconScreenSprite = nullptr;


	int ACTION_in_action_range = 0;

	static int ACTIONCAPTIONTYPE_NONE;
	static int ACTIONCAPTIONTYPE_TILE;
	static int ACTIONCAPTIONTYPE_XY;
	static int ACTIONCAPTIONTYPE_XYXY;
	static int ACTIONCAPTIONTYPE_NPC;
	static int ACTIONCAPTIONTYPE_AREA;


public:
	class Coords
	{
	private:
		shared_ptr<ActionManager> outerInstance = nullptr;

	public:
		int x = 0;
		int y = 0;

		Coords(shared_ptr<ActionManager> outerInstance, int x, int y);
	};

public:
	ArrayList<shared_ptr<Coords>> actionsThisFrame;// = make_shared<ArrayList><shared_ptr<Coords>>();


	ActionManager(shared_ptr<Engine> g);



	void deleteIfNoAction();


	/*
	   
	   static public boolean ACTION_check(String label)	//for action tiles. need a way to deinit.
	   {
	
	      if(player()==null)return false;
	
	      if(ACTION_in_action_range==0)
	      {
	         ACTION_in_action_range=1;
	
	         if(
	            textMan().GLOBAL_text_engine_state==0
	            &&player().GLOBAL_main_sprite_actions_off==0
	         )
	         {
	            if(ACTION_caption!=null)
	            {
	               ACTION_replace_caption(label);
	            }
	            else
	            {
	               ACTION_make_caption(label);
	               ACTION_caption.actioncaptiontype=ACTIONCAPTIONTYPE_TILE;
	            }
	
	            if(
	               ACTION_caption!=null
	               &&ControlsManager.BGCLIENT_ACTION_PRESSED==true
	               &&label.compareTo(ACTION_caption.getText)==0
	               &&ACTION_caption.actioncaptiontype==ACTIONCAPTIONTYPE_TILE
	            )
	            {
	               ACTION_delete_caption_sound();
	               return true;
	            }
	         }
	         else
	         {
	            if(
	               ACTION_caption!=null
	               &&label.compareTo(ACTION_caption.getText)==0
	               &&ACTION_caption.actioncaptiontype==ACTIONCAPTIONTYPE_TILE
	            )
	            {
	               ACTION_delete_caption_no_sound();
	            }
	         }
	      }
	
	      return false;
	
	   }*/


	bool xy(int x, int y, const string& label);


	bool area(shared_ptr<Area> a, const string& label);


	bool xyxy(int x, int y, int x2, int y2, const string& label);


	bool entity(shared_ptr<Entity> e, const string& label);


	bool checkAll(int x, int y, int x2, int y2, const string& label, int type, shared_ptr<Entity> e, shared_ptr<Area> a);


	void deleteCaptionNoSound();


	void deleteCaptionWithBlipSound();


	void makeCaption(const string& label);


	void replaceCaptionText(const string& label);


	void update();
};

