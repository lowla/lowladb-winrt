#include "pch.h"
#include "LDBObjectBuilder.h"

#include "LDBObject.h"
#include "LDBObjectId.h"

using namespace LowlaDB;
using namespace Platform;

LowlaDB::LDBObjectBuilder::LDBObjectBuilder()
{
	m_bson = CLowlaDBBson::create();
}

LDBObjectBuilder ^LowlaDB::LDBObjectBuilder::Builder()
{
	return ref new LDBObjectBuilder();
}

LDBObjectBuilder ^LDBObjectBuilder::AppendDouble(String ^name, double value)
{
	utf16string str((utf16char *)name->Data(), 0, name->Length());
	m_bson->appendDouble(str.c_str(utf16string::UTF8), value);
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::AppendString(String ^name, String ^value)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	utf16string strValue((utf16char *)value->Data(), 0, value->Length());
	m_bson->appendString(strName.c_str(utf16string::UTF8), strValue.c_str(utf16string::UTF8));
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::AppendObject(String ^name, LDBObject ^value)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	m_bson->appendObject(strName.c_str(utf16string::UTF8), value->AsBson());
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::AppendObjectId(String ^name, LDBObjectId ^value)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	m_bson->appendOid(strName.c_str(utf16string::UTF8), value->GetBytes());
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::AppendBool(String ^name, bool value)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	m_bson->appendBool(strName.c_str(utf16string::UTF8), value);
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::AppendDate(String ^name, Windows::Foundation::DateTime dt)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	/*
	SYSTEMTIME st19700101;
	memset(&st19700101, 0, sizeof(SYSTEMTIME));
	st19700101.wYear = 1970;
	st19700101.wMonth = 1;
	st19700101.wDay = 1;
	int64 base;
	SystemTimeToFileTime(&st19700101, (FILETIME *)&base);
	*/
	int64 base = 116444736000000000;

	// We need the delta in millis, not 100ns ticks hence / 10000
	int64 delta = (dt.UniversalTime - base) / 10000;

	m_bson->appendDate(strName.c_str(utf16string::UTF8), delta);
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::AppendInt(Platform::String ^name, int value)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	m_bson->appendInt(strName.c_str(utf16string::UTF8), value);
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::AppendLong(Platform::String ^name, int64_t value)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	m_bson->appendLong(strName.c_str(utf16string::UTF8), value);
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::StartArray(Platform::String ^name)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	m_bson->startArray(strName.c_str(utf16string::UTF8));
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::FinishArray()
{
	m_bson->finishArray();
	return this;
}

LDBObject ^LDBObjectBuilder::Finish()
{
	m_bson->finish();
	return ref new LDBObject(m_bson);
}