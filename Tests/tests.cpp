#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LowlaDB;

namespace Tests
{
    TEST_CLASS(LDBObjectBuilderTests)
    {
    public:
        TEST_METHOD(testItCanBuildDoubles)
        {
			LDBObject ^obj = LDBObjectBuilder::builder()->appendDouble("myfield", 3.14)->finish();
			Assert::IsTrue(obj->containsField("myfield"));
			Assert::AreEqual(3.14, obj->doubleForField("myfield"), 1e-10);
        }

		TEST_METHOD(testItCanBuildStrings)
		{
			LDBObject ^obj = LDBObjectBuilder::builder()->appendString("myfield", "mystring")->finish();
			Assert::IsTrue(obj->containsField("myfield"));
			Assert::AreEqual("mystring", obj->stringForField("myfield"));
		}

		TEST_METHOD(testItCanBuildObjects)
		{
			LDBObject ^subObj = LDBObjectBuilder::builder()->appendString("myfield", "mystring")->finish();
			LDBObject ^obj = LDBObjectBuilder::builder()->appendObject("myfield", subObj)->finish();
			Assert::IsTrue(obj->containsField("myfield"));
			Assert::AreEqual(subObj, obj->objectForField("myfield"));
		}

		TEST_METHOD(testItCanBuildObjecIds)
		{
			LDBObjectId ^oid = LDBObjectId::generate();
			LDBObject ^obj = LDBObjectBuilder::builder()->appendObjectId("myfield", oid)->finish();
			Assert::IsTrue(obj->containsField("myfield"));
			Assert::AreEqual(oid, obj->objectIdForField("myfield"));
		}
		/*
			-(void)testItCanBuildBools
		{
			LDBObject *obj = [[[LDBObjectBuilder builder] appendBool:YES forField : @"myfield"] finish];
			XCTAssert([obj containsField : @"myfield"]);
			XCTAssertEqual([obj boolForField : @"myfield"], YES);
		}

			-(void)testItCanBuildDates
		{
			NSDate *date = [NSDate date];
			LDBObject *obj = [[[LDBObjectBuilder builder] appendDate:date forField : @"myfield"] finish];
			XCTAssert([obj containsField : @"myfield"]);
			NSTimeInterval i1 = [date timeIntervalSince1970];
			NSTimeInterval i2 = [[obj dateForField : @"myfield"] timeIntervalSince1970];
			XCTAssertEqualWithAccuracy(i1, i2, 1e-3);
		}

			-(void)testItCanBuildInts
		{
			LDBObject *obj = [[[LDBObjectBuilder builder] appendInt:314 forField : @"myfield"] finish];
			XCTAssert([obj containsField : @"myfield"]);
			XCTAssertEqual([obj intForField : @"myfield"], 314);
		}

			-(void)testItCanBuildLongs
		{
			LDBObject *obj = [[[LDBObjectBuilder builder] appendLong:314000000000000 forField : @"myfield"] finish];
			XCTAssert([obj containsField : @"myfield"]);
			XCTAssertEqual([obj longForField : @"myfield"], 314000000000000);
		}

			-(void)testItCanBuildArrays
		{
			LDBObject *obj = [[[[[LDBObjectBuilder builder] startArrayForField:@"myarr"] appendInt:5 forField : @"0"] finishArray] finish];
			XCTAssert([obj containsField : @"myarr"]);
			LDBObject *arr = [obj arrayForField : @"myarr"];
			XCTAssertEqual([arr intForField : @"0"], 5);
		}
		*/

    };
}