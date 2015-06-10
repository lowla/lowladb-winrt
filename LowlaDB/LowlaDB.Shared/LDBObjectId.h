#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	public ref class LDBObjectId sealed
	{
	public:
		static LDBObjectId ^Generate();

		Platform::String ^ToHexString();

		bool Equals(Platform::Object ^obj);
		int GetHashCode();

	internal:
		LDBObjectId(const char *bytes);
		LDBObjectId(Platform::String ^hexString);
		const char *GetBytes();

	private:
		char m_bytes[CLowlaDBBson::OID_SIZE];
	};
}
