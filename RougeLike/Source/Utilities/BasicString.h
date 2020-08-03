#pragma once


class BasicString
{
public:
	BasicString() : mBuffer(nullptr), mLength(0), mCap(0) { }
	BasicString(const char* string);
	BasicString(const BasicString& string);
	BasicString(const char* string, unsigned int length);

	~BasicString();

	const char* c_str() const { return mBuffer; }

	BasicString substr(int start, int length) const;


	int length() const { return mLength; }
	bool empty() const { return mLength == 0; }

	BasicString& concat(const char* string);

	void clear();


	BasicString& operator = (const char* string);
	BasicString& operator = (const BasicString& string);



private:
	void newBufferSize(unsigned int size);
	void increaseBufferSize(int size);
	
	void assignTerminated(const char* string);
	void assign(const char* string, unsigned int length);

	void set(const char* string, unsigned int length);

private:
	char* mBuffer;
	int mLength;
	int mCap;
};

void myMemCpy(void *dest, const void *src, size_t n);


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