#pragma once

typedef std::unordered_map<std::string, std::string> stringMap;


// xml tile set/mapping attributes
class Attributes
{
public:
	stringMap& getMap() { return attributes; }


	void merge(Attributes otherAttributes)
	{
		attributes.insert(otherAttributes.getMap().begin(), otherAttributes.getMap().end());
	}


	void add(std::string name, std::string value)
	{
		attributes[name] = value;
	}


	bool contains(std::string name)
	{
		return (bool)attributes.count(name);
	}


	const std::string& getString(std::string name)
	{
#if _DEBUG
		if (attributes[name].empty())
			DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
		return attributes[name];
	}


	const int getInt(std::string name)
	{
#if _DEBUG
		// TODO: is this a value check or does it need to be .count == 0 like in XMLValueMap
		if (attributes[name].empty())
			DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
		return std::stoi(attributes[name]);
	}


	const float getFloat(std::string name)
	{
#if _DEBUG
		if (attributes[name].empty())
			DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
		return std::stof(attributes[name]);
	}


	void log()
	{
		DebugPrint(Log, "\nLoading level atrributes...\n");
		for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
		{
			DebugPrint(Log, "Atrribute %s has value %s\n", iter->first.c_str(), iter->second.c_str());
		}
		DebugPrint(Log, "\n");
	}


	bool empty() { return attributes.empty(); }


private:
	stringMap attributes;
};