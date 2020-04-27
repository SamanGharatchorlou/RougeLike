#pragma once

union value
{
	int i;
	float f;
	double d;
	char c[25];
};

using valueMap = std::unordered_map<std::string, value>;


class XMLValueMap
{
public:
	valueMap& getMap() { return map; }

	void add(const std::string& name, value value)
	{
		map[name] = value;
	}

	int getInt(const std::string& name)
	{
#if _DEBUG
		if (map.count(name) == 0)
			DebugPrint(Warning, "There is no value with the label: %s\n", name.c_str());
#endif
		return map[name].i;
	}

	float getFloat(const std::string& name)
	{
#if _DEBUG
		if (map.count(name) == 0)
			DebugPrint(Warning, "There is no value with the label: %s\n", name.c_str());
#endif
		return map[name].f;
	}

	double getDouble(const std::string& name)
	{
#if _DEBUG
		if (map.count(name) == 0)
			DebugPrint(Warning, "There is no value with the label: %s\n", name.c_str());
#endif
		return map[name].d;
	}

	char* getChar(const std::string& name)
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