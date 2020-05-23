#pragma once

#include <cstring>
#include <optional>
#include <ostream>
#include <string_view>
#include <type_traits>
#include "StringPack.hpp"

template <typename ET>
constexpr typename std::underlying_type <ET>::type toUnderlying(ET et)
{
	return static_cast<typename std::underlying_type<ET>::type>(et);
}

template <typename ET>
constexpr typename std::underlying_type <ET>::type & toUnderlyingRef(ET &et)
{
	return reinterpret_cast<typename std::underlying_type<ET>::type &>(et);
}

#define EnumClass(EnumName, EnumType, ...) \
	class EnumName { \
		friend std::ostream & operator << (std::ostream &os, EnumName e) \
		{ \
			os << static_cast<const char *>(e); \
			return os; \
		} \
	public: \
		enum EnumName ## __Internal : EnumType { \
			__VA_ARGS__, _size \
		}; \
		constexpr EnumName(EnumName ## __Internal value) : m_value{value} {} \
		constexpr explicit EnumName(EnumType value) : m_value{value} {} \
		constexpr bool operator == (EnumName other) const { return this->m_value == other.m_value; } \
		constexpr bool operator != (EnumName other) const { return this->m_value != other.m_value; } \
		constexpr bool operator == (EnumType other) const { return this->m_value == other; } \
		constexpr bool operator != (EnumType other) const { return this->m_value != other; } \
		operator const char *() { return m_strPack[m_value]; } \
		constexpr explicit operator EnumType() { return m_value; } \
		constexpr EnumType value() const { return m_value; } \
		static std::optional<EnumName> fromString(const char *s) \
		{ \
			for (size_t i = 0; i < m_strPack.size(); ++i) { \
				if (strcmp(m_strPack[i], s) == 0) \
					return EnumName ## __Internal{static_cast<EnumType>(i)}; \
			} \
			return {}; \
		} \
		static std::optional<EnumName> fromString(const std::string_view &s) \
		{ \
			for (size_t i = 0; i < m_strPack.size(); ++i) { \
				if (strncmp(m_strPack[i], s.data(), s.size()) == 0) \
					return EnumName ## __Internal{static_cast<EnumType>(i)}; \
			} \
			return {}; \
		} \
	private: \
		EnumName ## __Internal m_value; \
		static constexpr auto m_strPack = \
			makeStringPack<meta::countTokensTotalLength(#__VA_ARGS__)>( \
				meta::splitTokens<meta::countTokens(#__VA_ARGS__)>(#__VA_ARGS__)); \
	}
