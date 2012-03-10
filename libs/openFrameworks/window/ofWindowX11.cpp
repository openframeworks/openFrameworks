#include "ofWindowX11.h"

ofWindowX11::ofWindowX11()
{
}

ofWindowX11::~ofWindowX11()
{
}

void ofWindowX11::enableContext()
{
}

void ofWindowX11::initializeWindow()
{
	Display* disp;
	disp = XOpenDisplay(NULL);
	window = XCreateSimpleWindow(disp, RootWindow(disp, 0), 1, 1, 800, 600, 0, BlackPixel (disp, 0), BlackPixel(disp, 0));
}
