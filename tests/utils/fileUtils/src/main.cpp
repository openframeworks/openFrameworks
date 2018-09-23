#include "ofFileUtils.h"
#include "ofUtils.h"
#include "ofxUnitTests.h"

using namespace std;

std::filesystem::path initial_cwd;

class ofApp: public ofxUnitTestsApp{
	void run(){
		ofDirectory dir(".");
        dir.create(true);
        dir.exists();
		for(auto f: dir){
			f.setWriteable(true);
			if(f.isDirectory()){
				ofDirectory(f.path()).remove(true);
			}else{
				f.remove();
			}
		}
		ofxTest(ofDirectory(".").getFiles().empty(),"removing old tests files","other tests will fail too");



		//========================================================================
		ofLogNotice() << "testing ofFile";
		{
			ofFile("test.txt").create();
		}
		ofxTest(ofFile("test.txt").exists(),"ofFile::create && ofFile::exists");
		ofxTest(!ofFile("test2.txt").exists(),"!ofFile::exists");
		ofxTestEq(ofFile("test.txt").getExtension(),"txt","ofFile::getExtension",ofFile("test.txt").getExtension());
		ofxTestEq(ofFile("test.txt").getFileName(),"test.txt","ofFile::getFileName",ofFile("test.txt").getFileName());
		ofxTestEq(ofFile("test.txt").getBaseName(),"test","ofFile::getBaseName",ofFile("test.txt").getBaseName());
		ofxTestEq(ofFile("test.txt").getAbsolutePath(), ofFilePath::join(ofToDataPath("",true),"test.txt"),"ofFile::getAbsolutePath");

		if(ofGetTargetPlatform()!=OF_TARGET_MINGW && ofGetTargetPlatform()!=OF_TARGET_WINVS){
			// seems you can't effectively set a file to not be read in windows
			ofFile("noread").create();
			{
				ofFile fw("noread",ofFile::WriteOnly);
				fw << "testing";
			}
			boost::system::error_code error;
			boost::filesystem::permissions(ofToDataPath("noread"), boost::filesystem::no_perms, error);
			ofxTest(!error, "error setting no read permissions, " + error.message());
			if(!ofxTest(!ofFile("noread").canRead(),"!ofFile::canRead")){
				ofFile fr("noread");
				std::string str;
				fr >> str;
				std::cout << "testing if file can be really read" << std::endl;
				std::cout << str << std::endl;
			}
		}

		ofFile("nowrite").create();
		ofFile("nowrite").setWriteable(false);
		ofxTest(ofFile("nowrite").canRead(),"ofFile::canRead");
		ofxTest(!ofFile("nowrite").canWrite(),"!ofFile::canWrite");

		ofxTest(ofFile("test.txt").isFile(),"ofFile::isFile with existing file");
		ofxTest(!ofFile("test2.txt").isFile(),"!ofFile::isFile with non-existing file");
		ofxTest(!ofFile(".").isFile(),"!ofFile::isFile with folder");

		ofxTest(ofFile(".").isDirectory(),"ofFile::isDirectory with dir");
		ofxTest(!ofFile("nonexistent").isDirectory(),"ofFile::isDirectory with non existing");
		ofxTest(!ofFile("test.txt").isDirectory(),"ofFile::isDirectory with file");

		ofxTest(ofFile("test.txt").copyTo("test2.txt"),"ofFile::copyTo");
		ofxTest(ofFile("test2.txt").isFile(),"ofFile::copyTo exists");

		ofxTest(ofFile("test2.txt").moveTo("test3.txt"),"ofFile::moveTo");
		ofxTest(ofFile("test3.txt").isFile(),"ofFile::moveTo creates");
		ofxTest(!ofFile("test2.txt").exists(),"ofFile::moveTo removes");

		ofxTest(ofFile("test3.txt").moveTo("test2.txt"),"ofFile::moveTo");
		ofxTest(ofFile("test2.txt").isFile(),"ofFile::moveTo creates");
		ofxTest(!ofFile("test3.txt").exists(),"ofFile::moveTo removes");

		ofxTest(ofFile("test2.txt").remove(),"ofFile::remove");
		ofxTest(!ofFile("test2.txt").exists(),"ofFile::remove !exists");

		ofxTestEq(ofFile("test.txt").getSize(),uint64_t(0),"ofFile::getSize");
		{
			ofFile("test.txt",ofFile::WriteOnly) << "hola";
		}
		ofxTestEq(ofFile("test.txt").getSize(),uint64_t(4),"ofFile write and getSize");




		//========================================================================
		ofLogNotice() << "";
		ofLogNotice() << "testing ofDirectory";
		size_t numFilesCreated;
		if(ofGetTargetPlatform()!=OF_TARGET_MINGW && ofGetTargetPlatform()!=OF_TARGET_WINVS){
			numFilesCreated = 3;
		}else{
			numFilesCreated = 2;
		}
		if(!ofxTestEq(ofDirectory(".").getFiles().size(), numFilesCreated, "ofDirectory::ofDirectory with path")){
			ofLogError() << "data folder contains: ";
			for(auto & f: ofDirectory(".").getFiles()){
				ofLogError() << f.path();
			}
		}

		ofxTest(ofDirectory("d1").create(),"ofDirectory::create");
		ofxTest(ofDirectory("d1").isDirectory(),"ofDirectory::isDirectory");
		ofxTest(ofDirectory("d1").isDirectory(),"ofDirectory::exists");

		ofxTest(ofDirectory("d2/d3").create(true),"ofDirectory::create recursive");
		ofxTest(ofDirectory("d2/d3").isDirectory(),"ofDirectory::isDirectory recursive");
		ofxTest(ofDirectory("d2/d3").isDirectory(),"ofDirectory::exists recursive");

		ofxTest(ofDirectory("d1").canRead(),"ofDirectory::canRead");
		ofxTest(ofDirectory("d1").canWrite(),"ofDirectory::canWrite");
		if(ofGetTargetPlatform()!=OF_TARGET_MINGW && ofGetTargetPlatform()!=OF_TARGET_WINVS){
			// this doesn't make sense in windows
			ofxTest(ofDirectory("d1").canExecute(),"ofDirectory::canExecute");
		}

		ofDirectory("noreaddir").create();
		ofDirectory("noreaddir").setWriteable(false);
		ofxTest(ofDirectory("noreaddir").canRead(),"ofDirectory::canRead readonly");
		ofxTest(!ofDirectory("noreaddir").canWrite(),"!ofDirectory::canWrite readonly");

		ofxTest(ofFile("d2/f1").create(),"ofFile::create in dir");
		ofxTest(ofFile("d2/d3/f2").create(),"ofFile::create in dirs");
		ofxTest(ofDirectory("d2").copyTo("d4"),"ofDirectory::copyTo");
		ofxTest(ofDirectory("d4").isDirectory(),"ofDirectory::copyTo dir exists");
		ofxTest(ofDirectory("d4/d3").isDirectory(),"ofDirectory::copyTo recursive dir exists");
		ofxTest(ofFile("d4/f1").isFile(),"ofDirectory::copyTo f1 exists");
		ofxTest(ofFile("d4/d3/f2").isFile(),"ofDirectory::copyTo f2 exists");

		ofxTest(ofDirectory("d4").moveTo("d5"),"ofDirectory::moveTo");
		ofxTest(ofDirectory("d5").isDirectory(),"ofDirectory::moveTo dir exists");
		ofxTest(ofDirectory("d5/d3").isDirectory(),"ofDirectory::moveTo recursive dir exists");
		ofxTest(ofFile("d5/f1").isFile(),"ofDirectory::moveTo f1 exists");
		ofxTest(ofFile("d5/d3/f2").isFile(),"ofDirectory::moveTo f2 exists");

		ofxTest(ofDirectory("d5").renameTo("d4"),"ofDirectory::renameTo");
		ofxTest(ofDirectory("d4").isDirectory(),"ofDirectory::renameTo dir exists");
		ofxTest(ofDirectory("d4/d3").isDirectory(),"ofDirectory::renameTo recursive dir exists");
		ofxTest(ofFile("d4/f1").isFile(),"ofDirectory::renameTo f1 exists");
		ofxTest(ofFile("d4/d3/f2").isFile(),"ofDirectory::renameTo f2 exists");

		ofxTest(ofDirectory("d4").remove(true),"ofDirectory::remove recursive");
		ofxTest(!ofDirectory("d4").exists(),"!ofDirectory::exists after remove");



		//========================================================================
		ofLogNotice() << "";
		ofLogNotice() << "testing ofFilePath";
		ofxTestEq(ofFilePath::getFileExt("test.txt"),"txt","ofFilePath::getFileExt");
		ofxTestEq(ofFilePath::removeExt("test.txt"),"test","ofFilePath::removeExt");
		if(ofGetTargetPlatform()!=OF_TARGET_MINGW && ofGetTargetPlatform()!=OF_TARGET_WINVS){
			ofxTestEq(ofFilePath::removeExt("/home/user/file.txt"),"/home/user/file","ofFilePath::removeExt absolute path");
			ofxTestEq(ofFilePath::addLeadingSlash("test"),"/test","ofFilePath::addLeadingSlash");
			ofxTestEq(ofFilePath::addLeadingSlash("/test"),"/test","ofFilePath::addLeadingSlash");
			ofxTestEq(ofFilePath::addTrailingSlash("test"),"test/","ofFilePath::addTrailingSlash");
			ofxTestEq(ofFilePath::addTrailingSlash("test/"),"test/","ofFilePath::addTrailingSlash");
			ofxTestEq(ofFilePath::removeTrailingSlash("test/"),"test","ofFilePath::removeTrailingSlash");
			ofxTestEq(ofFilePath::getPathForDirectory("dir/other"),"dir/other/","ofFilePath::getPathForDirectory");
			ofxTestEq(ofFilePath::getPathForDirectory("dir/other/"),"dir/other/","ofFilePath::getPathForDirectory with trailing /");
			ofxTest(ofFilePath::isAbsolute("/test"),"ofFilePath::isAbsolute");
		}else{
			ofxTestEq(ofFilePath::removeExt("c:\\users\\user\\file.txt"),"c:\\users\\user\\file","ofFilePath::removeExt absolute path");
			ofxTestEq(ofFilePath::addLeadingSlash("test"),"\\test","ofFilePath::addLeadingSlash");
			ofxTestEq(ofFilePath::addLeadingSlash("\\test"),"\\test","ofFilePath::addLeadingSlash");
			ofxTestEq(ofFilePath::addTrailingSlash("test"),"test\\","ofFilePath::addTrailingSlash");
			ofxTestEq(ofFilePath::addTrailingSlash("test\\"),"test\\","ofFilePath::addTrailingSlash");
			ofxTestEq(ofFilePath::removeTrailingSlash("test\\"),"test","ofFilePath::removeTrailingSlash");
			ofxTestEq(ofFilePath::getPathForDirectory("dir\\other"),"dir\\other\\","ofFilePath::getPathForDirectory");
			ofxTestEq(ofFilePath::getPathForDirectory("dir\\other\\"),"dir\\other\\","ofFilePath::getPathForDirectory with trailing \\");
			ofxTest(ofFilePath::isAbsolute("c:\\test"),"ofFilePath::isAbsolute");
		}
		ofxTestEq(ofFilePath::getFileName("test/test.txt"),"test.txt","ofFilePath::getFileName",ofFilePath::getFileName("test/test.txt"));
		ofxTestEq(ofFilePath::getBaseName("test/test.txt"),"test","ofFilePath::getBaseName",ofFilePath::getBaseName("test/test.txt"));
		ofxTestEq(ofFilePath::getBaseName(ofFilePath::removeTrailingSlash(ofFilePath::getEnclosingDirectory("testdir/test.txt"))),"testdir","ofFilePath::getEnclosingDirectory",ofFilePath::getBaseName(ofFilePath::getEnclosingDirectory("testdir/test.txt")));
#ifdef TARGET_WIN32
		ofxTestEq(ofFilePath::join("d1","d2"),"d1\\d2","ofFilePath::join",ofFilePath::join("d1","d2"));
#else
		ofxTestEq(ofFilePath::join("d1","d2"),"d1/d2","ofFilePath::join",ofFilePath::join("d1","d2"));
#endif

		ofxTest(std::filesystem::exists(ofFile("test.txt")), "ofFile cast to filesystem::path");
		ofxTest(std::filesystem::exists(ofDirectory("d1")), "ofDirectory cast to filesystem::path");





		//========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4285";
        ofxTest(!ofDirectory::doesDirectoryExist("d5/d1"),"!ofDirectory::doesDirectoryExist");
        ofxTest(ofDirectory::createDirectory("d5/d1",true,true),"ofDirectory::create recursive");
        ofxTest(ofDirectory::createDirectory(ofToDataPath("d5/d2",true),false,true),"ofDirectory::create recursive");
        ofxTest(ofDirectory::createDirectory(ofToDataPath("d5/d3"),false,true),"ofDirectory::create recursive");


        //========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4299";
        ofxTestEq(std::filesystem::path(ofFilePath::getCurrentWorkingDirectory()), initial_cwd, "ofFilePath::getCurrentWorkingDirectory()");
		if(ofGetTargetPlatform()==OF_TARGET_OSX){
			ofxTestEq(ofToDataPath("",false),"../../../data/","ofToDataPath relative");
		}else if(ofGetTargetPlatform()==OF_TARGET_WINVS || ofGetTargetPlatform()==OF_TARGET_MINGW){
			ofxTestEq(ofToDataPath("",false),"data\\","ofToDataPath relative");
		}else{
			ofxTestEq(ofToDataPath("",false),"data/","ofToDataPath relative");
		}


        //========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4462";
		if(ofGetTargetPlatform()==OF_TARGET_WINVS || ofGetTargetPlatform()==OF_TARGET_MINGW){
			ofxTestEq(ofToDataPath("movies\\",true).back(), '\\', "absolute ofToDataPath with \\ should end in \\");
			ofxTestEq(ofToDataPath("movies",true).back(), 's', "absolute ofToDataPath without \\ should not end in \\");
			ofDirectory("movies").create();
			ofxTestEq(ofToDataPath("movies\\",true).back(), '\\', "absolute ofToDataPath with \\ should end in \\");
			ofxTestEq(ofToDataPath("movies",true).back(), 's', "absolute ofToDataPath without \\ should not end in \\");
		}else{
			ofxTestEq(ofToDataPath("movies/",true).back(), '/', "absolute ofToDataPath with / should end in /");
			ofxTestEq(ofToDataPath("movies",true).back(), 's', "absolute ofToDataPath without / should not end in /");
			ofDirectory("movies").create();
			ofxTestEq(ofToDataPath("movies/",true).back(), '/', "absolute ofToDataPath with / should end in /");
			ofxTestEq(ofToDataPath("movies",true).back(), 's', "absolute ofToDataPath without / should not end in /");
		}


        //========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4598";
		ofxTestEq(ofToDataPath("").back(), std::filesystem::path::preferred_separator, "ofToDataPath with empty string shouldn't crash");

        //========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4563";
#ifdef TARGET_LINUX 
        ofxTestEq(ofToDataPath("a.txt"), "data/a.txt","#4563 test1");
        ofxTestEq(ofToDataPath("data.txt"), "data/data.txt", "#4563 test2");
        ofxTestEq(ofToDataPath(""), "data/", "#4563 test3");
#elif defined(TARGET_OSX)
        ofxTestEq(ofToDataPath("a.txt"), "../../../data/a.txt","#4563 test1");
        ofxTestEq(ofToDataPath("data.txt"), "../../../data/data.txt", "#4563 test2");
        ofxTestEq(ofToDataPath(""), "../../../data/", "#4563 test3");
#endif


        //========================================================================
		// clean test files
		dir.open(".");
		for(auto f: dir){
			f.setWriteable(true);
			if(f.isDirectory()){
				ofDirectory(f.path()).remove(true);
			}else{
				f.remove();
			}
		}

        //========================================================================
        ofLogNotice() << "#4564";
        dir.remove(true);
		if(ofGetTargetPlatform()==OF_TARGET_WINVS || ofGetTargetPlatform()==OF_TARGET_MINGW){
			ofDirectory currentVideoDirectory(ofToDataPath("..\\..\\..\\video", true));
			auto path = currentVideoDirectory.path();
			std::string pathEnd("data\\..\\..\\..\\video\\");
			ofxTestEq(path.substr(path.size()-pathEnd.size()), pathEnd, "#4564");
		}else{
			ofDirectory currentVideoDirectory(ofToDataPath("../../../video", true));
			auto path = currentVideoDirectory.path();
			std::string pathEnd("data/../../../video/");
			ofxTestEq(path.substr(path.size()-pathEnd.size()), pathEnd, "#4564");
		}
	}
};


#include "ofAppNoWindow.h"
#include "ofAppRunner.h"
//========================================================================
int main( ){
    initial_cwd = std::filesystem::current_path();
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
