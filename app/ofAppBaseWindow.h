#ifndef _OF_BASE_WINDOW
#define _OF_BASE_WINDOW


#include "ofTypes.h"
//#include "ofEvents.h"

class ofBaseApp;


class ofAppBaseWindow{

public:

	ofAppBaseWindow(){};
	virtual ~ofAppBaseWindow(){};

	virtual void setupOpenGL(int w, int h, int screenMode)=0;
	virtual void initializeWindow()=0;
	virtual void runAppViaInfiniteLoop(ofBaseApp * appPtr)=0;

	virtual void hideCursor()=0;
	virtual void showCursor()=0;

	virtual void	setWindowPosition(int x, int y)=0;
	virtual void	setWindowShape(int w, int h)=0;

	virtual int		getFrameNum()=0;

	virtual ofPoint		getWindowPosition()=0;
	virtual ofPoint		getWindowSize()=0;
	virtual ofPoint		getScreenSize()=0;

	virtual int			getWindowMode()=0;
	virtual float		getFrameRate()=0;

	virtual void		setFrameRate(float targetRate)=0;
	virtual void		setWindowTitle(string title)=0;

	virtual void		setFullscreen(bool fullscreen)=0;
	virtual void		toggleFullscreen()=0;

	virtual void		enableSetupScreen()=0;
	virtual void		disableSetupScreen()=0;

	virtual void		exit()=0;

};

// this must be set from every windowing system implementation
extern ofAppBaseWindow * ofAppWindow;

#endif


