#ifndef __BITS_H__
#define __BITS_H__

// 1 << n for a bit number that can be out of range: weapon, entity type and similar numbers are -1 for none, and run
// past 31 in places. Shifting by those is undefined: x86 takes n modulo 32, arm's register shifts give 0, and the
// optimiser may assume it never happens. This gives arm's 0, which the game was built for.
static constexpr inline int bit32(int n) {	// (constexpr: some enum constants are made with it)
	return (n >= 0 && n < 32) ? (int)(1u << n) : 0;
}

#endif
