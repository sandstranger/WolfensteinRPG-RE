#include <stdexcept>

#include "IDIB.h"

IDIB::IDIB() {
}

IDIB::~IDIB() {
	// (its if (this) went: this is never null, and optimisation removed the check)
        if (this->pBmp) {
            free(this->pBmp);
            this->pBmp = nullptr;
        }
        if (this->pRGB888) {
            free(this->pRGB888);
            this->pRGB888 = nullptr;
        }
        if (this->pRGB565) {
            free(this->pRGB565);
            this->pRGB565 = nullptr;
        }
}

void IDIB::dispose(IDIB* dib) {
	if (!dib) {
		return;
	}
	dib->~IDIB();
	std::free(dib);
}
