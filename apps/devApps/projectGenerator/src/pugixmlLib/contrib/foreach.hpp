/*
 * Boost.Foreach support for pugixml classes.
 * This file is provided to the public domain.
 * Written by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 */

#ifndef HEADER_PUGIXML_FOREACH_HPP
#define HEADER_PUGIXML_FOREACH_HPP

#include "pugixml.hpp"

/*
 * These types add support for BOOST_FOREACH macro to xml_node and xml_document classes (child iteration only).
 * Example usage:
 * BOOST_FOREACH(xml_node n, doc) {}
 */

namespace boost
{
	template <typename> struct range_mutable_iterator;
	template <typename> struct range_const_iterator;

	template<> struct range_mutable_iterator<pugi::xml_node>
	{
		typedef pugi::xml_node::iterator type;
	};

	template<> struct range_const_iterator<pugi::xml_node>
	{
		typedef pugi::xml_node::iterator type;
	};

	template<> struct range_mutable_iterator<pugi::xml_document>
	{
		typedef pugi::xml_document::iterator type;
	};

	template<> struct range_const_iterator<pugi::xml_document>
	{
		typedef pugi::xml_document::iterator type;
	};
}

/*
 * These types add support for BOOST_FOREACH macro to xml_node and xml_document classes (child/attribute iteration).
 * Example usage:
 * BOOST_FOREACH(xml_node n, children(doc)) {}
 * BOOST_FOREACH(xml_node n, attributes(doc)) {}
 */

namespace pugi
{
	struct xml_node_children_adapter
	{
		typedef pugi::xml_node::iterator iterator;
		typedef pugi::xml_node::iterator const_iterator;

		xml_node node;

		const_iterator begin() const
		{
			return node.begin();
		}

		const_iterator end() const
		{
			return node.end();
		}
	};

	xml_node_children_adapter children(const pugi::xml_node& node)
	{
		xml_node_children_adapter result = {node};
		return result;
	}

	struct xml_node_attribute_adapter
	{
		typedef pugi::xml_node::attribute_iterator iterator;
		typedef pugi::xml_node::attribute_iterator const_iterator;

		xml_node node;

		const_iterator begin() const
		{
			return node.attributes_begin();
		}

		const_iterator end() const
		{
			return node.attributes_end();
		}
	};

	xml_node_attribute_adapter attributes(const pugi::xml_node& node)
	{
		xml_node_attribute_adapter result = {node};
		return result;
	}
}

#endif
