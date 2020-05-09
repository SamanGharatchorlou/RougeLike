#pragma once

//
//using valueMap = std::unordered_map<std::string, float>;
//
//
//class XMLValueMap
//{
//public:
//	valueMap& map() { return map; }
//
//	void add(const std::string& name, float value)
//	{
//		map[name] = value;
//	}
//
//	float getFloat(const std::string& name)
//	{
//#if _DEBUG
//		if (map.count(name) == 0)
//			DebugPrint(Warning, "There is no value with the label: %s\n", name.c_str());
//#endif
//		return map[name];
//	}
//
//	unsigned int size() { return map.size(); }
//
//private:
//	valueMap mMap;
//};