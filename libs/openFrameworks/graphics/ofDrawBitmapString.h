#pragma once

#include "ofPoint.h"
#include "ofConstants.h"

//------------------------------------------------------------------------------
/// \class ofDrawBitmapStream
/// \brief a stream interface to draw a bitmap string
///
/// ofDrawBitmapStream accepts variables via the ostream operator <<, builds a string,
/// and logs it when the stream is finished (via the destructor). All the stream
/// controls work (endl, flush, hex, etc).
///
/// Usage: ofDrawBitmapString(10, 10) << "a string" << 100 << 20.234f;
///    
/// It also accepts the legacy ofDrawBitmapString interface: 
///
/// Usage: ofDrawBitmapString("another string", 10, 10);
///
/// \note: this uses glDrawPixels and may be S L 0 W on some graphics cards ...
///
/// class idea from:
/// 	http://www.gamedev.net/community/forums/topic.asp?topic_id=525405&whichpage=1&#3406418
/// how to catch std::endl (which is actually a func pointer):
/// 	http://yvan.seth.id.au/Entries/Technology/Code/std__endl.html
///
/// \author Dan Wilcox <danomatika@gmail.com> danomatika.com
///
class ofDrawBitmapString{
    public:

		/// set the position
		ofDrawBitmapString(const ofPoint & p);
		ofDrawBitmapString(float x, float y, float z=0.0f);
		
		/// legacy function compatibility
		ofDrawBitmapString(string textString, const ofPoint & p);
		ofDrawBitmapString(string textString, float x, float y, float z=0.0f);
		
		/// does the actual printing on when the ostream is done
        ~ofDrawBitmapString();
		
		/// catch the << ostream with a template class to read any type of data
        template <class T> 
		ofDrawBitmapString& operator<<(const T& value){
            message << value;
            return *this;
        }

        /// catch the << ostream function pointers such as std::endl and std::hex
        ofDrawBitmapString& operator<<(std::ostream& (*func)(std::ostream&)){
			func(message);
            return *this;
        }
		
	private:
	
		ofPoint pos;				///< temp position
        std::ostringstream message;	///< temp buffer
		bool bPrinted;				///< has the msg been printed in the constructor?
		
		ofDrawBitmapString(ofDrawBitmapString const&) {}        				// not defined, not copyable
        ofDrawBitmapString& operator=(ofDrawBitmapString& from) {return *this;}	// not defined, not assignable
};
