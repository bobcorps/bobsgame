#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




Logger KeyboardMenuPanel::log = Logger("KeyboardScreen");


KeyboardMenuPanel::KeyboardMenuPanel()
{ //=========================================================================================================================

	// MenuPanel();

	//
	//   keyboardPanel = ms<DialogLayout>();
	//   keyboardPanel->setTheme("keyboardPanel");
	//
	//
	//   sp<Label> controlsPanelLabel = ms<Label>("Controls");
	//   controlsPanelLabel->setCanAcceptKeyboardFocus(false);
	//   controlsPanelLabel->setTheme("bigLabel");
	//
	//
	//   sp<Widget> keyboardImagePanel = ms<WidgetAnonymousInnerClassHelper>(this);
	//   keyboardImagePanel->setTheme("keyboardImagePanel");
	//
	//
	//   sp<Label> pressF1Label = ms<Label>("Press F1 to show controls at any time.");
	//   pressF1Label->setCanAcceptKeyboardFocus(false);
	//   pressF1Label->setTheme("bigLabel");
	//
	//
	//   okButton = ms<Button>("OK");
	//   okButton->setCanAcceptKeyboardFocus(false);
	//   okButton->addCallback([&] ()
	//      {
	//         doOK();
	//      }
	//   );
	//   okButton->setVisible(false);
	//
	//
	//   keyboardPanel->setHorizontalGroup(keyboardPanel->createParallelGroup(keyboardPanel->createParallelGroup().addMinGap(400), keyboardPanel->createSequentialGroup(keyboardPanel->createParallelGroup().addMinGap(50), keyboardPanel->createParallelGroup(keyboardPanel->createSequentialGroup().addGap().addWidget(controlsPanelLabel).addGap(), keyboardPanel->createSequentialGroup().addGap().addWidget(keyboardImagePanel).addGap(), keyboardPanel->createSequentialGroup().addGap().addWidget(pressF1Label).addGap(), keyboardPanel->createSequentialGroup().addGap().addWidget(okButton).addGap()), keyboardPanel->createParallelGroup().addMinGap(50))));
	//
	//
	//   keyboardPanel->setVerticalGroup(keyboardPanel->createSequentialGroup(keyboardPanel->createSequentialGroup().addMinGap(20), keyboardPanel->createParallelGroup(controlsPanelLabel), keyboardPanel->createSequentialGroup().addMinGap(20), keyboardPanel->createParallelGroup(keyboardImagePanel), keyboardPanel->createSequentialGroup().addMinGap(20), keyboardPanel->createParallelGroup(pressF1Label), keyboardPanel->createParallelGroup(okButton), keyboardPanel->createSequentialGroup().addMinGap(50)));
	//
	//
	//   //---------------------------------------------------------
	//   //layout
	//   //---------------------------------------------------------
	//
	//   insideScrollPaneLayout->setHorizontalGroup(insideScrollPaneLayout->createParallelGroup(insideScrollPaneLayout->createSequentialGroup().addGap().addWidget(keyboardPanel).addGap()));
	//
	//   insideScrollPaneLayout->setVerticalGroup(insideScrollPaneLayout->createSequentialGroup(insideScrollPaneLayout->createSequentialGroup().addGap(), insideScrollPaneLayout->createParallelGroup().addWidget(keyboardPanel), insideScrollPaneLayout->createSequentialGroup().addGap()));
	//
	//
	//   //---------------------
	//   //scrollpane
	//   //----------------------
	//
	//   scrollPane = ms<ScrollPane>(insideScrollPaneLayout);
	//
	//   scrollPane->setTheme(GUIManager::scrollPaneTheme);
	//   scrollPane->setCanAcceptKeyboardFocus(false);
	//   scrollPane->setExpandContentSize(true);
	//
	//
	//   //---------------------
	//   //add scrollpane to outside panel
	//   //----------------------
	//
	//   //mainPanelLayout.add(scrollPane);
	//   mainPanelLayout->setTheme("darkMenuNoMainPanel");
	//
	//   mainPanelLayout->setCanAcceptKeyboardFocus(false);
	//   mainPanelLayout->setHorizontalGroup(mainPanelLayout->createParallelGroup(scrollPane));
	//
	//   mainPanelLayout->setVerticalGroup(mainPanelLayout->createSequentialGroup(scrollPane));
	//
	//
	//   add(mainPanelLayout);
	//
	//
	//   keyboardPanel->adjustSize();
	//
	//
	//   //setActivated(true);
	//
	//   //scrollPane.setTheme("darkPanel");
	//   //insideScrollPaneLayout.setTheme(GUIManager.emptyDialogLayoutTheme);
}

KeyboardMenuPanel::WidgetAnonymousInnerClassHelper::WidgetAnonymousInnerClassHelper(sp<KeyboardMenuPanel> outerInstance)
{
	this->outerInstance = outerInstance;
}

void KeyboardMenuPanel::WidgetAnonymousInnerClassHelper::layout()
{
	//force the keyboard image to fit inside the layout/scrollpane/layout/layout
	//
	//   outerInstance->mainPanelLayout->setBorderSize(0, 20);
	//   outerInstance->insideScrollPaneLayout->setBorderSize(0, 20);
	//   outerInstance->scrollPane->setBorderSize(0, 20);
	//
	//   int w = (int)(outerInstance->mainPanelLayout->getWidth() * 0.80f);
	//   if (w < 0)
	//   {
	//      w = 128;
	//   }
	//
	//   //setMinSize(w,50);
	//   setMaxSize(w, 390);
	//   //adjustSize();
}

//The following method was originally marked 'synchronized':
bool KeyboardMenuPanel::getClickedOK_S()
{
	return _clickedOK;
}

//The following method was originally marked 'synchronized':
void KeyboardMenuPanel::setClickedOK_S(bool b)
{
	_clickedOK = b;
}

void KeyboardMenuPanel::update()
{ //=========================================================================================================================
	if (isActivated == true)
	{
		if (isScrollingDown == false)
		{
			ticksSinceTurnedOff = 0;
			ticksSinceTurnedOn += 32;

			scrollUp();
		}
		else
		{
			if (isScrollingDown == true)
			{
				ticksSinceTurnedOn = 0;
				ticksSinceTurnedOff += 32;

				scrollDown();
			}
		}
	}
}

void KeyboardMenuPanel::onScrolledUp()
{ //=========================================================================================================================

	//getGUI().setTooltipDelay(1);
}

void KeyboardMenuPanel::scrollDown()
{ //=========================================================================================================================
	if (ticksSinceTurnedOff <= fadeOutTime)
	{
		screenY = (float)(Easing::easeOutCubic(ticksSinceTurnedOff, 0, GLUtils::getViewportHeight(), fadeOutTime));
		layout();
	}
	else
	{
		isActivated = false;
		isScrollingDown = false;
		//MenuPanel::setVisible(false);
	}
}

void KeyboardMenuPanel::layout()
{ //=========================================================================================================================

	//   //login panel is centered
	//
	//   keyboardPanel->adjustSize();
	//   keyboardPanel->setPosition(insideScrollPaneLayout->getInnerX() + (insideScrollPaneLayout->getInnerWidth() - keyboardPanel->getWidth()) / 2, insideScrollPaneLayout->getInnerY() + (insideScrollPaneLayout->getInnerHeight() - keyboardPanel->getHeight()) / 2);

	MenuPanel::layout();
}

void KeyboardMenuPanel::setButtonsVisible(bool b)
{ //=========================================================================================================================

	//okButton->setVisible(b);
}

void KeyboardMenuPanel::doOK()
{ //=========================================================================================================================
	//
	//   GUI* gui = getGUI();
	//   if (gui != nullptr)
	//   {
	//      //setButtonsVisible(false);
	//
	//      //create thread, this needs to be a thread because Button.doCallback(Runnable) only calls Runnable.run() which does NOT create a thread.
	//      ms<Thread>([&] ()
	//         {
	//            try
	//            {
	//               Thread::currentThread().setName("KeyboardScreen_clickOK");
	//            }
	//            catch (SecurityException e)
	//            {
	//               e->printStackTrace();
	//            }
	//
	//            setActivated(false);
	//
	//            while (getIsScrollingDown())
	//            {
	//               try
	//               {
	//                  delay(500);
	//               }
	//               catch (exception& e)
	//               {
	//               }
	//            }
	//
	//            setClickedOK_S(true);
	//         }
	//      )
	//      .
	//      start();
	//   }
}

void KeyboardMenuPanel::renderBefore()
{ //=========================================================================================================================


	if (getIsScrollingDown() == true)
	{
		return;
	}
	if (getIsActivated() == false)
	{
		return;
	}
	//additional rendering calls go here (after gui is drawn)


	//Main.glowTileBackground.render();
}

void KeyboardMenuPanel::render()
{ //=========================================================================================================================


	if (getIsScrollingDown() == true)
	{
		return;
	}
	if (getIsActivated() == false)
	{
		return;
	}

	//additional rendering calls go here (after gui is drawn)
}

