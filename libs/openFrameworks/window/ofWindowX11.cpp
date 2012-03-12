#include "ofWindowX11.h"

static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

ofWindowX11::ofWindowX11()
{
}

ofWindowX11::~ofWindowX11()
{
}

void ofWindowX11::initializeWindow()
{
	Display* disp;
	disp = XOpenDisplay(NULL);
	window = XCreateSimpleWindow(disp, RootWindow(disp, 0), 1, 1, 800, 600, 0, BlackPixel (disp, 0), BlackPixel(disp, 0));

	XVisualInfo *vi = glXChooseVisual(disp, DefaultScreen(disp), dblBuf);

	XMapWindow(disp, window);

	glxContext = glXCreateContext(disp, vi, NULL, GL_TRUE);

	glXMakeCurrent(disp, window, glxContext);

	display = disp;
}

void ofWindowX11::enableContext()
{
	glXMakeCurrent(display, window, glxContext);
}

void ofWindowX11::processEvents()
{
	XEvent event;
	if(XCheckWindowEvent(display, window, ButtonPressMask|KeyPressMask, &event))
    {
        /*if(event.type == ResizeRequest)
        {
            width = event.xresizerequest.width;
            height = event.xresizerequest.height;
        }
        else if (event.type==KeyPress && XLookupString(&event.xkey,text,255,&key,0)==1)
        {
            keyPressed(int(text[0]));
        }
        else if (event.type==ButtonPress)
        {
            mousePressed(event.xbutton.x, event.xbutton.y, event.xbutton.button);
        }*/
    }
}

