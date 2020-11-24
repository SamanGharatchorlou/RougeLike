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


BasicString::BasicString(const char* string, unsigned int length)
{
	mLength = strlen(string);
	mCap = length + 1;
	mBuffer = new char[mCap];

	memcpy(mBuffer, string, mLength + 1);
}

BasicString::BasicString(float number)
{
	char tempBuffer[20];
	sprintf_s(tempBuffer, "%.f", number);

	mLength = strlen(tempBuffer);
	mCap = mLength + 1;

	mBuffer = new char[mCap];
	memcpy(mBuffer, tempBuffer, mCap);
}


BasicString::BasicString(float number, int precision)
{
	char tempBuffer[20];

	char formatBuffer[5] = "%.";
	_itoa(precision, formatBuffer + 2, 10);
	strcat(formatBuffer, "f\0");

	sprintf_s(tempBuffer, formatBuffer, number);

	mLength = strlen(tempBuffer);
	mCap = mLength + 1;

	mBuffer = new char[mCap];
	memcpy(mBuffer, tempBuffer, mCap);
}



BasicString::~BasicString()
{
	delete[] mBuffer;
	eliminate();
}


void BasicString::eliminate()
{
	mLength = 0;
	mCap = 0;
	mBuffer = nullptr;
}



void BasicString::set(const char* string)
{
	int strLength = strlen(string);

	if (strLength < mCap)
	{
		assignTerminated(string);
		mBuffer[strLength + 1] = '\0';
	}
	else
	{
		resizeBuffer(strLength + 1);
		set(string);
	}
}


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
		resizeBuffer(mLength + strlen(string) + 1);
		return concat(string);
	}
}


void BasicString::clear()
{
	memset(mBuffer, 0, mLength);
	mLength = 0;
}



const char* BasicString::findSubString(const BasicString& subString) const
{
	const char* value = strstr(mBuffer, subString.c_str());
	return strstr(mBuffer, subString.c_str());
}


void BasicString::getInput(const BasicString& message)
{
	printf("%s", message.c_str());
	
	clear();

	std::cin.get(mBuffer, mCap);
	std::cin.ignore();

	calculateLength();
}


// --- Private Functions --- //
void BasicString::assignTerminated(const char* string)
{
	mLength = strlen(string);
	memcpy(mBuffer, string, mLength + 1);
}


void BasicString::setNewBuffer(int size)
{
	delete[] mBuffer;
	eliminate();

	mCap = size + 1;
	mBuffer = new char[mCap];
}


void BasicString::resizeBuffer(int size)
{
	mCap = size;

	char* tempBuffer = new char[mCap];
	memcpy(tempBuffer, mBuffer, mLength + 1);

	delete[] mBuffer;
	mBuffer = tempBuffer;
}


// Operator overloads
BasicString& BasicString::operator = (const char* string)
{
	unsigned int length = strlen(string);
	if (length >= mCap)
		setNewBuffer(length);

	assignTerminated(string);
	return *this;
}

BasicString& BasicString::operator = (const BasicString& basicString)
{
	unsigned int length = basicString.length();
	if (length >= mCap)
		setNewBuffer(length);

	assignTerminated(basicString.c_str());
	return *this;
}


// TODO: is basicStringA is null then this breaks...
bool operator == (const BasicString& basicString, const char* string)
{
	return strncmp(basicString.c_str(), string, basicString.length() + 1) == 0;
}
bool operator == (const BasicString& basicStringA, const BasicString& basicStringB)
{
	return strncmp(basicStringA.c_str(), basicStringB.c_str(), basicStringA.length() + 1) == 0;
}


BasicString operator + (BasicString basicString, const char* string)
{
	return basicString.concat(string);
}
BasicString operator + (BasicString basicStringA, const BasicString& basicStringB)
{
	return basicStringA.concat(basicStringB.c_str());
}


