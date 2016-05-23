/*
	TipHelp.h
	(c)2008 Palestar, Jeff Everett

	Tip based help system
*/

#ifndef TIPHELP_H
#define TIPHELP_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"
#include "Interface/GameDocument.h"




// ok load up all the tips
// mark each tip for player as not viewed
// as a tip is viewed, mark it as viewed so we dont display it again
// if tips have been turned off, dont show them at that point either

// system design:
//	Tips need to be loaded up
//	each system that has a tip to be displayed, displays it in the viewgame as a potential tip
//		

// list of all ids used for tips
#define TIP_ERROR		0
#define TIP_WELCOME		1
#define TIP_ORBIT		2
#define TIP_FOLLOW		3
#define TIP_JUMP		4
#define TIP_ENHANCEMENT 5

#define TIP_LAST		5


// data for each tip
class TipHelpData 
{
public:
		TipHelpData();
		TipHelpData(unsigned int a_id, CharString a_title, CharString a_tip);
		
		CharString GetTip() const;
		CharString GetTipTitle()const ;
		unsigned int GetID() const;

private:
	CharString m_tip;
	CharString m_title;
	unsigned int m_id;

};


// basically a static container for all tips

class TipHelpSystem
{
	public:
		TipHelpSystem();

		static void					initialize();
		static TipHelpData *		GetTip(unsigned int a_id);
		static unsigned int			GetIDByTitle(CharString a_title);
		static TipHelpSystem *		system() {return s_pTipSystem;}
	private:

	private:
		// Static Data
		static Array<TipHelpData *> s_tipList;
		static TipHelpSystem *		s_pTipSystem;
		
};



//-----
inline CharString TipHelpData::GetTip() const 
{ 
	return m_tip;
}

inline CharString TipHelpData::GetTipTitle()const  
{
	return m_title;
}

inline unsigned int TipHelpData::GetID() const 
{
	return m_id;
}


extern TipHelpSystem s_TipHelpSystem;

#endif