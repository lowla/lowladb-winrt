#include "pch.h"
#include "LDBObject.h"

#include <codecvt>
#include <utf16string.h>

#include "LDBObjectId.h"

using namespace LowlaDB;

LDBObject::LDBObject(CLowlaDBBson::ptr bson) : m_bson(bson)
{
}

const char *LDBObject::asBson()
{
	return m_bson->data();
}

bool LDBObject::containsField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	return m_bson->containsKey(str.c_str(utf16string::UTF8));
}

double LDBObject::doubleForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	double answer;
	if (m_bson->doubleForKey(str.c_str(utf16string::UTF8), &answer)) {
		return answer;
	}
	return 0.0;
}

Platform::String ^LDBObject::stringForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	const char *answer;
	if (m_bson->stringForKey(str.c_str(utf16string::UTF8), &answer)) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return ref new Platform::String(&converter.from_bytes(answer).front());
	}
	return "";
}

LDBObject ^LDBObject::objectForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	CLowlaDBBson::ptr answer;
	if (m_bson->objectForKey(str.c_str(utf16string::UTF8), &answer)) {
		return ref new LDBObject(answer);
	}
	return nullptr;
}

LDBObjectId ^LDBObject::objectIdForField(Platform::String ^field)
{
	utf16string str((utf16char *)field->Data(), 0, field->Length());
	char answer[CLowlaDBBson::OID_SIZE];
	if (m_bson->oidForKey(str.c_str(utf16string::UTF8), answer)) {
		return ref new LDBObjectId(answer);
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