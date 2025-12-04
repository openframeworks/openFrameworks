#include "ofGraphicsCairo.h"
#if defined(OF_CAIRO)
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

static void ofBeginSaveScreen(const of::filesystem::path & fileName, ofCairoRenderer::Type type, bool bMultipage, bool b3D, ofRectangle outputsize){
	if( bScreenShotStarted ) ofEndSaveScreen();
	
	storedRenderer = ofGetCurrentRenderer();
	
	cairoScreenshot = std::make_unique<ofCairoRenderer>();
	cairoScreenshot->setup(fileName, type, bMultipage, b3D, outputsize);

	rendererCollection = std::make_shared<ofRendererCollection>();
	rendererCollection->renderers.push_back(storedRenderer);
	rendererCollection->renderers.push_back(cairoScreenshot);
	
	ofSetCurrentRenderer(rendererCollection, true);
	cairoScreenshot->background(cairoScreenshot->getStyle().bgColor);
	bScreenShotStarted = true;
}

//-----------------------------------------------------------------------------------
void ofBeginSaveScreenAsPDF(const of::filesystem::path & fileName, bool bMultipage, bool b3D, ofRectangle outputsize){
	ofBeginSaveScreen(fileName, ofCairoRenderer::PDF, bMultipage, b3D, outputsize);
}

//-----------------------------------------------------------------------------------
void ofEndSaveScreenAsPDF(){
	ofEndSaveScreen();
}

//-----------------------------------------------------------------------------------
void ofBeginSaveScreenAsSVG(const of::filesystem::path & fileName, bool bMultipage, bool b3D, ofRectangle outputsize){
	ofBeginSaveScreen(fileName, ofCairoRenderer::SVG, bMultipage, b3D, outputsize);
}

//-----------------------------------------------------------------------------------
void ofEndSaveScreenAsSVG(){
	ofEndSaveScreen();
}
#endif
