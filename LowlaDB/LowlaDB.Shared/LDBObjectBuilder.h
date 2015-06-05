#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	ref class LDBObject;
	ref class LDBObjectId;

	public ref class LDBObjectBuilder sealed
	{
	public:
		static LDBObjectBuilder ^builder();
		LDBObjectBuilder ^appendDouble(Platform::String ^name, double value);
		LDBObjectBuilder ^appendString(Platform::String ^name, Platform::String ^value);
		LDBObjectBuilder ^appendObject(Platform::String ^name, LDBObject ^value);
		LDBObjectBuilder ^appendObjectId(Platform::String ^name, LDBObjectId ^value);
		LDBObject ^finish();

	internal:
		LDBObjectBuilder();

	private:
		CLowlaDBBson::ptr m_bson;
	};
}
