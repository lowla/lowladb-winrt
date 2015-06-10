#pragma once

#include <lowladb.h>

#include "LDBObject.h"

namespace LowlaDB
{
	ref class LDBCursor;
	ref class LDBDb;
	ref class LDBWriteResult;

	public ref class LDBCollection sealed
	{
	public:
		LDBDb ^GetDatabase();
		Platform::String ^GetName();
		LDBWriteResult ^Insert(LDBObject ^object);
		LDBWriteResult ^InsertArray(const Platform::Array<LDBObject ^> ^arr);
		LDBCursor ^Find();
		LDBCursor ^Find(LDBObject ^query);
		LDBObject ^FindOne();
		LDBObject ^FindOne(LDBObject ^query);
		LDBWriteResult ^Remove(LDBObject ^query);
		LDBWriteResult ^Update(LDBObject ^query, LDBObject ^object);
		LDBWriteResult ^Update(LDBObject ^query, LDBObject ^object, bool upsert, bool multi);

	internal:
		LDBCollection(LDBDb ^db, Platform::String ^name);
		CLowlaDBCollection::ptr GetPcoll();

	private:
		void EnsureOpen();
		LDBDb ^m_db;
		Platform::String ^m_name;
		CLowlaDBCollection::ptr m_pcoll;
	};
}
