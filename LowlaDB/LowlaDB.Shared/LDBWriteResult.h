#pragma once

#include <lowladb.h>

namespace LowlaDB
{
	ref class LDBObject;

	public ref class LDBWriteResult sealed
	{
	public:
		int GetDocumentCount();
		LDBObject ^GetDocument(int n);

	internal:
		LDBWriteResult(CLowlaDBWriteResult::ptr wr);

	private:
		CLowlaDBWriteResult::ptr m_pwr;
	};
}
