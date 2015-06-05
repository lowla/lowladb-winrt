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

LDBObjectBuilder ^LowlaDB::LDBObjectBuilder::builder()
{
	return ref new LDBObjectBuilder();
}

LDBObjectBuilder ^LDBObjectBuilder::appendDouble(String ^name, double value)
{
	utf16string str((utf16char *)name->Data(), 0, name->Length());
	m_bson->appendDouble(str.c_str(utf16string::UTF8), value);
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::appendString(String ^name, String ^value)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	utf16string strValue((utf16char *)value->Data(), 0, value->Length());
	m_bson->appendString(strName.c_str(utf16string::UTF8), strValue.c_str(utf16string::UTF8));
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::appendObject(String ^name, LDBObject ^value)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	m_bson->appendObject(strName.c_str(utf16string::UTF8), value->asBson());
	return this;
}

LDBObjectBuilder ^LDBObjectBuilder::appendObjectId(String ^name, LDBObjectId ^value)
{
	utf16string strName((utf16char *)name->Data(), 0, name->Length());
	m_bson->appendOid(strName.c_str(utf16string::UTF8), value->bytes());
	return this;
}

LDBObject ^LDBObjectBuilder::finish()
{
	m_bson->finish();
	return ref new LDBObject(m_bson);
}