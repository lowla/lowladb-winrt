#include "pch.h"
#include "LDBObject.h"

#include <codecvt>
#include <utf16string.h>

#include "LDBObjectBuilder.h"
#include "LDBObjectId.h"

using namespace LowlaDB;
using namespace Windows::Data::Json;

LDBObject::LDBObject(CLowlaDBBson::ptr bson) : m_bson(bson)
{
}

LDBObject ^LDBObject::ObjectWithJson(Platform::String ^json)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string utf = converter.to_bytes(json->Data());

	CLowlaDBBson::ptr bson = lowladb_json_to_bson(utf.c_str());
	if (bson) {
		return ref new LDBObject(bson);
	}
	return nullptr;
}

const char *LDBObject::AsBson()
{
	return m_bson->data();
}

bool LDBObject::ContainsField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	return m_bson->containsKey(str.c_str(utf16string::UTF8));
}

double LDBObject::DoubleForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	double answer;
	if (m_bson->doubleForKey(str.c_str(utf16string::UTF8), &answer)) {
		return answer;
	}
	return 0.0;
}

Platform::String ^LDBObject::StringForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	const char *answer;
	if (m_bson->stringForKey(str.c_str(utf16string::UTF8), &answer)) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return ref new Platform::String(&converter.from_bytes(answer).front());
	}
	return "";
}

LDBObject ^LDBObject::ObjectForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	CLowlaDBBson::ptr answer;
	if (m_bson->objectForKey(str.c_str(utf16string::UTF8), &answer)) {
		return ref new LDBObject(answer);
	}
	return nullptr;
}

LDBObjectId ^LDBObject::ObjectIdForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	char answer[CLowlaDBBson::OID_SIZE];
	if (m_bson->oidForKey(str.c_str(utf16string::UTF8), answer)) {
		return ref new LDBObjectId(answer);
	}
	return nullptr;
}

bool LDBObject::BoolForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	bool answer;
	if (m_bson->boolForKey(str.c_str(utf16string::UTF8), &answer)) {
		return answer;
	}
	return false;
}

Windows::Foundation::DateTime LDBObject::DateForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	int64 answer;
	if (m_bson->dateForKey(str.c_str(utf16string::UTF8), &answer)) {
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

		// Multiply by 10000 to convert millis to 100ns ticks
		Windows::Foundation::DateTime ret;
		ret.UniversalTime = answer * 10000 + base;
		return ret;
	}
	return Windows::Foundation::DateTime();
}

int64_t LDBObject::DateRawForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	int64 answer;
	if (m_bson->dateForKey(str.c_str(utf16string::UTF8), &answer)) {
		return answer;
	}
	return 0;
}

int LDBObject::IntForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	int answer;
	if (m_bson->intForKey(str.c_str(utf16string::UTF8), &answer)) {
		return answer;
	}
	return 0;
}

int64_t LDBObject::LongForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	int64_t answer;
	if (m_bson->longForKey(str.c_str(utf16string::UTF8), &answer)) {
		return answer;
	}
	return 0;
}

LDBObject ^LDBObject::ArrayForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	CLowlaDBBson::ptr answer;
	if (m_bson->arrayForKey(str.c_str(utf16string::UTF8), &answer)) {
		return ref new LDBObject(answer);
	}
	return nullptr;
}

bool LDBObject::Equals(Platform::Object ^obj)
{
	LDBObject ^o = dynamic_cast<LDBObject ^>(obj);
	if (!o) {
		return false;
	}
	if (m_bson->size() != o->m_bson->size()) {
		return false;
	}
	return 0 == memcmp(m_bson->data(), o->m_bson->data(), m_bson->size());
}

int LDBObject::GetHashCode()
{
	int answer = 0;
	const char *start = m_bson->data();
	size_t len = m_bson->size();
	for (size_t i = 0; i < len; ++i) {
		answer = (answer * 31) ^ start[i];
	}
	return answer;
}