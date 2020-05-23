#pragma once

#include <cstdint>

using UID = uint32_t;

class Serial {
public:
	static const UID EmptyUid = 0;
	static const UID MinUid = 1;

	Serial() : nextUid{MinUid} {}

	UID next() { return nextUid++; }
	void reset() { nextUid = MinUid; }

private:
	UID nextUid;
};
