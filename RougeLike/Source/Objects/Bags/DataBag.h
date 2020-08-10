#pragma once


class DataBag
{
public:
	virtual void readData(const XMLNode node);
	virtual ~DataBag() { };

	virtual bool contains(const BasicString& name) const { return false; }
	virtual bool isEmpty() const { return false; }

protected:
	virtual StringMap readValues(XMLNode node) const;

	virtual void fillData(const StringMap& stringMap) = 0;
};