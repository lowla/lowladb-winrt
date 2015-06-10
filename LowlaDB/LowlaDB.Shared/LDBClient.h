#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	ref class LDBDb;

	public ref class LDBClient sealed
	{
	public:
		void DropDatabase(Platform::String ^dbName);
		LDBDb ^GetDatabase(Platform::String ^dbName);

	};
}
