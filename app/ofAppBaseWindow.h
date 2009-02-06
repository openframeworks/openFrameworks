#ifndef _OF_BASE_WINDOW
#define _OF_BASE_WINDOW


#include "ofTypes.h"

class ofBaseApp;


class ofAppBaseWindow{

public:

	ofAppBaseWindow(){};
	virtual ~ofAppBaseWindow(){};

	virtual void setupOpenGL(int w, int h, int screenMode){};
	virtual void initializeWindow(){};
	virtual void runAppViaInfiniteLoop(ofBaseApp * appPtr){};
	
	virtual void hideCursor(){};
	virtual void showCursor(){};
	
	virtual void	setWindowPosition(int x, int y){};
	virtual void	setWindowShape(int w, int h){};
	
	virtual int		getFrameNum(){};
	virtual	float	getFrameRate(){};

	virtual ofPoint	getWindowPosition(){};
	virtual ofPoint	getWindowSize(){};
	virtual ofPoint	getScreenSize(){};
	
	
	virtual void	setFrameRate(float targetRate){};
	virtual void	setWindowTitle(string title){};
	
	virtual void	setFullscreen(bool fullscreen){};
	virtual void	toggleFullscreen(){};
	
	virtual void	enableSetupScreen(){};
	virtual void	disableSetupScreen(){};
	

};

#endif


