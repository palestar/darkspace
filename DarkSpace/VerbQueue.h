/*
	VerbQueue.h

	This object is used to store queued verbs sorted by priority for a client
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef VERBQUEUE_H
#define VERBQUEUE_H

#include "Standard/Queue.h"
#include "Factory/WidgetWrap.h"

#if 0
//----------------------------------------------------------------------------

class VerbQueue
{
public:
	// Types
	struct QueuedVerb
	{
		QueuedVerb();
		QueuedVerb( const QueuedVerb & copy );
		QueuedVerb( const WidgetKey & nNoun, const WidgetWrap & verb, int nPriority );

		WidgetKey				nNoun;
		WidgetWrap				verb;
		int						nPriority;

		QueuedVerb &			operator=( const QueuedVerb & copy );
	};

	// Construction
	VerbQueue();

	// Accessors
	int				bytes() const;

	// Mutators
	bool			pop( QueuedVerb & verb );							// returns true if verb was poppped, false if none available
	void			push( const WidgetKey & nNoun,						// push a verb into the queue
						const WidgetWrap & verb, 
						int nPriority );
	void			remove( const WidgetKey & nNoun );					// removes all verbs for the specified noun

	VerbQueue &		operator=( const VerbQueue & copy );

private:
	Queue< QueuedVerb >			m_Verbs;
	int							m_nBytes;
};

//----------------------------------------------------------------------------

inline int VerbQueue::bytes() const
{
	return m_nBytes;
}

//----------------------------------------------------------------------------

inline VerbQueue::QueuedVerb::QueuedVerb() : nPriority( 0 )
{}

inline VerbQueue::QueuedVerb::QueuedVerb( const QueuedVerb & copy ) 
	: nNoun( copy.nNoun ), verb( copy.verb ), nPriority( copy.nPriority )
{}

inline VerbQueue::QueuedVerb::QueuedVerb( const WidgetKey & _nNoun, const WidgetWrap & _verb, int _nPriority )
	: nNoun( _nNoun ), verb( _verb ), nPriority( _nPriority )
{}

inline VerbQueue::QueuedVerb & VerbQueue::QueuedVerb::operator=( const QueuedVerb & copy )
{
	nNoun = copy.nNoun;
	verb = copy.verb;
	nPriority = copy.nPriority;
	return *this;
}

//----------------------------------------------------------------------------

#endif

#endif

//----------------------------------------------------------------------------
//EOF
