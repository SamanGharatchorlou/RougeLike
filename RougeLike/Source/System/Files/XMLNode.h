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

	XMLNode first() const { return XMLNode(node->first_node()); }
	XMLNode next() const { return XMLNode(node->next_sibling()); }

	const BasicString name() const { return BasicString(node->name()); }
	const BasicString value() const { return BasicString(node->value()); }

	bool isEmpty() const { return node == nullptr; }

private:
	xmlNode node;

};