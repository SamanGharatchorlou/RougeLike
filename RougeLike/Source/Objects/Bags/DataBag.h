#pragma once

#include "pch.h"

class DataBag
{
public:
	virtual void readData(const XMLParser& parser, const BasicString& nodeName);
	virtual ~DataBag() { };

	virtual bool contains(const BasicString& name) const { return false; }
	virtual bool isEmpty() const { return false; }

protected:
	virtual StringMap readValues(XMLNode node) const;

	virtual void fillData(const StringMap& stringMap) = 0;
};