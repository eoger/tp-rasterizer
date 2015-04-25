// --------------------------------------------------------------------------- INCLUDES HEADER

#include <stdlib.h>

#include "Image_png.h"
#include "png.h"


// --------------------------------------------------------------------------- PNG FUNCTION

namespace Image
{
	
	
	bool savePng ( FILE * pFile,
				 const unsigned char * aPixels,
				 unsigned int width,
				 unsigned int height,
				 unsigned int chanelCount,
				 unsigned int bytePerChanel,
				 bool flipY )
	{
		png_structp PngWrite ;
		png_infop PngInfo ;
		png_byte ** RowPointers ;
		unsigned char * Pixels16bit = NULL ;
		
		PngWrite = png_create_write_struct (PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);
		
		if ( !PngWrite )
		{
			return false;
		}
		
		PngInfo = png_create_info_struct (PngWrite);
		
		if ( !PngInfo )
		{
			png_destroy_write_struct (&PngWrite, (png_infopp)NULL);
			return false;
		}
		
		if ( setjmp (png_jmpbuf (PngWrite)) )
		{
			png_destroy_write_struct (&PngWrite, &PngInfo);
			return false;
		}
		
		png_init_io (PngWrite, pFile);
		
		switch ( chanelCount )
		{
			case 4:
				png_set_IHDR (PngWrite, PngInfo, width, height, 8 * bytePerChanel, PNG_COLOR_TYPE_RGBA,
							  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
				break;
				
			case 3:
				png_set_IHDR (PngWrite, PngInfo, width, height, 8 * bytePerChanel, PNG_COLOR_TYPE_RGB,
							  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
				break;
				
			case 2:
				png_set_IHDR (PngWrite, PngInfo, width, height, 8 * bytePerChanel, PNG_COLOR_TYPE_GRAY_ALPHA,
							  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
				break;
				
			case 1:
				png_set_IHDR (PngWrite, PngInfo, width, height, 8 * bytePerChanel, PNG_COLOR_TYPE_GRAY,
							  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
				break;
				
			default:
				png_destroy_write_struct (&PngWrite, &PngInfo);
				return false;
		}
		
		RowPointers = new png_byte * [ height ];
		
		if ( bytePerChanel == 2 )
		{
			unsigned int stride = width * chanelCount * 2 ;
			unsigned int size = width * height * chanelCount ;
			
			Pixels16bit = (unsigned char *) malloc ( size * 2 );
			
			for ( unsigned int i = 0 ; i < size ; i ++ )
			{
				Pixels16bit[2 * i] = aPixels[2 * i + 1] ;
				Pixels16bit[2 * i + 1] = aPixels[2 * i] ;
			}
			
			for ( unsigned int i = 0 ; i < height ; i++ )
			{
				RowPointers[i] = (png_byte *) (Pixels16bit + i * stride);
			}
		}
		else
		{
			unsigned int stride = width * chanelCount;
			
			for ( unsigned int i = 0 ; i < height ; i ++ )
			{
				RowPointers[i] = (png_byte *) (aPixels + i * stride);
			}
		}
		
		png_set_rows (PngWrite, PngInfo, RowPointers);
		png_write_png (PngWrite, PngInfo, 0, NULL);
		
		png_write_end (PngWrite, PngInfo);
		
		png_destroy_write_struct (&PngWrite, &PngInfo);
		
		delete [ ] RowPointers;
		
		if ( Pixels16bit )
		{
			free ( Pixels16bit );
		}
		
		return true;
	}
	
	bool savePng ( const char * chFilePath,
				 const unsigned char * aPixels,
				 unsigned int width,
				 unsigned int height,
				 unsigned int chanelCount,
				 unsigned int bytePerChanel,
				 bool flipY )
	{
		FILE * pFile = fopen ( chFilePath, "wb" );
		
		if ( !pFile )
		{
			return false;
		}
		
		if ( !Image::savePng (pFile, aPixels, width, height, chanelCount, bytePerChanel, flipY ) )
		{
			fclose ( pFile );
			
			return false;
		}
		
		fclose ( pFile );
		
		return true;
	}
}



