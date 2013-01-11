/*
    BMP texture loader
    � Keith O'Conor 2005
    keith.oconor @ {cs.tcd.ie, gmail.com}

    adapted for openFrameworks by Theodore Watson - theo@openframeworks.cc
*/

#include "texture3DS.h"

texture3DS::texture3DS(string filename, const int textureId){
	
    ofImage img;
    if( !img.loadImage(filename) ){
		ofLog(OF_LOG_ERROR, "texture3DS ERROR:  Could not open %s", filename.c_str());
    }

    m_width     = img.width;
    m_height    = img.height;
    m_bpp       = img.bpp;

    if( m_width <= 0 || m_height <= 0){
		ofLog(OF_LOG_ERROR, "texture3DS ERROR: Something wrong with %s - dimensions less than 0", filename.c_str());

    }

    if(m_bpp != 32 && m_bpp != 24){		
		ofLog(OF_LOG_ERROR, "texture3DS ERROR: Invalid texture color depth %s  must be uncompressed 24/32bpp png, jpg, bmp, tga", filename.c_str());
        return;
    }

    // Determine format
    int fileFormat, internalFormat;
    switch(m_bpp){
        case 24:fileFormat = GL_RGB; internalFormat = GL_RGB; break;
        case 32:fileFormat = GL_RGBA; internalFormat = GL_RGBA; break;
        default:
			ofLog(OF_LOG_ERROR, "texture3DS ERROR: Invalid texture color depth %s  must be uncompressed 24/32bpp", filename.c_str());

			return;
            break;
    }


    // FLIP THE PIXELS
    //we need to flip the image vertically
    unsigned char * ptrToPixels = img.getPixels();
    int bytesPP = (m_bpp / 8);

	//this extra allocation is because of a glu bug - http://osdir.com/ml/video.mesa3d.devel/2005-02/msg00035.html
    unsigned char * flippedPixels = new unsigned char[bytesPP * m_width * (m_height+1)];
    unsigned char * ptrToFlipped = flippedPixels;

    int numBytesPerLine = m_width * bytesPP;

    //go to the beginning of the bottom line of pixels of the image
    ptrToPixels += ( numBytesPerLine * (m_height-1) );

    //then step through line at a time copying the whole line from the original
    //to the flipped pixels
    //- the original starts at the bottom and goes up
    //- the flipped starts at the top and goes down
    for(int y = 0; y < m_height; y++){
        memcpy(ptrToFlipped, ptrToPixels, numBytesPerLine * sizeof( char ) );
        ptrToPixels   -= numBytesPerLine;
        ptrToFlipped  += numBytesPerLine;
    }


	// Bind texture ID to load
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Upload texture to card with bound texture ID
#ifdef TARGET_OPENGLES
    glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, fileFormat, GL_UNSIGNED_BYTE, flippedPixels);
#else
    gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, m_width, m_height, fileFormat, GL_UNSIGNED_BYTE, flippedPixels);
#endif

    ofLog(OF_LOG_NOTICE, "texture3DS Texture %s loaded", filename.c_str());
}
