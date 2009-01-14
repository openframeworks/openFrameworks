#ifndef _OF_SIMPLE_APP
#define _OF_SIMPLE_APP



class ofSimpleApp {
	
	public:
        ofSimpleApp() {
            mouseX = mouseY = 0;
		}

		virtual ~ofSimpleApp(){}
		
		virtual void setup(){}
		virtual void update(){}
		virtual void draw(){}
		virtual void exit(){}
		

		virtual void resized(int w, int h){}
		
		virtual void keyPressed( int key ){}
		virtual void keyReleased( int key ){}
		
		virtual void mouseMoved( int x, int y ){}
		virtual void mouseDragged( int x, int y, int button ){}
		virtual void mousePressed( int x, int y, int button ){}
		virtual void mouseReleased(){}
		virtual void mouseReleased(int x, int y, int button ){}
	
		virtual void audioReceived( float * input, int bufferSize, int nChannels ){} 
		virtual void audioRequested( float * output, int bufferSize, int nChannels ){}	

		int mouseX, mouseY;			// for processing heads
};

#endif	


