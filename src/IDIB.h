#ifndef __IDIB_H__
#define __IDIB_H__

class IDIB
{
private:

public:
	uint8_t* pBmp;
	uint32_t* pRGB888;
	uint16_t* pRGB565;
	unsigned int cntRGB;
	unsigned int nColorScheme;
	unsigned int ncTransparent;
	int width;
	int height;
	int depth;
	unsigned int pitch;

	// Constructor
	IDIB();
	// Destructor
	~IDIB();
	static void dispose(IDIB* dib);	// its buffers and itself (malloc'd): dib can be null
};

#endif