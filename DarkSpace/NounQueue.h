/*
	NounQueue.h

	This object is used to store queued nouns sorted by priority for a client
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef NOUNQUEUE_H
#define NOUNQUEUE_H

#include "VerbQueue.h"

#if 0
//----------------------------------------------------------------------------

class NounQueue 
{
public:
	// Types
	struct QueuedNoun
	{
		QueuedNoun();
		QueuedNoun( const QueuedNoun & copy );
		QueuedNoun( const WidgetKey & nParent, const WidgetKey & nNoun, const WidgetWrap & instance, int nPriority );
		
		WidgetKey				nParent;
		WidgetKey				nNoun;
		WidgetWrap				instance;
		int						nPriority;

		VerbQueue				VerbQueue;			
		
		QueuedNoun &			operator=( const QueuedNoun & copy );
	};

	// Construction
	NounQueue();

	// Accessors
	bool			inQueue( const WidgetKey & nNoun ) const;			// is noun in queue
	int				bytes() const;										// number of bytes in queue

	// Mutators
	bool			pop( QueuedNoun & noun );							// removes the top-most noun from the queue
	void			push( const WidgetKey & nParent,					// pushes a noun into the queue, sorted by priority
						const WidgetKey & nNoun,
						const WidgetWrap & instance, 
						int nPriority );
	bool			pushVerb( const WidgetKey & nNoun,					// push a verb into the queue, returns false if noun is not in this queue
						const WidgetWrap & verb, 
						int nPriority );
	void			remove( const WidgetKey & nNoun );					// removes the specified noun from the queue

private:
	// Types
	typedef Hash< WidgetKey, QueuedNoun >		NounHash;
	typedef Queue< NounHash::Iterator >			NounItQueue;

	int				m_nBytes;
	NounHash		m_Nouns;
	NounItQueue		m_NounQueue; 
};

//----------------------------------------------------------------------------

inline bool NounQueue::inQueue( const WidgetKey & nNoun ) const
{
	return m_Nouns.find( nNoun ).valid();
}

inline int NounQueue::bytes() const
{
	return m_nBytes;
}

//----------------------------------------------------------------------------

inline NounQueue::QueuedNoun::QueuedNoun() : nPriority( 0 )
{}

inline NounQueue::QueuedNoun::QueuedNoun( const QueuedNoun & copy ) 
	: nParent( copy.nParent ), nNoun( copy.nNoun ), instance( copy.instance ), nPriority( copy.nPriority ), VerbQueue( copy.VerbQueue )
{}

inline NounQueue::QueuedNoun::QueuedNoun( const WidgetKey & _nParent, const WidgetKey & _nNoun, const WidgetWrap & _instance, int _nPriority )
	: nParent( _nParent ), nNoun( _nNoun ), instance( _instance ), nPriority( _nPriority )
{}

inline NounQueue::QueuedNoun & NounQueue::QueuedNoun::operator=( const QueuedNoun & copy )
{
	nParent = copy.nParent;
	nNoun = copy.nNoun;
	instance = copy.instance;
	nPriority = copy.nPriority;
	VerbQueue = copy.VerbQueue;
	return *this;
}

//----------------------------------------------------------------------------

#endif

#endif

//----------------------------------------------------------------------------
//EOF
