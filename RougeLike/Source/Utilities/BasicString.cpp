#include "pch.h"
#include "BasicString.h"

#include <cstring>


BasicString::BasicString(const char* string)
{
	mCap = strlen(string) + 1;
	mBuffer = new char[mCap];
	assign(string);
	printf("new call\n");
}

BasicString::BasicString(const BasicString& string) : BasicString(string.c_str()) { }

BasicString::~BasicString()
{
	delete[] mBuffer;
}


BasicString& BasicString::concat(const char* string)
{
	if (mLength + strlen(string) < mCap)
	{
		strcat_s(mBuffer, mCap, string);
		mLength = strlen(mBuffer);
		return *this;
	}
	else
	{
		increaseBufferSize(mLength + strlen(string));
		return concat(string);
	}
}


void BasicString::clear()
{
	memset(mBuffer, 0, mLength);
	mLength = 0;
}


// --- Private Functions --- //
void BasicString::assign(const char* string)
{
	mLength = strlen(string);
	memcpy(mBuffer, string, mLength + 1);
}



void BasicString::newBufferSize(int size)
{
	mCap = size + 1;

	delete[] mBuffer;
	mBuffer = new char[mCap];
}


void BasicString::increaseBufferSize(int size)
{
	mCap = size + 1;

	char* tempBuffer = new char[mCap];
	memcpy(tempBuffer, mBuffer, mLength + 1);

	delete[] mBuffer;
	mBuffer = tempBuffer;;
}



// Operator overloads
BasicString& BasicString::operator = (const char* string)
{
	if (strlen(string) >= mCap)
	{
		int newLength = strlen(string);
		newBufferSize(newLength);
	}

	assign(string);
	return *this;
}


bool operator == (const BasicString& basicString, const char* string)
{
	return strncmp(basicString.c_str(), string, basicString.length()) == 0;
}
bool operator == (const BasicString& basicStringA, const BasicString& basicStringB)
{
	return strncmp(basicStringA.c_str(), basicStringB.c_str(), basicStringA.length()) == 0;
}


BasicString operator + (BasicString basicString, const char* string)
{
	return basicString.concat(string);
}
BasicString operator + (BasicString basicStringA, const BasicString& basicStringB)
{
	return basicStringA.concat(basicStringA.c_str());
}