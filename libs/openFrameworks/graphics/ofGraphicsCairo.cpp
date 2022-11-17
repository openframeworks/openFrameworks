#include "ofGraphicsCairo.h"
#include "ofRendererCollection.h"
#include "ofCairoRenderer.h"

static std::shared_ptr<ofCairoRenderer> cairoScreenshot;
static std::shared_ptr<ofBaseRenderer> storedRenderer;
static std::shared_ptr<ofRendererCollection> rendererCollection;
static bool bScreenShotStarted = false;


static void ofEndSaveScreen(){
	if( bScreenShotStarted ){

		if( cairoScreenshot ){
			cairoScreenshot->close();
			rendererCollection.reset();
			cairoScreenshot.reset();
		}
		if( storedRenderer ){
			ofSetCurrentRenderer(storedRenderer,true);
			storedRenderer.reset();
		}

		bScreenShotStarted = false;
	}

}

static void ofBeginSaveScreen(std::string filename, ofCairoRenderer::Type type, bool bMultipage, bool b3D, ofRectangle outputsize){
	if( bScreenShotStarted ) ofEndSaveScreen();
	
	storedRenderer = ofGetCurrentRenderer();
	
	cairoScreenshot = std::make_unique<ofCairoRenderer>();
	cairoScreenshot->setup(filename, type, bMultipage, b3D, outputsize);

	rendererCollection = std::make_shared<ofRendererCollection>();
	rendererCollection->renderers.push_back(storedRenderer);
	rendererCollection->renderers.push_back(cairoScreenshot);
	
	ofSetCurrentRenderer(rendererCollection, true);
	cairoScreenshot->background(cairoScreenshot->getStyle().bgColor);
	bScreenShotStarted = true;
}

//-----------------------------------------------------------------------------------
void ofBeginSaveScreenAsPDF(std::string filename, bool bMultipage, bool b3D, ofRectangle outputsize){
	ofBeginSaveScreen(filename, ofCairoRenderer::PDF, bMultipage, b3D, outputsize);
}

//-----------------------------------------------------------------------------------
void ofEndSaveScreenAsPDF(){
	ofEndSaveScreen();
}

//-----------------------------------------------------------------------------------
void ofBeginSaveScreenAsSVG(std::string filename, bool bMultipage, bool b3D, ofRectangle outputsize){
	ofBeginSaveScreen(filename, ofCairoRenderer::SVG, bMultipage, b3D, outputsize);
}

//-----------------------------------------------------------------------------------
void ofEndSaveScreenAsSVG(){
	ofEndSaveScreen();
}