#pragma once
#include "..\stdafx.h"

#if defined(_WIN32)
#define likely(x)       x
#define unlikely(x)     x
#else
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif

namespace Pattern
{
	using Range_t = std::pair<size_t, size_t>;
	extern Range_t Textsegment;
	extern Range_t Datasegment;

	// Find a single pattern in the range.
	size_t _Findpattern(Range_t &Range, std::vector<uint8_t> Pattern, std::vector<uint8_t> Mask);

	// Scan until the end of the range and return all result.
	std::vector<size_t> Findpatterns(Range_t &Range, std::vector<uint8_t> Pattern, std::vector<uint8_t> Mask);

	// Create a pattern or mask from a readable string.
	std::vector<uint8_t> Stringtopattern(std::string Humanreadable);
	std::vector<uint8_t> Stringtomask(std::string Humanreadable);
}

#define Findpattern(Segment, String) Pattern::_Findpattern(Segment, Pattern::Stringtopattern(String), Pattern::Stringtomask(String))