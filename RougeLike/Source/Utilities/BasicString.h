#pragma once


class BasicString
{
public:
	BasicString() : mBuffer(nullptr), mLength(0), mCap(0) { }
	BasicString(const char* string);
	BasicString(const BasicString& string);
	BasicString(const char* string, unsigned int length);
	BasicString(float number);
	BasicString(float number, int precision);

	~BasicString();

	const char* c_str() const { return mBuffer; }

	void set(const char* string);

	BasicString substr(int start, int length) const;

	int length() const { return mLength; }
	int bufferLength() const { return mCap; }
	bool empty() const { return mLength == 0; }
	void calculateLength() { mLength = strlen(mBuffer); }

	BasicString& concat(const char* string);

	void clear();
	void eliminate(); // Warning: does not delete buffer, but sets to nullptr

	char*& buffer() { return mBuffer; }

	BasicString& operator = (const char* string);
	BasicString& operator = (const BasicString& string);

	const char* findSubString(const BasicString& subString) const;

	void getInput(const BasicString& message);

private:
	void setNewBuffer(int size);
	void resizeBuffer(int size);
	
	void assignTerminated(const char* string);


private:
	char* mBuffer;
	int mLength;
	int mCap;
};


BasicString operator + (BasicString basicString, const char* string);
BasicString operator + (BasicString basicStringA, const BasicString& basicStringB);

bool operator == (const BasicString& basicString, const char* string);
bool operator == (const BasicString& basicStringA, const BasicString& basicStringB);


/* 
Define BasicString hash for use in maps
djb2 hash function by Dan Bernstein. http://www.cse.yorku.ca/~oz/hash.html.
*/
namespace std
{
	template <>
	struct hash<BasicString>
	{
		std::size_t operator()(const BasicString& string) const
		{
			const char* str = string.c_str();
			unsigned long hash = 5381;
			unsigned int c;

			while (c = *str++)
				hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

			return hash;
		}
	};
}