#include "ofVideoPlayer.h"
#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofShader.h"


static ofShader & getShaderNV12_NV21();
static ofShader & getShaderPlanarYUV();
static ofShader * getShader(ofPixelFormat pixelFormat);
static string getVertexShaderSource();
static string getFragmentShaderSource(ofPixelFormat pixelFormat);

//---------------------------------------------------------------------------
ofVideoPlayer::ofVideoPlayer (){
	bUseTexture			= true;
	playerTex			= NULL;
	internalPixelFormat = OF_PIXELS_RGB;
	height 				= 0;
	width 				= 0;
	shader 				= NULL;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPlayer(shared_ptr<ofBaseVideoPlayer> newPlayer){
	player = newPlayer;
	setPixelFormat(internalPixelFormat);	//this means that it will try to set the pixel format you have been using before. 
											//if the format is not supported ofVideoPlayer's internalPixelFormat will be updated to that of the player's
}

//---------------------------------------------------------------------------
shared_ptr<ofBaseVideoPlayer> ofVideoPlayer::getPlayer(){
	return player;
}

//we only set pixel format on the player if it exists. 
//if the movie is already loaded then we can't update the format.
//also if the format is not supported we get the format from the player instead.
//--------------------------------------------------------------------
bool ofVideoPlayer::setPixelFormat(ofPixelFormat pixelFormat) {
	if( player ){
		if( player->isLoaded() ){
			ofLogWarning("ofVideoPlayer") << "setPixelFormat(): can't set pixel format of a loaded movie";
			internalPixelFormat = player->getPixelFormat(); 
			return false;
		}else{
			if( player->setPixelFormat(pixelFormat) ){		
				internalPixelFormat = player->getPixelFormat();  //we do this as either way we want the players format
			}else{
				internalPixelFormat = player->getPixelFormat();  //we do this as either way we want the players format
				return false; 					
			}
		}
	}else{
		internalPixelFormat = pixelFormat;	
	}
	return true;
}

//---------------------------------------------------------------------------
ofPixelFormat ofVideoPlayer::getPixelFormat(){
	if( player ){
		internalPixelFormat = player->getPixelFormat();
	}
	return internalPixelFormat;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::loadMovie(string name){
	if( !player ){
		setPlayer( shared_ptr<OF_VID_PLAYER_TYPE>(new OF_VID_PLAYER_TYPE) );
		player->setPixelFormat(internalPixelFormat);
	}
	
	bool bOk = player->loadMovie(name);
	width	 = player->getWidth();
	height	 = player->getHeight();

	if( bOk){
        moviePath = name;
        if(bUseTexture){
        	if(player->getTexture()==NULL){
				if(width!=0 && height!=0) {
					tex.resize(player->getPixelsRef().getNumPlanes());
					for(int i=0;i<player->getPixelsRef().getNumPlanes();i++){
						tex[i].allocate(player->getPixelsRef().getPlane(i));
						if(ofGetGLProgrammableRenderer() && internalPixelFormat == OF_PIXELS_GRAY){
							tex[i].setRGToRGBASwizzles(true);
						}
					}
				}
        	}else{
        		playerTex = player->getTexture();
        	}
        	if(ofIsGLProgrammableRenderer()){
        		shader = getShader(internalPixelFormat);
        		if(shader && !shader->isLoaded()){
        			shader->setupShaderFromSource(GL_VERTEX_SHADER,getVertexShaderSource());
        			shader->setupShaderFromSource(GL_FRAGMENT_SHADER,getFragmentShaderSource(internalPixelFormat));
        			shader->bindDefaults();
        			shader->linkProgram();
        		}
        	}
        }
    }
	
	return bOk;
}

//---------------------------------------------------------------------------
string ofVideoPlayer::getMoviePath(){
    return moviePath;	
}

//---------------------------------------------------------------------------
unsigned char * ofVideoPlayer::getPixels(){
	if( player ){
		return player->getPixels();
	}
	return NULL;	
}

//---------------------------------------------------------------------------
ofPixelsRef ofVideoPlayer::getPixelsRef(){
	return player->getPixelsRef();
}

//---------------------------------------------------------------------------
//for getting a reference to the texture
ofTexture & ofVideoPlayer::getTextureReference(int plane){
	if(playerTex == NULL){
		return tex[plane];
	}
	else{
		return (*playerTex)[plane];
	}
}

ofVec2f ofVideoPlayer::getTextureScale(int plane){
	ofClamp(plane,0,tex.size()-1);
	return ofVec2f(getTextureReference(plane).getWidth()/getWidth(),getTextureReference(plane).getHeight()/getHeight());
}


//---------------------------------------------------------------------------
bool ofVideoPlayer::isFrameNew(){
	if( player ){
		return player->isFrameNew();
	}
	return false;
}

//--------------------------------------------------------------------
void ofVideoPlayer::update(){
	if( player ){

		player->update();
		width = player->getWidth();
		height = player->getHeight();
		
		if( bUseTexture && player->isFrameNew() ) {
			
			playerTex = player->getTexture();
			
			if(playerTex == NULL){
				tex.resize(player->getPixelsRef().getNumPlanes());
				if(player->getWidth() != 0 && player->getHeight() != 0) {
					for(int i=0;i<player->getPixelsRef().getNumPlanes();i++){
						ofPixels plane = player->getPixelsRef().getPlane(i);
						bool bDiffPixFormat = ( tex[i].bAllocated() && tex[i].texData.glTypeInternal != ofGetGLInternalFormatFromPixelFormat(plane.getPixelFormat()) );
						if(width==0 || height==0 || bDiffPixFormat ){
							tex[i].allocate(plane);
							if(ofGetGLProgrammableRenderer() && internalPixelFormat == OF_PIXELS_GRAY){
								tex[i].setRGToRGBASwizzles(true);
							}
						}
						tex[i].loadData(plane);
					}
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::closeMovie(){
	close();
}

//---------------------------------------------------------------------------
void ofVideoPlayer::close(){
	if( player ){
		player->close();
	}
	tex.clear();
}

//--------------------------------------------------------
void ofVideoPlayer::play(){
	if( player ){
		player->play();
	}
}

//--------------------------------------------------------
void ofVideoPlayer::stop(){
	if( player ){
		player->stop();
	}
}

//--------------------------------------------------------
void ofVideoPlayer::setVolume(float volume){
	if( player ){
		if ( volume > 1.0f ){
			ofLogWarning("ofVideoPlayer") << "setVolume(): expected range is 0-1, limiting requested volume " << volume << " to 1.0";
			volume = 1.0f;
		}
		player->setVolume(volume);
	}
}


//--------------------------------------------------------
void ofVideoPlayer::setLoopState(ofLoopType state){
	if( player ){
		player->setLoopState(state);
	}
}

ofLoopType ofVideoPlayer::getLoopState(){
	if( player ){
		return player->getLoopState();
	}else{
		return OF_LOOP_NONE;
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPosition(float pct){
	if( player ){
		player->setPosition(pct);
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setFrame(int frame){
	if( player ){
		player->setFrame(frame);
	}
}


//---------------------------------------------------------------------------
float ofVideoPlayer::getDuration(){
	if( player ){
		return player->getDuration();
	}
	
	return 0.0;
}

//---------------------------------------------------------------------------
float ofVideoPlayer::getPosition(){
	if( player ){
		return player->getPosition();
	}
	return 0.0;
}

//---------------------------------------------------------------------------
int ofVideoPlayer::getCurrentFrame(){
	if( player ){
		return player->getCurrentFrame();
	}
	return 0;
}


//---------------------------------------------------------------------------
bool ofVideoPlayer::getIsMovieDone(){
	if( player ){
		return player->getIsMovieDone();
	}
	return false;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::firstFrame(){
	if( player ){
		player->firstFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::nextFrame(){
	if( player ){
		player->nextFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::previousFrame(){
	if( player ){
		player->previousFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setSpeed(float _speed){
	if( player ){
		player->setSpeed(_speed);
	}
}

//---------------------------------------------------------------------------
float ofVideoPlayer::getSpeed(){
	if( player ){
		return player->getSpeed();
	}
	return 0.0;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPaused(bool _bPause){
	if( player ){
		player->setPaused(_bPause);
	}
}

//------------------------------------
void ofVideoPlayer::setUseTexture(bool bUse){
	bUseTexture = bUse;
	if(bUse && player && !player->getTexture() && getWidth()!=0 && getHeight()!=0){
		for(int i=0;i<player->getPixelsRef().getNumPlanes();i++){
			ofPixels plane = player->getPixelsRef().getPlane(i);
			bool bDiffPixFormat = ( tex[i].bAllocated() && tex[i].texData.glTypeInternal != ofGetGLInternalFormatFromPixelFormat(plane.getPixelFormat()) );
			if(!tex[i].isAllocated() || bDiffPixFormat){
				tex[i].allocate(plane);
			}
			if(ofGetGLProgrammableRenderer() && internalPixelFormat == OF_PIXELS_GRAY){
				tex[i].setRGToRGBASwizzles(true);
			}
		}
	}
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPercent(float xPct, float yPct){
	getTextureReference().setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPoint(float x, float y){
	getTextureReference().setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofVideoPlayer::resetAnchor(){
	getTextureReference().resetAnchor();
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y, float _w, float _h){
	if(shader){
		shader->begin();

		switch(internalPixelFormat){
			case OF_PIXELS_YUY2:
				//TODO
				break;
			case OF_PIXELS_NV12:
			case OF_PIXELS_NV21:
				shader->setUniformTexture("Ytex",getTextureReference(0),0);
				shader->setUniformTexture("UVtex",getTextureReference(1),1);
				shader->setUniform2f("tex_scaleY",getTextureScale(0).x,getTextureScale(0).y);
				shader->setUniform2f("tex_scaleUV",getTextureScale(1).x,getTextureScale(1).y);
				break;
			case OF_PIXELS_YV12:
				shader->setUniformTexture("Ytex",getTextureReference(0),0);
				shader->setUniformTexture("Utex",getTextureReference(2),1);
				shader->setUniformTexture("Vtex",getTextureReference(1),2);
				shader->setUniform2f("tex_scaleY",getTextureScale(0).x,getTextureScale(0).y);
				shader->setUniform2f("tex_scaleU",getTextureScale(2).x,getTextureScale(2).y);
				shader->setUniform2f("tex_scaleV",getTextureScale(1).x,getTextureScale(1).y);
				break;
			case OF_PIXELS_I420:
				shader->setUniformTexture("Ytex",getTextureReference(0),0);
				shader->setUniformTexture("Utex",getTextureReference(1),1);
				shader->setUniformTexture("Vtex",getTextureReference(2),2);
				shader->setUniform2f("tex_scaleY",getTextureScale(0).x,getTextureScale(0).y);
				shader->setUniform2f("tex_scaleU",getTextureScale(1).x,getTextureScale(1).y);
				shader->setUniform2f("tex_scaleV",getTextureScale(2).x,getTextureScale(2).y);
				break;
			default:
				break;
		}
	}
	getTextureReference().draw(_x, _y, _w, _h);	
	if(shader){
		shader->end();
	}
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y){
	draw(_x, _y, width, height);
}

//------------------------------------
int ofVideoPlayer::getTotalNumFrames(){
	if( player ){
		return player->getTotalNumFrames();
	}
	return 0;
}

//----------------------------------------------------------
float ofVideoPlayer::getWidth(){
	if( player ){
		width = player->getWidth();
	}
	return (float)width;
}

//----------------------------------------------------------
float ofVideoPlayer::getHeight(){
	if( player ){
		height = player->getHeight();
	}
	return (float)height;
}

//----------------------------------------------------------
bool ofVideoPlayer::isPaused(){
	if( player ){
		return player->isPaused();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isLoaded(){
	if( player ){
		return player->isLoaded();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isPlaying(){
	if( player ){
		return player->isPlaying();
	}
	return false;
}


#define stringify(x) string("#version 440\n")+#x

static string VERTEX_SHADER = stringify(
	uniform mat4 projectionMatrix;
	uniform mat4 modelViewMatrix;
	uniform mat4 textureMatrix;
	uniform mat4 modelViewProjectionMatrix;

	in vec4  position;
	in vec2  texcoord;
	in vec4  color;
	in vec3  normal;

	out vec4 colorVarying;
	out vec2 texCoordVarying;
	out vec4 normalVarying;

	void main()
	{
		colorVarying = color;
		texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
		gl_Position = modelViewProjectionMatrix * position;
	}
);

static string FRAGMENT_SHADER_YUY2 = stringify(
	uniform sampler2DRect src_tex_unit0;
	uniform vec4 globalColor;

	in vec4 colorVarying;
	in vec2 texCoordVarying;

	out vec4 fragColor;) + ""

    "const vec3 offset = vec3(-0.0625, -0.5, -0.5);\n"
    "const vec3 rcoeff = vec3(1.164, 0.000, 1.596);\n"
    "const vec3 gcoeff = vec3(1.164,-0.391,-0.813);\n"
    "const vec3 bcoeff = vec3(1.164, 2.018, 0.000);\n"


	"void main(){\n"
    "    float r,g,b;\n"
    "    vec3 yuv;\n"
	"    int texX = int(texCoordVarying.x);\n"
	"    yuv.x=texture(src_tex_unit0,texCoordVarying).r;\n"
	"    if(texX%2==1){\n"
    "        yuv.y=texture(src_tex_unit0,vec2(texCoordVarying.x-1.0,texCoordVarying.y)).g;\n"
    "        yuv.z=texture(src_tex_unit0,texCoordVarying).g;\n"
	"    }else{\n"
	"        yuv.y=texture(src_tex_unit0,texCoordVarying).g;\n"
	"        yuv.z=texture(src_tex_unit0,vec2(texCoordVarying.x+1.0,texCoordVarying.y)).g;\n"
	"    }\n"
    "    yuv += offset;\n"
    "    r = dot(yuv, rcoeff);\n"
    "    g = dot(yuv, gcoeff);\n"
    "    b = dot(yuv, bcoeff);\n"
    "    fragColor=vec4(r,g,b,1.0) * globalColor;\n"
	"}\n";

static string FRAGMENT_SHADER_NV12_NV21 = stringify(
	uniform sampler2DRect Ytex;
	uniform sampler2DRect UVtex;
	uniform vec4 globalColor;

	in vec4 colorVarying;
	in vec2 texCoordVarying;

	out vec4 fragColor;) + ""

    "const vec3 offset = vec3(-0.0625, -0.5, -0.5);\n"
    "const vec3 rcoeff = vec3(1.164, 0.000, 1.596);\n"
    "const vec3 gcoeff = vec3(1.164,-0.391,-0.813);\n"
    "const vec3 bcoeff = vec3(1.164, 2.018, 0.000);\n"


	"void main(){\n"
    "    float r,g,b;\n"
    "    vec3 yuv;\n"
	"    yuv.x=texture(Ytex,texCoordVarying).r;\n"
	"    yuv.yz=texture(UVtex,texCoordVarying * vec2(0.5,0.5)).%r%g;\n"
    "    yuv += offset;\n"
    "    r = dot(yuv, rcoeff);\n"
    "    g = dot(yuv, gcoeff);\n"
    "    b = dot(yuv, bcoeff);\n"
    "    fragColor=vec4(r,g,b,1.0) * globalColor;\n"
	"}\n";

static string FRAGMENT_SHADER_PLANAR_YUV = stringify(
	uniform sampler2DRect Ytex;
	uniform sampler2DRect Utex;
	uniform sampler2DRect Vtex;
    uniform vec2 tex_scaleY;
    uniform vec2 tex_scaleU;
    uniform vec2 tex_scaleV;
	uniform vec4 globalColor;

	in vec4 colorVarying;
	in vec2 texCoordVarying;

	out vec4 fragColor;) + ""

    "const vec3 offset = vec3(-0.0625, -0.5, -0.5);\n"
    "const vec3 rcoeff = vec3(1.164, 0.000, 1.596);\n"
    "const vec3 gcoeff = vec3(1.164,-0.391,-0.813);\n"
    "const vec3 bcoeff = vec3(1.164, 2.018, 0.000);\n"


	"void main(){\n"
    "    float r,g,b;\n"
    "    vec3 yuv;\n"
	"    yuv.x=texture(Ytex,texCoordVarying * tex_scaleY).r;\n"
	"    yuv.y=texture(Utex,texCoordVarying * tex_scaleU).r;\n"
	"    yuv.z=texture(Vtex,texCoordVarying * tex_scaleV).r;\n"
    "    yuv += offset;\n"
    "    r = dot(yuv, rcoeff);\n"
    "    g = dot(yuv, gcoeff);\n"
    "    b = dot(yuv, bcoeff);\n"
    "    fragColor=vec4(r,g,b,1.0) * globalColor;\n"
	"}\n";

static ofShader & getShaderPlanarYUY2(){
	static ofShader * shader = new ofShader;
	return *shader;
}

static ofShader & getShaderNV12_NV21(){
	static ofShader * shader = new ofShader;
	return *shader;
}

static ofShader & getShaderPlanarYUV(){
	static ofShader * shader = new ofShader;
	return *shader;
}

static ofShader * getShader(ofPixelFormat pixelFormat){
	ofShader * shader = NULL;
	switch(pixelFormat){
		case OF_PIXELS_YUY2:
			shader = &getShaderPlanarYUY2();
			break;
		case OF_PIXELS_NV12:
		case OF_PIXELS_NV21:
			shader = &getShaderNV12_NV21();
			break;
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			shader = &getShaderPlanarYUV();
			break;
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
		case OF_PIXELS_RGB565:
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
		case OF_PIXELS_GRAY:
		default:
			break;
	}
	return shader;
}


static string getVertexShaderSource(){
	return VERTEX_SHADER;
}

static string getFragmentShaderSource(ofPixelFormat pixelFormat){
	string src;
	switch(pixelFormat){
		case OF_PIXELS_YUY2:
			src = FRAGMENT_SHADER_YUY2;
			break;
		case OF_PIXELS_NV12:
			src = FRAGMENT_SHADER_NV12_NV21;
			ofStringReplace(src,"%r%g","rg");
			break;
		case OF_PIXELS_NV21:
			src = FRAGMENT_SHADER_NV12_NV21;
			ofStringReplace(src,"%r%g","gr");
			break;
		case OF_PIXELS_YV12:
		case OF_PIXELS_I420:
			src = FRAGMENT_SHADER_PLANAR_YUV;
			break;
		case OF_PIXELS_RGB:
		case OF_PIXELS_BGR:
		case OF_PIXELS_RGB565:
		case OF_PIXELS_RGBA:
		case OF_PIXELS_BGRA:
		case OF_PIXELS_GRAY:
		default:
			break;
	}
	return src;
}
