/*
	WindowMessage.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef WINDOWMESSAGE_H
#define WINDOWMESSAGE_H

#include "Gui3d/WindowText.h"

//----------------------------------------------------------------------------

class WindowMessage : public WindowText
{
public:
	// Construction
						WindowMessage( NodeWindow * pParent, const char * pMessage );
	// NodeWindow interface
	void				onUpdate( float t );
	// Accessors
	float				time() const;
	// Mutator
	void				setTime( float t );
protected:
	// Data
	float				m_Time;
};

//----------------------------------------------------------------------------

inline float WindowMessage::time() const
{
	return m_Time;
}

inline void WindowMessage::setTime( float t )
{
	m_Time = t;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
