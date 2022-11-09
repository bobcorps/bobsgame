#include "stdafx.h"

//------------------------------------------------------------------------------
//Copyright Robert Pelloni.
//All Rights Reserved.
//------------------------------------------------------------------------------


//#pragma once




Logger CreateNewAccountState::log = Logger("CreateNewAccountState");


CreateNewAccountState::CreateNewAccountState()
{ //=========================================================================================================================


	createNewAccountMenuPanel = ms<CreateNewAccountMenuPanel>();

	//createNewAccountGUI = new GUI(createNewAccount, GLUtils::TWLrenderer);
	//createNewAccountGUI->applyTheme(GLUtils::TWLthemeManager);
}

void CreateNewAccountState::update()
{ //=========================================================================================================================

	createNewAccountMenuPanel->update();
}

void CreateNewAccountState::render()
{ //=========================================================================================================================
	{
		//SlickCallable.leaveSafeBlock();//weird slick texture errors if i dont do this
		createNewAccountMenuPanel->renderBefore();
		//createNewAccountGUI->update();
		createNewAccountMenuPanel->render();
	}
	//SlickCallable.enterSafeBlock();
}

void CreateNewAccountState::cleanup()
{ //=========================================================================================================================
	//createNewAccountGUI->destroy();
}








Logger CreateNewAccountMenuPanel::log = Logger("CreateNewAccount");


CreateNewAccountMenuPanel::CreateNewAccountMenuPanel()
{ //=========================================================================================================================

  //MenuPanel();

  //
  //   panel = ms<DialogLayout>();
  //   panel->setTheme("darkPanel");
  //
  //
  //   sp<Label> createAccountPanelLabel = ms<Label>("Create Account");
  //   createAccountPanelLabel->setCanAcceptKeyboardFocus(false);
  //   createAccountPanelLabel->setTheme("bigLabel");
  //
  //
  //   errorLabel = ms<Label>(" ");
  //   errorLabel->setTheme("errorLabel");
  //   errorLabel->setCanAcceptKeyboardFocus(false);
  //
  //   statusLabel = ms<Label>(" ");
  //   statusLabel->setCanAcceptKeyboardFocus(false);
  //
  //   //---------------------------------------------------------
  //   //username
  //   //---------------------------------------------------------
  //   //		nameEditField = ms<EditField>();
  //   //		nameEditField.setText("");
  //   //		nameEditField.setMaxTextLength(40);
  //   //		nameEditField.addCallback(ms<Callback>()
  //   //		{
  //   //			@Override
  //   //			public void callback(int key)
  //   //			{
  //   //				//username cannot contain `
  //   //			}
  //   //		});
  //   //		Label nameLabel = ms<Label>("Username");
  //   //		nameLabel.setLabelFor(nameEditField);
  //   //		nameLabel.setCanAcceptKeyboardFocus(false);
  //
  //
  //   //---------------------------------------------------------
  //   //email address
  //   //---------------------------------------------------------
  //   emailEditField = ms<EditField>();
  //   emailEditField->setText("");
  //   emailEditField->setMaxTextLength(40);
  //   emailEditField->addCallback(ms<CallbackAnonymousInnerClassHelper>(this));
  //   emailLabel = ms<Label>("Email Address:");
  //   emailLabel->setLabelFor(emailEditField);
  //   emailLabel->setCanAcceptKeyboardFocus(false);
  //
  //
  //   //---------------------------------------------------------
  //   //password
  //   //---------------------------------------------------------
  //   passwordEditField = ms<EditField>();
  //   passwordEditField->setPasswordMasking(true);
  //   passwordLabel = ms<Label>("Password:");
  //   passwordLabel->setLabelFor(passwordEditField);
  //
  //
  //   passwordEditField->addCallback(ms<CallbackAnonymousInnerClassHelper>2(this));
  //
  //
  //   //---------------------------------------------------------
  //   //confirm password
  //   //---------------------------------------------------------
  //   confirmPasswordEditField = ms<EditField>();
  //   confirmPasswordEditField->setPasswordMasking(true);
  //   confirmPasswordLabel = ms<Label>("Confirm Password:");
  //   confirmPasswordLabel->setLabelFor(confirmPasswordEditField);
  //
  //
  //   confirmPasswordEditField->addCallback(ms<CallbackAnonymousInnerClassHelper>3(this));
  //
  //
  //   //---------------------------------------------------------
  //   //email box
  //   //---------------------------------------------------------
  //
  //   emailDialogLayout = ms<DialogLayout>();
  //   emailDialogLayout->setCanAcceptKeyboardFocus(false);
  //
  //   emailDialogLayout->setHorizontalGroup(emailDialogLayout->createSequentialGroup(emailDialogLayout->createParallelGroup(emailDialogLayout->createSequentialGroup().addGap().addWidgets(emailLabel).addGap()), emailDialogLayout->createParallelGroup(emailDialogLayout->createSequentialGroup().addGap().addWidgets(emailEditField).addGap())));
  //   //usernameDialogLayout.createSequentialGroup().addGap().addWidgets(nameLabel).addGap(),
  //   //usernameDialogLayout.createSequentialGroup().addGap().addWidgets(nameEditField).addGap(),
  //
  //   emailDialogLayout->setVerticalGroup(emailDialogLayout->createSequentialGroup(emailDialogLayout->createParallelGroup().addWidgets(emailLabel, emailEditField)));
  //   //usernameDialogLayout.createParallelGroup().addWidgets(nameLabel, nameEditField),
  //
  //
  //   //---------------------------------------------------------
  //   //password box
  //   //---------------------------------------------------------
  //
  //   passwordDialogLayout = ms<DialogLayout>();
  //   passwordDialogLayout->setCanAcceptKeyboardFocus(false);
  //
  //   passwordDialogLayout->setHorizontalGroup(passwordDialogLayout->createSequentialGroup(passwordDialogLayout->createParallelGroup(passwordDialogLayout->createSequentialGroup().addGap(), passwordDialogLayout->createParallelGroup().addWidgets(passwordLabel, confirmPasswordLabel), passwordDialogLayout->createSequentialGroup().addGap()), passwordDialogLayout->createParallelGroup(passwordDialogLayout->createSequentialGroup().addGap().addWidgets(passwordEditField).addGap(), passwordDialogLayout->createSequentialGroup().addGap().addWidgets(confirmPasswordEditField).addGap())));
  //
  //   passwordDialogLayout->setVerticalGroup(passwordDialogLayout->createSequentialGroup(passwordDialogLayout->createParallelGroup().addWidgets(passwordLabel, passwordEditField), passwordDialogLayout->createParallelGroup().addWidgets(confirmPasswordLabel, confirmPasswordEditField)));
  //
  //
  //   //---------------------------------------------------------
  //   //ok button
  //   //---------------------------------------------------------
  //   okButton = ms<Button>("Ok!");
  //   okButton->setCanAcceptKeyboardFocus(false);
  //   okButton->setTheme("button");
  //   okButton->addCallback([&] ()
  //      {
  //         doCreateAccount();
  //      }
  //   );
  //
  //   //---------------------------------------------------------
  //   //cancel button
  //   //---------------------------------------------------------
  //   cancelButton = ms<Button>("Cancel");
  //   cancelButton->setCanAcceptKeyboardFocus(false);
  //   cancelButton->setTheme("button");
  //   cancelButton->addCallback([&] ()
  //      {
  //         doCancel();
  //      }
  //   );
  //
  //
  //   //---------------------------------------------------------
  //   //layout
  //   //---------------------------------------------------------
  //
  //   panel->setHorizontalGroup(panel->createParallelGroup(panel->createParallelGroup().addMinGap(360), panel->createSequentialGroup().addGap().addWidgets(createAccountPanelLabel).addGap(), panel->createSequentialGroup().addGap().addWidgets(errorLabel).addGap(), panel->createSequentialGroup().addGap().addWidgets(statusLabel).addGap(), panel->createParallelGroup(panel->createSequentialGroup().addGap(), panel->createSequentialGroup().addWidgets(emailDialogLayout), panel->createSequentialGroup().addWidgets(passwordDialogLayout), panel->createSequentialGroup().addGap()), panel->createSequentialGroup().addGap().addWidgets(okButton).addGap(), panel->createSequentialGroup().addGap().addWidgets(cancelButton).addGap()));
  //   //panel.createSequentialGroup().addGap().addWidgets(socialDialogLayout).addGap(),
  //
  //
  //   panel->setVerticalGroup(panel->createSequentialGroup(panel->createSequentialGroup().addWidgets(createAccountPanelLabel).addGap(), panel->createSequentialGroup().addWidgets(errorLabel), panel->createSequentialGroup().addWidgets(statusLabel), panel->createSequentialGroup().addWidgets(emailDialogLayout), panel->createSequentialGroup().addWidgets(passwordDialogLayout), panel->createSequentialGroup().addWidgets(okButton), panel->createSequentialGroup().addWidgets(cancelButton)));
  //   //panel.createSequentialGroup().addGap(),
  //   //panel.createSequentialGroup().addWidgets(socialDialogLayout),
  //   //,panel.createSequentialGroup().addGap()
  //
  //
  //   //---------------------------------------------------------
  //   //layout
  //   //---------------------------------------------------------
  //
  //   insideScrollPaneLayout->setHorizontalGroup(insideScrollPaneLayout->createParallelGroup(insideScrollPaneLayout->createSequentialGroup().addGap().addWidget(panel).addGap()));
  //
  //   insideScrollPaneLayout->setVerticalGroup(insideScrollPaneLayout->createSequentialGroup(insideScrollPaneLayout->createSequentialGroup().addGap(), insideScrollPaneLayout->createParallelGroup().addWidget(panel), insideScrollPaneLayout->createSequentialGroup().addGap()));
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
  //   //scrollPane.setExpandContentSize(false);
  //
  //
  //   //---------------------
  //   //add scrollpane to outside panel
  //   //----------------------
  //
  //   //mainPanelLayout.add(scrollPane);
  //
  //   mainPanelLayout->setCanAcceptKeyboardFocus(false);
  //   mainPanelLayout->setHorizontalGroup(mainPanelLayout->createParallelGroup(scrollPane));
  //
  //   mainPanelLayout->setVerticalGroup(mainPanelLayout->createSequentialGroup(scrollPane));
  //
  //
  //   add(mainPanelLayout);
  //

	setActivated(true);
}

//
//CreateNewAccount::CallbackAnonymousInnerClassHelper::CallbackAnonymousInnerClassHelper(sp<CreateNewAccount> outerInstance)
//{
//   this->outerInstance = outerInstance;
//}
//
//void CreateNewAccount::CallbackAnonymousInnerClassHelper::callback(int key)
//{
//}
//
//CreateNewAccount::CallbackAnonymousInnerClassHelper2::CallbackAnonymousInnerClassHelper2(sp<CreateNewAccount> outerInstance)
//{
//   this->outerInstance = outerInstance;
//}
//
//void CreateNewAccount::CallbackAnonymousInnerClassHelper2::callback(int key)
//{
//   if (key == Event::KEY_RETURN)
//   {
//      outerInstance->doCreateAccount();
//   }
//   else
//   {
//      if (outerInstance->passwordEditField->getTextLength() > 0)
//      {
//      }
//      else
//      {
//         if (outerInstance->passwordEditField->getTextLength() == 0)
//         {
//         }
//      }
//   }
//}
//
//CreateNewAccount::CallbackAnonymousInnerClassHelper3::CallbackAnonymousInnerClassHelper3(sp<CreateNewAccount> outerInstance)
//{
//   this->outerInstance = outerInstance;
//}
//
//void CreateNewAccount::CallbackAnonymousInnerClassHelper3::callback(int key)
//{
//   if (key == Event::KEY_RETURN)
//   {
//      outerInstance->doCreateAccount();
//   }
//   else
//   {
//      if (outerInstance->confirmPasswordEditField->getTextLength() > 0)
//      {
//      }
//      else
//      {
//         if (outerInstance->confirmPasswordEditField->getTextLength() == 0)
//         {
//         }
//      }
//   }
//}

void CreateNewAccountMenuPanel::init()
{ //=========================================================================================================================
}

void CreateNewAccountMenuPanel::setButtonsVisible(bool b)
{ //=========================================================================================================================
  //
  //   emailDialogLayout->setVisible(b);
  //   passwordDialogLayout->setVisible(b);
  //   okButton->setVisible(b);
  //
  //   if (Main::introMode == false)
  //   {
  //      cancelButton->setVisible(b);
  //   }
}

void CreateNewAccountMenuPanel::onScrolledUp()
{ //=========================================================================================================================

  //emailEditField->requestKeyboardFocus();
}

void CreateNewAccountMenuPanel::doCancel()
{ //=========================================================================================================================

	setActivated(false);
	setButtonsVisible(true);
	//   statusLabel->setText(" ");
	//   errorLabel->setText(" ");
}

void CreateNewAccountMenuPanel::doCreateAccount()
{ //=========================================================================================================================
  //
  //   GUI* gui = getGUI();
  //   if (gui != nullptr)
  //   {
  //      setButtonsVisible(false);
  //
  //
  //      //create thread, this needs to be a thread because Button.doCallback only creates a Runnable and calls Runnable.run() which does NOT create a thread
  //      ms<Thread>([&] ()
  //         {
  //            try
  //            {
  //               Thread::currentThread().setName("CreateNewAccount_doCreateAccount");
  //            }
  //            catch (SecurityException e)
  //            {
  //               e->printStackTrace();
  //            }
  //
  //            statusLabel->setText(" ");
  //            errorLabel->setText(" ");
  //
  //            if (emailEditField->getText()->contains("`"))
  //            {
  //               errorLabel->setText("Email must not contain `");
  //               setButtonsVisible(true);
  //               return;
  //            }
  //            if (emailEditField->getText().length() == 0)
  //            {
  //               errorLabel->setText("Enter your email address.");
  //               setButtonsVisible(true);
  //               return;
  //            }
  //            if (emailEditField->getText()->contains("@") == false)
  //            {
  //               errorLabel->setText("Email address must contain @");
  //               setButtonsVisible(true);
  //               return;
  //            }
  //
  //            if (passwordEditField->getText()->contains("`"))
  //            {
  //               errorLabel->setText("Password must not contain `");
  //               setButtonsVisible(true);
  //               return;
  //            }
  //            if (passwordEditField->getText().length() == 0)
  //            {
  //               errorLabel->setText("Please enter a password.");
  //               setButtonsVisible(true);
  //               return;
  //            }
  //            if (passwordEditField->getText().equals(confirmPasswordEditField->getText()) == false)
  //            {
  //               errorLabel->setText("Passwords do not match.");
  //               setButtonsVisible(true);
  //               return;
  //            }
  //
  //
  //            //check if email address is valid
  //            //may not contain `
  //            //make sure passwords match
  //
  //            //say "trying to connect to server"
  //
  //            errorLabel->setText(" ");
  //            statusLabel->setText("Connecting to server...");
  //
  //            getServerConnection()->connectToServer();
  //
  //            //-------------------------------
  //            //check to see if connected every 1 second
  //            //when connected, proceed.
  //            //-------------------------------
  //            int tries = 0;
  //            bool connected = false;
  //            while (connected == false)
  //            {
  //               connected = getServerConnection()->getConnectedToServer_S();
  //
  //               if (connected == false)
  //               {
  //                  tries++;
  //
  //                  //make dots cycle
  //                  string dots = "";
  //                  for (int i = 0; i < tries % 4; i++)
  //                  {
  //                     dots = dots.concat(".");
  //                  }
  //
  //                  errorLabel->setText(" ");
  //                  statusLabel->setText("Connecting to server" + dots);
  //
  //                  if (tries > 10)
  //                  {
  //                     tries = 0;
  //                     statusLabel->setText(" ");
  //                     errorLabel->setText("Error: Could not connect to server.");
  //
  //                     setButtonsVisible(true);
  //
  //                     return;
  //                  }
  //
  //                  try
  //                  {
  //                     delay(1000);
  //                  }
  //                  catch (exception& e)
  //                  {
  //                     e.printStackTrace();
  //                  }
  //               }
  //            }
  //
  //            if (connected == true)
  //            {
  //               errorLabel->setText(" ");
  //               statusLabel->setText("Connected! Creating Account...");
  //            }
  //
  //            getServerConnection()->sendCreateAccountRequest(emailEditField->getText(), passwordEditField->getText());
  //
  //            //-------------------------------
  //            //check to see if response every 1 second
  //            //-------------------------------
  //            int createAccountTries = 0;
  //            bool gotResponse = false;
  //            while (gotResponse == false)
  //            {
  //               gotResponse = getServerConnection()->getGotCreateAccountResponse_S();
  //
  //               if (gotResponse == false)
  //               {
  //                  createAccountTries++;
  //                  if (createAccountTries > 10)
  //                  {
  //                     createAccountTries = 0;
  //                     statusLabel->setText(" ");
  //                     errorLabel->setText("Error: Timed out creating account. The servers may be overloaded, please wait a few minutes and try again.");
  //
  //                     setButtonsVisible(true);
  //
  //                     return;
  //                  }
  //
  //                  try
  //                  {
  //                     delay(1000);
  //                  }
  //                  catch (exception& e)
  //                  {
  //                     e.printStackTrace();
  //                  }
  //               }
  //            }
  //
  //            errorLabel->setText(" ");
  //            statusLabel->setText("Your account was created! Please check your email and confirm your account.");
  //
  //            try
  //            {
  //               delay(3000);
  //            }
  //            catch (exception& e)
  //            {
  //               e.printStackTrace();
  //            }
  //
  //
  //            //setEnabled(false);
  //            setActivated(false);
  //            setButtonsVisible(true);
  //            //statusLabel.setText(" ");
  //            //errorLabel.setText(" ");
  //         }
  //      )
  //      .
  //      start();
  //   }
}

void CreateNewAccountMenuPanel::layout()
{ //=========================================================================================================================


  //Login panel is centered
  //
  //   panel->adjustSize();
  //
  //   panel->setPosition(insideScrollPaneLayout->getInnerX() + (insideScrollPaneLayout->getInnerWidth() - panel->getWidth()) / 2, insideScrollPaneLayout->getInnerY() + (insideScrollPaneLayout->getInnerHeight() - panel->getHeight()) / 2);


	MenuPanel::layout();
}

void CreateNewAccountMenuPanel::update()
{ //=========================================================================================================================

	MenuPanel::update();


	Main::glowTileBackgroundMenuPanel->update();
	//
	//
	//   if (Main::introMode)
	//   {
	//      cancelButton->setVisible(false);
	//      cancelButton->setEnabled(false);
	//   }
	//
	//
	//   if (getIsActivated() == false)
	//   {
	//      if (getIsScrollingDown() == false)
	//      {
	//         if (Main::introMode)
	//         {
	//            Main::mainObject->stateManager->setState(Main::mainObject->youWillBeNotifiedState);
	//         }
	//         else
	//         {
	//            Main::mainObject->stateManager->setState(Main::mainObject->loginState);
	//         }
	//      }
	//   }
}

void CreateNewAccountMenuPanel::setActivated(bool b)
{ //=========================================================================================================================

	if (Main::introMode == true && b == false)
	{
		return;
	}
	MenuPanel::setActivated(b);
}

void CreateNewAccountMenuPanel::renderBefore()
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


	Main::glowTileBackgroundMenuPanel->render();
}

void CreateNewAccountMenuPanel::render()
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

