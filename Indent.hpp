#pragma once

#include <iostream>

class Indent {
public:
	Indent(unsigned level = 0) : m_level{level} {}

	Indent & operator ++ ()
	{
		++m_level;
		return *this;
	}

	Indent & operator -- ()
	{
		assert(m_level != 0);
		--m_level;
		return *this;
	}

private:
	unsigned m_level;

	friend std::ostream & operator << (std::ostream &os, const Indent &obj)
	{
		for (unsigned i = 0u; i < obj.m_level; ++i)
			os << '\t';
		return os;
	}
};
