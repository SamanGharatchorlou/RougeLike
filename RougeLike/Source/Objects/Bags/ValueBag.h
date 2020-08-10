#pragma once

#include "DataBag.h"


class ValueBag : public DataBag
{
public:
	ValueBag() { };
	ValueBag(XMLNode node);
	ValueBag(FloatMap valueMap) { mData = valueMap; };
	~ValueBag() { };

	float get(const BasicString& value) const;

	bool contains(const BasicString& value) const override { return mData.contains(value); }
	bool isEmpty()							const override { return mData.empty(); }


protected:
	void fillData(const StringMap& stringMap) override;

private:
	FloatMap mData;
};