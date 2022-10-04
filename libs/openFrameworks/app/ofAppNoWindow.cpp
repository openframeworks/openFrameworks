#include "ofAppNoWindow.h"
#include "ofBaseApp.h"
#include "ofGraphics.h"
#include "ofPath.h"
#include "of3dGraphics.h"
#include <memory>


#if defined TARGET_OSX || defined TARGET_LINUX
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <signal.h>

struct termios orig_termios;
struct sigaction act_open;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}


void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
	// setup new_termios for raw keyboard input
    cfmakeraw(&new_termios);
	// handle "\n" properly
	new_termios.c_oflag |= OPOST;
	//new_termios.c_oflag |= ONLCR;
	// set the new_termios
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
	return 0;
//    struct timeval tv = { 0L, 0L };
//    fd_set fds;
//    FD_SET(0, &fds);
//    return select(1, &fds, nullptr, nullptr, &tv);
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

#endif

class ofNoopRenderer: public ofBaseRenderer{
public:
	ofNoopRenderer():graphics3d(this){}
private:
	static const std::string TYPE;
	const std::string & getType(){return TYPE;}

	void startRender(){}
	void finishRender(){}
	void update(){}

	void draw(const ofPolyline & poly) const{}
	void draw(const ofPath & shape) const{}
	void draw(const of3dPrimitive&, ofPolyRenderMode) const{}
	void draw(const ofNode&) const{}
	void draw(const ofMesh & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const{}
	void draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{}
	void draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{}
	void draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{}
	void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const{}

	//--------------------------------------------
	// transformations
	void pushView(){};
	void popView(){};

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport){};
	void viewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true){};
	void setupScreenPerspective(float width = 0, float height = 0, float fov = 60, float nearDist = 0, float farDist = 0){}
	void setupScreenOrtho(float width = 0, float height = 0, float nearDist = -1, float farDist = 1){};
	ofRectangle getCurrentViewport() const{return ofRectangle();};
	ofRectangle getNativeViewport() const{return ofRectangle();};
	int getViewportWidth() const{return 0;};
	int getViewportHeight() const{return 0;};

	void setCoordHandedness(ofHandednessType handedness){};
	ofHandednessType getCoordHandedness() const{return OF_LEFT_HANDED;};

	//our openGL wrappers
	void pushMatrix(){};
	void popMatrix(){};
	void translate(float x, float y, float z = 0){};
	void translate(const glm::vec3 & p){};
	void scale(float xAmnt, float yAmnt, float zAmnt = 1){};
	void rotateRad(float radians, float vecX, float vecY, float vecZ){};
	void rotateXRad(float radians){};
	void rotateYRad(float radians){};
	void rotateZRad(float radians){};
	void rotateRad(float radians){};

	// screen coordinate things / default gl values
	void setupGraphicDefaults(){};
	void setupScreen(){};

	void setOrientation(ofOrientation, bool){}
	bool isVFlipped() const{return true;}
	glm::mat4 getCurrentMatrix(ofMatrixMode) const{return glm::mat4(1.0);};
	glm::mat4 getCurrentOrientationMatrix() const{return glm::mat4(1.0);}
	void matrixMode(ofMatrixMode){}
	void loadIdentityMatrix(){}
	void loadMatrix(const glm::mat4&){}
	void loadMatrix(const float*){}
	void multMatrix(const glm::mat4&){}
	void multMatrix(const float*){}
	void loadViewMatrix(const glm::mat4&){}
	void multViewMatrix(const glm::mat4&){}
	glm::mat4 getCurrentViewMatrix() const{return glm::mat4(1.0);}
	glm::mat4 getCurrentNormalMatrix() const{return glm::mat4(1.0);}
	void enableAntiAliasing(){}
	void disableAntiAliasing(){}


	// drawing modes
	void setRectMode(ofRectMode mode){}
	ofRectMode getRectMode(){return OF_RECTMODE_CORNER;}
	void setFillMode(ofFillFlag fill){}
	ofFillFlag getFillMode(){return OF_FILLED;}
	void setLineWidth(float lineWidth){}
	void setBlendMode(ofBlendMode blendMode){}
	void setLineSmoothing(bool smooth){}
	void setCircleResolution(int res){};
	void enablePointSprites(){};
	void disablePointSprites(){};
	void setDepthTest(bool){};

	// color options
	void setColor(int r, int g, int b){}; // 0-255
	void setColor(int r, int g, int b, int a){}; // 0-255
	void setColor(const ofColor & color){};
	void setColor(const ofColor & color, int _a){};
	void setColor(int gray){}; // new set a color as grayscale with one argument
	void setHexColor( int hexColor ){}; // hex, like web 0xFF0033;

	// bg color
	ofColor getBackgroundColor(){return ofColor(200);}
	void setBackgroundColor(const ofColor & color){}
	bool getBackgroundAuto(){
		return true;
	}
	void background(const ofColor & c){};
	void background(float brightness){};
	void background(int hexColor, float _a=255.0f){};
	void background(int r, int g, int b, int a=255){};

	void setBackgroundAuto(bool bManual){};		// default is true

	void clear(){};
	void clear(float r, float g, float b, float a=0){};
	void clear(float brightness, float a=0){};
	void clearAlpha(){};

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const{}
	void drawRectangle(float x, float y, float z, float w, float h) const{}
	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const{}
	void drawCircle(float x, float y, float z, float radius) const{}
	void drawSphere(float x, float y, float z, float radius) const{}
	void drawEllipse(float x, float y, float z, float width, float height) const{}
	void drawString(std::string text, float x, float y, float z) const{}
	void drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const{}

	void setBitmapTextMode(ofDrawBitmapMode mode){}
	ofStyle getStyle() const{ return ofStyle(); }
	void pushStyle(){}
	void popStyle(){}
	void setStyle(const ofStyle & style){}
	void setCurveResolution(int res){}
	void setPolyMode(ofPolyWindingMode){}

	void bind(const ofCamera & camera, const ofRectangle & viewport){};
	void unbind(const ofCamera & camera){};
	ofPath & getPath(){ return path; }
	const of3dGraphics & get3dGraphics() const{
		return graphics3d;
	}

	of3dGraphics & get3dGraphics(){
		return graphics3d;
	}
	of3dGraphics graphics3d;
	ofPath path;
};


const std::string ofNoopRenderer::TYPE="NOOP";

//----------------------------------------------------------
ofAppNoWindow::ofAppNoWindow()
:coreEvents(new ofCoreEvents)
,currentRenderer(new ofNoopRenderer){
	ofAppPtr = nullptr;
	width = 0;
	height = 0;
}


//----------------------------------------------------------
void ofAppNoWindow::setup(const ofWindowSettings & settings){
	width = settings.getWidth();
	height = settings.getHeight();
}

//----------------------------------------------------------
void ofAppNoWindow::update(){

    /// listen for escape
    #ifdef TARGET_WIN32
    if (GetAsyncKeyState(VK_ESCAPE))
    	events().notifyKeyPressed(OF_KEY_ESC);
    #endif

	#if defined TARGET_OSX || defined TARGET_LINUX
	while ( kbhit() )
	{
		int key = getch();
		if ( key == 27 )
		{
			events().notifyKeyPressed(OF_KEY_ESC);
		}
		else if ( key == /* ctrl-c */ 3 )
		{
			ofLogNotice("ofAppNoWindow") << "Ctrl-C pressed" << std::endl;
			break;
		}
		else
		{
			events().notifyKeyPressed(key);
		}
	}
	#endif


	events().notifyUpdate();
}

//----------------------------------------------------------
void ofAppNoWindow::draw(){
	events().notifyDraw();
}

//------------------------------------------------------------
void ofAppNoWindow::exitApp(){
	ofLogVerbose("ofAppNoWindow") << "terminating headless (no window) app!";


#if defined TARGET_OSX || defined TARGET_LINUX
    // this doesn't exist on windows and gives linking errors, so commented out.
	reset_terminal_mode();
#endif

	OF_EXIT_APP(0);
}

//----------------------------------------------------------
glm::vec2 ofAppNoWindow::getWindowPosition(){
	return {0.f, 0.f};
}

//----------------------------------------------------------
glm::vec2 ofAppNoWindow::getWindowSize(){
	return {width, height};
}

//----------------------------------------------------------
glm::vec2 ofAppNoWindow::getScreenSize(){
	return {width, height};
}


//----------------------------------------------------------
int	ofAppNoWindow::getWidth(){
	return width;
}

//----------------------------------------------------------
int	ofAppNoWindow::getHeight(){
	return height;
}


//----------------------------------------------------------
ofCoreEvents & ofAppNoWindow::events(){
	return *coreEvents;
}

std::shared_ptr<ofBaseRenderer> & ofAppNoWindow::renderer(){
	return currentRenderer;
}

