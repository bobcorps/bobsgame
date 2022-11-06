#include "stdafx.h"

#include "Gwen/Platform.h"
#include <Gwen/Controls/Base.h>
#include <Gwen/Controls/DockBase.h>
#include "Gwen/Controls/Label.h"
#include "Gwen/Controls/TabControl.h"
#include "Gwen/Controls/HorizontalSplitter.h"
#include "Gwen/Controls/ListBox.h"
#include "Gwen/Controls/HorizontalSlider.h"
#include "Gwen/Controls/StatusBar.h"
#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/ImagePanel.h>
#include <Gwen/Controls/Rectangle.h>
#include "Gwen/Controls/DockedTabControl.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/CollapsibleList.h"
#include "Gwen/Controls/Layout/Position.h"

#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Controls/ListBox.h>
#include <Gwen/Controls/Base.h>
#include <Gwen/Controls/Property/BaseProperty.h>
#include <Gwen/Controls/Property/ColorSelector.h>
#include <Gwen/Controls/Property/ComboBox.h>
#include <Gwen/Controls/Property/Checkbox.h>
#include <Gwen/Controls/Property/TextWithButton.h>
#include <Gwen/Controls/Property/Listbox.h>
#include <Gwen/Controls/Properties.h>
#include <Gwen/Controls/PropertyTree.h>
#include <Gwen/Controls/CheckBox.h>
#include <Gwen/Controls/ColorPicker.h>
#include <Gwen/Controls/ComboBox.h>
#include <map>

using namespace Gwen;
using namespace Gwen::Controls;




//GWEN_CONTROL_CONSTRUCTOR(GameSequenceEditorControl)
GameSequenceEditorControl::GameSequenceEditorControl(Gwen::Controls::Base* pParent, const Gwen::String& pName, shared_ptr<OKGame >b) : Base(pParent, pName)
{//=========================================================================================================================

	this->bobsGame = b;

	SetSize(GLUtils::getViewportWidth(), GLUtils::getViewportHeight());
	//SetDeleteOnClose(false);
	Dock(Pos::Fill);

	/*
	None
	Left
	Right
	Top
	Bottom
	CenterV
	CenterH
	Fill
	Center
	*/
	//int w = GLUtils::getViewportWidth();
	//int h = GLUtils::getViewportHeight();



	applyButtonsBase = make_shared<Base>(this);
	applyButtonsBase->Dock(Pos::Top);
	//applyButtonsBase->SetSize(w, 20);
	{

		applyButtonsBasePositioner = make_shared<Layout>::Position(applyButtonsBase);
		applyButtonsBasePositioner->Dock(Pos::Left | Pos::Center);
		//applyButtonsBasePositioner->SetSize(w/2,20);

		loadButton = make_shared<Button>(applyButtonsBase);
		loadButton->SetText(L"Load or create new");
		//loadButton->SetToolTip("Load an existing game or create a new one.");
		loadButton->Dock(Pos::Left | Pos::Center);
		loadButton->SetWidth(150);
		loadButton->onPress.Add(this, &GameSequenceEditorControl::onLoadOrCreateButton);

		saveButton = make_shared<Button>(applyButtonsBase);
		saveButton->SetText(L"Save");
		//saveButton->SetToolTip("Saves to XML in " + string(SDL_GetPrefPath("OK Corporation", "bob's game")) + ".  Renames existing file with versioning.");
		saveButton->Dock(Pos::Left | Pos::Center);
		saveButton->SetWidth(50);
		saveButton->onPress.Add(this, &GameSequenceEditorControl::onSaveButton);

		uploadButton = make_shared<Button>(applyButtonsBase);
		uploadButton->SetText(L"Upload");
		//uploadButton->SetToolTip("Upload this sequence to the server so others can play it.  You will keep ownership rights and reuploading modified versions will overwrite the version on the server.  You can only upload one new sequence every 10 minutes.");
		uploadButton->Dock(Pos::Left | Pos::Center);
		uploadButton->SetWidth(50);
		uploadButton->onPress.Add(this, &GameSequenceEditorControl::onUploadButton);

		exitButton = make_shared<Button>(applyButtonsBase);
		exitButton->SetText(L"Exit To Title Screen");
		//exitButton->SetToolTip("Leave (Ask to save changes).");
		exitButton->Dock(Pos::Right | Pos::Center);
		exitButton->SetWidth(150);
		exitButton->onPress.Add(this, &GameSequenceEditorControl::onExitButton);

	}



	mainWindowSplitter = make_shared<Gwen>::Controls::HorizontalSplitter(this);
	mainWindowSplitter->GetSplitter()->onDragged.Add(this, &GameSequenceEditorControl::doResize);
	mainWindowSplitter->Dock(Pos::Top);




	leftBase = make_shared<Base>(mainWindowSplitter);
	mainWindowSplitter->SetPanel(0, leftBase);
	{

		//settings panel
		gameTypesWindow = make_shared<WindowControl>(leftBase);
		gameTypesWindow->SetTitle("Available Game Types");
		gameTypesWindow->SetToolTip("");
		gameTypesWindow->SetClosable(false);
		gameTypesWindow->DisableResizing();
		gameTypesWindow->Dock(Pos::Fill);
		{

			gameTypesListBoxBase = make_shared<Base>(gameTypesWindow);
			gameTypesListBoxBase->Dock(Pos::Left);
			{
				gameTypesListBox = make_shared<ListBox>(gameTypesListBoxBase);
				gameTypesListBox->Dock(Pos::Fill);
				gameTypesListBox->SetAllowMultiSelect(false);


				populateGameTypesListBox();

//				gameTypesButtonBase = make_shared<Base>(gameTypesListBoxBase);
//				gameTypesButtonBase->Dock(Pos::Bottom);
//				gameTypesButtonBase->SetSize(gameTypesListBoxBase->Width(), 20);
//				{
//					downloadGameTypesButton = make_shared<Button>(gameTypesButtonBase);
//					downloadGameTypesButton->SetText(L"Download More Game Types");
//					downloadGameTypesButton->SetToolTip("");
//					downloadGameTypesButton->Dock(Pos::Left | Pos::CenterV);
//					downloadGameTypesButton->onPress.Add(this, &GameSequenceEditorControl::onDownloadGameTypesButton);
//				}

			}

			gameTypesAddRemoveButtonsBase = make_shared<Base>(gameTypesWindow);
			gameTypesAddRemoveButtonsBase->Dock(Pos::Right);
			{

				gameTypesAddRemoveButtonsPositioner = make_shared<Layout>::Position(gameTypesAddRemoveButtonsBase);
				gameTypesAddRemoveButtonsPositioner->Dock(Pos::Top);

				selectAddButton = make_shared<Button>(gameTypesAddRemoveButtonsBase);
				selectAddButton->SetText(L"Add ->");
				//selectAddButton->SetToolTip(b->_Info.tip);
				selectAddButton->Dock(Pos::Top);
				selectAddButton->SetWidth(70);
				selectAddButton->SetHeight(20);
				selectAddButton->onPress.Add(this, &GameSequenceEditorControl::onAddButton);

				selectRemoveButton = make_shared<Button>(gameTypesAddRemoveButtonsBase);
				selectRemoveButton->SetText(L"<- Remove");
				//selectRemoveButton->SetToolTip(b->_Info.tip);
				selectRemoveButton->Dock(Pos::Top);
				selectRemoveButton->SetWidth(70);
				selectRemoveButton->SetHeight(20);
				selectRemoveButton->onPress.Add(this, &GameSequenceEditorControl::onRemoveButton);

			}

		}
	}

	rightBase = make_shared<Base>(mainWindowSplitter);
	mainWindowSplitter->SetPanel(1, rightBase);
	{

		//window control current game sequence
		//splitter
		//left game sequence list
		//right game sequence properties

		currentGameSequenceTextBoxesBase = make_shared<Base>(rightBase);
		currentGameSequenceTextBoxesBase->Dock(Pos::Top);
		{

			Layout::Position* positioner = make_shared<Layout>::Position(currentGameSequenceTextBoxesBase);
			positioner->Dock(Pos::Top);
			positioner->SetSize(currentGameSequenceTextBoxesBase->Width(),10);

			currentGameSequenceNameTextBoxBase = make_shared<Base>(currentGameSequenceTextBoxesBase);
			currentGameSequenceNameTextBoxBase->Dock(Pos::Top);
			{
				currentGameSequenceNameLabel = make_shared<Label>(currentGameSequenceNameTextBoxBase);
				currentGameSequenceNameLabel->Dock(Pos::Left | Pos::CenterV);
				currentGameSequenceNameLabel->SetSize(100, 20);
				currentGameSequenceNameLabel->SetText("Name:");

				currentGameSequenceNameTextBox = make_shared<TextBox>(currentGameSequenceNameTextBoxBase);
				currentGameSequenceNameTextBox->Dock(Pos::Left | Pos::CenterV);
				currentGameSequenceNameTextBox->SetSize(currentGameSequenceNameTextBoxBase->Width() - 150, 20);
			}

			currentGameSequenceDescriptionTextBoxBase = make_shared<Base>(currentGameSequenceTextBoxesBase);
			currentGameSequenceDescriptionTextBoxBase->Dock(Pos::Top);
			{
				currentGameSequenceDescriptionLabel = make_shared<Label>(currentGameSequenceDescriptionTextBoxBase);
				currentGameSequenceDescriptionLabel->Dock(Pos::Left);
				currentGameSequenceDescriptionLabel->SetSize(100, 20);
				currentGameSequenceDescriptionLabel->SetText("Description:");

				currentGameSequenceDescriptionTextBox = make_shared<TextBox>(currentGameSequenceDescriptionTextBoxBase);
				currentGameSequenceDescriptionTextBox->Dock(Pos::Left);
				currentGameSequenceDescriptionTextBox->SetSize(currentGameSequenceDescriptionTextBoxBase->Width() - 150, 20);
			}

		}

		currentGameSequenceWindow = make_shared<WindowControl>(rightBase);
		currentGameSequenceWindow->SetTitle("Current Sequence");
		currentGameSequenceWindow->SetToolTip("");
		currentGameSequenceWindow->SetClosable(false);
		currentGameSequenceWindow->DisableResizing();
		currentGameSequenceWindow->Dock(Pos::Top);
		{

			currentGameSequenceListBox = make_shared<ListBox>(currentGameSequenceWindow);
			currentGameSequenceListBox->Dock(Pos::Fill);
			currentGameSequenceListBox->SetAllowMultiSelect(false);
			currentGameSequenceListBox->SetSize(currentGameSequenceWindow->Width() - 30, currentGameSequenceWindow->Height() - 60);

		}



		currentGameSequenceButtonBase = make_shared<Base>(rightBase);
		currentGameSequenceButtonBase->Dock(Pos::Top);
		{
			upButton = make_shared<Button>(currentGameSequenceButtonBase);
			upButton->SetText(L"Up");
			upButton->SetToolTip("");
			upButton->Dock(Pos::Left | Pos::CenterV);
			upButton->onPress.Add(this, &GameSequenceEditorControl::onUpButton);

			downButton = make_shared<Button>(currentGameSequenceButtonBase);
			downButton->SetText(L"Down");
			downButton->SetToolTip("");
			downButton->Dock(Pos::Left | Pos::CenterV);
			downButton->onPress.Add(this, &GameSequenceEditorControl::onDownButton);
		}

	}

	GetParent()->SizeToChildren();
	SizeToChildren();



//	mainWindowSplitter->SetSplitterWidth(w / 5 * 3);
//
//	settingsWindow->SetSize(w * 3 / 5, h - 20);
//
//	settingsScrollControl->SetWidth(settingsWindow->Width() - 20);
//
//	blockWindowSplitter->SetSize(blockWindow->Width(), blockWindow->Height());
//	blockWindowSplitter->SetSplitterWidth(blockWindow->Width() / 3 * 1);
//	blockListBase->SetSize(blockWindow->Width() / 3 * 1, blockWindowSplitter->Height());
//	blockSelectionListBox->SetSize(blockListBase->Width() - 30, blockListBase->Height() - 60);
//	blockButtonBase->SetSize(blockListBase->Width(), 20);
//	blockPropertiesScrollControl->SetSize(blockWindow->Width() / 3 * 2 - 15, blockWindowSplitter->Height());
//
//	pieceWindowSplitter->SetSize(pieceWindow->Width(), pieceWindow->Height());
//	pieceWindowSplitter->SetSplitterWidth(pieceWindow->Width() / 3 * 1);
//	pieceListBase->SetSize(pieceWindow->Width() / 3 * 1, pieceWindowSplitter->Height());
//	pieceSelectionListBox->SetSize(pieceListBase->Width() - 30, pieceListBase->Height() - 60);
//	pieceButtonBase->SetSize(pieceListBase->Width(), 20);
//	piecePropertiesScrollControl->SetSize(pieceWindow->Width() / 3 * 2 - 15, pieceWindowSplitter->Height());
//
//	previewBase->SetSize(w * 2 / 5, h - 20);
//	applyButtonsBase->SetSize(previewBase->Width(), 20);
//	darkRectangle->SetSize(previewBase->Width(), previewBase->Height() - 20);

	doResize();

	//mainWindowSplitter->SetSplitterWidth(w/2);
	//doResize();
}



void GameSequenceEditorControl::checkResize()
{//=========================================================================================================================
	if (lastW != GLUtils::getViewportWidth() || lastH != GLUtils::getViewportHeight())
	{

		doResize();

		lastW = GLUtils::getViewportWidth();
		lastH = GLUtils::getViewportHeight();

	}
}
void GameSequenceEditorControl::doResize()
{//=========================================================================================================================

	SetSize(GLUtils::getViewportWidth(), GLUtils::getViewportHeight());
	GetParent()->SizeToChildren();

	int w = GLUtils::getViewportWidth();
	int h = GLUtils::getViewportHeight();
	{

		applyButtonsBase->SetSize(w, 20);
		{
			applyButtonsBasePositioner->SetSize(w/2, 20);
		}

		mainWindowSplitter->SetSize(w, h - 20);
		{
			int sw = mainWindowSplitter->GetSplitter()->X();
			if (500 > sw)sw = 500;
			leftBase->SetSize(sw, mainWindowSplitter->Height());
			{
				gameTypesWindow->SetSize(leftBase->Width(), leftBase->Height());
				{

					gameTypesListBoxBase->SetSize(gameTypesWindow->Width() - 100, gameTypesWindow->Height());
					{
						gameTypesListBox->SetSize(gameTypesListBoxBase->Width() - 30, gameTypesListBoxBase->Height() - 60);
					}

					gameTypesAddRemoveButtonsBase->SetSize(80, gameTypesWindow->Height());
					{
						gameTypesAddRemoveButtonsPositioner->SetHeight(gameTypesAddRemoveButtonsBase->Height() / 2 - 20);
					}
				}
			}

			rightBase->SetSize(w - mainWindowSplitter->GetSplitter()->X(), mainWindowSplitter->Height());
			{
				currentGameSequenceWindow->SetSize(rightBase->Width(), rightBase->Height() - 80);
				{
					currentGameSequenceListBox->SetSize(currentGameSequenceWindow->Width() - 30, currentGameSequenceWindow->Height() - 60);
				}

				currentGameSequenceButtonBase->SetSize(rightBase->Width(), 20);
				{

				}

				currentGameSequenceTextBoxesBase->SetSize(rightBase->Width(), 50);
				{
					currentGameSequenceNameTextBoxBase->SetSize(currentGameSequenceTextBoxesBase->Width(), 20);
					{
						currentGameSequenceNameTextBox->SetSize(currentGameSequenceNameTextBoxBase->Width() - 150, 20);
					}

					currentGameSequenceDescriptionTextBoxBase->SetSize(currentGameSequenceTextBoxesBase->Width(), 20);
					{
						currentGameSequenceDescriptionTextBox->SetSize(currentGameSequenceNameTextBoxBase->Width() - 150, 20);
					}
				}

			}
		}
	}

	SizeToChildren(true, true);

}

#include <fstream>
#include <iostream>
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/Delegate.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Process.h"
#include "Poco/DirectoryIterator.h"
using Poco::DirectoryIterator;
using Poco::File;
using Poco::Process;
using Poco::Path;


//shared_ptr<GameSequence> GameSequenceEditorControl::getGameSequenceByName(string name)
//{//=========================================================================================================================
//
//	shared_ptr<GameSequence >bt = nullptr;
//	for (int i = 0; i<bobsGame->loadedGameSequences.size(); i++)
//	{
//		shared_ptr<GameSequence >b = bobsGame->loadedGameSequences.get(i);
//		if (b->name == name)
//		{
//			bt = b;
//		}
//	}
//	return bt;
//
//}

void GameSequenceEditorControl::populateGameTypesListBox()
{//=========================================================================================================================

	ArrayList<pair<shared_ptr<GameType>, pair<string, shared_ptr<OKColor>>>> gamesStringColor = bobsGame->getSortedGameTypes();
	for (int i = 0; i < gamesStringColor.size(); i++)
	{
		pair<shared_ptr<GameType>, pair<string, shared_ptr<OKColor>>> gameTypeStringColorPairPair = gamesStringColor.get(i);
		shared_ptr<GameType >g = gameTypeStringColorPairPair.first;
		pair<string, shared_ptr<OKColor>> stringColorPair = gameTypeStringColorPairPair.second;
		string name = stringColorPair.first;
		shared_ptr<OKColor >color = stringColorPair.second;

		Layout::TableRow *row = gameTypesListBox->AddItem(name, g->uuid);
		//row->onRowSelected.Add(this, &GameSequenceEditorControl::onGameTypesListSelect);
		if (g->yourVote!="none")row->SetTextColor(Gwen::Color(128, 128, 128));
		else row->SetTextColor(Gwen::Color(color->ri(), color->gi(), color->bi()));

	}
}

void GameSequenceEditorControl::populateGameSequencesListBox()
{//=========================================================================================================================

	ArrayList<pair<shared_ptr<GameSequence>, pair<string, shared_ptr<OKColor>>>> gamesStringColor = bobsGame->getSortedGameSequences();
	for (int i = 0; i < gamesStringColor.size(); i++)
	{
		pair<shared_ptr<GameSequence>, pair<string, shared_ptr<OKColor>>> gameSequenceStringColorPairPair = gamesStringColor.get(i);
		shared_ptr<GameSequence >g = gameSequenceStringColorPairPair.first;
		pair<string, shared_ptr<OKColor>> stringColorPair = gameSequenceStringColorPairPair.second;
		string name = stringColorPair.first;
		shared_ptr<OKColor >color = stringColorPair.second;

		Layout::TableRow *row = gameSequencesListBox->AddItem(name, g->uuid);
		row->onRowSelected.Add(this, &GameSequenceEditorControl::onGameSequencesListSelect);
		if (g->yourVote!="none")row->SetTextColor(Gwen::Color(128, 128, 128));
		else row->SetTextColor(Gwen::Color(color->ri(), color->gi(), color->bi()));

	}
}

void GameSequenceEditorControl::saveGameSequenceListToCurrentGameSequence()
{//=========================================================================================================================

	//traverse the listbox and get all the names of the games
	//clear the game array
	//add all the names
	//set the name and description from the textboxes

	currentGameSequence->importExport_gameUUIDs.clear();
	for(int i=0; i<currentGameSequenceListBox->GetNumRows(); i++)
	{
		Layout::TableRow* row = currentGameSequenceListBox->GetRow(i);

		string uuid = row->GetName().c_str();

		shared_ptr<GameType >gt = bobsGame->getGameTypeByUUID(uuid);
		currentGameSequence->importExport_gameUUIDs.add(gt->uuid);
	}

	string name = currentGameSequenceNameTextBox->GetText().c_str();
	OKGame::log->debug(name);

	while (name.find("`") != string::npos)
	{
		string s = name.substr(0, name.find("`"));
		name = name.substr(name.find("`") + 1);
		s += name;
	}

	if (name == "")name = "New Game Sequence";
	bool taken = false;
	for (int i = 0; i < bobsGame->loadedGameSequences.size(); i++)
	{
		shared_ptr<GameSequence >s = bobsGame->loadedGameSequences.get(i);
		if (s != currentGameSequence && s->name == name)taken = true;
	}
	if (taken)
	{
		int n = 0;
		for (int i = 0; i < bobsGame->loadedGameSequences.size(); i++)
		{
			shared_ptr<GameSequence >s = bobsGame->loadedGameSequences.get(i);
			if (s != currentGameSequence && s->name == name + " " + to_string(n))
			{
				n++;
				i = 0;
			}
		}
		name = name + " " + to_string(n);
	}

	currentGameSequence->name = name;

	string desc = currentGameSequenceDescriptionTextBox->GetText().c_str();
	OKGame::log->debug(desc);
	if (desc == "")desc = "This is my new game sequence description.";
	currentGameSequence->description = desc;

}

void GameSequenceEditorControl::initFromCurrentGameSequence()
{//=========================================================================================================================
		//fill the name and description text boxes and populate the listbox
	currentGameSequenceListBox->Clear();
	GetCanvas()->DoThink();
	currentGameSequenceListBox->UnselectAll();

	for(int i=0;i<currentGameSequence->importExport_gameUUIDs.size();i++)
	{
		string otheruuid = currentGameSequence->importExport_gameUUIDs.get(i);
		shared_ptr<GameType >gt = bobsGame->getGameTypeByUUID(otheruuid);
		currentGameSequenceListBox->AddItem(gt->name, gt->uuid);
	}
	currentGameSequenceNameTextBox->SetText(currentGameSequence->name);
	currentGameSequenceDescriptionTextBox->SetText(currentGameSequence->description);

}


void GameSequenceEditorControl::onGameSequencesListSelect(shared_ptr<Base> control)
{//=========================================================================================================================

	Layout::TableRow* row = (Layout::TableRow*)control;
	string uuid = row->GetName().c_str();

	shared_ptr<GameSequence >s = bobsGame->getGameSequenceByUUID(uuid);

	if (s == nullptr)
	{
		OKGame::log->error("Could not find game sequence with uuid:" + uuid);
		return;
	}

	currentGameSequence = s;

	initFromCurrentGameSequence();

}

void GameSequenceEditorControl::saveCurrentGameSequenceToXML()
{//=========================================================================================================================

	bobsGame->saveGameSequenceToXML(currentGameSequence, false);
	bobsGame->loadGameSequenceUUIDsToGamesArray(currentGameSequence);

}

void GameSequenceEditorControl::onLoadOrCreateButton(shared_ptr<Base> control)
{//=========================================================================================================================

	windowOpen = true;
	//ask to save changes to current game

	//opening a new game will overwrite the currently open game
	//make sure you have saved before editing a different game

	askToSaveBase = make_shared<WindowControl>(GetCanvas());
	askToSaveBase->SetTitle("Save changes?");
	askToSaveBase->SetSize(200, 60);
	askToSaveBase->MakeModal(true);
	//exitAskToSaveWindow->Position(Pos::Center);
	askToSaveBase->SetPos(GLUtils::getViewportWidth() / 2 - 100, GLUtils::getViewportHeight() / 2 - 30);
	askToSaveBase->SetDeleteOnClose(true);
	//exitAskToSaveWindow->SetClosable(false);
	{

		askToSaveYesButton = make_shared<Button>(askToSaveBase);
		askToSaveYesButton->SetText(L"Save");
		//askToSaveYesButton->SetToolTip(b->_Info.tip);
		askToSaveYesButton->Dock(Pos::Center | Pos::Left);
		askToSaveYesButton->SetWidth(80);
		askToSaveYesButton->onPress.Add(this, &GameSequenceEditorControl::saveAndOpen);

		askToSaveNoButton = make_shared<Button>(askToSaveBase);
		askToSaveNoButton->SetText(L"Don't Save");
		//askToSaveNoButton->SetToolTip(b->_Info.tip);
		askToSaveNoButton->Dock(Pos::Center | Pos::Left);
		askToSaveNoButton->SetWidth(100);
		askToSaveNoButton->onPress.Add(this, &GameSequenceEditorControl::dontSaveAndOpen);

	}

}

void GameSequenceEditorControl::saveAndOpen(shared_ptr<Base> control)
{//=========================================================================================================================

	askToSaveBase->CloseButtonPressed();

	onSaveButton(control);

	openLoadOrCreateDialog(true);

	windowOpen = false;

}

void GameSequenceEditorControl::dontSaveAndOpen(shared_ptr<Base> control)
{//=========================================================================================================================

	askToSaveBase->CloseButtonPressed();

	openLoadOrCreateDialog(true);

	windowOpen = false;

}



void GameSequenceEditorControl::openLoadOrCreateDialog(bool allowedToClose = true)
{//=========================================================================================================================

	windowOpen = true;

	int w = GLUtils::getViewportWidth() / 2;
	int h = GLUtils::getViewportHeight() / 2;

	loadOrCreateGameSequenceWindow = make_shared<WindowControl>(GetCanvas());
	{
		loadOrCreateGameSequenceWindow->SetTitle("Load existing or create new game sequence");
		loadOrCreateGameSequenceWindow->SetSize(w, h);
		loadOrCreateGameSequenceWindow->MakeModal(true);
		//loadOrCreateGameSequenceWindow->Position(Pos::Center);
		loadOrCreateGameSequenceWindow->SetPos(GLUtils::getViewportWidth() / 2 - w / 2, GLUtils::getViewportHeight() / 2 - h / 2);
		loadOrCreateGameSequenceWindow->SetDeleteOnClose(true);
		if (allowedToClose)
		{
			loadOrCreateGameSequenceWindow->SetClosable(true);
		}
		else
		{
			loadOrCreateGameSequenceWindow->SetClosable(false);
		}

		gameSequencesListBox = make_shared<ListBox>(loadOrCreateGameSequenceWindow);
		{
			gameSequencesListBox->SetMargin(Margin(0, 0, 0, 0));
			gameSequencesListBox->Dock(Pos::Top);
			gameSequencesListBox->SetAllowMultiSelect(false);
			gameSequencesListBox->SetWidth(loadOrCreateGameSequenceWindow->Width());
			gameSequencesListBox->SetHeight(loadOrCreateGameSequenceWindow->Height());
			//Layout::TableRow* row;
			gameSequencesListBox->SetSize(w, h - 75);

			//populate list with existing game types
			populateGameSequencesListBox();

		}

		gameSequenceSelectLabel = make_shared<Label>(loadOrCreateGameSequenceWindow);
		gameSequenceSelectLabel->Dock(Pos::Top);
		gameSequenceSelectLabel->SetText("");
		gameSequenceSelectLabel->SetHeight(20);
		gameSequenceSelectLabel->SetTextColor(Gwen::Color(255,0,128,255));

		gameSequenceSelectButtonBase = make_shared<Base>(loadOrCreateGameSequenceWindow);
		gameSequenceSelectButtonBase->Dock(Pos::Bottom);
		gameSequenceSelectButtonBase->SetWidth(loadOrCreateGameSequenceWindow->Width());
		gameSequenceSelectButtonBase->SetHeight(20);
		{
			editSelectedGameSequenceButton = make_shared<Button>(gameSequenceSelectButtonBase);
			editSelectedGameSequenceButton->SetText(L"Edit");
			//editSelectedGameSequenceButton->SetToolTip(b->_Info.tip);
			editSelectedGameSequenceButton->Dock(Pos::Left | Pos::CenterV);
			//editSelectedGameSequenceButton->SetWidth(160);
			editSelectedGameSequenceButton->onPress.Add(this, &GameSequenceEditorControl::editSelectedGameSequence);

			createNewGameSequenceButton = make_shared<Button>(gameSequenceSelectButtonBase);
			createNewGameSequenceButton->SetText(L"Create new");
			//createNewGameSequenceButton->SetToolTip(b->_Info.tip);
			createNewGameSequenceButton->Dock(Pos::Left | Pos::CenterV);
			//createNewGameSequenceButton->SetWidth(160);
			createNewGameSequenceButton->onPress.Add(this, &GameSequenceEditorControl::createNewGameSequence);

			duplicateGameSequenceButton = make_shared<Button>(gameSequenceSelectButtonBase);
			duplicateGameSequenceButton->SetText(L"Duplicate");
			//duplicateGameSequenceButton->SetToolTip(b->_Info.tip);
			duplicateGameSequenceButton->Dock(Pos::Left | Pos::CenterV);
			//duplicateGameSequenceButton->SetWidth(240);
			duplicateGameSequenceButton->onPress.Add(this, &GameSequenceEditorControl::duplicateGameSequence);

			deleteGameSequenceButton = make_shared<Button>(gameSequenceSelectButtonBase);
			deleteGameSequenceButton->SetText(L"Delete");
			//deleteGameSequenceButton->SetToolTip(b->_Info.tip);
			deleteGameSequenceButton->Dock(Pos::Left | Pos::CenterV);
			//deleteGameSequenceButton->SetWidth(240);
			deleteGameSequenceButton->onPress.Add(this, &GameSequenceEditorControl::deleteGameSequence);
		}
	}

}


void GameSequenceEditorControl::editSelectedGameSequence(shared_ptr<Base> control)
{//=========================================================================================================================

 //make sure the type is editable, if not, do nothing, or dont even allow the button to get here in the first place

 //load xml into settings, init currentGameType prop tree, blocklist, piecelist
 //should already be done because onselect
	if (gameSequencesListBox->IsAnyRowSelected() == false)return;

	//bob can edit anything
#ifndef _DEBUG
//	if (currentGameSequence->builtInType)
//	{
//		//create modal notice that you can only duplicate? or maybe have an error label
//		gameSequenceSelectLabel->SetText("Built in sequences must be duplicated in order to edit them.");
//	}
//	else

	if (currentGameSequence->downloaded)
	{
		//create modal notice that you can only duplicate? or maybe have an error label
		gameSequenceSelectLabel->SetText("Downloaded sequences must be duplicated in order to edit them.");
	}
	else
#endif
	{
		loadOrCreateGameSequenceWindow->CloseButtonPressed();
		gameSequenceSelectLabel->SetText("");

		windowOpen = false;
	}
}

void GameSequenceEditorControl::createNewGameSequence(shared_ptr<Base> control)
{//=========================================================================================================================
 //create new currentGameType with defaults and close the list
	shared_ptr<GameSequence >s = make_shared<GameSequence>();
	s->name += to_string(bobsGame->loadedGameSequences.size());
	//bobsGame->loadedGameSequences.add(s);

	currentGameSequence = s;
	initFromCurrentGameSequence();

	//add to list and select it

	Layout::TableRow *row = gameSequencesListBox->AddItem(s->name, s->uuid);
	row->onRowSelected.Add(this, &GameSequenceEditorControl::onGameSequencesListSelect);
	gameSequencesListBox->SetSelectedRow(row);
	//onGameSequencesListSelect(row);

	loadOrCreateGameSequenceWindow->CloseButtonPressed();

	windowOpen = false;

}

void GameSequenceEditorControl::duplicateGameSequence(shared_ptr<Base> control)
{//=========================================================================================================================

 //the only thing we would do is load the selected game type and then append the name with "copy"
	if (gameSequencesListBox->IsAnyRowSelected() == false)return;

	//OKGame::log->debug(to_string(currentGameType->pieceTypes.size()));

	shared_ptr<GameSequence >s = make_shared<GameSequence>();
	string uuid = s->uuid;
	*s = *currentGameSequence;
	s->uuid = uuid;
	//s->builtInType = false;
	s->downloaded = false;
	//OKGame::log->debug(to_string(s->pieceTypes.size()));


	s->name += " Copy";
	bool taken = false;
	for (int i = 0; i < bobsGame->loadedGameSequences.size(); i++)
	{
		shared_ptr<GameSequence >g = bobsGame->loadedGameSequences.get(i);
		if (g != s && g->name == s->name)taken = true;
	}
	if (taken)
	{
		int n = 0;
		for (int i = 0; i < bobsGame->loadedGameSequences.size(); i++)
		{
			shared_ptr<GameSequence >g = bobsGame->loadedGameSequences.get(i);
			if (g != s && g->name == s->name + " " + to_string(n))
			{
				n++;
				i = 0;
			}
		}
		s->name = s->name + " " + to_string(n);
	}

	//bobsGame->loadedGameSequences.add(s);

	currentGameSequence = s;
	initFromCurrentGameSequence();

	Layout::TableRow *row = gameSequencesListBox->AddItem(s->name, s->uuid);
	row->onRowSelected.Add(this, &GameSequenceEditorControl::onGameSequencesListSelect);
	gameSequencesListBox->SetSelectedRow(row);
	//onGameSequencesListSelect(row);

	loadOrCreateGameSequenceWindow->CloseButtonPressed();

	windowOpen = false;
}

void GameSequenceEditorControl::deleteGameSequence(shared_ptr<Base> control)
{//=========================================================================================================================

	if (gameSequencesListBox->IsAnyRowSelected() == false)return;

	Layout::TableRow* row = gameSequencesListBox->GetSelectedRow();
	string uuid = row->GetName().c_str();

	shared_ptr<GameSequence >bt = bobsGame->getGameSequenceByUUID(uuid);

	if (bt == nullptr)
	{
		OKGame::log->error("Could not find GameSequence with uuid:" + uuid);
		return;
	}

//	if (bt->builtInType)
//	{
//		gameSequenceSelectLabel->SetText("Cannot delete built in sequence.");
//		return;
//	}

	if (bobsGame->loadedGameSequences.contains(bt))bobsGame->loadedGameSequences.remove(bt);

	gameSequenceSelectLabel->SetText("");

	gameSequencesListBox->UnselectAll();

	//delete it from the list
	gameSequencesListBox->RemoveItem(row);
	GetCanvas()->DoThink();

	//if (bt->loadedFilename != "")
	{
		//delete the filename (let's just rename it to deleted)
		//store the filename when load it
		string userDataPathString = FileUtils::appDataPath + "gameSequences/";
		File xmlFile(userDataPathString + bt->uuid+".xml");
		int num = 0;
		while (xmlFile.exists())
		{
			num++;
			xmlFile = File(userDataPathString + bt->uuid + ".deleted" + to_string(num));
		}
		xmlFile = File(userDataPathString + bt->uuid + ".xml");
		if (xmlFile.exists())
		{
			xmlFile.renameTo(userDataPathString + bt->uuid + ".deleted" + to_string(num));
		}
	}

	if (gameSequencesListBox->GetNumRows() == 0)
	{
		currentGameSequence = nullptr;
	}
	else
	{
		gameSequencesListBox->SelectByIndex(0);
		onGameSequencesListSelect((Controls::Base*)gameSequencesListBox->GetRow(0));
	}

}


void GameSequenceEditorControl::onSaveButton(shared_ptr<Base> control)
{//=========================================================================================================================

 //save current game to xml in userdata, maybe upload to server as userid:gametype if user is logged in
 //if current game with that name exists in either folder, say this name is already being used
	saveGameSequenceListToCurrentGameSequence();

	//save to xml
	saveCurrentGameSequenceToXML();

	//save new or duplicated gametype to loadedGameSequence list since we don't do this now until save
	if (bobsGame->loadedGameSequences.contains(currentGameSequence) == false)
		bobsGame->loadedGameSequences.add(currentGameSequence);


}

void GameSequenceEditorControl::onUploadButton(shared_ptr<Base> control)
{//=========================================================================================================================

	onSaveButton(control);

	if (currentGameSequence->importExport_gameUUIDs.size() == 0)return;
	if (currentGameSequence->importExport_gameUUIDs.size() == 1)
	{
		shared_ptr<WindowControl >result = make_shared<WindowControl>(GetCanvas());
		result->SetTitle("Result");
		result->SetSize(300, 60);
		result->MakeModal(true);
		result->SetPos(GLUtils::getViewportWidth() / 2 - 150, GLUtils::getViewportHeight() / 2 - 30);
		result->SetDeleteOnClose(true);

		shared_ptr<Label> label = make_shared<Label>(result);
		label->SetText("You cannot upload sequences with only one game type.");
		label->Dock(Pos::Fill);
		return;
	}

	GameSequence g;
	g = *currentGameSequence;

	std::stringstream ss;
	boost::archive::xml_oarchive oarchive(ss);
	oarchive << BOOST_SERIALIZATION_NVP(g);

	string zip = FileUtils::zipStringToBase64String(ss.str());


	//GameType:XML:name:uuid
	bobsGame->getServerConnection()->connectAndAuthorizeAndQueueWriteToChannel_S(OKNet::OK_Game_GameTypesAndSequences_Upload_Request+"GameSequence:" + zip + ":`" + g.name + "`:" + g.uuid + ":" + OKNet::endline);

	string response = "";
	int tries = 0;
	while (tries<10)
	{
		tries++;
		Main::delay(500);
		response = bobsGame->getServerConnection()->getAndResetOKGameGameTypesAndSequencesUploadResponse_S();
		if (response != "")
		{
			break;
		}
	}

	if (response == "")response = "Did not get a response from the server. Try again later.";

	shared_ptr<WindowControl >result = make_shared<WindowControl>(GetCanvas());
	result->SetTitle("Result");
	result->SetSize(300, 60);
	result->MakeModal(true);
	result->SetPos(GLUtils::getViewportWidth() / 2 - 150, GLUtils::getViewportHeight() / 2 - 30);
	result->SetDeleteOnClose(true);

	shared_ptr<Label> label = make_shared<Label>(result);
	label->SetText(response);
	label->Dock(Pos::Fill);

}

void GameSequenceEditorControl::onExitButton(shared_ptr<Base> control)
{//=========================================================================================================================

 //Utility::Format(L"Window %i", m_iWindowCount)

	askToSaveBase = make_shared<WindowControl>(GetCanvas());
	askToSaveBase->SetTitle("Save changes?");
	askToSaveBase->SetSize(300, 60);
	askToSaveBase->MakeModal(true);
	//exitAskToSaveWindow->Position(Pos::Center);
	askToSaveBase->SetPos(GLUtils::getViewportWidth() / 2 - 150, GLUtils::getViewportHeight() / 2 - 30);
	askToSaveBase->SetDeleteOnClose(true);
	//exitAskToSaveWindow->SetClosable(false);
	{

		askToSaveYesButton = make_shared<Button>(askToSaveBase);
		askToSaveYesButton->SetText(L"Save and Leave");
		//askToSaveYesButton->SetToolTip(b->_Info.tip);
		askToSaveYesButton->Dock(Pos::Left | Pos::Center);
		askToSaveYesButton->SetWidth(120);
		askToSaveYesButton->onPress.Add(this, &GameSequenceEditorControl::saveAndExit);

		askToSaveNoButton = make_shared<Button>(askToSaveBase);
		askToSaveNoButton->SetText(L"Leave without saving");
		//askToSaveNoButton->SetToolTip(b->_Info.tip);
		askToSaveNoButton->Dock(Pos::Left | Pos::Center);
		askToSaveNoButton->SetWidth(150);
		askToSaveNoButton->onPress.Add(this, &GameSequenceEditorControl::dontSaveAndExit);

		//		cancelExitButton = make_shared<Button>(askToSaveBase);
		//		cancelExitButton->SetText(L"Cancel");
		//		//cancelExitButton->SetToolTip(b->_Info.tip);
		//		cancelExitButton->Dock(Pos::Left | Pos::Center);
		//		cancelExitButton->SetWidth(60);
		//		cancelExitButton->onPress.Add(this, &GameSequenceEditorControl::cancelExit);
	}

}

void GameSequenceEditorControl::saveAndExit(shared_ptr<Base> control)
{//=========================================================================================================================
 //save the game to appdata or home/userid-gamename
 //make sure that there arent any unallowable characters, sanitize, etc
 //ask the user if they want to overwrite if the filename exists

	onSaveButton(control);

	//go to title screen
	askToSaveBase->CloseButtonPressed();
	exit = true;
}

void GameSequenceEditorControl::dontSaveAndExit(shared_ptr<Base> control)
{//=========================================================================================================================
 //go back to title screen
	askToSaveBase->CloseButtonPressed();
	exit = true;

}


void GameSequenceEditorControl::onCurrentGameSequenceListRowSelect(shared_ptr<Base> control)
{//=========================================================================================================================

	//don't need to do anything for this i don't think

}


void GameSequenceEditorControl::onAddButton(shared_ptr<Base> control)
{//=========================================================================================================================

	if (gameTypesListBox->IsAnyRowSelected() == false)return;

	Layout::TableRow* row = gameTypesListBox->GetSelectedRow();
	string uuid = row->GetName().c_str();

	shared_ptr<GameType >gt = bobsGame->getGameTypeByUUID(uuid);

	Layout::TableRow* newRow = currentGameSequenceListBox->AddItem(gt->name,gt->uuid);

	currentGameSequence->importExport_gameUUIDs.add(gt->uuid);

	currentGameSequenceListBox->SetSelectedRow(newRow);

}

void GameSequenceEditorControl::onRemoveButton(shared_ptr<Base> control)
{//=========================================================================================================================

	if (currentGameSequenceListBox->IsAnyRowSelected() == false)return;

	Layout::TableRow* row = currentGameSequenceListBox->GetSelectedRow();
	//string uuid = row->GetName().c_str();

	for (int i = 0; i < currentGameSequenceListBox->GetNumRows(); i++)
	{
		if(row==currentGameSequenceListBox->GetRow(i))
		{
			currentGameSequence->importExport_gameUUIDs.removeAt(i);
			currentGameSequenceListBox->RemoveItem(row);
			break;
		}
	}
	currentGameSequenceListBox->UnselectAll();

}

void GameSequenceEditorControl::onUpButton(shared_ptr<Base> control)
{//=========================================================================================================================

	if (currentGameSequenceListBox->IsAnyRowSelected() == false)return;

	Layout::TableRow* row = currentGameSequenceListBox->GetSelectedRow();
	string uuid = row->GetName().c_str();
	shared_ptr<GameType >selectedGame = bobsGame->getGameTypeByUUID(uuid);

	int index = 0;
	for (int i = 0; i < currentGameSequenceListBox->GetNumRows(); i++)
	{
		if (row == currentGameSequenceListBox->GetRow(i))
		{
			index = i;
			break;
		}
	}

	if(index>0)
	{
		currentGameSequence->importExport_gameUUIDs.removeAt(index);
		currentGameSequenceListBox->Clear();
		GetCanvas()->DoThink();

		currentGameSequence->importExport_gameUUIDs.insert(index - 1, selectedGame->uuid);
		for (int i = 0; i<currentGameSequence->importExport_gameUUIDs.size(); i++)
		{
			string newUUID = currentGameSequence->importExport_gameUUIDs.get(i);
			shared_ptr<GameType >game = bobsGame->getGameTypeByUUID(newUUID);
			currentGameSequenceListBox->AddItem(game->name, game->uuid);
		}
		currentGameSequenceListBox->SetSelectedRow(currentGameSequenceListBox->GetRow(index - 1));
	}


}

void GameSequenceEditorControl::onDownButton(shared_ptr<Base> control)
{//=========================================================================================================================

	if (currentGameSequenceListBox->IsAnyRowSelected() == false)return;

	Layout::TableRow* row = currentGameSequenceListBox->GetSelectedRow();
	string uuid = row->GetName().c_str();

	shared_ptr<GameType >selectedGame = bobsGame->getGameTypeByUUID(uuid);

	int index = currentGameSequenceListBox->GetNumRows() - 1;
	for (int i = 0; i < currentGameSequenceListBox->GetNumRows(); i++)
	{
		if (row == currentGameSequenceListBox->GetRow(i))
		{
			index = i;
			break;
		}
	}

	if (index < currentGameSequenceListBox->GetNumRows() - 1)
	{
		currentGameSequence->importExport_gameUUIDs.removeAt(index);
		currentGameSequenceListBox->Clear();
		GetCanvas()->DoThink();

		currentGameSequence->importExport_gameUUIDs.insert(index+1, selectedGame->uuid);
		for (int i = 0; i<currentGameSequence->importExport_gameUUIDs.size(); i++)
		{
			string newUUID = currentGameSequence->importExport_gameUUIDs.get(i);
			shared_ptr<GameType >game = bobsGame->getGameTypeByUUID(newUUID);
			currentGameSequenceListBox->AddItem(game->name, game->uuid);
		}
		currentGameSequenceListBox->SetSelectedRow(currentGameSequenceListBox->GetRow(index + 1));
	}


}





//=========================================================================================================================
void OKGame::gameSequenceEditorMenuUpdate()
{//=========================================================================================================================

 //	if (gameSequenceEditorMenu == nullptr)
 //	{
 //		gameSequenceEditorMenu = make_shared<Menu>(this);
 //
 //		gameSequenceEditorMenu->add("Back To Game", "Back To Game", OKColor::white);
 //		gameSequenceEditorMenu->add("Music Volume: " + to_string((int)(music->getVolume() * 100)) + "%", "Music Volume", OKColor::white);
 //		gameSequenceEditorMenu->add("Quit Game And Return To Title Screen", "Quit Game And Return To Title Screen", OKColor::white);
 //
 //		gameSequenceEditorMenu->cursorPosition = gameSequenceEditorMenuCursorPosition;
 //	}

	if (gameSequenceEditor == nullptr)
	{
		gameSequenceEditor = make_shared<GameSequenceEditorControl>(Main::gwenCanvas,"GameSequenceEditorControl",this);


		gameSequenceEditor->openLoadOrCreateDialog(false);

		chatEnabled = false;
	}



	bool leaveMenu = false;

	if(gameSequenceEditor!=nullptr)
	{

		gameSequenceEditor->checkResize();

		if (gameSequenceEditor->exit == true)
		{

			//remove unsaved game sequences
//			for (int i = 0; i<loadedGameSequences.size(); i++)
//			{
//				if (loadedGameSequences.get(i)->loadedFilename == "")
//				{
//					loadedGameSequences.removeAt(i);
//					i--;
//				}
//			}

			leaveMenu = true;
			delete gameSequenceEditor;
			gameSequenceEditor = nullptr;
		}
	}




	//	if (getControlsManager()->miniGame_UP_Pressed())
	//	{
	//		gameSequenceEditorMenu->up();
	//	}
	//
	//	if (getControlsManager()->miniGame_DOWN_Pressed())
	//	{
	//		gameSequenceEditorMenu->down();
	//	}
	//

	//
	//	if (getControlsManager()->miniGame_CONFIRM_Pressed())
	//	{
	//
	//		if (gameSequenceEditorMenu->isSelectedID("Quit Game And Return To Title Screen"))
	//		{
	//			leaveMenu = true;
	//			titleMenuShowing = true;
	//			initPlayer();
	//		}
	//
	//	}
	//
	//	if (getControlsManager()->miniGame_CANCEL_Pressed())
	//	{
	//		leaveMenu = true;
	//	}

	if (leaveMenu)
	{

		chatEnabled = true;

		initPlayer();

		gameSequenceEditorMenuShowing = false;

		startScreenMenuShowing = true;

		if (gameSequenceEditorMenu != nullptr)
		{
			gameSequenceEditorMenuCursorPosition = gameSequenceEditorMenu->cursorPosition;
			delete gameSequenceEditorMenu;
			gameSequenceEditorMenu = nullptr;
		}
	}
}

//=========================================================================================================================
void OKGame::gameSequenceEditorMenuRender()
{//=========================================================================================================================

	GLUtils::drawFilledRect(255, 255, 255, 0, (float)getWidth(), 0, (float)getHeight(), 1.0f);
	//
	//	shared_ptr<OKTexture> t = keyboardTexture;
	//
	//	if (gameSequenceEditorMenu == nullptr)return;
	//
	//	if (t != nullptr)
	//	{
	//		gameSequenceEditorMenu->setGraphic(t, getWidth() / 8 * 6, getHeight() / 10);
	//	}
	//
	//	gameSequenceEditorMenu->render();

	Main::gwenCanvas->RenderCanvas();


}


