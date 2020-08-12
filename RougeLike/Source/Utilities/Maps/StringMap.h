#pragma once

#include "DataMap.h"


class StringMap : public DataMap<BasicString,BasicString>
{
public:
	StringMap() { }
	StringMap(const XMLNode& node) { fill(node); }
	void fill(const XMLNode& node) override;

	float getFloat(const BasicString& key) const;
	int getInt(const BasicString& key) const;
	VectorF getVector(const BasicString& x, const BasicString& y) const;
};