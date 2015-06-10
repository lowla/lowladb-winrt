#include "pch.h"
#include "LDBCursor.h"

#include "LDBCollection.h"
#include "LDBObject.h"

using namespace LowlaDB;

LDBCursor::LDBCursor(LDBCollection ^coll, LDBObject ^query, LDBObject ^keys)
{
	const char *bson = nullptr;
	if (query) {
		bson = query->AsBson();
	}
	m_pcursor = CLowlaDBCursor::create(coll->GetPcoll(), bson);
}

LDBCursor::LDBCursor(CLowlaDBCursor::ptr pcursor) : m_pcursor(pcursor)
{
}

int64 LDBCursor::Count()
{
	return m_pcursor->count();
}

LDBCursor ^LDBCursor::Limit(int limit)
{
	return ref new LDBCursor(m_pcursor->limit(limit));
}

LDBCursor ^LDBCursor::Sort(LDBObject ^sort)
{
	return ref new LDBCursor(m_pcursor->sort(sort->AsBson()));
}

bool LDBCursor::HasNext()
{
	if (!m_readStarted) {
		m_nextRecord = m_pcursor->next();
		m_readStarted = true;
	}
	return !!m_nextRecord;
}

LDBObject ^LDBCursor::Next()
{
	if (HasNext()) {
		LDBObject ^answer = ref new LDBObject(m_nextRecord);
		m_nextRecord = m_pcursor->next();
		return answer;
	}
	return nullptr;
}

LDBObject ^LDBCursor::One()
{
	return Limit(1)->Next();
}