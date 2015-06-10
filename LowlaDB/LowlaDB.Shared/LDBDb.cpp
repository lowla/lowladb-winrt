#include "pch.h"
#include "LDBClient.h"
#include "LDBCollection.h"
#include "LDBDb.h"

#include <codecvt>
#include <utf16string.h>

using namespace LowlaDB;

LDBDb::LDBDb(Platform::String ^name) : m_name(name)
{
}

Platform::String ^LDBDb::GetName()
{
	return m_name;
}

LDBCollection ^LDBDb::GetCollection(Platform::String ^name)
{
	return ref new LDBCollection(this, name);
}

Platform::Array<Platform::String ^> ^LDBDb::CollectionNames()
{
	EnsureOpen();
	std::vector<utf16string> names;
	m_pdb->collectionNames(&names);

	Platform::Array<Platform::String ^> ^answer = ref new Platform::Array<Platform::String ^>(names.size());

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	for (size_t i = 0; i < names.size(); ++i) {
		answer[i] = ref new Platform::String(&converter.from_bytes(names[i].c_str(utf16string::UTF8)).front());
	}
	return answer;
}

void LDBDb::EnsureOpen()
{
	if (!m_pdb) {
		utf16string str((utf16char *)m_name->Data(), 0, m_name->Length());
		m_pdb = CLowlaDB::open(str.c_str(utf16string::UTF8));
	}
}

CLowlaDB::ptr LDBDb::GetPdb()
{
	return m_pdb;
}