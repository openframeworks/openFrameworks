#include "ofWindowManagerX11.h"

#include "ofWindowX11.h"

ofWindowManagerX11::ofWindowManagerX11()
{
}

ofWindowManagerX11::~ofWindowManagerX11()
{
}

ofWindow* ofWindowManagerX11::createWindow()
{
	ofWindow* win = new ofWindowX11();
	return win;
}
void ofWindowManagerX11::init()
{
}

void ofWindowManagerX11::processEvents()
{

}

void ofWindowManagerX11::postDraw()
{
	        //glXSwapBuffers(display, window);
}
