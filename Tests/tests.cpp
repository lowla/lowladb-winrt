#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace LowlaDB;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<int64>(const int64& t)
			{
				RETURN_WIDE_STRING(t);
			}
		}
	}
}

namespace Tests
{
    TEST_CLASS(LDBObjectBuilderTests)
    {
    public:
        TEST_METHOD(testItCanBuildDoubles)
        {
			LDBObject ^obj = LDBObjectBuilder::Builder()->AppendDouble("myfield", 3.14)->Finish();
			Assert::IsTrue(obj->ContainsField("myfield"));
			Assert::AreEqual(3.14, obj->DoubleForField("myfield"), 1e-10);
        }

		TEST_METHOD(testItCanBuildStrings)
		{
			LDBObject ^obj = LDBObjectBuilder::Builder()->AppendString("myfield", "mystring")->Finish();
			Assert::IsTrue(obj->ContainsField("myfield"));
			Assert::AreEqual("mystring", obj->StringForField("myfield"));
		}

		TEST_METHOD(testItCanBuildObjects)
		{
			LDBObject ^subObj = LDBObjectBuilder::Builder()->AppendString("myfield", "mystring")->Finish();
			LDBObject ^obj = LDBObjectBuilder::Builder()->AppendObject("myfield", subObj)->Finish();
			Assert::IsTrue(obj->ContainsField("myfield"));
			Assert::AreEqual(subObj, obj->ObjectForField("myfield"));
		}

		TEST_METHOD(testItCanBuildObjecIds)
		{
			LDBObjectId ^oid = LDBObjectId::Generate();
			LDBObject ^obj = LDBObjectBuilder::Builder()->AppendObjectId("myfield", oid)->Finish();
			Assert::IsTrue(obj->ContainsField("myfield"));
			Assert::AreEqual(oid, obj->ObjectIdForField("myfield"));
		}

		TEST_METHOD(testItCanBuildBools)
		{
			LDBObject ^obj = LDBObjectBuilder::Builder()->AppendBool("myfield", true)->Finish();
			Assert::IsTrue(obj->ContainsField("myfield"));
			Assert::IsTrue(obj->BoolForField("myfield"));
		}

		TEST_METHOD(testItCanBuildDates)
		{
			SYSTEMTIME st;
			memset(&st, 0, sizeof(SYSTEMTIME));
			st.wYear = 1970;
			st.wMonth = 1;
			st.wDay = 2;
			int64 utc;
			SystemTimeToFileTime(&st, (FILETIME *)&utc);
			Windows::Foundation::DateTime dt;
			dt.UniversalTime = utc;
			LDBObject ^obj = LDBObjectBuilder::Builder()->AppendDate("myfield", dt)->Finish();
			Assert::IsTrue(obj->ContainsField("myfield"));

			Assert::AreEqual(dt.UniversalTime, obj->DateForField("myfield").UniversalTime);
			Assert::AreEqual(60 * 60 * 24 * 1000LL, obj->DateRawForField("myfield"));
		}

		TEST_METHOD(testItCanBuildInts)
		{
			LDBObject ^obj = LDBObjectBuilder::Builder()->AppendInt("myfield", 314)->Finish();
			Assert::IsTrue(obj->ContainsField("myfield"));
			Assert::AreEqual(314, obj->IntForField("myfield"));
		}

		TEST_METHOD(testItCanBuildLongs)
		{
			LDBObject ^obj = LDBObjectBuilder::Builder()->AppendLong("myfield", 314000000000000)->Finish();
			Assert::IsTrue(obj->ContainsField("myfield"));
			Assert::AreEqual(314000000000000, obj->LongForField("myfield"));
		}

		TEST_METHOD(testItCanBuildArrays)
		{
			LDBObject ^obj = LDBObjectBuilder::Builder()->StartArray("myarr")->AppendInt("0", 5)->FinishArray()->Finish();
			Assert::IsTrue(obj->ContainsField("myarr"));
			LDBObject ^arr = obj->ArrayForField("myarr");
			Assert::AreEqual(5, arr->IntForField("0"));
		}
    };

	TEST_CLASS(LDBDbTests)
	{
	public:
		TEST_METHOD_INITIALIZE(setUp)
		{
			m_client = ref new LDBClient();
			m_client->DropDatabase("mydb");
			m_db = m_client->GetDatabase("mydb");
		}

		TEST_METHOD_CLEANUP(tearDown)
		{
			m_db = nullptr;
			m_client->DropDatabase("mydb");
			m_client = nullptr;
		}

		TEST_METHOD(testItCanCreateDatabaseReferences)
		{
			Assert::AreEqual("mydb", m_db->GetName());
		}

		TEST_METHOD(testItCanCreateCollectionReferences)
		{
			LDBCollection ^coll = m_db->GetCollection("mycoll.dotted");
			Assert::AreEqual(m_db, coll->GetDatabase());
			Assert::AreEqual("mycoll.dotted", coll->GetName());
		}

		TEST_METHOD(testCollectionNames)
		{
			// No collections to start with
			Array<String ^> ^check = m_db->CollectionNames();
			Assert::AreEqual(0U, check->Length);
			
			// Create a collection object - this doesn't actually create the collection yet
			LDBCollection ^coll = m_db->GetCollection(L"coll");
			check = m_db->CollectionNames();
			Assert::AreEqual(0U, check->Length);

			// Insert a document - this will create the collection
			LDBObject ^object = LDBObjectBuilder::Builder()->AppendString("myfield", "mystring")->Finish();
			coll->Insert(object);
			check = m_db->CollectionNames();
			Assert::AreEqual(1U, check->Length);
			Assert::AreEqual("coll", check[0]);

			// And another collection
			coll = m_db->GetCollection("coll2.sub");
			coll->Insert(object);
			check = m_db->CollectionNames();
			Assert::AreEqual(2U, check->Length);
			Assert::AreEqual("coll", check[0]);
			Assert::AreEqual("coll2.sub", check[1]);
		}


	private:
		LDBClient ^m_client;
		LDBDb ^m_db;
	};

	TEST_CLASS(LDBCursorTests)
	{
	public:
		TEST_METHOD_INITIALIZE(setUp)
		{
			m_client = ref new LDBClient();
			m_client->DropDatabase("mydb");
			m_db = m_client->GetDatabase("mydb");
			m_coll = m_db->GetCollection("mycoll");
		}

		TEST_METHOD_CLEANUP(tearDown)
		{
			m_coll = nullptr;
			m_db = nullptr;
			m_client->DropDatabase("mydb");
			m_client = nullptr;
		}

		TEST_METHOD(testCount)
		{
			m_coll->Insert(LDBObject::ObjectWithJson("{\"a\":1}"));
			m_coll->Insert(LDBObject::ObjectWithJson("{\"a\":2}"));
			m_coll->Insert(LDBObject::ObjectWithJson("{\"a\":3}"));

			Assert::AreEqual(3LL, m_coll->Find()->Count());
			Assert::AreEqual(1LL, m_coll->Find(LDBObject::ObjectWithJson("{\"a\":2}"))->Count());
			Assert::AreEqual(2LL, m_coll->Find()->Limit(2)->Count());
			Assert::AreEqual(3LL, m_coll->Find()->Limit(20)->Count());
		}

		TEST_METHOD(testSort)
		{
			m_coll->Insert(LDBObject::ObjectWithJson("{\"a\":1, \"b\":1}"));
			m_coll->Insert(LDBObject::ObjectWithJson("{\"a\":2, \"b\":20}"));
			m_coll->Insert(LDBObject::ObjectWithJson("{\"a\":2, \"b\":30}"));

			LDBObject ^sort = LDBObjectBuilder::Builder()->AppendInt("a", 1)->AppendInt("b", -1)->Finish();

			LDBCursor ^cursor = m_coll->Find();
			cursor = cursor->Sort(sort);
			LDBObject ^doc = cursor->Next();
			Assert::AreEqual(1, doc->IntForField("a"));
			doc = cursor->Next();
			Assert::AreEqual(2, doc->IntForField("a"));
			Assert::AreEqual(30, doc->IntForField("b"));
			doc = cursor->Next();
			Assert::AreEqual(2, doc->IntForField("a"));
			Assert::AreEqual(20, doc->IntForField("b"));
			doc = cursor->Next();
			Assert::IsNull(doc);
		}

	private:
		LDBClient ^m_client;
		LDBDb ^m_db;
		LDBCollection ^m_coll;
	};

	TEST_CLASS(LDBCollectionTests)
	{
		TEST_METHOD_INITIALIZE(setUp)
		{
			m_client = ref new LDBClient();
			m_client->DropDatabase("mydb");
			m_db = m_client->GetDatabase("mydb");
			m_coll = m_db->GetCollection("mycoll");
		}

		TEST_METHOD_CLEANUP(tearDown)
		{
			m_coll = nullptr;
			m_db = nullptr;
			m_client->DropDatabase("mydb");
			m_client = nullptr;
		}

		TEST_METHOD(testItCanCreateSingleStringDocuments)
		{
			LDBObject ^object = LDBObjectBuilder::Builder()->AppendString("myfield", "mystring")->Finish();
			LDBWriteResult ^wr = m_coll->Insert(object);

			Assert::AreEqual(1, wr->GetDocumentCount());
			Assert::IsNotNull(wr->GetDocument(0)->ObjectIdForField("_id"));
		}

		TEST_METHOD(testItCreatesANewIdForEachDocument)
		{
			LDBObject ^object = LDBObjectBuilder::Builder()->AppendString("myfield", "mystring")->Finish();
			LDBWriteResult ^wr = m_coll->Insert(object);
			LDBWriteResult ^wr2 = m_coll->Insert(object);
			Assert::AreNotEqual(wr->GetDocument(0)->ObjectIdForField("_id")->ToHexString(),
				wr2->GetDocument(0)->ObjectIdForField("_id")->ToHexString());
		}

		TEST_METHOD(testItCanFindTheFirstDocument)
		{
			LDBObject ^object = LDBObjectBuilder::Builder()->AppendString("myfield", "mystring")->Finish();
			LDBWriteResult ^wr = m_coll->Insert(object);
			LDBObject ^found = m_coll->FindOne();
			Platform::String ^check = found->StringForField("myfield");
			Platform::String ^checkId = found->ObjectIdForField("_id")->ToHexString();
			Assert::AreEqual(check, "mystring");
			Assert::AreEqual(checkId, wr->GetDocument(0)->ObjectIdForField("_id")->ToHexString());
		}
		
		TEST_METHOD(testItCanFindTwoDocuments)
		{
			LDBObject ^object = LDBObjectBuilder::Builder()->AppendString("myfield", "mystring1")->Finish();
			LDBWriteResult ^wr1 = m_coll->Insert(object);
			object = LDBObjectBuilder::Builder()->AppendString("myfield", "mystring2")->Finish();
			LDBWriteResult ^wr2 = m_coll->Insert(object);

			LDBCursor ^cursor = m_coll->Find();
			Assert::IsTrue(cursor->HasNext());
			LDBObject ^check1 = cursor->Next();
			Assert::IsTrue(cursor->HasNext());
			LDBObject ^check2 = cursor->Next();
			Assert::IsFalse(cursor->HasNext());

			Assert::AreEqual(wr1->GetDocument(0)->ObjectIdForField("_id"), check1->ObjectIdForField("_id"));
			Assert::AreEqual(wr2->GetDocument(0)->ObjectIdForField("_id"), check2->ObjectIdForField("_id"));
		}
		
		TEST_METHOD(testItCannotInsertDocumentsWithDollarFields)
		{
			LDBObject ^object = LDBObjectBuilder::Builder()->AppendString("$myfield", "mystring")->Finish();

			auto f1 = [this, object] { m_coll->Insert(object); };
			Assert::ExpectException<Platform::Exception ^>(f1);
		}

		TEST_METHOD(testItCannotInsertDocumentsWithDollarFieldsViaArray)
		{
			LDBObject ^object = LDBObjectBuilder::Builder()->AppendString("$myfield", "mystring")->Finish();

			Platform::Array<LDBObject ^> ^arr = ref new Platform::Array<LDBObject ^>(1);
			arr[0] = object;
			auto f1 = [this, arr] { m_coll->InsertArray(arr); };
			Assert::ExpectException<Platform::Exception ^>(f1);
		}

		TEST_METHOD(testItCanRemoveADocument)
		{
			LDBObject ^object1 = LDBObjectBuilder::Builder()->AppendInt("a", 1)->Finish();
			m_coll->Insert(object1);
			LDBObject ^object2 = LDBObjectBuilder::Builder()->AppendInt("a", 2)->Finish();
			m_coll->Insert(object2);
			LDBObject ^object3 = LDBObjectBuilder::Builder()->AppendInt("a", 3)->Finish();
			m_coll->Insert(object3);

			LDBObject ^query = LDBObjectBuilder::Builder()->AppendInt("a", 2)->Finish();
			LDBWriteResult ^wr = m_coll->Remove(query);
			Assert::AreEqual(1, wr->GetDocumentCount());

			LDBCursor ^cursor = m_coll->Find();
			LDBObject ^doc = cursor->Next();
			Assert::AreEqual(1, doc->IntForField("a"));
			doc = cursor->Next();
			Assert::AreEqual(3, doc->IntForField("a"));
			doc = cursor->Next();
			Assert::IsNull(doc);
		}

		TEST_METHOD(testItCanRemoveAllDocuments)
		{
			LDBObject ^object1 = LDBObjectBuilder::Builder()->AppendInt("a", 1)->Finish();
			m_coll->Insert(object1);
			LDBObject ^object2 = LDBObjectBuilder::Builder()->AppendInt("a", 2)->Finish();
			m_coll->Insert(object2);
			LDBObject ^object3 = LDBObjectBuilder::Builder()->AppendInt("a", 3)->Finish();
			m_coll->Insert(object3);

			LDBWriteResult ^wr = m_coll->Remove(nullptr);
			Assert::AreEqual(3, wr->GetDocumentCount());

			LDBCursor ^cursor = m_coll->Find();
			LDBObject ^doc = cursor->Next();
			Assert::IsNull(doc);
		}

		TEST_METHOD(testItCanUpdateADocument)
		{
			LDBObject ^object1 = LDBObjectBuilder::Builder()->AppendInt("a", 1)->Finish();
			m_coll->Insert(object1);
			LDBObject ^object2 = LDBObjectBuilder::Builder()->AppendInt("a", 2)->Finish();
			m_coll->Insert(object2);

			LDBObject ^query = LDBObjectBuilder::Builder()->AppendInt("a", 2)->Finish();
			LDBObject ^update = LDBObjectBuilder::Builder()->AppendInt("a", 3)->Finish();

			LDBWriteResult ^wr = m_coll->Update(query, update);
			Assert::AreEqual(1, wr->GetDocumentCount());

			LDBCursor ^cursor = m_coll->Find();
			LDBObject ^obj = cursor->Next();
			Assert::AreEqual(1, obj->IntForField("a"));
			obj = cursor->Next();
			Assert::AreEqual(3, obj->IntForField("a"));
			obj = cursor->Next();
			Assert::IsNull(obj);
		}

	private:
		LDBClient ^m_client;
		LDBDb ^m_db;
		LDBCollection ^m_coll;
	};
}