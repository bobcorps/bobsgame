#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------

//#pragma once



Logger Entity::log = Logger("Entity");

const int Entity::DOWN = 0;
const int Entity::UP = 1;
const int Entity::LEFT = 2;
const int Entity::RIGHT = 3;
const int Entity::UPLEFT = 4;
const int Entity::DOWNRIGHT = 6;
const int Entity::DOWNLEFT = 7;
const int Entity::UPRIGHT = 5;

Entity::Entity()
{ //=========================================================================================================================
}

Entity::Entity(sp<Engine> g, sp<Map> m)
{ //=========================================================================================================================
	this->e = g;

	this->map = m;

	if (getEventData() != nullptr)this->event = ms<Event>(g, getEventData(), this);
}

Entity::Entity(sp<Engine> g, sp<EntityData> entityData, sp<Map> m)
{ //=========================================================================================================================

	this->e = g;

	initEntity(entityData);

	this->map = m;

	if (getEventData() != nullptr)this->event = ms<Event>(g, getEventData(), this);
}

void Entity::initEntity(sp<EntityData> entityData)
{ //=========================================================================================================================

	if (entityData == nullptr)
	{
		entityData = ms<EntityData>(-1, "", "", 0, 0, 0, false, false, 0, 1.0f, 12, false, false, false, false, false, 0, 0, false, false, true, nullptr, "");
		log.warn("entityData was null in Entity.init()");
	}
	this->data = entityData;

	this->mapX = entityData->getSpawnXPixelsHQ();
	this->mapY = entityData->getSpawnYPixelsHQ();

	this->alpha = entityData->getToAlpha();

	this->setFrame(entityData->getInitialFrame());



	//initSpriteAsset();
}

void Entity::initCurrentAnimationFromSprite()
{ //=========================================================================================================================

	setCurrentAnimationBySpriteFrame(getInitialFrame());

	if (getRandomFrames())
	{
		selectRandomFrame(getCurrentAnimationStartFrame(), getCurrentAnimationLastFrame());
	}

	if (shadowClipPerPixel == nullptr)
	{
		shadowClipPerPixel = new float[sprite->getImageWidth()];
		for (int i = 0; i < sprite->getImageWidth(); i++)
		{
			shadowClipPerPixel[i] = 1.0f;
		}
	}
}

void Entity::update()
{ //=========================================================================================================================

	//if(getEngine()==null)setGame(mapAsset.getEngine());

	if (event != nullptr)
	{
		//sp<Event> e = getEventManager()->getEventByIDCreateIfNotExist(getEventData()->getID());

		getEventManager()->addToEventQueueIfNotThere(event); //events update their own network data inside their run function
	}

	if (sprite == nullptr)
	{
		//first we check if we have the spriteAsset loaded, if not we make a server request.
		//when that request returns in a different thread, it will set the SpriteAsset in the HashMap and this will return the SpriteAsset instead of calling to the server.
		sprite = getSpriteManager()->getSpriteByNameOrRequestFromServerIfNotExist(getSpriteName());

		if (sprite != nullptr)
		{
			initCurrentAnimationFromSprite();
		}

		return;
	}

	sprite->update();

	if (alpha != getToAlpha())
	{
		if (alpha > getToAlpha())
		{
			alpha -= 0.001f * getEngine()->engineTicksPassed();
			if (alpha < getToAlpha())
			{
				alpha = getToAlpha();
			}
		}
		if (alpha < getToAlpha())
		{
			alpha += 0.001f * getEngine()->engineTicksPassed();
			if (alpha > getToAlpha())
			{
				alpha = getToAlpha();
			}
		}
	}

	if (deleteWhenAlphaZero == true)
	{
		if (alpha == 0.0f)
		{
			deleteFromMapEntityListAndReleaseTexture(); //TODO this needs to be moved to some entity manager so the pointer can be deleted externally
		}
	}

	//TODO: handle eventBehavior
	//"MoveToArea:"+a.getID()+","+bWalk+","+bHit+","+bPath+","+bAnim+","+bDiag);
	//"MoveToEntity:"+e.getID()+","+bWalk+","+bHit+","+bPath+","+bAnim+","+bDiag);
	//"StandAndShuffle"
	//"StandAndShuffleAndFacePlayer"

	//TODO: handle normal behavior

	updateTimers();

	//do animation if was scripted in tools
	doAnimation();
}

void Entity::updateTimers()
{ //=========================================================================================================================

	if (ticksSinceLastMovement < 0)ticksSinceLastMovement = 0;

	//add change in time to remaining time from last movement
	ticksSinceLastMovement += (int)getEngine()->engineTicksPassed();


	if (ticksSinceLastMovement >= getTicksPerPixelMoved())
	{
		pixelsToMoveThisFrame = ((float)(ticksSinceLastMovement) / (float)(getTicksPerPixelMoved()));

		//now pixelsToMoveThisFrame is something like 3.24543
		//movements for Entity and Character are based on 1.0f though, so a character will move 3.0f and leave .24543

		//if we store the remainder of the ticks we didn't use in our 1.0f whole units, it will be correct.
		ticksSinceLastMovement %= (int)getTicksPerPixelMoved();

		//however, the Player uses float based values for everything and moves fractions of pixels.
		//Player should set ticksSinceLastMovement to zero for correct timing.
		if (dynamic_cast<Player*>(this) != nullptr)
		{
			ticksSinceLastMovement = 0;
		}
	}
	else
	{
		pixelsToMoveThisFrame = 0;
	}

	//if(pixelsToMoveThisFrame>20)pixelsToMoveThisFrame=1;

	animationTicksCounter += (int)getEngine()->engineTicksPassed();
}

void Entity::renderDebugBoxes()
{ //=========================================================================================================================

	float zoom = getCameraman()->getZoom();

	float screenRight = getScreenLeft() + (getWidth() * zoom);
	float screenBottom = getScreenTop() + (getHeight() * zoom);

	//outline
	GLUtils::drawBox(getScreenLeft(), screenRight - 1, getScreenTop(), screenBottom - 1, 0, 255, 0); //-1 so the box is inside one pixel

	//hitbox
	GLUtils::drawBox(getScreenLeft() + (getHitBoxFromLeft() * zoom), (screenRight - (getHitBoxFromRight() * zoom)) - 1, getScreenTop() + (getHitBoxFromTop() * zoom), (screenBottom - (getHitBoxFromBottom() * zoom)) - 1, 255, 0, 0);

	for (int i = 0; i < getConnectionTYPEIDList()->size(); i++)
	{
		//draw connections to doors
		if (OKString::startsWith(getConnectionTYPEIDList()->at(i), "DOOR."))
		{
			//go through doorlist
			for (int d = 0; d < (int)getMap()->doorList.size(); d++)
			{
				sp<Door> door = getMap()->doorList.at(d);

				if (getConnectionTYPEIDList()->at(i) == "DOOR." + door->getName())
				{
					float dx = door->getScreenLeft() + (door->getWidth() / 2) * zoom;
					float dy = door->getScreenTop() + (door->getHeight()) * zoom;

					GLUtils::drawArrowLine(getScreenLeft() + (getWidth() / 2) * zoom, getScreenMiddleHitboxY(), dx, dy, 0, 255, 0);
				}
			}
		}
		else
		{
			//draw connections to areas

			if (getMap() != nullptr && getMap()->currentState != nullptr)
			{
				//go through area hashlist
				//					Enumeration<Area> aEnum = getMap()->currentState.areaByNameHashtable.elements();
				//					while(aEnum.hasMoreElements())
				//					{
				//						Area a = aEnum.nextElement();

				for (int j = 0; j < (int)getMap()->currentState->areaList.size(); j++)
				{
					sp<Area> a = getMap()->currentState->areaList.at(j);

					if (getConnectionTYPEIDList()->at(i) == a->getName())
					{
						float ax = a->screenLeft() + (a->getWidth() / 2) * zoom;
						float ay = a->screenTop() + (a->getHeight() / 2) * zoom;

						GLUtils::drawArrowLine(getScreenLeft() + (getWidth() / 2) * zoom, getScreenMiddleHitboxY(), ax, ay, 0, 255, 0);
					}
				}
			}

			//if not found, go through warparea list
			for (int j = 0; j < (int)getMap()->warpAreaList.size(); j++)
			{
				sp<Area> a = getMap()->warpAreaList.at(j);

				if (getConnectionTYPEIDList()->at(i) == a->getName())
				{
					float ax = a->screenLeft() + (a->getWidth() / 2) * zoom;
					float ay = a->screenTop() + (a->getHeight() / 2) * zoom;

					GLUtils::drawArrowLine(getScreenLeft() + (getWidth() / 2) * zoom, getScreenMiddleHitboxY(), ax, ay, 0, 255, 0);
				}
			}
		}
	}
}

void Entity::renderDebugInfo()
{ //=========================================================================================================================

	float x = getScreenLeft();
	float y = getScreenTop();

	int strings = -1;

	if (sprite->getDisplayName() == "No Name" == false)
	{
		GLUtils::drawOutlinedString("entityName: " + getName(), x, y - 36, OKColor::yellow);
		GLUtils::drawOutlinedString("displayName: " + sprite->getDisplayName(), x, y - 27, OKColor::green);
	}
	else
	{
		GLUtils::drawOutlinedString("entityName: " + getName(), x, y - 27, OKColor::yellow);
	}

	GLUtils::drawOutlinedString("id: " + to_string(getID()), x, y - 18, OKColor::white);
	GLUtils::drawOutlinedString("SpriteAsset Name: " + sprite->getName(), x, y - 9, OKColor::white);

	if (getEventData() != nullptr)
	{
		GLUtils::drawOutlinedString("Has Event: " + to_string(getEventData()->getID()), x, y + (++strings * 9), OKColor::red);
	}

	//GL.drawOutlinedString("mapX: "+getMapXPixelsHQ+" mapY: "+getMapYPixelsHQ, x, y+(++strings*9),Color.white);
	//GL.drawOutlinedString("getScreenX: "+screenXPixelsHQ+" getScreenY: "+screenYPixelsHQ, x, y+(++strings*9),Color.white);
	//GL.drawOutlinedString("w: "+width()+" h: "+height(), x, y+(++strings*9),Color.white);
	//GL.drawOutlinedString("getScale: "+getScale, x, y+(++strings*9),Color.white);
	//GL.drawOutlinedString("alpha: "+alpha, x, y+(++strings*9),Color.white);

	//if(movementDirection==UP)GL.drawOutlinedString("movementDirection: Up", x, y+(++strings*9),Color.white);
	//if(movementDirection==DOWN)GL.drawOutlinedString("movementDirection: Down", x, y+(++strings*9),Color.white);
	//if(movementDirection==LEFT)GL.drawOutlinedString("movementDirection: Left", x, y+(++strings*9),Color.white);
	//if(movementDirection==RIGHT)GL.drawOutlinedString("movementDirection: Right", x, y+(++strings*9),Color.white);

	//if (this->getClass().equals(char::typeid) || this->getClass().equals(Player::typeid) || this->getClass().equals(RandomCharacter::typeid))
	if (dynamic_cast<Character*>(this) != nullptr || 
		dynamic_cast<Player*>(this) != nullptr || 
		dynamic_cast<RandomCharacter*>(this) != nullptr)
	{
		GLUtils::drawOutlinedString("getTicksPerPixelMoved: " + to_string(getTicksPerPixelMoved()), x, y + (++strings * 9), OKColor::white);
		GLUtils::drawOutlinedString("pixelsToMoveThisFrame: " + to_string(pixelsToMoveThisFrame), x, y + (++strings * 9), OKColor::white);
	}

	//GL.drawOutlinedString("animationTicksCounter: "+animationTicksCounter, x, y+(++strings*9),Color.white);

	if (getRandomFrames())
	{
		GLUtils::drawOutlinedString("Random Frames", x, y + (++strings * 9), OKColor::red);
	}
	if (getDisableShadow())
	{
		GLUtils::drawOutlinedString("Disable Shadow", x, y + (++strings * 9), OKColor::red);
	}

	if (getRenderOrder() != RenderOrder::GROUND)
	{
		GLUtils::drawOutlinedString("RenderOrder: " + to_string((int)getRenderOrder()), x, y + (++strings * 9), OKColor::red);
	}
	if (getAlwaysOnTop())
	{
		GLUtils::drawOutlinedString("getAlwaysOnTop", x, y + (++strings * 9), OKColor::red);
	}
	if (getAboveWhenEqual())
	{
		GLUtils::drawOutlinedString("getAboveWhenEqual", x, y + (++strings * 9), OKColor::red);
	}
	if (getAboveTopLayer())
	{
		GLUtils::drawOutlinedString("getAboveTopLayer", x, y + (++strings * 9), OKColor::red);
	}
	if (getAlwaysOnBottom())
	{
		GLUtils::drawOutlinedString("getAlwaysOnBottom", x, y + (++strings * 9), OKColor::red);
	}
	if (isWalkingIntoPlayerThisFrame)
	{
		GLUtils::drawOutlinedString("isWalkingIntoPlayerThisFrame", x, y + (++strings * 9), OKColor::red);
	}
	if (isWalkingIntoWallThisFrame)
	{
		GLUtils::drawOutlinedString("isWalkingIntoWallThisFrame", x, y + (++strings * 9), OKColor::red);
	}
	if (getIgnoreHitPlayer())
	{
		GLUtils::drawOutlinedString("getIgnoreHitPlayer", x, y + (++strings * 9), OKColor::red);
	}
	if (getMovementAnimationDisabled())
	{
		GLUtils::drawOutlinedString("getMovementAnimationDisabled", x, y + (++strings * 9), OKColor::red);
	}
	if (getPushable())
	{
		GLUtils::drawOutlinedString("getPushable", x, y + (++strings * 9), OKColor::red);
	}
	if (getNonWalkable())
	{
		GLUtils::drawOutlinedString("getNonWalkable", x, y + (++strings * 9), OKColor::red);
	}
	if (getIgnoreHitLayer())
	{
		GLUtils::drawOutlinedString("getIgnoreHitLayer", x, y + (++strings * 9), OKColor::red);
	}
	//if(ignore_fx_layer)GL.drawOutlinedString("ignore_fx_layer", x, y+(++strings*9),Color.red);

	GLUtils::drawOutlinedString("MiddleY: " + to_string(getMiddleY()), x, y + (++strings * 9), OKColor::green);

	if (currentAreaTYPEIDTarget != "" && currentAreaTYPEIDTarget.length() > 0)
	{
		GLUtils::drawOutlinedString("Current Target TYPEID: " + currentAreaTYPEIDTarget, x, y + (++strings * 9), OKColor::green);
	}
	if (currentAreaTYPEIDTarget != "" && currentAreaTYPEIDTarget.length() > 0)
	{
		GLUtils::drawOutlinedString("Current Target Name: " + getCurrentAreaTargetName(), x, y + (++strings * 9), OKColor::green);
	}

	/*
	public int voice_pitch=0;
	               
	public int frameInSequence=0; // was anim_frame_count
	public int currentSequenceLength=8;//was amt_frames
	public int totalRenders=0;//total frames for all animations
	public int frameIndexInTexture=0;//based on current sequence (getAnimDirection()) and currentAnimationFrameInSequence, this is what will be rendered
	               
	public int hitBoxFromLeftPixelsHQ = 0;
	public int hitBoxFromTopPixelsHQ = 0;
	public int hitBoxFromRightPixelsHQ = 0;
	public int hitBoxFromBottomPixelsHQ = 0;
	               
	public boolean animateThroughFrames;
	public boolean randomTimeBetweenAnimation;
	public int getTicksBetweenFrames;
	public int ticksBetweenAnimation;
	public int walkSpeed;
	public boolean getOnlyHereDuringEvent;
	               
	public boolean pull_player=false;
	public boolean push_player=false;
	public boolean shadow = true;
	public float shadowClip=0;
	               
	int ticksSinceLastMovement = 0;
	               
	int movementsThisFrame = 0;
	public float pixelsToMoveThisFrame = 0;
	               
	vector<String> getBehaviorList = ms<ArrayList><String>();
	vector<String> connectionList = ms<ArrayList><String>();
	               
	               */
}


string Entity::getCurrentAreaTargetName()
{ //=========================================================================================================================
	sp<Area> a = getMap()->getAreaOrWarpAreaByTYPEID(currentAreaTYPEIDTarget);
	if (a == nullptr)
	{
		return "ERROR: Area not found.";
	}
	return a->getName();
}

void Entity::render(float mapAlpha)
{ //=========================================================================================================================

	if (draw == false || sprite == nullptr || sprite->texture == nullptr)
	{
		return;
	}

	render(mapAlpha * this->alpha, this->sprite->texture, this->sprite->shadowTexture);

	//overrode this so i can send in arbitrary texture, really only used for random sprites which contain their own unique texture reference, and not the one contained in the spriteAsset object.
}

void Entity::render(float alpha, sp<OKTexture> texture, sp<OKTexture> shadowTexture)
{ //=========================================================================================================================

	float zoom = getCameraman()->getZoom();

	float tx0 = 0.0f;
	float tx1 = 1.0f;
	float ty0 = 0.0f;
	float ty1 = 1.0f;

	float x0 = 0;
	float x1 = 0;
	float y0 = 0;
	float y1 = 0;

	//YAY
	//SPRITE JITTERING IS COMPLETELY FIXED!
	//had to base it on the OFFSET of the floored scaled mapScreenX vs non-floored, since the map drawing itself is based on that to begin with to prevent shimmering

	float offsetX = (getMap()->screenX() * zoom) - ((float)(floor(getMap()->screenX() * zoom)));
	float offsetY = (getMap()->screenY() * zoom) - ((float)(floor(getMap()->screenY() * zoom)));

	
	//------------------------
	//draw shadow first
	//------------------------
	if (shadowTexture != nullptr && getDisableShadow() == false)
	{
		if (clipShadow == false)
		{
			tx0 = 0.0f;
			tx1 = ((float)(sprite->getImageWidth()) / (float)(texture->getTextureWidth()));
			ty0 = (((float)(sprite->getImageHeight())) * getFrame()) / texture->getTextureHeight();
			ty1 = ((((float)(sprite->getImageHeight())) * (getFrame() + 1)) - 1) / texture->getTextureHeight();

			x0 = getScreenLeft() - offsetX;
			x1 = x0 + getWidth() * zoom;
			y0 = (getScreenTop() - offsetY) + ((getHeight() * getShadowStart()) * zoom);
			y1 = y0 + ((getHeight() * shadowSize) * zoom);

			GLUtils::drawTexture(shadowTexture, tx0, tx1, ty0, ty1, x0, x1, y0, y1, shadowAlpha, GLUtils::FILTER_LINEAR);
		}
		else
		{
			if (clipShadow == true && shadowClipPerPixel != nullptr)
			{
				//bind texture here.
				glBindTexture(GL_TEXTURE_2D, shadowTexture->getTextureID());

				for (int x = 0; x < sprite->getImageWidth(); x++)
				{
					//float w = 1.0f/(float)texture.getTextureWidth();

					tx0 = ((float)(x) / (float)(texture->getTextureWidth()));
					tx1 = ((float)(x + 1.0f) / (float)(texture->getTextureWidth()));
					ty0 = (((float)(sprite->getImageHeight())) * getFrame()) / texture->getTextureHeight();
					ty1 = ((((float)(sprite->getImageHeight())) * (getFrame() + 1)) - 1) / texture->getTextureHeight();

					float pixelSize = 2.0f;
					if (sprite->useHQ2X)pixelSize = 1.0f;
					x0 = (float)((getScreenLeft() - offsetX) + (x * pixelSize * getScale() * zoom));
					x1 = x0 + (pixelSize * getScale() * zoom);
					y0 = (float)((getScreenTop() - offsetY) + ((getHeight() * getShadowStart()) * zoom));
					y1 = y0 + ((getHeight() * shadowSize) * zoom);

					if (shadowClipPerPixel[x] < 1.0f)
					{
						y1 = (float)(floor(y0 + ((y1 - y0) * shadowClipPerPixel[x])));
						ty1 = ((((((float)(sprite->getImageHeight())) * (getFrame())) + ((float)(sprite->getImageHeight()) * shadowClipPerPixel[x])) - 1) / texture->getTextureHeight());
					}

					//don't bind texture
					GLUtils::drawTexture(tx0, tx1, ty0, ty1, x0, x1, y0, y1, shadowAlpha, GLUtils::FILTER_LINEAR);
				}
			}
		}
	}

	//------------------
	//now draw actual sprite
	//------------------


	x0 = (float)((getScreenLeft() - offsetX));
	y0 = (float)((getScreenTop() - offsetY));
	x1 = (float)((x0 + getWidth() * zoom));
	y1 = (float)((y0 + getHeight() * zoom));

	sprite->drawFrame(texture, getFrame(), x0, x1, y0, y1, alpha, GLUtils::FILTER_NEAREST);

	//		if(texture!=null)
	//		{
	//			tx0 = 0.0f;
	//			tx1 = ((float)sprite.getWidth()/(float)texture.getTextureWidth());
	//			ty0 = (((float)sprite.getHeight())*frameIndexInTexture)/(float)texture.getTextureHeight();
	//			ty1 = (((float)sprite.getHeight())*(frameIndexInTexture+1))/(float)texture.getTextureHeight();
	//
	//
	//
	//			//x0 = (float)Math.floor(getScreenLeft());
	//			//x1 = (float)Math.floor(screenRight());
	//			//y0 = (float)Math.floor(getScreenTop());
	//			//y1 = (float)Math.floor(screenBottom());
	//
	//
	//
	//
	//			x0 = getScreenLeft()-offsetX;
	//			y0 = getScreenTop()-offsetY;
	//			x1 = (float) (x0+getWidth()*drawScale);
	//			y1 = (float) (y0+getHeight()*drawScale);
	//
	//
	//
	//			GLUtils.drawTexture(texture,tx0,tx1,ty0,ty1,x0,x1,y0,y1,alpha,GLUtils.FILTER_NEAREST);
	//		}

	//------------------
	//special case for rendering doors action icon doorknob glow thing. maybe should override render in door for this.
	//------------------
	//if (this->getClass().equals(Door::typeid))
	if (dynamic_cast<Door*>(this) != NULL)
	{
		Door* d = static_cast<Door*>(this);
		d->renderActionIcon();
	}
}

sp<Map> Entity::getCurrentMap()
{ //=========================================================================================================================

	log.warn("getCurrentMap() in Entity");
	//(exception())->printStackTrace();

	return EnginePart::getCurrentMap();
}

sp<Map> Entity::getMap()
{ //=========================================================================================================================

	if (this->map == nullptr)
	{
		return EnginePart::getCurrentMap();
	}

	//sp<Map> map = getMapManager()->getMapByIDBlockUntilLoaded(getMapID());

	return this->map;
}

bool Entity::shouldDraw()
{ //=========================================================================================================================

	if (sprite == nullptr || sprite->texture == nullptr || isWithinScreenBounds() == false)
	{
		draw = false;
	}
	else
	{
		draw = true;
	}

	return draw; //TODO: also check to make sure it has graphics, visible flags, etc
}

bool Entity::isWithinScreenBounds()
{ //=========================================================================================================================

	float mapCameraXPixelsHQ = (float)(getMap()->mapCamX());
	float mapCameraYPixelsHQ = (float)(getMap()->mapCamY());

	//*************
	//flooring these fixes all the sprite jitter
	//nope, don't need to do this now, basing all the coords on the offset of the floored mapScreenXY in render, like the map chunk offsets.
	//*************
	float left = (float)(getX());
	float right = (float)(getX() + getWidth());
	float top = (float)(getY());
	float bottom = (float)(getY() + getHeight());

	float screenleft = mapCameraXPixelsHQ;
	float screenright = mapCameraXPixelsHQ + getEngine()->getWidthRelativeToZoom();
	float screentop = mapCameraYPixelsHQ;
	float screenbottom = mapCameraYPixelsHQ + getEngine()->getHeightRelativeToZoom();

	float shadowLength = ((getHeight() * getShadowStart()) + (getHeight() * shadowSize)) - getHeight();

	if (right < screenleft || left > screenright || bottom + shadowLength < screentop || top > screenbottom) //so shadow can show even if sprite walks above screen
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Entity::checkHitBoxAgainstHitLayerAndNonWalkableEntitiesInDirection(int dir)
{ //=========================================================================================================================
	bool hitting_wall = true;

	if (getIgnoreHitLayer() == true)
	{
		return false;
	}

	if (dir == UP)
	{
		if (checkXYAgainstHitLayerAndNonWalkableEntities(getLeft() + 1, getTop()) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getMiddleX() - 1, getTop()) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getMiddleX(), getTop()) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getRight() - 1, getTop()) == false)
		{
			hitting_wall = false;
		}
	}
	else
	{
		if (dir == DOWN)
		{
			if (checkXYAgainstHitLayerAndNonWalkableEntities(getLeft() + 1, getBottom()) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getMiddleX() - 1, getBottom()) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getMiddleX(), getBottom()) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getRight() - 1, getBottom()) == false)
			{
				hitting_wall = false;
			}
		}
		else
		{
			if (dir == LEFT)
			{
				if (checkXYAgainstHitLayerAndNonWalkableEntities(getLeft(), getBottom() - 1) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getLeft(), getBottom() - 4) == false)
				{
					hitting_wall = false;
				}
			}
			else
			{
				if (dir == RIGHT)
				{
					if (checkXYAgainstHitLayerAndNonWalkableEntities(getRight() - 1, getBottom() - 1) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getRight() - 1, getBottom() - 4) == false)
					{
						hitting_wall = false;
					}
				}
			}
		}
	}

	///todo: i split off the check against player() into its own function above, also need to call this separately.
	isWalkingIntoPlayerThisFrame = isTouchingPlayerInDirection(dir);

	///todo: dont combine this, find where this function is called and do both.
	//if(this!=getPlayer()&&hitting_wall==false)hitting_wall=check_collide_fx(dir); //&&dont_walk_in_fx_layer

	isWalkingIntoWallThisFrame = hitting_wall;

	return hitting_wall;
}

bool Entity::checkMiddlePixelAgainstHitLayerAndNonWalkableEntitiesInDirection(int dir)
{ //=========================================================================================================================
	bool hitting_wall = true;

	if (getIgnoreHitLayer() == true)
	{
		return false;
	}

	if (dir == UP)
	{
		if (checkXYAgainstHitLayerAndNonWalkableEntities(getMiddleX() - 1, getTop()) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getMiddleX(), getTop()) == false)
		{
			hitting_wall = false;
		}
	}
	else
	{
		if (dir == DOWN)
		{
			if (checkXYAgainstHitLayerAndNonWalkableEntities(getMiddleX() - 1, getBottom()) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getMiddleX(), getBottom()) == false)
			{
				hitting_wall = false;
			}
		}
		else
		{
			if (dir == LEFT)
			{
				if (checkXYAgainstHitLayerAndNonWalkableEntities(getLeft(), getMiddleY() - 1) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getLeft(), getMiddleY()) == false)
				{
					hitting_wall = false;
				}
			}
			else
			{
				if (dir == RIGHT)
				{
					if (checkXYAgainstHitLayerAndNonWalkableEntities(getRight() - 1, getMiddleY() - 1) == false && checkXYAgainstHitLayerAndNonWalkableEntities(getRight() - 1, getMiddleY()) == false)
					{
						hitting_wall = false;
					}
				}
			}
		}
	}

	///todo: i split off the check against player() into its own function above, also need to call this separately.
	isWalkingIntoPlayerThisFrame = isTouchingPlayerInDirection(dir);

	///todo: dont combine this, find where this function is called and do both.
	//if(this!=getPlayer()&&hitting_wall==false)hitting_wall=check_collide_fx(dir); //&&dont_walk_in_fx_layer

	isWalkingIntoWallThisFrame = hitting_wall;

	return hitting_wall;
}

bool Entity::checkPathBlockedXY(float mapXHQ, float mapYHQ)
{ //=========================================================================================================================

	float x = mapXHQ;
	float y = mapYHQ;

	bool hit = false;
	int w = ((int)(getRight() - getLeft()) / 2);

	//y is getMiddleY()
	//float topHit = y - ((getMiddleY() - getY()) - (16 * 3)) - 8;
	//float bottomHit = y + (getHeight() - (getMiddleY() - getY()));

	if (getMap()->getHitLayerValueAtXYPixels(x, y) || getMap()->getHitLayerValueAtXYPixels(x, y + 16) || getMap()->getHitLayerValueAtXYPixels(x, y - 16) || getMap()->getHitLayerValueAtXYPixels(x - w, y) || getMap()->getHitLayerValueAtXYPixels(x + w, y))
	//getHit += getMap()->check_hit_layer_xy(x,topHit);
	//getHit += getMap()->check_hit_layer_xy(x,bottomHit);
	{
		hit = true;
	}

	if (hit == false)
	{
		//go through all mapsprites, check if map characters

		for (int i = 0; i < (int)getMap()->activeEntityList.size(); i++)
		{
			sp<Entity> m = getMap()->activeEntityList.at(i);

			if (m.get() != this &&
				(dynamic_cast<Door*>(m.get()) != NULL) == false &&
				(dynamic_cast<RandomCharacter*>(m.get()) != NULL) == false &&
				(dynamic_cast<Character*>(m.get()) != NULL) == false &&
				m->getNonWalkable() == true && x < m->getRight() && x > m->getLeft() && y < m->getBottom() && y > m->getTop())
			{
				return true; //TODO use touching functions - using full hitbox.
			}

			if (m.get() != this &&
				(
					(dynamic_cast<Character*>(m.get()) != NULL) ||
					(dynamic_cast<RandomCharacter*>(m.get()) != NULL) ||
					(dynamic_cast<Player*>(m.get()) != NULL)
				)
				&& x < m->getMiddleX() + 6 && x > m->getMiddleX() - 6 && y < m->getMiddleY() + 6 && y > m->getMiddleY() - 6)
			{
				return true; //TODO use touching functions - not using full hitbox.
			}
		}
	}
	return hit;
}

bool Entity::checkXYAgainstNonWalkableEntities(float x, float y)
{ //=========================================================================================================================

	if (getEngine()->hitLayerEnabled == false)
	{
		return false;
	}

	for (int s = 0; s < (int)getMap()->activeEntityList.size(); s++)
	{
		sp<Entity> e = getMap()->activeEntityList.at(s);

		if (e->getNonWalkable() == false)
		{
			continue;
		}

		if (e.get() == this)
		{
			continue;
		}

		float left = e->getLeft();
		float right = e->getRight();
		float top = e->getTop();
		float bottom = e->getBottom();

		if (x >= left && x <= right && y >= top && y <= bottom)
		{
			return true; //TODO use touching functions
		}
	}

	for (int s = 0; s < (int)getMap()->doorList.size(); s++)
	{
		sp<Door> e = getMap()->doorList.at(s);

		if (e->getNonWalkable() == false || e->isOpen())
		{
			continue;
		}

		//if(e.equals(this))continue;

		float left = e->getLeft();
		float right = e->getRight();
		float top = e->getTop();
		float bottom = e->getBottom();

		if (x >= left && x <= right && y >= top && y <= bottom)
		{
			return true; //TODO use touching functions
		}
	}

	return false;
}

bool Entity::checkXYAgainstHitLayerAndNonWalkableEntities(float x, float y)
{ //=========================================================================================================================
	if (getMap()->getHitLayerValueAtXYPixels(x, y) == false && checkXYAgainstNonWalkableEntities(x, y) == false)
	{
		return false;
	}

	return true;
}

bool Entity::checkHitBoxAndMovePixelInDirection(int dir)
{ //=========================================================================================================================
	bool moved = true;

	if (checkHitBoxAgainstHitLayerAndNonWalkableEntitiesInDirection(dir) == false)
	{
		if (isWalkingIntoPlayerThisFrame == false || getIgnoreHitPlayer() == true)
		{
			movePixelInDirection(dir);
			movementDirection = dir;
		}
	}
	else
	{
		moved = false;
	}

	return moved;
}

bool Entity::checkMiddlePixelHitAndMovePixelInDirection(int dir)
{ //=========================================================================================================================
	bool moved = true;

	if (checkMiddlePixelAgainstHitLayerAndNonWalkableEntitiesInDirection(dir) == false)
	{
		if (isWalkingIntoPlayerThisFrame == false || getIgnoreHitPlayer() == true)
		{
			movePixelInDirection(dir);
			movementDirection = dir;
		}
	}
	else
	{
		moved = false;
	}

	return moved;
}

void Entity::movePixelInDirection(int dir)
{ //=========================================================================================================================

	if (dir == DOWNLEFT)
	{
		decX();
		incY();
	}
	else if (dir == DOWNRIGHT)
	{
		incX();
		incY();
	}
	else if (dir == UPLEFT)
	{
		decX();
		decY();
	}
	else if (dir == UPRIGHT)
	{
		incX();
		decY();
	}
	else if (dir == UP)
	{
		decY();
	}
	else if (dir == DOWN)
	{
		incY();
	}
	else if (dir == LEFT)
	{
		decX();
	}
	else if (dir == RIGHT)
	{
		incX();
	}
}

bool Entity::ifCanMoveAPixelThisFrameSubtractAndReturnTrue()
{ //=========================================================================================================================

	if (pixelsToMoveThisFrame < 0.0f)
	{
		pixelsToMoveThisFrame = 0.0f;
	}

	if (pixelsToMoveThisFrame >= 1.0f)
	{
		pixelsToMoveThisFrame -= 1.0f;

		if (pixelsToMoveThisFrame < 0.0f)
		{
			pixelsToMoveThisFrame = 0.0f;
		}

		return true;
	}
	return false;

	/*if(can_walk()==true)
	{
	   ms=0;
	   return 1;
	}
	else
	return 0;*/
}

bool Entity::canCreateAtXY(float x, float y)
{ //=========================================================================================================================
	//check getHit and fx for x, x-8, y, y+6.
	if (getMap()->getHitLayerValueAtXYPixels(x - 8, y + 6) == false && getMap()->getCameraBoundsFXLayerAtXYPixels(x - 8, y + 6) == 0 && getMap()->getHitLayerValueAtXYPixels(x - 8, y) == false && getMap()->getCameraBoundsFXLayerAtXYPixels(x - 8, y) == 0 && getMap()->getHitLayerValueAtXYPixels(x + 8, y) == false && getMap()->getCameraBoundsFXLayerAtXYPixels(x + 8, y) == 0 && getMap()->getHitLayerValueAtXYPixels(x + 8, y + 6) == false && getMap()->getCameraBoundsFXLayerAtXYPixels(x + 8, y + 6) == 0 && getMap()->getHitLayerValueAtXYPixels(x, y) == false && getMap()->getCameraBoundsFXLayerAtXYPixels(x, y) == 0 && getMap()->getHitLayerValueAtXYPixels(x, y + 6) == false && getMap()->getCameraBoundsFXLayerAtXYPixels(x, y + 6) == 0)
	{
		return true;
	}

	return false;
}

void Entity::setFeetAtMapXY(float mapXPixels2X, float mapYPixels2X)
{ //===========================================================================================================================

	//int tileSize = 16;

	//setX((mapXPixels2X+(tileSize/2))-getWidth()/2);
	//setY((mapYPixels2X+tileSize)-getHeight());

	setX(mapXPixels2X - getMiddleOffsetX());
	setY(mapYPixels2X - getMiddleOffsetY());
}

int Entity::getFrame()
{ //=========================================================================================================================
	return frameIndexInTexture;
}

void Entity::setFrame(int f)
{ //=========================================================================================================================
	frameIndexInTexture = f;
}

void Entity::resetAnimationTimer()
{ //=========================================================================================================================
	animationTicksCounter = 0;
}

bool Entity::haveTicksPassedSinceLastAnimated_ResetIfTrue(int ticks)
{ //=========================================================================================================================
	if (animationTicksCounter >= ticks)
	{
		resetAnimationTimer();
		return true;
	}
	else
	{
		return false;
	}
}

sp<SpriteAnimationSequence> Entity::getCurrentAnimation()
{ //=========================================================================================================================
	if (sprite == nullptr)
	{
		return nullptr;
	}
	if (currentAnimation == nullptr)
	{
		return nullptr;
	}
	return currentAnimation;
}

void Entity::setCurrentAnimation(sp<SpriteAnimationSequence> a)
{ //=========================================================================================================================
	currentAnimation = a;
}

void Entity::setCurrentAnimationByName(const string& name)
{ //=========================================================================================================================
	if (sprite == nullptr)
	{
		log.error("Sprite is null in Entity: " + getName() + " while setting AnimationByName");
		return;
	}
	sp<SpriteAnimationSequence> a = sprite->getAnimationByName(name);
	if (a == nullptr)
	{
		log.error("Animation name: " + name + " not found in Sprite: " + sprite->getName() + " in Entity: " + getName());
		return;
	}
	currentAnimation = a;
}

void Entity::setCurrentAnimationByDirection(int dir)
{ //=========================================================================================================================
	string sequenceName = "";
	if (dir == Entity::UP)
	{
		sequenceName = "Up";
	}
	if (dir == Entity::DOWN)
	{
		sequenceName = "Down";
	}
	if (dir == Entity::LEFT)
	{
		sequenceName = "Left";
	}
	if (dir == Entity::RIGHT)
	{
		sequenceName = "Right";
	}
	if (dir == Entity::UPLEFT)
	{
		sequenceName = "UpLeft";
	}
	if (dir == Entity::UPRIGHT)
	{
		sequenceName = "UpRight";
	}
	if (dir == Entity::DOWNLEFT)
	{
		sequenceName = "DownLeft";
	}
	if (dir == Entity::DOWNRIGHT)
	{
		sequenceName = "DownRight";
	}

	if (sprite == nullptr)
	{
		log.error("Sprite is null in Entity: " + getName() + " while setting AnimationByName");
		return;
	}
	sp<SpriteAnimationSequence> a = sprite->getAnimationByName(sequenceName);
	if (a == nullptr)
	{
		log.error("Animation name: " + sequenceName + " not found in Sprite: " + sprite->getName() + " in Entity: " + getName());
		return;
	}
	currentAnimation = a;
}

int Entity::getSpriteLastFrame()
{ //=========================================================================================================================
	if (sprite == nullptr)
	{
		return 0;
	}
	if (sprite->getNumFrames() == 0)
	{
		return 0;
	}
	return sprite->getNumFrames() - 1;
}

sp<SpriteAnimationSequence> Entity::getAnimationBySpriteFrame(int frame)
{ //=========================================================================================================================
	if (sprite == nullptr)
	{
		log.error("Sprite is null in Entity: " + getName() + " while getting AnimationByFrame");
	}
	sp<SpriteAnimationSequence> a = sprite->getAnimationByFrame(frame);

	if (a == nullptr && sprite->getName() == "Camera" == false && sprite->getName() == "none" == false)
	{
		log.error("Animation for frame: " + to_string(frame) + " not found in Sprite: " + sprite->getName() + " in Entity: " + getName());
	}

	return a;
}

void Entity::setCurrentAnimationBySpriteFrame(int frame)
{ //=========================================================================================================================
	currentAnimation = getAnimationBySpriteFrame(frame);
}

int Entity::getCurrentAnimationNumberOfFrames()
{ //=========================================================================================================================
	if (sprite == nullptr)
	{
		return 0;
	}
	if (currentAnimation == nullptr)
	{
		return sprite->getNumFrames();
	}
	return sprite->getAnimationNumFramesByAnimation(currentAnimation);
}

int Entity::getCurrentAnimationStartFrame()
{ //=========================================================================================================================
	if (sprite == nullptr)
	{
		return 0;
	}
	if (currentAnimation == nullptr)
	{
		return 0;
	}
	return currentAnimation->frameStart;
}

int Entity::getCurrentAnimationLastFrame()
{ //=========================================================================================================================
	if (sprite == nullptr)
	{
		return 0;
	}
	if (currentAnimation == nullptr)
	{
		return getSpriteLastFrame();
	}
	return getCurrentAnimationStartFrame() + getCurrentAnimationNumberOfFrames() - 1;
}


string Entity::getCurrentAnimationName()
{ //=========================================================================================================================
	if (sprite == nullptr)
	{
		return "none";
	}
	if (currentAnimation == nullptr)
	{
		return "none";
	}
	if (currentAnimation->frameSequenceName.length() == 0)
	{
		return "none";
	}
	if (currentAnimation->frameSequenceName == "Frame0")
	{
		return "none";
	}

	return currentAnimation->frameSequenceName;
}

int Entity::getCurrentFrameOffsetInCurrentAnimation()
{ //=========================================================================================================================

	//this can return a negative value if the current frame is not in the current animation

	return getFrame() - getCurrentAnimationStartFrame();
}

void Entity::setFrameOffsetInCurrentAnimation(int frameOffset)
{ //=========================================================================================================================
	setFrame(getCurrentAnimationStartFrame() + frameOffset);
}

void Entity::setFrameInAllFrames(int frame)
{ //=========================================================================================================================

	if (frame > getSpriteLastFrame())
	{
		frame = getSpriteLastFrame();
	}

	setFrame(frame);

	setCurrentAnimationBySpriteFrame(frame);
}

void Entity::selectRandomFrame(int from, int toIncluding)
{ //=========================================================================================================================
	int oldFrame = getFrame();

	if (toIncluding > from)
	{
		while (getFrame() == oldFrame)
		{
			setFrame(from + Math::randUpToIncluding(toIncluding - from));
		}
	}

	setCurrentAnimationBySpriteFrame(getFrame());
}

void Entity::selectRandomFrameInAllFrames()
{ //=========================================================================================================================
	selectRandomFrame(0, getSpriteLastFrame());
}

void Entity::selectRandomFrameInCurrentAnimation()
{ //=========================================================================================================================
	selectRandomFrame(getCurrentAnimationStartFrame(), getCurrentAnimationLastFrame());
}

void Entity::incrementAnimationFrame(int from, int toIncluding)
{ //=========================================================================================================================

	setFrame(getFrame() + 1);

	if (getFrame() > toIncluding)
	{
		setFrame(from);
	}
}

void Entity::incrementAnimationFrameInAllFrames()
{ //=========================================================================================================================
	incrementAnimationFrame(0, getSpriteLastFrame());
}

void Entity::incrementAnimationFrameInCurrentAnimation()
{ //=========================================================================================================================
	incrementAnimationFrame(getCurrentAnimationStartFrame(), getCurrentAnimationLastFrame());
}

void Entity::stopAnimation()
{ //=========================================================================================================================
	setAnimateThroughAllFrames(false);
	setAnimateThroughCurrentAnimation(false);
	setLoopAnimation(false);
}

void Entity::setAnimateLoopThroughCurrentAnimation()
{ //=========================================================================================================================
	setFrameToCurrentAnimationStart();
	setAnimateThroughCurrentAnimation(true);
	setLoopAnimation(true);
}

void Entity::setAnimateLoopThroughAllFrames()
{ //=========================================================================================================================
	setAnimateThroughAllFrames(true);
	setLoopAnimation(true);
}

void Entity::setFrameToAllFramesZero()
{ //=========================================================================================================================
	setFrame(0);
}

void Entity::setFrameToCurrentAnimationStart()
{ //=========================================================================================================================
	setFrame(getCurrentAnimationStartFrame());
}

void Entity::setAnimateOnceThroughCurrentAnimation()
{ //=========================================================================================================================
	setFrameToCurrentAnimationStart();
	setAnimateThroughCurrentAnimation(true);
	setLoopAnimation(false);
}

void Entity::setAnimateOnceThroughAllFrames()
{ //=========================================================================================================================
	setFrameToAllFramesZero();
	setAnimateThroughAllFrames(true);
	setLoopAnimation(false);
}

void Entity::doAnimation()
{ //=========================================================================================================================

	if (sprite == nullptr)
	{
		return;
	}

	if (getAnimatingThroughCurrentAnimation() == true || getAnimatingThroughAllFrames() == true)
	{
		//if(getAnimatingThroughCurrentAnimation()==true&&currentAnimation==null)return;

		//			if(getName().equals("Player"))
		//			{
		//				log.debug("hello");
		//
		//			}

		int startFrame = 0;
		int lastFrame = getSpriteLastFrame();

		if (getAnimatingThroughCurrentAnimation() == true)
		{
			lastFrame = getCurrentAnimationLastFrame();
			startFrame = getCurrentAnimationStartFrame();
		}

		if (getFrame() < startFrame || getFrame() > lastFrame)
		{
			setFrame(startFrame);
		}

		//initialize ticks between loop for this loop (done once on the first frame)
		if (getFrame() == startFrame)
		{
			if (getRandomUpToTicksBetweenAnimationLoop() == true)
			{
				ticksBetweenAnimationLoopThisLoop = Math::randUpToIncluding(getTicksBetweenAnimationLoop());
			}
			else
			{
				ticksBetweenAnimationLoopThisLoop = getTicksBetweenAnimationLoop();
			}
		}

		//set ticks to wait on this frame
		int count = getTicksBetweenFrames();
		if (getFrame() == lastFrame)
		{
			if (getLoopAnimation() == false)
			{
				return;
			}
			else
			{
				count = ticksBetweenAnimationLoopThisLoop;
			}
		}

		if (haveTicksPassedSinceLastAnimated_ResetIfTrue(count))
		{
			if (getAnimatingThroughAllFrames() == true)
			{
				if (getRandomFrames())
				{
					selectRandomFrameInAllFrames();
				}
				else
				{
					incrementAnimationFrameInAllFrames();
				}
			}
			else
			{
				if (getAnimatingThroughCurrentAnimation() == true)
				{
					if (getRandomFrames())
					{
						selectRandomFrameInCurrentAnimation();
					}
					else
					{
						incrementAnimationFrameInCurrentAnimation();
					}
				}
			}
		}
	}
}

void Entity::setAlphaImmediately(float a)
{ //=========================================================================================================================

	if (a > 1.0f)
	{
		a = 1.0f;
	}
	if (a < 0.0f)
	{
		a = 0.0f;
	}
	alpha = a;
}

void Entity::fadeOutAndDelete()
{ //=========================================================================================================================
	if (alpha > 0)
	{
		setToAlpha(0.0f);
	}
	else
	{
		deleteWhenAlphaZero = true;
	}
}

void Entity::deleteFromMapEntityListAndReleaseTexture()
{ //=========================================================================================================================

	sp<Map>m = getMap();
	if (m != nullptr)
	{
		for (int i=0; i<m->activeEntityList.size();i++)
		{
			sp<Entity> se = m->activeEntityList.at(i);
			if (se.get() == this)
			{
				m->activeEntityList.erase(m->activeEntityList.begin() + i);
			}
		}
		//if (m->activeEntityList.contains(this))
		//{
		//	m->activeEntityList.remove(this);
		//}
	}

	if ((dynamic_cast<Player*>(this) != NULL) || (dynamic_cast<Character*>(this) != NULL) || (dynamic_cast<RandomCharacter*>(this) != NULL))
	{
		Character* r = static_cast<Character*>(this);
		r->uniqueTexture->release();
		//delete r->uniqueTexture;
		r->uniqueTexture = nullptr;

	}
}

void Entity::addEventBehavior(const string& s)
{ //=========================================================================================================================
	eventBehaviorList.add(s); //TODO: handle this stuff right!
}

sp<OKBool> Entity::checkServerTalkedToTodayValueAndResetAfterSuccessfulReturn()
{
	// TODO
	return ms<OKBool>();
}

void Entity::tellServerTalkedToToday()
{
	// TODO
}

float Entity::getDistanceFromEntity(sp<Entity> e)
{ //=========================================================================================================================
	return Math::distance(getMiddleX(), getMiddleY(), e->getMiddleX(), e->getMiddleY());
}

sp<Entity> Entity::findNearestEntity()
{ //=========================================================================================================================

	sp<Entity> nearestEntity = nullptr;

	int shortestdist = 65535;

	for (int n = 0; n < (int)getMap()->activeEntityList.size(); n++)
	{
		sp<Entity> currentEntity = getMap()->activeEntityList.at(n);

		if (this != currentEntity.get())
		{
			float x = getMiddleX() - (currentEntity->getMiddleX());
			float y = getMiddleY() - (currentEntity->getMiddleY());

			x = abs(x);
			y = abs(y);

			int dist = (int)(sqrt((x * x) + (y * y)));

			if (dist < shortestdist)
			{
				shortestdist = dist;
				nearestEntity = currentEntity;
			}
		}
	}

	return nearestEntity;
}

sp<Entity> Entity::findNearestEntityInDirection(int dir)
{ //=========================================================================================================================

	//this checks a direction and finds the closest entity within the entity boundaries in that direction

	sp<Entity> nearest_entity = nullptr;

	float shortestdist = 65535;

	float radiusX = getWidth() / 2;
	float radiusY = (getHeight() - getHitBoxFromTop()) / 2;

	for (int n = 0; n < (int)getMap()->activeEntityList.size(); n++)
	{
		sp<Entity> e = getMap()->activeEntityList.at(n);

		if (this != e.get())
		{
			float eMiddleX = e->getMiddleX();
			float eMiddleY = e->getMiddleY();
			float eRadiusX = e->getWidth() / 2;
			float eRadiusY = (e->getHeight() - e->getHitBoxFromTop()) / 2;

			if (dir == UP)
			{
				if (getMiddleY() >= eMiddleY && getMiddleX() + radiusX >= eMiddleX - eRadiusX && getMiddleX() - radiusX <= eMiddleX + eRadiusX)
				{
					float dist = eMiddleY - getMiddleY();
					if (dist < shortestdist)
					{
						shortestdist = dist;
						nearest_entity = e;
					}
				}
			}
			else
			{
				if (dir == DOWN)
				{
					if (getMiddleY() <= eMiddleY && getMiddleX() + radiusX >= eMiddleX - eRadiusX && getMiddleX() - radiusX <= eMiddleX + eRadiusX)
					{
						float dist = getMiddleY() - eMiddleY;
						if (dist < shortestdist)
						{
							shortestdist = dist;
							nearest_entity = e;
						}
					}
				}
				else
				{
					if (dir == LEFT)
					{
						if (getMiddleX() >= eMiddleX && getMiddleY() + radiusY >= eMiddleY - eRadiusY && getMiddleY() - radiusY <= eMiddleY + eRadiusY)
						{
							float dist = eMiddleX - getMiddleX();
							if (dist < shortestdist)
							{
								shortestdist = dist;
								nearest_entity = e;
							}
						}
					}
					else
					{
						if (dir == RIGHT)
						{
							if (getMiddleX() <= eMiddleX && getMiddleY() + radiusY >= eMiddleY - eRadiusY && getMiddleY() - radiusY <= eMiddleY + eRadiusY)
							{
								float dist = getMiddleX() - eMiddleX;
								if (dist < shortestdist)
								{
									shortestdist = dist;
									nearest_entity = e;
								}
							}
						}
					}
				}
			}
		}
	}

	return nearest_entity;
}

bool Entity::isWalkingIntoEntity(sp<Entity> entity)
{ //=========================================================================================================================

	bool walkingIntoDoor = false;

	switch(movementDirection)
	{
		case UP: 
		{
			if (
				entity->isXYTouchingMyHitBox(getMiddleX() - 1, getTop() - 2) || 
				entity->isXYTouchingMyHitBox(getMiddleX(), getTop() - 2)
				)
			{
				walkingIntoDoor = true;
			}
			break;
		}
		case DOWN: 
		{
			if (
				entity->isXYTouchingMyHitBox(getMiddleX() - 1, getBottom() + 1) || 
				entity->isXYTouchingMyHitBox(getMiddleX(), getBottom() + 1) ||
				entity->isXYTouchingMyHitBox(getMiddleX(), getBottom())//added this for elevator, might break some stuff?
				)
			{
				walkingIntoDoor = true;
			}
			break;
		}
		case LEFT: 
		{
			if (
				entity->isXYTouchingMyHitBox(getLeft() - 2, getMiddleY()) || 
				entity->isXYTouchingMyHitBox(getLeft() - 2, getMiddleY() - 1)
				) //getTop()+2) - getBottom()-1)
			{
				//TODO: should check middle for hitBoxLeft() and hitBoxRight() too, in case the player NPC is ever big enough to "straddle" a block. actually, i should check every %8 (%16 now i guess)
				walkingIntoDoor = true;
			}
			break;
		}
		case RIGHT: 
		{
			if (
				entity->isXYTouchingMyHitBox(getRight() + 1, getMiddleY()) || 
				entity->isXYTouchingMyHitBox(getRight() + 1, getMiddleY() - 1)
				) //getTop()+2) - getBottom()-1)
			{
				walkingIntoDoor = true;
			}
			break;
		}
		case UPLEFT: 
		{
			if (
				entity->isXYTouchingMyHitBox(getMiddleX() - 1, getTop() - 2) || 
				entity->isXYTouchingMyHitBox(getMiddleX(), getTop() - 2) ||
				entity->isXYTouchingMyHitBox(getLeft() - 2, getMiddleY()) ||
				entity->isXYTouchingMyHitBox(getLeft() - 2, getMiddleY() - 1) ||
				entity->isXYTouchingMyHitBox(getLeft(), getTop())//upleft corner
				)
			{
				walkingIntoDoor = true;
			}
			break;
		}
		case UPRIGHT: 
		{
			if (
				entity->isXYTouchingMyHitBox(getMiddleX() - 1, getTop() - 2) || 
				entity->isXYTouchingMyHitBox(getMiddleX(), getTop() - 2) ||
				entity->isXYTouchingMyHitBox(getRight() + 1, getMiddleY()) ||
				entity->isXYTouchingMyHitBox(getRight() + 1, getMiddleY() - 1) ||
				entity->isXYTouchingMyHitBox(getRight(), getTop())//ur corner
				)
			{
				walkingIntoDoor = true;
			}
			break;
		}
		case DOWNLEFT: 
		{
			if (
				entity->isXYTouchingMyHitBox(getMiddleX() - 1, getBottom() + 1) ||
				entity->isXYTouchingMyHitBox(getMiddleX(), getBottom() + 1) ||
				entity->isXYTouchingMyHitBox(getLeft() - 2, getMiddleY()) ||
				entity->isXYTouchingMyHitBox(getLeft() - 2, getMiddleY() - 1) ||
				entity->isXYTouchingMyHitBox(getLeft(), getBottom())//dl corner
				)
			{
				walkingIntoDoor = true;
			}
			break;
		}
		case DOWNRIGHT: 
		{
			if (
				entity->isXYTouchingMyHitBox(getMiddleX() - 1, getBottom() + 1) || 
				entity->isXYTouchingMyHitBox(getMiddleX(), getBottom() + 1) ||
				entity->isXYTouchingMyHitBox(getRight() + 1, getMiddleY()) ||
				entity->isXYTouchingMyHitBox(getRight() + 1, getMiddleY() - 1) ||
				entity->isXYTouchingMyHitBox(getRight(), getBottom())//dr corner
				)
			{
				walkingIntoDoor = true;
			}
			break;
		}


	}


//	if (movementDirection == UP)
//	{
//		if (entity->isXYTouchingMyHitBox(getMiddleX() - 1, getTop() - 2) || entity->isXYTouchingMyHitBox(getMiddleX(), getTop() - 2))
//		{
//			walkingIntoDoor = true;
//		}
//	}
//	else
//	{
//		if (movementDirection == DOWN)
//		{
//			if (entity->isXYTouchingMyHitBox(getMiddleX() - 1, getBottom() + 1) || entity->isXYTouchingMyHitBox(getMiddleX(), getBottom() + 1))
//			{
//				walkingIntoDoor = true;
//			}
//		}
//		else
//		{
//			if (movementDirection == LEFT)
//			{
//				if (entity->isXYTouchingMyHitBox(getLeft() - 2, getMiddleY()) || entity->isXYTouchingMyHitBox(getLeft() - 2, getMiddleY() - 1)) //getTop()+2) - getBottom()-1)
//				{
//					//TODO: should check middle for hitBoxLeft() and hitBoxRight() too, in case the player NPC is ever big enough to "straddle" a block. actually, i should check every %8 (%16 now i guess)
//					walkingIntoDoor = true;
//				}
//			}
//			else
//			{
//				if (movementDirection == RIGHT)
//				{
//					if (entity->isXYTouchingMyHitBox(getRight() + 1, getMiddleY()) || entity->isXYTouchingMyHitBox(getRight() + 1, getMiddleY() - 1)) //getTop()+2) - getBottom()-1)
//					{
//						walkingIntoDoor = true;
//					}
//				}
//			}
//		}
//	}
	return walkingIntoDoor;
}

bool Entity::isWalkingIntoArea(sp<Area> area)
{ //=========================================================================================================================

	bool walkingIntoArea = false;


	switch (movementDirection)
	{
		case UP:
		{
			if (
				area->isXYTouchingMyBoundary(getMiddleX() - 1, getTop() - 2) ||
				area->isXYTouchingMyBoundary(getMiddleX(), getTop() - 2)
				)
			{
				walkingIntoArea = true;
			}
			break;
		}
		case DOWN:
		{
			if (
				area->isXYTouchingMyBoundary(getMiddleX() - 1, getBottom() + 1) ||
				area->isXYTouchingMyBoundary(getMiddleX(), getBottom() + 1)
				)
			{
				walkingIntoArea = true;
			}
			break;
		}
		case LEFT:
		{
			if (
				area->isXYTouchingMyBoundary(getLeft() - 2, getMiddleY()) ||
				area->isXYTouchingMyBoundary(getLeft() - 2, getMiddleY() - 1)
				) //getTop()+2) - getBottom()-1)
			{
				//TODO: should check middle for hitBoxLeft() and hitBoxRight() too, in case the player NPC is ever big enough to "straddle" a block. actually, i should check every %8 (%16 now i guess)
				walkingIntoArea = true;
			}
			break;
		}
		case RIGHT:
		{
			if (
				area->isXYTouchingMyBoundary(getRight() + 1, getMiddleY()) ||
				area->isXYTouchingMyBoundary(getRight() + 1, getMiddleY() - 1)
				) //getTop()+2) - getBottom()-1)
			{
				walkingIntoArea = true;
			}
			break;
		}
		case UPLEFT:
		{
			if (
				area->isXYTouchingMyBoundary(getMiddleX() - 1, getTop() - 2) ||
				area->isXYTouchingMyBoundary(getMiddleX(), getTop() - 2) ||
				area->isXYTouchingMyBoundary(getLeft() - 2, getMiddleY()) ||
				area->isXYTouchingMyBoundary(getLeft() - 2, getMiddleY() - 1) ||
				area->isXYTouchingMyBoundary(getLeft(), getTop())//upleft corner
				)
			{
				walkingIntoArea = true;
			}
			break;
		}
		case UPRIGHT:
		{
			if (
				area->isXYTouchingMyBoundary(getMiddleX() - 1, getTop() - 2) ||
				area->isXYTouchingMyBoundary(getMiddleX(), getTop() - 2) ||
				area->isXYTouchingMyBoundary(getRight() + 1, getMiddleY()) ||
				area->isXYTouchingMyBoundary(getRight() + 1, getMiddleY() - 1) ||
				area->isXYTouchingMyBoundary(getRight(), getTop())//ur corner
				)
			{
				walkingIntoArea = true;
			}
			break;
		}
		case DOWNLEFT:
		{
			if (
				area->isXYTouchingMyBoundary(getMiddleX() - 1, getBottom() + 1) ||
				area->isXYTouchingMyBoundary(getMiddleX(), getBottom() + 1) ||
				area->isXYTouchingMyBoundary(getLeft() - 2, getMiddleY()) ||
				area->isXYTouchingMyBoundary(getLeft() - 2, getMiddleY() - 1) ||
				area->isXYTouchingMyBoundary(getLeft(), getBottom())//dl corner
				)
			{
				walkingIntoArea = true;
			}
			break;
		}
		case DOWNRIGHT:
		{
			if (
				area->isXYTouchingMyBoundary(getMiddleX() - 1, getBottom() + 1) ||
				area->isXYTouchingMyBoundary(getMiddleX(), getBottom() + 1) ||
				area->isXYTouchingMyBoundary(getRight() + 1, getMiddleY()) ||
				area->isXYTouchingMyBoundary(getRight() + 1, getMiddleY() - 1) ||
				area->isXYTouchingMyBoundary(getRight(), getBottom())//dr corner
				)
			{
				walkingIntoArea = true;
			}
			break;
		}


	}



//	if (movementDirection == UP)
//	{
//		if (area->isXYTouchingMyBoundary(getMiddleX() - 1, getTop() - 2) || area->isXYTouchingMyBoundary(getMiddleX(), getTop() - 2))
//		{
//			walkingIntoArea = true;
//		}
//	}
//	else
//	{
//		if (movementDirection == DOWN)
//		{
//			if (area->isXYTouchingMyBoundary(getMiddleX() - 1, getBottom() + 1) || area->isXYTouchingMyBoundary(getMiddleX(), getBottom() + 1))
//			{
//				walkingIntoArea = true;
//			}
//		}
//		else
//		{
//			if (movementDirection == LEFT)
//			{
//				if (area->isXYTouchingMyBoundary(getLeft() - 2, getMiddleY()) || area->isXYTouchingMyBoundary(getLeft() - 2, getMiddleY() - 1)) //getTop()+2) - getBottom()-1)
//				{
//					//TODO: should check middle for hitBoxLeft() and hitBoxRight() too, in case the player NPC is ever big enough to "straddle" a block. actually, i should check every %8 (%16 now i guess)
//					walkingIntoArea = true;
//				}
//			}
//			else
//			{
//				if (movementDirection == RIGHT)
//				{
//					if (area->isXYTouchingMyBoundary(getRight() + 1, getMiddleY()) || area->isXYTouchingMyBoundary(getRight() + 1, getMiddleY() - 1)) //getTop()+2) - getBottom()-1)
//					{
//						walkingIntoArea = true;
//					}
//				}
//			}
//		}
//	}


	return walkingIntoArea;
}

bool Entity::isEntityHitBoxTouchingMyHitBox(sp<Entity> e)
{ //=========================================================================================================================
	return isEntityHitBoxTouchingMyHitBoxByAmount(e, 0);
}

bool Entity::isNearestEntityHitBoxTouchingMyHitBox()
{ //=========================================================================================================================
	return isNearestEntityHitBoxTouchingMyHitBoxByAmount(0);
}

bool Entity::isAreaCenterTouchingMyHitBox(sp<Area> a)
{ //=========================================================================================================================
	return isAreaCenterTouchingMyHitBoxByAmount(a, 0);
}

bool Entity::isAreaBoundaryTouchingMyHitBox(sp<Area> a)
{ //=========================================================================================================================
	return isAreaBoundaryTouchingMyHitBoxByAmount(a, 0);
}

bool Entity::isXYTouchingMyHitBox(float x, float y)
{ //=========================================================================================================================
	return isXYTouchingMyHitBoxByAmount(x, y, 0);
}

bool Entity::isXYXYTouchingMyHitBox(float left, float top, float right, float bottom)
{ //=========================================================================================================================
	return isXYXYTouchingMyHitBoxByAmount(left, top, right, bottom, 0);
}

bool Entity::isAreaBoundaryTouchingMyMiddleXY(sp<Area> a)
{ //=========================================================================================================================
	return isAreaBoundaryTouchingMyMiddleXYByAmount(a, 0);
}

bool Entity::isEntityMiddleXYTouchingMyMiddleXY(sp<Entity> e)
{ //=========================================================================================================================
	return isEntityMiddleXYTouchingMyMiddleXYByAmount(e, 1);
}

bool Entity::isAreaCenterTouchingMyMiddleXY(sp<Area> a)
{ //=========================================================================================================================
	return isAreaCenterTouchingMyMiddleXYByAmount(a, 1);
}

bool Entity::isXYTouchingMyMiddleXY(float x, float y)
{ //=========================================================================================================================
	return isXYTouchingMyMiddleXYByAmount(x, y, 1);
}

bool Entity::isXYXYTouchingMyMiddleXY(float left, float top, float right, float bottom)
{ //=========================================================================================================================
	return isXYXYTouchingMyMiddleXYByAmount(left, top, right, bottom, 0);
}

bool Entity::isEntityHitBoxTouchingMyHitBoxByAmount(sp<Entity> e, int amt)
{ //=========================================================================================================================
	return Math::isXYXYTouchingXYXYByAmount(getLeft(), getTop(), getRight(), getBottom(), e->getLeft(), e->getTop(), e->getRight(), e->getBottom(), amt);
}

bool Entity::isNearestEntityHitBoxTouchingMyHitBoxByAmount(int amt)
{ //=========================================================================================================================
	return isEntityHitBoxTouchingMyHitBoxByAmount(findNearestEntity(), amt);
}

bool Entity::isAreaCenterTouchingMyHitBoxByAmount(sp<Area> a, int amt)
{ //=========================================================================================================================
	return isXYTouchingMyHitBoxByAmount(a->middleX(), a->middleY(), amt);
}

bool Entity::isAreaBoundaryTouchingMyHitBoxByAmount(sp<Area> a, int amt)
{ //=========================================================================================================================
	return isXYXYTouchingMyHitBoxByAmount(a->getLeft(), a->getTop(), a->getRight(), a->getBottom(), amt);
}

bool Entity::isXYTouchingMyHitBoxByAmount(float x, float y, int amt)
{ //=========================================================================================================================
	return isXYXYTouchingMyHitBoxByAmount(x, y, x, y, amt);
}

bool Entity::isXYXYTouchingMyHitBoxByAmount(float left, float top, float right, float bottom, int amt)
{ //=========================================================================================================================
	return Math::isXYXYTouchingXYXYByAmount(getLeft(), getTop(), getRight(), getBottom(), left, top, right, bottom, amt);
}

bool Entity::isAreaBoundaryTouchingMyMiddleXYByAmount(sp<Area> a, int amt)
{ //=========================================================================================================================
	return isXYXYTouchingMyMiddleXYByAmount(a->getLeft(), a->getTop(), a->getRight(), a->getBottom(), amt);
}

bool Entity::isEntityMiddleXYTouchingMyMiddleXYByAmount(sp<Entity> e, int amt)
{ //=========================================================================================================================
	return isXYTouchingMyMiddleXYByAmount(e->getMiddleX(), e->getMiddleY(), amt);
}

bool Entity::isAreaCenterTouchingMyMiddleXYByAmount(sp<Area> a, int amt)
{ //=========================================================================================================================
	return isXYTouchingMyMiddleXYByAmount(a->middleX(), a->middleY(), amt);
}

bool Entity::isXYTouchingMyMiddleXYByAmount(float x, float y, int amt)
{ //=========================================================================================================================
	return isXYXYTouchingMyMiddleXYByAmount(x, y, x, y, amt);
}

bool Entity::isXYXYTouchingMyMiddleXYByAmount(float left, float top, float right, float bottom, int amt)
{ //=========================================================================================================================
	return Math::isXYXYTouchingXYXYByAmount(getMiddleX(), getMiddleY(), getMiddleX(), getMiddleY(), left, top, right, bottom, amt);
}

bool Entity::isTouchingPlayerInDirection(int dir)
{ //=========================================================================================================================

	if (getPlayer() == nullptr)
	{
		return false;
	}

	bool touching_player_entity = false;

	float pTop = getPlayer()->getTop();
	float pBottom = getPlayer()->getBottom();
	float pLeft = getPlayer()->getLeft();
	float pRight = getPlayer()->getRight();

	if (getPlayer() == nullptr)
	{
		return false;
	}
	if (this == getPlayer().get())
	{
		return false;
	}

	if (dir == UP)
	{
		if (((getLeft() >= pLeft && getLeft() <= pRight) || (getRight() <= pRight && getRight() >= pLeft)) && (getTop() - 1 <= pBottom && getTop() - 1 >= pTop)) //hitBoxRight() side of e is within sprite width - hitBoxLeft() side of e is within sprite width - TODO use touching functions
		{
			touching_player_entity = true;
		}
	}
	else
	{
		if (dir == DOWN)
		{
			if (((getLeft() >= pLeft && getLeft() <= pRight) || (getRight() <= pRight && getRight() >= pLeft)) && (getBottom() + 1 >= pTop && getBottom() + 1 <= pBottom))
			{
				touching_player_entity = true; //hitBoxRight() side of e is within sprite width - hitBoxLeft() side of e is within sprite width
			}
		}
		else
		{
			if (dir == LEFT)
			{
				if (((getTop() >= pTop && getTop() <= pBottom) || (getBottom() >= pTop && getBottom() <= pBottom)) && (getLeft() < pRight && getLeft() > pLeft))
				{
					touching_player_entity = true; //hitBoxBottom() side of e is within sprite getHit height - hitBoxTop() side of e is within sprite getHit height
				}
			}
			else
			{
				if (dir == RIGHT)
				{
					if (((getTop() >= pTop && getTop() <= pBottom) || (getBottom() >= pTop && getBottom() <= pBottom)) && (getRight() > pLeft && getRight() < pRight))
					{
						touching_player_entity = true; //hitBoxBottom() side of e is within sprite getHit height - hitBoxTop() side of e is within sprite getHit height
					}
				}
			}
		}
	}

	return touching_player_entity;
}

bool Entity::isHitBoxTouchingEntityInDirectionByAmount(sp<Entity> e, int direction, int amt)
{ //=========================================================================================================================
	return isHitBoxTouchingXYXYInDirectionByAmount(e->getLeft(), e->getTop(), e->getRight(), e->getBottom(), direction, amt);
}

bool Entity::isHitBoxTouchingXYInDirectionByAmount(float x, float y, int direction, int amt)
{ //=========================================================================================================================

	//TODO: make this xy point shine or highlighted or something

	//TODO: should draw all getHit boxes for characters

	//this uses point collision

	return isHitBoxTouchingXYXYInDirectionByAmount(x, y, x, y, direction, amt);
}

bool Entity::isHitBoxTouchingXYXYInDirectionByAmount(float left, float top, float right, float bottom, int direction, int amt)
{ //=========================================================================================================================

	if (left == -1)
	{
		left = getMiddleX();
	}
	if (top == -1)
	{
		top = getMiddleY();
	}
	if (right == -1)
	{
		right = getMiddleX();
	}
	if (bottom == -1)
	{
		bottom = getMiddleY();
	}

	bool detected = false;

	if (direction == UP)
	{
		if ((getLeft()) <= (right) && (getRight()) >= (left) && (getTop()) <= (bottom) && ((getTop() - 2) - amt) >= (top))
		{
			detected = true; //TODO use touching functions
		}
	}

	if (direction == DOWN)
	{
		if ((getLeft()) <= (right) && (getRight()) >= (left) && (getBottom()) <= (bottom) && ((getBottom() + 2) + amt) >= (top))
		{
			detected = true;
		}
	}

	if (direction == LEFT)
	{
		if (((getLeft() - 2) - amt) <= (right) && (getLeft()) >= (left) && (getTop()) <= (bottom) && (getBottom()) >= (top))
		{
			detected = true;
		}
	}
	if (direction == RIGHT)
	{
		if ((getRight()) <= (right) && ((getRight() + 2) + amt) >= (left) && (getTop()) <= (bottom) && (getBottom()) >= (top))
		{
			detected = true;
		}
	}

	if (direction == UPLEFT)
	{
		if (((getLeft() - 2) - amt) <= (right) && (getLeft()) >= (left) && (getTop()) <= (bottom) && ((getTop() - 2) - amt) >= (top))
		{
			detected = true;
		}
	}
	if (direction == UPRIGHT)
	{
		if ((getRight()) <= (right) && ((getRight() + 2) + amt) >= (left) && (getTop()) <= (bottom) && ((getTop() - 2) - amt) >= (top))
		{
			detected = true;
		}
	}
	if (direction == DOWNLEFT)
	{
		if (((getLeft() - 2) - amt) <= (right) && (getLeft()) >= (left) && (getBottom()) <= (bottom) && (getBottom() + amt) >= (top))
		{
			detected = true;
		}
	}
	if (direction == DOWNRIGHT)
	{
		if ((getRight()) <= (right) && ((getRight() + 2) + amt) >= (left) && (getBottom()) <= (bottom) && (getBottom() + amt) >= (top))
		{
			detected = true;
		}
	}
	return detected;
}

float Entity::getTop()
{ //=========================================================================================================================
	return (getY() + (getHitBoxFromTop()));
}

float Entity::getLeft()
{ //=========================================================================================================================
	return (getX() + (getHitBoxFromLeft()));
}

float Entity::getRight()
{ //=========================================================================================================================
	return (getX() + getWidth()) - (getHitBoxFromRight());
}

float Entity::getBottom()
{ //=========================================================================================================================
	return (getY() + getHeight()) - (getHitBoxFromBottom());
}

float Entity::getMiddleX()
{ //=========================================================================================================================
	return (getLeft() + ((getRight() - getLeft()) / 2));
}

float Entity::getMiddleY()
{ //=========================================================================================================================
	return (getBottom() - (((getHeight() - (getHitBoxFromTop())) - getHitBoxFromBottom()) / 2));
}

float Entity::getRoundedMiddleX()
{
	return round(getMiddleX());
}

float Entity::getRoundedMiddleY()
{
	return round(getMiddleY());
}

float Entity::getMiddleOffsetX()
{
	return getMiddleX() - getX();
}

float Entity::getMiddleOffsetY()
{
	return getMiddleY() - getY();
}

float Entity::getScreenX()
{ //=========================================================================================================================

	float zoom = getCameraman()->getZoom();

	float mapCameraXPixelsHQ = getMap()->mapCamX();

	float left = getX() + standJitterX;
	float right = left + getWidth();
	float screenleft = mapCameraXPixelsHQ;
	float screenright = mapCameraXPixelsHQ + getEngine()->getWidthRelativeToZoom();

	float screenXPixelsHQ = (left - screenleft);

	//if overlapping left side of screen
	if (right >= screenleft && left < screenleft)
	{
		screenXPixelsHQ = (0.0f - (screenleft - left));
	}

	//if onscreen and not overlapping the left side
	if (left >= screenleft && left < screenright)
	{
		screenXPixelsHQ = (left - screenleft);
	}

	return screenXPixelsHQ > zoom;
}

float Entity::getScreenY()
{ //=========================================================================================================================

	float zoom = getCameraman()->getZoom();
	float mapCameraYPixelsHQ = getMap()->mapCamY();

	float top = getY() + standJitterY;
	float bottom = top + getHeight();
	float screentop = mapCameraYPixelsHQ;
	float screenbottom = mapCameraYPixelsHQ + getEngine()->getHeightRelativeToZoom();

	float screenYPixelsHQ = top - screentop;

	//if overlapping top side of screen
	if (bottom >= screentop && top < screentop)
	{
		screenYPixelsHQ = 0.0f - (screentop - top);
	}

	//if onscreen and not overlapping the top side
	if (top >= screentop && top < screenbottom)
	{
		screenYPixelsHQ = top - screentop;
	}

	return screenYPixelsHQ > zoom;
}

float Entity::getScreenLeft()
{ //=========================================================================================================================
	return getScreenX();
}

float Entity::getScreenTop()
{ //=========================================================================================================================
	return getScreenY();
}

float Entity::getScreenMiddleHitboxY()
{ //=========================================================================================================================
	float zoom = getCameraman()->getZoom();
	return (float)((getScreenTop() + (getHeight() * zoom)) - ((getHeight() * zoom - (getHitBoxFromTop()) * zoom) / 2));
}

float Entity::getHitBoxFromLeft()
{ //=========================================================================================================================
	if (sprite == nullptr || currentAnimation == nullptr)
	{
		return 0;
	}

	return currentAnimation->hitBoxFromLeftPixels1X * 2 * getScale();
}

float Entity::getHitBoxFromRight()
{ //=========================================================================================================================
	if (sprite == nullptr || currentAnimation == nullptr)
	{
		return 0;
	}

	return currentAnimation->hitBoxFromRightPixels1X * 2 * getScale();
}

float Entity::getHitBoxFromTop()
{ //=========================================================================================================================
	if (sprite == nullptr || currentAnimation == nullptr)
	{
		return 0;
	}

	return currentAnimation->hitBoxFromTopPixels1X * 2 * getScale();
}

float Entity::getHitBoxFromBottom()
{ //=========================================================================================================================
	if (sprite == nullptr || currentAnimation == nullptr)
	{
		return 0;
	}

	return currentAnimation->hitBoxFromBottomPixels1X * 2 * getScale();
}

float Entity::getShadowStart()
{ //=========================================================================================================================
	if (sprite == nullptr || currentAnimation == nullptr)
	{
		return 0;
	}

	return (float)(currentAnimation->hitBoxFromTopPixels1X) / (float)(sprite->getImageHeight());
}

float Entity::getX()
{
	return mapX;
}

float Entity::getY()
{
	return mapY;
}

void Entity::setX(float x)
{
	mapX = x;
}

void Entity::setY(float y)
{
	mapY = y;
}

void Entity::incX()
{
	mapX++;
}

void Entity::incY()
{
	mapY++;
}

void Entity::decX()
{
	mapX--;
}

void Entity::decY()
{
	mapY--;
}

float Entity::getWidth()
{ //=========================================================================================================================

	if (sprite != nullptr)
	{
		int w = sprite->getImageWidth();
		if (sprite->useHQ2X == false)w *= 2;

		return (w * getScale());
	}
	else
	{
		return 0;
	}
}

float Entity::getHeight()
{ //=========================================================================================================================
	if (sprite != nullptr)
	{
		int h = sprite->getImageHeight();
		if (sprite->useHQ2X == false)h *= 2;
		return (h * getScale());
	}
	else
	{
		return 0;
	}
}

sp<EntityData> Entity::getData()
{
	return data;
}

string& Entity::getName()
{
	return getData()->getName();
}


string& Entity::getComment()
{
	return getData()->getComment();
}

int Entity::getID()
{
	return getData()->getID();
}

//int Entity::getMapID()
//{
//	return getData()->getMapID();
//}


string& Entity::getSpriteName()
{
	return getData()->getSpriteName();
}

bool Entity::getIsNPC()
{
	return getData()->getIsNPC();
}

bool Entity::getPushable()
{
	return getData()->getPushable();
}

bool Entity::getNonWalkable()
{
	return getData()->getNonWalkable();
}

float Entity::getToAlpha()
{
	return getData()->getToAlpha();
}

float Entity::getScale()
{
	return getData()->getScale();
}

bool Entity::getDisableShadow()
{
	return getData()->getDisableShadow();
}

bool Entity::getAboveWhenEqual()
{
	return getData()->getAboveWhenEqual();
}

bool Entity::getAlwaysOnTop()
{
	return getData()->getAlwaysOnTop();
}

bool Entity::getAlwaysOnBottom()
{
	return getData()->getAlwaysOnBottom();
}

RenderOrder Entity::getRenderOrder()
{
	return getData()->getRenderOrder();
}

bool Entity::getAboveTopLayer()
{
	return getData()->getAboveTopLayer();
}

int Entity::getInitialFrame()
{
	return getData()->getInitialFrame();
}

bool Entity::getAnimatingThroughAllFrames()
{
	return getData()->getAnimatingThroughAllFrames();
}

bool Entity::getRandomFrames()
{
	return getData()->getRandomFrames();
}

bool Entity::getRandomUpToTicksBetweenFrames()
{
	return getData()->getRandomUpToTicksBetweenFrames();
} //TODO

bool Entity::getRandomUpToTicksBetweenAnimationLoop()
{
	return getData()->getRandomUpToTicksBetweenAnimationLoop();
}

int Entity::getTicksBetweenFrames()
{
	return getData()->getTicksBetweenFrames();
}

int Entity::getTicksBetweenAnimationLoop()
{
	return getData()->getTicksBetweenAnimationLoop();
}

float Entity::getTicksPerPixelMoved()
{
	return getData()->getTicksPerPixelMoved();
}

sp<EventData> Entity::getEventData()
{
	return getData()->getEventData();
}

bool Entity::getOnlyHereDuringEvent()
{
	return getData()->getOnlyHereDuringEvent();
}

float Entity::getVoicePitch()
{
	return getData()->getVoicePitch();
}

bool Entity::getMovementAnimationDisabled()
{
	return getData()->getMovementAnimationDisabled();
}

bool Entity::getIgnoreHitLayer()
{
	return getData()->getHitLayerDisabled();
}

bool Entity::getIgnoreHitPlayer()
{
	return getData()->getIgnoreHitPlayer();
}

bool Entity::getPullPlayer()
{
	return getData()->getPullPlayer();
}

bool Entity::getPushPlayer()
{
	return getData()->getPushPlayer();
}

bool Entity::getAnimatingThroughCurrentAnimation()
{
	return getData()->getAnimatingThroughCurrentAnimation();
}

/**
Returns whether animation should loop.
*/
bool Entity::getLoopAnimation()
{
	return getData()->getLoopAnimation();
}

sp<vector<string>> Entity::getConnectionTYPEIDList()
{
	return getData()->getConnectionTYPEIDList();
}

sp<vector<string>> Entity::getBehaviorList()
{
	return getData()->getBehaviorList();
}


string Entity::getTYPEIDString()
{
	return getData()->getTYPEIDString();
}

void Entity::setID(int s)
{
	getData()->setID(s);
}

void Entity::setName(const string& s)
{
	getData()->setName(s);
}

void Entity::setComment(const string& s)
{
	getData()->setComment(s);
}

void Entity::setSpawnXPixelsHQ(float s)
{
	getData()->setSpawnXPixels1X(s / 2.0f);
}

void Entity::setSpawnYPixelsHQ(float s)
{
	getData()->setSpawnYPixels1X(s / 2.0f);
}

void Entity::setIsNPC(bool s)
{
	getData()->setIsNPC(s);
}

void Entity::setPushable(bool s)
{
	getData()->setPushable(s);
}

void Entity::setNonWalkable(bool s)
{
	getData()->setNonWalkable(s);
}

void Entity::setToAlpha(float alpha)
{
	getData()->setToAlpha(alpha);
}

void Entity::setScale(float s)
{
	getData()->setScale(s);
}

void Entity::setDisableShadow(bool s)
{
	getData()->setDisableShadow(s);
}

void Entity::setInitialFrame(int f)
{
	getData()->setInitialFrame(f);
}

void Entity::setRandomFrames(bool s)
{
	getData()->setRandomFrames(s);
}

void Entity::setRandomUpToTicksBetweenFrames(bool s)
{
	getData()->setRandomUpToTicksBetweenFrames(s);
} //TODO

void Entity::setRandomUpToTicksBetweenAnimationLoop(bool s)
{
	getData()->setRandomUpToTicksBetweenAnimationLoop(s);
}

void Entity::setTicksBetweenFrames(int s)
{
	getData()->setTicksBetweenFrames(s);
}

void Entity::setTicksBetweenAnimationLoop(int s)
{
	getData()->setTicksBetweenAnimationLoop(s);
}

void Entity::setTicksPerPixelMoved(float s)
{
	getData()->setTicksPerPixelMoved(s);
}

void Entity::setRenderOrder(RenderOrder s)
{
	getData()->setRenderOrder(s);
}

void Entity::setAboveTopLayer(bool s)
{
	getData()->setAboveTopLayer(s);
}

void Entity::setAboveWhenEqual(bool s)
{
	getData()->setAboveWhenEqual(s);
}

void Entity::setAlwaysOnBottom(bool s)
{
	getData()->setAlwaysOnBottom(s);
}

void Entity::setAlwaysOnTop(bool s)
{
	getData()->setAlwaysOnTop(s);
}

void Entity::setOnlyHereDuringEvent(bool s)
{
	getData()->setOnlyHereDuringEvent(s);
}

void Entity::setVoicePitch(float s)
{
	getData()->setVoicePitch(s);
}

void Entity::setAnimationDisabled(bool s)
{
	getData()->setAnimationDisabled(s);
}

void Entity::setHitLayerDisabled(bool s)
{
	getData()->setHitLayerDisabled(s);
}

void Entity::setIgnoreHitPlayer(bool s)
{
	getData()->setIgnoreHitPlayer(s);
}

//void Entity::setEventID(int s)
//{
//	getData()->setEventID(s);
//}

void Entity::setAnimateThroughAllFrames(bool s)
{
	getData()->setAnimateThroughAllFrames(s);
}

void Entity::setAnimateThroughCurrentAnimation(bool s)
{
	getData()->setAnimateThroughCurrentAnimation(s);
}

void Entity::setLoopAnimation(bool s)
{
	getData()->setLoopAnimation(s);
}

