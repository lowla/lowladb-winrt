#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	public ref class LDBObjectId sealed
	{
	public:
		static LDBObjectId ^generate();

		Platform::String ^hexString();

		bool Equals(Platform::Object ^obj);
		int GetHashCode();

	internal:
		LDBObjectId(const char *bytes);
		LDBObjectId(Platform::String ^hexString);
		const char *bytes();

	private:
		char m_bytes[CLowlaDBBson::OID_SIZE];
	};
}
