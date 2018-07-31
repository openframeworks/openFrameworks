#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){
		ofxTestEq(ofTrimFront("    trim this string    "),"trim this string    ","trimfront");
		ofxTestEq(ofTrimBack("    trim this string    "),"    trim this string","trimback");
		ofxTestEq(ofTrim("    trim this string    "),"trim this string","trim");

		auto split0 = ofSplitString("hi this is a split test", " ");
		ofxTestEq(split0.size(),6u,"split size");
		ofxTestEq(split0[0],"hi","split 0");
		ofxTestEq(split0[1],"this","split 1");
		ofxTestEq(split0[2],"is","split 2");
		ofxTestEq(split0[3],"a","split 3");
		ofxTestEq(split0[4],"split","split 4");
		ofxTestEq(split0[5],"test","split 5");


		auto split1 = ofSplitString(" hi this is a split test ", " ");
		ofxTestEq(split1.size(),8u,"split no trim size");
		ofxTestEq(split1[0],"","split no trim 0");
		ofxTestEq(split1[1],"hi","split no trim 1");
		ofxTestEq(split1[2],"this","split no trim 2");
		ofxTestEq(split1[3],"is","split no trim 3");
		ofxTestEq(split1[4],"a","split no trim 4");
		ofxTestEq(split1[5],"split","split no trim 5");
		ofxTestEq(split1[6],"test","split no trim 6");
		ofxTestEq(split1[7],"","split no trim 7");

		auto split2 = ofSplitString(" hi this is a split test ", " ", true, true);
		ofxTestEq(split2.size(),6u,"split trim size");
		ofxTestEq(split2[0],"hi","split trim 0");
		ofxTestEq(split2[1],"this","split trim 1");
		ofxTestEq(split2[2],"is","split trim 2");
		ofxTestEq(split2[3],"a","split trim 3");
		ofxTestEq(split2[4],"split","split trim 4");
		ofxTestEq(split2[5],"test","split trim 5");

		auto split3 = ofSplitString(" hi      this is a split test ", " ", true, true);
		ofxTestEq(split2.size(),6u,"split trim2 size");
		ofxTestEq(split2[0],"hi","split trim2 0");
		ofxTestEq(split2[1],"this","split trim2 1");
		ofxTestEq(split2[2],"is","split trim2 2");
		ofxTestEq(split2[3],"a","split trim2 3");
		ofxTestEq(split2[4],"split","split trim2 4");
		ofxTestEq(split2[5],"test","split trim2 5");

		ofxTestEq(ofJoinString({"hi","this","is","a","join","test"}," "),"hi this is a join test","join string");
		ofxTestEq(ofJoinString({"hi"}," "),"hi","join string 1 element");
		ofxTestEq(ofJoinString({}," "),"","join string 0 elements");

		std::string replace = "hi this is a replace test";
		ofStringReplace(replace,"replace","replaceeee");
		ofxTestEq(replace , "hi this is a replaceeee test","replace string element");

        ofxTestEq(ofToLower("AbCéÉBbCcc"),"abcéébbccc","tolower");
        ofxTestEq(ofToUpper("AbCéÉBbCcc"),"ABCÉÉBBCCC","toupper");

		// test #4363
		std::vector<string> strs;
		strs.push_back("hi");
		strs.push_back("this");
		strs.push_back("is");
		strs.push_back("a");
		strs.push_back("join");
		strs.push_back("test");
		ofxTestEq(ofJoinString(strs,","),"hi,this,is,a,join,test","test #4363");
	}
};

//========================================================================
int main( ){
	ofInit();
	auto window = make_shared<ofAppNoWindow>();
	auto app = make_shared<ofApp>();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(window, app);
	return ofRunMainLoop();

}
