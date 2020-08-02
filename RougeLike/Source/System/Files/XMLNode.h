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

	const std::string name() const { return std::string(node->name()); }
	const std::string value() const { return std::string(node->value()); }

	bool isEmpty() const { return node == nullptr; }

private:
	xmlNode node;

};