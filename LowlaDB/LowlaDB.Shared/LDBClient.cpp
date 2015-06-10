#include "pch.h"
#include "LDBClient.h"
#include "LDBDb.h"

#include <utf16string.h>

using namespace LowlaDB;

void LDBClient::DropDatabase(Platform::String ^dbName)
{
	utf16string str((utf16char *)dbName->Data(), 0, dbName->Length());
	lowladb_db_delete(str.c_str(utf16string::UTF8));
}

LDBDb ^LDBClient::GetDatabase(Platform::String ^dbName)
{
	return ref new LDBDb(dbName);
}