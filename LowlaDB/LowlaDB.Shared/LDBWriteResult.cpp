#include "pch.h"
#include "LDBWriteResult.h"

#include "LDBObject.h"

using namespace LowlaDB;

namespace LowlaDB
{
	LDBWriteResult::LDBWriteResult(CLowlaDBWriteResult::ptr pwr) : m_pwr(pwr)
	{
	}

	int LDBWriteResult::GetDocumentCount()
	{
		return m_pwr->documentCount();
	}

	LDBObject ^LDBWriteResult::GetDocument(int n)
	{
		return ref new LDBObject(m_pwr->document(n));
	}
}