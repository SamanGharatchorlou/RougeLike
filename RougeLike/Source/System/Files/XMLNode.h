#pragma once

typedef rapidxml::xml_node<>* xmlNode;
typedef rapidxml::xml_attribute<>* xmlAttributes;


class XMLNode
{
public:
	explicit operator bool() const noexcept
	{
		return node != nullptr;
	}

public:
	explicit XMLNode(xmlNode xmlNode) : node(xmlNode) { }

	Attributes attributes() const;
	StringMap stringMap() const;
	ValueMap valueMap() const;

	XMLNode first() const { return XMLNode(node->first_node()); }
	XMLNode first(const BasicString& label) const { return XMLNode(node->first_node(label.c_str())); }

	XMLNode next() const { return XMLNode(node->next_sibling()); }
	XMLNode next(const BasicString& label) const { return XMLNode(node->next_sibling(label.c_str())); }

	const BasicString name() const { return BasicString(node->name()); }
	const BasicString value() const { return BasicString(node->value()); }

	bool isEmpty() const { return node == nullptr; }


private:
	xmlNode node;
};