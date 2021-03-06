#pragma once

// xml tile set/mapping attributes
class StringMap
{
public:
	StringMap& getMap() { return attributes; }

	void set(StringMap stringMap);
	void merge(StringMap otherAttributes);


	void add(const BasicString& name, BasicString value);


	bool contains(const BasicString& name) const;


	const BasicString& getString(const BasicString& name) const;


	const int getInt(const BasicString& name) const;


	const int getBool(const BasicString& name) const;


	const float getFloat(const BasicString& name) const;

	void log() const;;


	bool empty() const { return attributes.empty(); }


private:
	StringMap attributes;
};