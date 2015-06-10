#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	ref class LDBObjectId;

	public ref class LDBObject sealed
	{
	public:
		static LDBObject ^ObjectWithJson(Platform::String ^json);

		bool ContainsField(Platform::String ^field);

		double DoubleForField(Platform::String ^field);
		Platform::String ^StringForField(Platform::String ^field);
		LDBObject ^ObjectForField(Platform::String ^field);
		LDBObjectId ^ObjectIdForField(Platform::String ^field);
		bool BoolForField(Platform::String ^field);
		Windows::Foundation::DateTime DateForField(Platform::String ^field);
		int64_t DateRawForField(Platform::String ^field);
		int IntForField(Platform::String ^field);
		int64_t LongForField(Platform::String ^field);
		LDBObject ^ArrayForField(Platform::String ^field);

		bool Equals(Platform::Object ^obj);
		int GetHashCode();

	internal:
		LDBObject(CLowlaDBBson::ptr m_bson);
		const char *AsBson();

	private:
		CLowlaDBBson::ptr m_bson;
	};
}
