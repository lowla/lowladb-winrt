#include "pch.h"
#include "LDBObjectId.h"

#include <codecvt>
#include <utf16string.h>

using namespace LowlaDB;

LDBObjectId ^LDBObjectId::generate()
{
	char buffer[CLowlaDBBson::OID_SIZE];
	CLowlaDBBson::oidGenerate(buffer);
	return ref new LDBObjectId(buffer);
}

Platform::String ^LDBObjectId::hexString()
{
	char buffer[CLowlaDBBson::OID_STRING_SIZE];
	CLowlaDBBson::oidToString(m_bytes, buffer);
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return ref new Platform::String(&converter.from_bytes(buffer).front());
}

const char *LDBObjectId::bytes()
{
	return m_bytes;
}

bool LDBObjectId::Equals(Platform::Object ^obj)
{
	LDBObjectId ^o = dynamic_cast<LDBObjectId ^>(obj);
	if (!o) {
		return false;
	}
	return 0 == memcmp(m_bytes, o->m_bytes, CLowlaDBBson::OID_SIZE);
}

int LDBObjectId::GetHashCode()
{
	int answer = 0;
	for (size_t i = 0; i < CLowlaDBBson::OID_SIZE; ++i) {
		answer = (answer * 31) ^ m_bytes[i];
	}
	return answer;
}

LDBObjectId::LDBObjectId(const char *bytes)
{
	memcpy(m_bytes, bytes, CLowlaDBBson::OID_SIZE);
}

LDBObjectId::LDBObjectId(Platform::String ^hexString)
{
	utf16string str((utf16char *)hexString->Data(), 0, hexString->Length());
	CLowlaDBBson::oidFromString(m_bytes, str.c_str());
}
