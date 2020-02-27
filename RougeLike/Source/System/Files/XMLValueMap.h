#pragma once

union value
{
	int i;
	float f;
	double d;
	char c[20];
};

using valueMap = std::unordered_map<std::string, value>;


class XMLValueMap
{
public:
	valueMap& getMap() { return map; }

	void add(std::string name, value value)
	{
		map[name] = value;
	}

	int getInt(std::string name)
	{
#if _DEBUG
		if (map.count(name) == 0)
			DebugPrint(Warning, "There is no value with the label: %s\n", name.c_str());
#endif
		return map[name].i;
	}

	float getFloat(std::string name)
	{
#if _DEBUG
		if (map.count(name) == 0)
			DebugPrint(Warning, "There is no value with the label: %s\n", name.c_str());
#endif
		return map[name].f;
	}

	double getDouble(std::string name)
	{
#if _DEBUG
		if (map.count(name) == 0)
			DebugPrint(Warning, "There is no value with the label: %s\n", name.c_str());
#endif
		return map[name].d;
	}

	char* getChar(std::string name)
	{
#if _DEBUG
		if (map.count(name) == 0)
			DebugPrint(Warning, "There is no value with the label: %s\n", name.c_str());
#endif
		return map[name].c;
	}

	unsigned int size() { return map.size(); }

private:
	valueMap map;
};