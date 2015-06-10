#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	ref class LDBCollection;

	public ref class LDBDb sealed
	{
	public:
		Platform::String ^GetName();
		LDBCollection ^GetCollection(Platform::String ^name);
		Platform::Array<Platform::String ^> ^CollectionNames();

	internal:
		LDBDb(Platform::String ^name);
		void EnsureOpen();
		CLowlaDB::ptr GetPdb();

	private:
		Platform::String ^m_name;
		CLowlaDB::ptr m_pdb;
	};
}
