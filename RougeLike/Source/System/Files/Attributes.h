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


	void add(const std::string& name, std::string value)
	{
		attributes[name] = value;
	}


	bool contains(const std::string& name) const
	{
		return (bool)attributes.count(name);
	}


	const std::string& getString(const std::string& name)
	{
#if _DEBUG
		if(!contains(name))
			DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
		std::string value = attributes[name];
		return attributes[name];
	}


	const int getInt(const std::string& name)
	{
#if _DEBUG
		// TODO: is this a value check or does it need to be .count == 0 like in XMLValueMap
		if (!contains(name))
			DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
		return std::stoi(attributes[name]);
	}


	const int getBool(const std::string& name)
	{
#if _DEBUG
		// TODO: is this a value check or does it need to be .count == 0 like in XMLValueMap
		if (!contains(name))
			DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
		return strcmp(attributes[name].c_str(), "true") == 0;
	}


	const float getFloat(const std::string& name)
	{
#if _DEBUG
		if (!contains(name))
			DebugPrint(Warning, "There is no attribute with the label: %s\n", name.c_str());
#endif
		return std::stof(attributes[name]);
	}


	void log() const
	{
		DebugPrint(Log, "\nLoading level atrributes...\n");
		for (auto iter = attributes.begin(); iter != attributes.end(); iter++)
		{
			DebugPrint(Log, "Atrribute %s has value %s\n", iter->first.c_str(), iter->second.c_str());
		}
		DebugPrint(Log, "\n");
	}


	bool empty() const { return attributes.empty(); }


private:
	stringMap attributes;
};