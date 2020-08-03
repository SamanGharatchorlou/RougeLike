#include "pch.h"
#include "BasicString.h"

#include <cstring>


BasicString::BasicString(const char* string)
{
	if (string)
	{
		mLength = strlen(string);
		mCap = mLength + 1;
		mBuffer = new char[mCap];

		assignTerminated(string);
	}
	else
	{
		mBuffer = nullptr;
		mLength = 0;
		mCap = 0;
	}

}

BasicString::BasicString(const BasicString& string) : BasicString(string.c_str()) { }


void myMemCpy(void *dest, const void *src, size_t n)
{
	// Typecast src and dest addresses to (char *) 
	char *csrc = (char *)src;
	char *cdest = (char *)dest;

	// Copy contents of src[] to dest[] 
	for (int i = 0; i < n; i++)
		cdest[i] = csrc[i];
}

BasicString::BasicString(const char* string, unsigned int length)
{
	mLength = length;
	mCap = length + 1;
	mBuffer = new char[mCap];
	mBuffer[mLength] = '\0';
	memcpy(mBuffer, string, length);
}

BasicString::~BasicString()
{
	delete[] mBuffer;
	mLength = 0;
	mCap = 0;
	mBuffer = nullptr;
}

void BasicString::set(const char* string, unsigned int length)
{
	assign(string, length);
	mBuffer[length + 1] = '\0';
	mLength = length;
}


// TODO: verify this works
BasicString BasicString::substr(int start, int length) const
{
	const char* backEndString = &mBuffer[start];
	BasicString subbedStr(backEndString, length);
	return subbedStr;
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
void BasicString::assignTerminated(const char* string)
{
	unsigned int length = strlen(string);
	memcpy(mBuffer, string, length + 1);
}

void BasicString::assign(const char* string, unsigned int length)
{
	memcpy(mBuffer, string, length);
}



void BasicString::newBufferSize(unsigned int size)
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
	unsigned int length = strlen(string);
	if (length >= mCap)
		newBufferSize(length);

	assignTerminated(string);
	mLength = length;
	return *this;
}

BasicString& BasicString::operator = (const BasicString& basicString)
{
	unsigned int length = basicString.length();
	if (length >= mCap)
		newBufferSize(length);

	assignTerminated(basicString.c_str());
	mLength = length;
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
	return basicStringA.concat(basicStringB.c_str());
}


