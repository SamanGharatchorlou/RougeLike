#pragma once


class BasicString
{
public:
	BasicString(const char* string);
	BasicString(const BasicString& string);

	~BasicString();

	const char* c_str() const { return mBuffer; }
	int length() const { return mLength; }

	BasicString& concat(const char* string);

	void clear();


	BasicString& operator = (const char* string);




private:
	void newBufferSize(int size);
	void increaseBufferSize(int size);
	
	void assign(const char* string);

	bool empty() const { return mLength == 0; }


private:
	char* mBuffer;
	int mLength;
	int mCap;
};



BasicString operator + (BasicString basicString, const char* string);
BasicString operator + (BasicString basicStringA, const BasicString& basicStringB);

bool operator == (const BasicString& basicString, const char* string);
bool operator == (const BasicString& basicStringA, const BasicString& basicStringB);
