#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	ref class LDBCollection;
	ref class LDBObject;

	public ref class LDBCursor sealed
	{
	public:
		int64 Count();
		LDBCursor ^Limit(int limit);
		LDBCursor ^Sort(LDBObject ^sort);
		bool HasNext();
		LDBObject ^Next();
		LDBObject ^One();

	internal:
		LDBCursor(LDBCollection ^coll, LDBObject ^query, LDBObject ^keys);
		LDBCursor(CLowlaDBCursor::ptr pcursor);

	private:
		CLowlaDBCursor::ptr m_pcursor;
		bool m_readStarted;
		CLowlaDBBson::ptr m_nextRecord;
	};
}
