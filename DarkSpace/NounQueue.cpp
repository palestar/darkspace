/*
	NounQueue.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/

#include "NounQueue.h"

#if 0

//----------------------------------------------------------------------------

NounQueue::NounQueue() : m_nBytes( 0 )
{}

bool NounQueue::pop( QueuedNoun & noun )
{
	m_NounQueue.reset();
	if ( m_NounQueue.valid() )
	{
		noun = *(*m_NounQueue);
		m_nBytes -= noun.instance.size();

		m_NounQueue.pop();
		m_Nouns.remove( noun.nNoun );

		return true;
	}

	return false;
}

void NounQueue::push( const WidgetKey & nParent, const WidgetKey & nNoun, const WidgetWrap & instance, int nPriority )
{
	// make sure the noun isn't already in the queue
	if ( inQueue( nNoun ) )
		remove( nNoun );

	// queue the noun sorted by priority, lower prority levels are at the top
	m_NounQueue.reset();
	while( m_NounQueue.valid() && (*(*m_NounQueue)).nPriority <= nPriority)
		m_NounQueue.next();

	if ( m_NounQueue.valid() )
		m_NounQueue.insert( m_Nouns.insert( nNoun, QueuedNoun( nParent, nNoun, instance, nPriority )) );
	else
		m_NounQueue.push( m_Nouns.insert( nNoun, QueuedNoun( nParent, nNoun, instance, nPriority )) );

	m_nBytes += instance.size();
}

bool NounQueue::pushVerb( const WidgetKey & nNoun, const WidgetWrap & verb, int nPriority )
{
	NounHash::Iterator i = m_Nouns.find( nNoun );
	if (! i.valid() )
		return false;

	(*i).VerbQueue.push( nNoun, verb, nPriority );
	return true;
}

void NounQueue::remove( const WidgetKey & nNoun )
{
	NounHash::Iterator i = m_Nouns.find( nNoun );
	if ( i.valid() )
	{
		m_nBytes -= (*i).instance.size();

		// linear search of the queue to remove this item, no other way..
		m_NounQueue.reset();
		while( m_NounQueue.valid() )
		{
			if ( (*m_NounQueue) == i )
			{
				m_NounQueue.pop();
				break;
			}
			m_NounQueue.next();
		}
		m_Nouns.remove( nNoun );
	}
}

#endif

//----------------------------------------------------------------------------
//EOF
