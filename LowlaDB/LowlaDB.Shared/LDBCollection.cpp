#include "pch.h"

#include <TeamstudioException.h>

#include "LDBCollection.h"
#include "LDBCursor.h"
#include "LDBDb.h"
#include "LDBObject.h"
#include "LDBWriteResult.h"

#include <utf16string.h>

using namespace LowlaDB;

LDBCollection::LDBCollection(LDBDb ^db, Platform::String ^name) : m_db(db), m_name(name)
{
}

LDBDb ^LDBCollection::GetDatabase()
{
	return m_db;
}

Platform::String ^LDBCollection::GetName()
{
	return m_name;
}

LDBWriteResult ^LDBCollection::Insert(LDBObject ^object)
{
	EnsureOpen();
	const char *bson = object->AsBson();
	try {
		CLowlaDBWriteResult::ptr wr = m_pcoll->insert(bson);
		return ref new LDBWriteResult(wr);
	}
	catch (TeamstudioException &e) {
		throw ref new Platform::Exception(E_INVALIDARG);
	}
}

LDBWriteResult ^LDBCollection::InsertArray(const Platform::Array<LDBObject ^> ^arr)
{
	EnsureOpen();
	std::vector<const char *> bsonArr;
	for (size_t i = 0; i < arr->Length; ++i) {
		bsonArr.push_back(arr[i]->AsBson());
	}
	try {
		CLowlaDBWriteResult::ptr wr = m_pcoll->insert(bsonArr);
		return ref new LDBWriteResult(wr);
	}
	catch (TeamstudioException &e) {
		throw ref new Platform::Exception(E_INVALIDARG);
	}
}

LDBCursor ^LDBCollection::Find()
{
	return ref new LDBCursor(this, nullptr, nullptr);
}

LDBCursor ^LDBCollection::Find(LDBObject ^query)
{
	return ref new LDBCursor(this, query, nullptr);
}

LDBObject ^LDBCollection::FindOne()
{
	return FindOne(nullptr);
}

LDBObject ^LDBCollection::FindOne(LDBObject ^query)
{
	LDBCursor ^cursor = ref new LDBCursor(this, query, nullptr);
	return cursor->One();
}

LDBWriteResult ^LDBCollection::Remove(LDBObject ^query)
{
	EnsureOpen();
	const char *bson = nullptr;
	if (query) {
		bson = query->AsBson();
	}
	CLowlaDBWriteResult::ptr wr = m_pcoll->remove(bson);
	return ref new LDBWriteResult(wr);
}

LDBWriteResult ^LDBCollection::Update(LDBObject ^query, LDBObject ^object)
{
	return Update(query, object, false, false);
}

LDBWriteResult ^LDBCollection::Update(LDBObject ^query, LDBObject ^object, bool upsert, bool multi)
{
	EnsureOpen();
	const char *queryBson = query->AsBson();
	const char *objectBson = object->AsBson();
	try {
		CLowlaDBWriteResult::ptr wr = m_pcoll->update(queryBson, objectBson, upsert, multi);
		return ref new LDBWriteResult(wr);
	}
	catch (TeamstudioException const &e) {
		throw ref new Platform::Exception(E_INVALIDARG);
	}
}

CLowlaDBCollection::ptr LDBCollection::GetPcoll()
{
	return m_pcoll;
}

void LDBCollection::EnsureOpen()
{
	if (!m_pcoll) {
		utf16string str((utf16char *)m_name->Data(), 0, m_name->Length());
		m_db->EnsureOpen();
		m_pcoll = m_db->GetPdb()->createCollection(str.c_str(utf16string::UTF8));
	}
}