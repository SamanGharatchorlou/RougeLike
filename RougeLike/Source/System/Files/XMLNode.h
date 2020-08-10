#pragma once


class XMLNode
{
public:
	explicit operator bool() const noexcept
	{
		return node != nullptr;
	}

private:
	using xmlNodePtr = rapidxml::xml_node<>*;
	using xmlNodeAttributes = rapidxml::xml_attribute<>*;


public:
	explicit XMLNode(xmlNodePtr xmlNode) : node(xmlNode) { }

	DataMap<BasicString> nodeAttributes() const;
	DataMap<BasicString> stringMap() const;
	DataMap<float> floatMap() const;


	XMLNode child() const { return XMLNode(node->first_node()); }
	XMLNode child(const BasicString& label) const { return XMLNode(node->first_node(label.c_str())); }

	XMLNode next() const { return XMLNode(node->next_sibling()); }
	XMLNode next(const BasicString& label) const { return XMLNode(node->next_sibling(label.c_str())); }

	const BasicString name() const { return BasicString(node->name()); }
	const BasicString value() const { return BasicString(node->value()); }

	bool isEmpty() const { return node == nullptr; }

	float getFloat() const;
	int getInt() const;
	const BasicString getString() const { return BasicString(node->value()); }

private:
	xmlNodePtr node;
};


VectorF getXYAttributes(const XMLNode node);