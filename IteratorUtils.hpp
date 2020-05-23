#pragma once

#include <iterator>

template <typename Iter>
class IteratorRange {
public:
	IteratorRange(Iter begin, Iter end) : m_begin{begin}, m_end{end} {}

	Iter begin() const { return m_begin; }
	Iter end() const { return m_end; }

private:
	Iter m_begin, m_end;
};

template <typename Container>
class FromSecond : public IteratorRange <typename Container::iterator> {
public:
	FromSecond(Container &c) : IteratorRange <typename Container::iterator>{std::next(c.begin()), c.end()} {}
};

template <typename Container>
class ConstFromSecond : public IteratorRange <typename Container::const_iterator> {
public:
	ConstFromSecond(const Container &c) : IteratorRange <typename Container::const_iterator>{std::next(c.cbegin()), c.cend()} {}
};
