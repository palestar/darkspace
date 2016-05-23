/*
	TipHelp.cpp
	(c)2008 Palestar, Jeff Everett

	the data for each help tip entry

	Thinking this might want to be a UI of its own that 
	maintains everything, but we will see

*/

#include "Debug/Assert.h"
#include "File/FileDisk.h"
#include "Standard/Settings.h"
#include "Standard/StringBuffer.h"
#include "System/Keyboard.h"
#include "Interface/ViewGame.h"
#include "Interface/TipHelp.h"

TipHelpSystem *		TipHelpSystem::s_pTipSystem = NULL;
Array<TipHelpData *> TipHelpSystem::s_tipList;

TipHelpData::TipHelpData() :
	m_id(0)
{}


TipHelpData::TipHelpData(unsigned int a_id, CharString a_title, CharString a_tip) :
	m_id(a_id),
	m_title(a_title),
	m_tip(a_tip)
{}



TipHelpSystem::TipHelpSystem()
{
	s_pTipSystem = this;
	TipHelpSystem::initialize();
}

void TipHelpSystem::initialize()
{

	TipHelpData *tip;
	// for now just have all tips hard coded..once this system is all in place, I will add them to the LocalizedString 
	// TODO: add to LocalizedString
	tip = new TipHelpData(TIP_ERROR,"ErrorTip","ErrorTip.");
	s_tipList.push(tip);

	tip = new TipHelpData(TIP_WELCOME,"Welcome to Darkspace","Welcome to <color;ff00ffff>Darkspace</color>.  These popup text hints will help you learn the finer points of playing <color;ff00ffff>Darkspace</color>.\n\nWe hope you enjoy the game!");
	s_tipList.push(tip);

	tip = new TipHelpData(TIP_ORBIT,"Orbiting a Planet","You may select the Orbit Planet button located in the bottom center of the screen or hitting the 'O' key.  This will have your ship attempt to locate the best possible route, including using jump drive and gates, to go the the targeted planet.\n\nOnce at the planet, the ship will enter into orbit.  This option is available anytime you have selected a planet that can be Orbited by your ship.");
	s_tipList.push(tip);

	tip = new TipHelpData(TIP_FOLLOW,"Following another ship","Tip data on following another ship");
	s_tipList.push(tip);

	tip = new TipHelpData(TIP_JUMP,"Jump Drives","Tip data about jumping.");
	s_tipList.push(tip);

	tip = new TipHelpData(TIP_ENHANCEMENT,"Enhancements","You have just picked up an <color;ff00ffff>Enhancement</color>.  Select the Enhancement in your cargo bay and then click the 'Use' button near the target information pane, or press 'A' to apply it to your ship.  The <color;ff3344ff>F7</color> button will bring up the Enhancement window, allowing you to view your Enhancements as well as remove unwanted ones.");
	s_tipList.push(tip);
}

// get the tip based on the ID
TipHelpData *TipHelpSystem::GetTip(unsigned int a_id)
{
	// if we dont havea tip system, create one
	if(TipHelpSystem::system() == NULL)
		new TipHelpSystem();

	for(int i=0;i<s_tipList.size();i++)
	{
		TipHelpData *pTip = s_tipList[i];
		if(pTip->GetID() == a_id)
			return pTip;
	}
	return NULL;
}

// find the tip id based on the title
unsigned int TipHelpSystem::GetIDByTitle(CharString a_title)
{
	// if we dont havea tip system, create one
	if(TipHelpSystem::system() == NULL)
		new TipHelpSystem();

	for(int i=0;i<s_tipList.size();i++)
	{
		TipHelpData *pTip = s_tipList[i];
		if(pTip->GetTipTitle() == a_title)
			return pTip->GetID();
	}
	return 0;
}


