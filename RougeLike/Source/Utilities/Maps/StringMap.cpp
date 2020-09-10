#include "pch.h"
#include "StringMap.h"


void StringMap::fill(const XMLNode& node)
{
	XMLNode childNode = node.child();
	while (childNode)
	{
		mData[childNode.name()] = childNode.value();
		childNode = childNode.next();
	}
}


float StringMap::getFloat(const BasicString& key) const
{
	return (float)atof(mData.at(key).c_str());
}


int StringMap::getInt(const BasicString& key) const
{
	return atoi(mData.at(key).c_str());
}


VectorF StringMap::getVector(const BasicString& x, const BasicString& y) const 
{
	float xVec = getFloat(x);
	float yVec = getFloat(y);
	return VectorF(xVec, yVec);
}