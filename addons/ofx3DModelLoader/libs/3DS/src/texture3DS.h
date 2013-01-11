#pragma once

#include "ofMain.h"


class texture3DS{
private:
    int m_width;
    int m_height;
    int m_bpp;
    int m_format;

public:
	texture3DS(std::string filename, const int textureId);

    int getWidth(){return m_width;}
    int getHeight(){return m_width;}
    int getBpp(){return m_bpp;}

};

