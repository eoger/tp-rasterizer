#ifndef IMAGE_PNG
#define IMAGE_PNG

// --------------------------------------------------------------------------- INCLUDE

#include <stdio.h>


// --------------------------------------------------------------------------- CImage::Formats

namespace Image
{
	bool savePng ( FILE * pFile,
				 const unsigned char * aPixels,
				 unsigned int width,
				 unsigned int height,
				 unsigned int chanelCount,
				 unsigned int bytePerChanel,
				 bool flipY = false );
	
	bool savePng ( const char * chFilePath,
				 const unsigned char * aPixels,
				 unsigned int width,
				 unsigned int height,
				 unsigned int chanelCount,
				 unsigned int bytePerChanel,
				 bool flipY = false );
	
};

#endif
