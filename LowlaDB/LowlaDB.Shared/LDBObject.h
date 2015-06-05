#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	ref class LDBObjectId;

	public ref class LDBObject sealed
	{
	public:
		bool containsField(Platform::String ^field);

		double doubleForField(Platform::String ^field);
		Platform::String ^stringForField(Platform::String ^field);
		LDBObject ^objectForField(Platform::String ^field);
		LDBObjectId ^objectIdForField(Platform::String ^field);

		bool Equals(Platform::Object ^obj);
		int GetHashCode();

	internal:
		LDBObject(CLowlaDBBson::ptr m_bson);
		const char *asBson();

	private:
		CLowlaDBBson::ptr m_bson;
	};
}
