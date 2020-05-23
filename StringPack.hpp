#pragma once

#include <array>
#include <string_view>

namespace meta {
	constexpr bool isspace(char c)
	{
		return c == ' ' || c == '\t' || c == '\n' || c == '\r';
	}

	template <char Separator = ','>
	constexpr size_t countTokensTotalLength(const char *s)
	{
		size_t result = 0;
		while (*s) {
			while (*s && isspace(*s))
				++s;
			if (*s) {
				++result;
				while (*s && *s != Separator) {
					++s;
					++result;
				}

				if (*s)
					++s;
			}
		}

		return result;
	}

	template <char Separator = ','>
	constexpr size_t countTokens(const char *s)
	{
		size_t result = 1;
		for (; *s; ++s) {
			if (*s == Separator)
				++result;
		}
		return result;
	}

	template <size_t TokenCnt, char Separator = ','>
	constexpr auto splitTokens(const char *s)
	{
		std::array <std::string_view, TokenCnt> result;
		size_t i = 0;

		while (i != TokenCnt) {
			while (*s && isspace(*s))
				++s;

			const char *begin = s;
			while (*s && *s != Separator)
				++s;
			result[i++] = std::string_view{begin, static_cast<std::string_view::size_type>(s - begin)};
			++s;
		}
		return result;
	}
};

template <size_t Length, size_t Tokens>
class StringPack {
public:
	constexpr StringPack(std::initializer_list <const char *> tokens) : m_data{}, m_idx{}
	{
		size_t cnt = 0, idx = 0;
		for (const char *s : tokens)
			addString(cnt, idx, s);
	}

	constexpr StringPack(const std::array <std::string_view, Tokens> &tokens) : m_data{}, m_idx{}
	{
		size_t cnt = 0, idx = 0;
		for (const auto &s : tokens)
			addString(cnt, idx, s);
	}

	constexpr const char * operator [] (const size_t idx) const
	{
		return &m_data[m_idx[idx]];
	}

	template <size_t Idx>
	constexpr const char * at() const
	{
		static_assert(Idx >= 0 && Idx < Tokens);
		return &m_data[m_idx[Idx]];
	}

	constexpr size_t size() const
	{
		return Tokens;
	}

private:
	template <typename T>
	constexpr void addString(size_t &cnt, size_t &idx, const T &str)
	{
		m_idx[idx++] = cnt;
		if constexpr(std::is_same_v <T, const char *>) {
			for (size_t i = 0; str[i] != '\0'; ++i)
				m_data[cnt++] = str[i];
		} else {
			for (size_t i = 0; i != str.size(); ++i)
				m_data[cnt++] = str[i];
		}
		m_data[cnt++] = '\0';
	}

	std::array <char, Length> m_data;
	std::array <size_t, Tokens> m_idx;
};

template <typename... Literal, size_t... Length>
constexpr auto makeStringPack(const Literal (&...str)[Length])
{
	return StringPack<(... + Length), sizeof...(Literal)>{str...};
}

template <size_t Length, size_t Tokens>
constexpr auto makeStringPack(const std::array <std::string_view, Tokens> &tokens)
{
	return StringPack<Length, Tokens>{tokens};
}
