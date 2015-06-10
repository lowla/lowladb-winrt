#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	ref class LDBObject;
	ref class LDBObjectId;

	public ref class LDBObjectBuilder sealed
	{
	public:
		static LDBObjectBuilder ^Builder();
		LDBObjectBuilder ^AppendDouble(Platform::String ^name, double value);
		LDBObjectBuilder ^AppendString(Platform::String ^name, Platform::String ^value);
		LDBObjectBuilder ^AppendObject(Platform::String ^name, LDBObject ^value);
		LDBObjectBuilder ^AppendObjectId(Platform::String ^name, LDBObjectId ^value);
		LDBObjectBuilder ^AppendBool(Platform::String ^name, bool value);
		LDBObjectBuilder ^AppendDate(Platform::String ^name, Windows::Foundation::DateTime dt);
		LDBObjectBuilder ^AppendInt(Platform::String ^name, int value);
		LDBObjectBuilder ^AppendLong(Platform::String ^name, int64_t value);

		LDBObjectBuilder ^StartArray(Platform::String ^name);
		LDBObjectBuilder ^FinishArray();

		LDBObject ^Finish();

	internal:
		LDBObjectBuilder();

	private:
		CLowlaDBBson::ptr m_bson;
	};
}
