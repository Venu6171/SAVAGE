#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SAVAGE::Math;

namespace MathTest
{
	TEST_CLASS(Vector3Test)
	{
	public:
		TEST_METHOD(TestConstructor)
		{
			Vector3 v;
			Assert::AreEqual(v.x, 0.0f, 0.0001f);
			Assert::AreEqual(v.y, 0.0f, 0.0001f);
			Assert::AreEqual(v.z, 0.0f, 0.0001f);
		}

		TEST_METHOD(TestConstructor1)
		{
			Vector3 v{ 1.0f };
			Assert::AreEqual(v.x, 1.0f, 0.0001f);
			Assert::AreEqual(v.y, 1.0f, 0.0001f);
			Assert::AreEqual(v.z, 1.0f, 0.0001f);
		}

		// TODO: Add more test methods for Vector3 helpers in EngineMath.h

		TEST_METHOD(TestConstructor2)
		{
			Vector3 v{ 1.0f, 1.0f, 1.0f };
			Assert::AreEqual(v.x, 1.0f, 0.0001f);
			Assert::AreEqual(v.y, 1.0f, 0.0001f);
			Assert::AreEqual(v.z, 1.0f, 0.0001f);
		}

		TEST_METHOD(TestConstrutor3)
		{
			Vector3 v{ 1.0f, 1.0f, 1.0f };
			Assert::AreEqual(v.One.x, 1.0f);
			Assert::AreEqual(v.One.y, 1.0f);
			Assert::AreEqual(v.One.z, 1.0f);
		}

		TEST_METHOD(TestConstructor4)
		{

		}

	};

}
