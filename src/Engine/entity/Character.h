//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------

#pragma once
#include "oktypes.h"
#include "Entity.h"
class Logger;

class PathFinder;
class Engine;
class EntityData;
class OKTexture;
class Caption;
class Sprite;
class Area;

class Character : public Entity , public std::enable_shared_from_this<Character>
{
public:
	static Logger log;

	int standing_cycles = 0;

	int pixelsToWalk = 0;
	int xPixelCounter = 0; //used for setting diagonal ratio, walking in perfect diagonal line
	int yPixelCounter = 0;

	int ticksToStand = 0;

	int animationDirection = 0;

	shared_ptr<PathFinder> pathfinder = nullptr;

	int pathPosition = 0;

	int pathFindWaitTicks = 0;

	float lastMapX = 0;
	float lastMapY = 0;
	int ticksSinceMoved = 0;

	bool moved = false; //locationChanged
	bool standing = false;
	bool running = false;

	shared_ptr<OKTexture> uniqueTexture = nullptr; //if this is set it will render with this, otherwise it will try to render the normal spriteAsset.texture in Entity.render()

	bool showName = false;
	shared_ptr<Caption> nameCaption = nullptr;

	bool showAccountType = false;
	shared_ptr<Caption> accountTypeCaption = nullptr;

	shared_ptr<OKColor> nameColor = OKColor::white;
	shared_ptr<OKColor> accountTypeNameColor = OKColor::white;
	string accountTypeName = "";

	bool isMale = false;
	bool isFemale = false;

	int standingTicksBetweenFrames = 0;
	int rotationAnimationSpeedTicks = 160;

	Character();
	//Character(shared_ptr<Engine> g);

	Character(shared_ptr<Engine> g, shared_ptr<EntityData> data, shared_ptr<Map> m);

	void initCharacter();

	Character(shared_ptr<Engine> g, string name, shared_ptr<Sprite> sprite, shared_ptr<Area> a, shared_ptr<Map> m);

	virtual void initCurrentAnimationFromSprite() override;

	virtual void render(float alpha) override;

	virtual void update() override;

	void setAnimationByDirection(int dir);

	bool canDoCharacterMovementOrStandingAnimation();

	void doCharacterAnimation(); //does animation and turns if needed

	void checkIfMoved();

	void dontLookAtEntity(shared_ptr<Entity> e); //first id is entity to be avoiding LOOKING,second id is one to NOT BE LOOKED AT

	void lookAtEntity(shared_ptr<Entity> e);

	void lookAtEntityButNotOppositeWalkingDirection(shared_ptr<Entity> stared_at_entity);

	void setAppearanceFromCharacterAppearanceString(string s);

	void generateUniqueTexture(int genderIndex, int archetypeIndex, int shoeColorIndex, int shirtColorIndex, int pantsColorIndex, int skinColorIndex, int eyeColorIndex, int hairColorIndex);

	void setShowName(bool b);

	void setShowAccountType(bool b);

	void setCharacterNameAndCaption(shared_ptr<OKColor> nameColor, const string& name, shared_ptr<OKColor> accountTypeNameColor, const string& accountTypeName);

	//
	//	
	//	public void setPriorityFromFXLayer()
	//	{
	//
	//		//this checks the current position to see if it needs to set the background priority up or down
	//
	//
	//
	//		int under2=2;
	//		int over1=1;
	//
	//		if(getMap()->getCameraBoundsFXLayerAtXYPixels(getLeft()+2,getBottom()-2)==under2
	//				||getMap()->getCameraBoundsFXLayerAtXYPixels(getRight()-2,getBottom()-2)==under2
	//				||getMap()->getCameraBoundsFXLayerAtXYPixels(getLeft()+2,getTop()+2)==under2
	//				||getMap()->getCameraBoundsFXLayerAtXYPixels(getRight()-2,getTop()+2)==under2
	//				||getMap()->getCameraBoundsFXLayerAtXYPixels(getMiddleX(),getBottom()-(hitBoxTop()/2))==under2)
	//		{
	//
	//			setRenderOrder(RenderOrder.GROUND);
	//
	//		}
	//		else if(getMap()->getCameraBoundsFXLayerAtXYPixels(getLeft()+2,getBottom()-2)==over1
	//				||getMap()->getCameraBoundsFXLayerAtXYPixels(getRight()-2,getBottom()-2)==over1
	//				||getMap()->getCameraBoundsFXLayerAtXYPixels(getLeft()+2,getTop()+2)==over1
	//				||getMap()->getCameraBoundsFXLayerAtXYPixels(getRight()-2,getTop()+2)==over1
	//				||getMap()->getCameraBoundsFXLayerAtXYPixels(getMiddleX(),getBottom()-(hitBoxTop()/2))==over1)
	//		{
	//			setRenderOrder(RenderOrder.ABOVE);
	//		}
	//		else
	//		{
	//			setRenderOrder(RenderOrder.GROUND);
	//		}
	//
	//
	//	}

	ArrayList<shared_ptr<Entity>>* getOnScreenNonCharacterEntitiesWithinRangeAmount(int amt);

	bool checkTouchingAnyEntityInEntityList(ArrayList<shared_ptr<Entity>> &list, float x, float y);

	bool checkHitLayerAndTouchingAnyEntityInEntityList(ArrayList<shared_ptr<Entity>> &list, float x, float y);

	bool checkTouchingAnyOnScreenNonCharacterNonWalkableEntities(float x, float y);

	void setShadowClip();

	int pathTried = 0;

	float finalPathX = 0;
	float finalPathY = 0;

	/// <summary>
	/// return 0 if not there.
	/// if path is blocked, will wait 500 ticks and try again (for someone to move) 3 times, and then return -1 if still fail.
	/// return 1 if there.
	/// 
	/// </summary>
	int walkToXYWithPathFinding(float x, float y);

	void checkHitBoxAndWalkDirection(int dir);

	void walkDirectionNoCheckHit(int direction);

	void walkRandomlyAroundRoomAndStop();

	void walkRandomlyAroundRoom();

	int walkRandomlyWithinXYXY(float x1, float y1, float x2, float y2);

	void twitchAroundRoom();

	shared_ptr<Character> findNearestCharacter();

	int walkToXYLRToUD(float x, float y);

	int walkToXYUDToLR(float toX, float toY);

	bool walkToXYNoCheckHit(float toX, float toY);

	bool walkToXYNoCheckHitOLD(float x, float y);

	bool walkToXYUntilHitWall(float x, float y);

	int walkToXYWithBasicHitCheck(float x, float y);

	int walkToXYNoHitAvoidOthersPushMain(float x, float y);

	int walkToXYStopForOtherEntitiesWithinAmt(float x, float y, int amt); //walk_to_xy_stop_for_other_entitys_amt

	void walkDirectionAvoidOtherEntities(int direction);

	///TEMP TEMP TEMP

	bool walkToXYIntelligentHitPushOthers(float x, float y);

	bool walkToXYIntelligentHitAvoidOthers(float x, float y);

	int walk_to_xy_intelligenthit_stopforothers_pushmain(float x, float y);

	void walkStraightFromPointToPoint(float x1, float y1, float x2, float y2);

	void walkStraightFromPointToPointAndStop(float x1, float y1, float x2, float y2);

	void walkAwayFromPoint(float x, float y);

	int walkDistance(int direction);

	int avoidEntity(shared_ptr<Entity> e, int amt); //returns 1 if not in entity area,use it to do something else outside,standing,staring,walking randomly etc

	int avoidNearestEntity(int avoid_amt);

	int avoidNearestCharacter(int avoid_amt);

	void pushableCrowdBehavior();

	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================BEGIN BEGIN BEGIN
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================
	///================================================

	//TODO: problems:

	//entity still gets stuck facing down sometimes
	//entitys are too jittery
	//walking into corners back and forth looks bad.

	//solutions:
	//entity can stop momentarily if there is someone in their way, unless they are walking faster

	//entitys can all push each other out of the way and never change direction when stuck in a corner

	//entitys should pathfind their way out of corners and not randomly decide a direction

	//jitter is caused by pushing back and forth. entitys should communicate to decide who is pushing and who is being pushed?

	int walk_to_xy_intelligenthit_avoidothers_pushmain(float x, float y);

	virtual void renderDebugBoxes() override;
};

