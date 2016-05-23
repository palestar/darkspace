/*
	VerbQueue.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/

#include "VerbQueue.h"

#if 0

//----------------------------------------------------------------------------

VerbQueue::VerbQueue() : m_nBytes( 0 )
{}

bool VerbQueue::pop( QueuedVerb & verb )
{
	m_Verbs.reset();
	if ( m_Verbs.valid() )
	{
		verb = *m_Verbs;
		m_Verbs.pop();

		m_nBytes -= verb.verb.size();
		return true;
	}

	return false;
}

void VerbQueue::push( const WidgetKey & nNoun, const WidgetWrap & verb, int nPriority )
{
	// queue the verb sorted by priority, lower prority levels are at the top
	m_Verbs.reset();
	while( m_Verbs.valid() && (*m_Verbs).nPriority <= nPriority)
		m_Verbs.next();

	if ( m_Verbs.valid() )
		m_Verbs.insert( QueuedVerb( nNoun, verb, nPriority ) );
	else
		m_Verbs.push( QueuedVerb( nNoun, verb, nPriority ) );

	m_nBytes += verb.size();
}

void VerbQueue::remove( const WidgetKey & nNoun )
{
	m_Verbs.reset();
	while( m_Verbs.valid() )
	{
		if ( (*m_Verbs).nNoun == nNoun )
		{
			m_nBytes -= (*m_Verbs).verb.size();
			m_Verbs.pop();
		}
		else
			m_Verbs.next();
	}
}

VerbQueue & VerbQueue::operator=( const VerbQueue & copy )
{
	m_Verbs = copy.m_Verbs;
	return *this;
}

#endif

//----------------------------------------------------------------------------
//EOF
