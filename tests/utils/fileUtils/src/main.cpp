#include "ofFileUtils.h"
#include "ofUtils.h"
#include "ofxUnitTests.h"

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
		test(ofDirectory(".").getFiles().empty(),"removing old tests files","other tests will fail too");



		//========================================================================
		ofLogNotice() << "testing ofFile";
		{
			ofFile("test.txt").create();
		}
		test(ofFile("test.txt").exists(),"ofFile::create && ofFile::exists");
		test(!ofFile("test2.txt").exists(),"!ofFile::exists");
		test_eq(ofFile("test.txt").getExtension(),"txt","ofFile::getExtension",ofFile("test.txt").getExtension());
		test_eq(ofFile("test.txt").getFileName(),"test.txt","ofFile::getFileName",ofFile("test.txt").getFileName());
		test_eq(ofFile("test.txt").getBaseName(),"test","ofFile::getBaseName",ofFile("test.txt").getBaseName());
		test_eq(ofFile("test.txt").getAbsolutePath(), ofFilePath::join(ofToDataPath("",true),"test.txt"),"ofFile::getAbsolutePath");

		if(ofGetTargetPlatform()!=OF_TARGET_MINGW && ofGetTargetPlatform()!=OF_TARGET_WINVS){
			// seems you can't effectively set a file to not be read in windows
			ofFile("noread").create();
			{
				ofFile fw("noread",ofFile::WriteOnly);
				fw << "testing";
			}
			boost::system::error_code error;
			boost::filesystem::permissions(ofToDataPath("noread"), boost::filesystem::no_perms, error);
			test(!error, "error setting no read permissions, " + error.message());
			if(!test(!ofFile("noread").canRead(),"!ofFile::canRead")){
				ofFile fr("noread");
				std::string str;
				fr >> str;
				cout << "testing if file can be really read" << endl;
				cout << str << endl;
			}
		}

		ofFile("nowrite").create();
		ofFile("nowrite").setWriteable(false);
		test(ofFile("nowrite").canRead(),"ofFile::canRead");
		test(!ofFile("nowrite").canWrite(),"!ofFile::canWrite");

		test(ofFile("test.txt").isFile(),"ofFile::isFile with existing file");
		test(!ofFile("test2.txt").isFile(),"!ofFile::isFile with non-existing file");
		test(!ofFile(".").isFile(),"!ofFile::isFile with folder");

		test(ofFile(".").isDirectory(),"ofFile::isDirectory with dir");
		test(!ofFile("nonexistent").isDirectory(),"ofFile::isDirectory with non existing");
		test(!ofFile("test.txt").isDirectory(),"ofFile::isDirectory with file");

		test(ofFile("test.txt").copyTo("test2.txt"),"ofFile::copyTo");
		test(ofFile("test2.txt").isFile(),"ofFile::copyTo exists");

		test(ofFile("test2.txt").moveTo("test3.txt"),"ofFile::moveTo");
		test(ofFile("test3.txt").isFile(),"ofFile::moveTo creates");
		test(!ofFile("test2.txt").exists(),"ofFile::moveTo removes");

		test(ofFile("test3.txt").moveTo("test2.txt"),"ofFile::moveTo");
		test(ofFile("test2.txt").isFile(),"ofFile::moveTo creates");
		test(!ofFile("test3.txt").exists(),"ofFile::moveTo removes");

		test(ofFile("test2.txt").remove(),"ofFile::remove");
		test(!ofFile("test2.txt").exists(),"ofFile::remove !exists");

		test_eq(ofFile("test.txt").getSize(),uint64_t(0),"ofFile::getSize");
		{
			ofFile("test.txt",ofFile::WriteOnly) << "hola";
		}
		test_eq(ofFile("test.txt").getSize(),uint64_t(4),"ofFile write and getSize");




		//========================================================================
		ofLogNotice() << "";
		ofLogNotice() << "testing ofDirectory";
		size_t numFilesCreated;
		if(ofGetTargetPlatform()!=OF_TARGET_MINGW && ofGetTargetPlatform()!=OF_TARGET_WINVS){
			numFilesCreated = 3;
		}else{
			numFilesCreated = 2;
		}
		if(!test_eq(ofDirectory(".").getFiles().size(), numFilesCreated, "ofDirectory::ofDirectory with path")){
			ofLogError() << "data folder contains: ";
			for(auto & f: ofDirectory(".").getFiles()){
				ofLogError() << f.path();
			}
		}

		test(ofDirectory("d1").create(),"ofDirectory::create");
		test(ofDirectory("d1").isDirectory(),"ofDirectory::isDirectory");
		test(ofDirectory("d1").isDirectory(),"ofDirectory::exists");

		test(ofDirectory("d2/d3").create(true),"ofDirectory::create recursive");
		test(ofDirectory("d2/d3").isDirectory(),"ofDirectory::isDirectory recursive");
		test(ofDirectory("d2/d3").isDirectory(),"ofDirectory::exists recursive");

		test(ofDirectory("d1").canRead(),"ofDirectory::canRead");
		test(ofDirectory("d1").canWrite(),"ofDirectory::canWrite");
		if(ofGetTargetPlatform()!=OF_TARGET_MINGW && ofGetTargetPlatform()!=OF_TARGET_WINVS){
			// this doesn't make sense in windows
			test(ofDirectory("d1").canExecute(),"ofDirectory::canExecute");
		}

		ofDirectory("noreaddir").create();
		ofDirectory("noreaddir").setWriteable(false);
		test(ofDirectory("noreaddir").canRead(),"ofDirectory::canRead readonly");
		test(!ofDirectory("noreaddir").canWrite(),"!ofDirectory::canWrite readonly");

		test(ofFile("d2/f1").create(),"ofFile::create in dir");
		test(ofFile("d2/d3/f2").create(),"ofFile::create in dirs");
		test(ofDirectory("d2").copyTo("d4"),"ofDirectory::copyTo");
		test(ofDirectory("d4").isDirectory(),"ofDirectory::copyTo dir exists");
		test(ofDirectory("d4/d3").isDirectory(),"ofDirectory::copyTo recursive dir exists");
		test(ofFile("d4/f1").isFile(),"ofDirectory::copyTo f1 exists");
		test(ofFile("d4/d3/f2").isFile(),"ofDirectory::copyTo f2 exists");

		test(ofDirectory("d4").moveTo("d5"),"ofDirectory::moveTo");
		test(ofDirectory("d5").isDirectory(),"ofDirectory::moveTo dir exists");
		test(ofDirectory("d5/d3").isDirectory(),"ofDirectory::moveTo recursive dir exists");
		test(ofFile("d5/f1").isFile(),"ofDirectory::moveTo f1 exists");
		test(ofFile("d5/d3/f2").isFile(),"ofDirectory::moveTo f2 exists");

		test(ofDirectory("d5").renameTo("d4"),"ofDirectory::renameTo");
		test(ofDirectory("d4").isDirectory(),"ofDirectory::renameTo dir exists");
		test(ofDirectory("d4/d3").isDirectory(),"ofDirectory::renameTo recursive dir exists");
		test(ofFile("d4/f1").isFile(),"ofDirectory::renameTo f1 exists");
		test(ofFile("d4/d3/f2").isFile(),"ofDirectory::renameTo f2 exists");

		test(ofDirectory("d4").remove(true),"ofDirectory::remove recursive");
		test(!ofDirectory("d4").exists(),"!ofDirectory::exists after remove");



		//========================================================================
		ofLogNotice() << "";
		ofLogNotice() << "testing ofFilePath";
		test_eq(ofFilePath::getFileExt("test.txt"),"txt","ofFilePath::getFileExt");
		test_eq(ofFilePath::removeExt("test.txt"),"test","ofFilePath::removeExt");
		if(ofGetTargetPlatform()!=OF_TARGET_MINGW && ofGetTargetPlatform()!=OF_TARGET_WINVS){
			test_eq(ofFilePath::removeExt("/home/user/file.txt"),"/home/user/file","ofFilePath::removeExt absolute path");
			test_eq(ofFilePath::addLeadingSlash("test"),"/test","ofFilePath::addLeadingSlash");
			test_eq(ofFilePath::addLeadingSlash("/test"),"/test","ofFilePath::addLeadingSlash");
			test_eq(ofFilePath::addTrailingSlash("test"),"test/","ofFilePath::addTrailingSlash");
			test_eq(ofFilePath::addTrailingSlash("test/"),"test/","ofFilePath::addTrailingSlash");
			test_eq(ofFilePath::removeTrailingSlash("test/"),"test","ofFilePath::removeTrailingSlash");
			test_eq(ofFilePath::getPathForDirectory("dir/other"),"dir/other/","ofFilePath::getPathForDirectory");
			test_eq(ofFilePath::getPathForDirectory("dir/other/"),"dir/other/","ofFilePath::getPathForDirectory with trailing /");
			test(ofFilePath::isAbsolute("/test"),"ofFilePath::isAbsolute");
		}else{
			test_eq(ofFilePath::removeExt("c:\\users\\user\\file.txt"),"c:\\users\\user\\file","ofFilePath::removeExt absolute path");
			test_eq(ofFilePath::addLeadingSlash("test"),"\\test","ofFilePath::addLeadingSlash");
			test_eq(ofFilePath::addLeadingSlash("\\test"),"\\test","ofFilePath::addLeadingSlash");
			test_eq(ofFilePath::addTrailingSlash("test"),"test\\","ofFilePath::addTrailingSlash");
			test_eq(ofFilePath::addTrailingSlash("test\\"),"test\\","ofFilePath::addTrailingSlash");
			test_eq(ofFilePath::removeTrailingSlash("test\\"),"test","ofFilePath::removeTrailingSlash");
			test_eq(ofFilePath::getPathForDirectory("dir\\other"),"dir\\other\\","ofFilePath::getPathForDirectory");
			test_eq(ofFilePath::getPathForDirectory("dir\\other\\"),"dir\\other\\","ofFilePath::getPathForDirectory with trailing \\");
			test(ofFilePath::isAbsolute("c:\\test"),"ofFilePath::isAbsolute");
		}
		test_eq(ofFilePath::getFileName("test/test.txt"),"test.txt","ofFilePath::getFileName",ofFilePath::getFileName("test/test.txt"));
		test_eq(ofFilePath::getBaseName("test/test.txt"),"test","ofFilePath::getBaseName",ofFilePath::getBaseName("test/test.txt"));
		test_eq(ofFilePath::getBaseName(ofFilePath::removeTrailingSlash(ofFilePath::getEnclosingDirectory("testdir/test.txt"))),"testdir","ofFilePath::getEnclosingDirectory",ofFilePath::getBaseName(ofFilePath::getEnclosingDirectory("testdir/test.txt")));
#ifdef TARGET_WIN32
		test_eq(ofFilePath::join("d1","d2"),"d1\\d2","ofFilePath::join",ofFilePath::join("d1","d2"));
#else
		test_eq(ofFilePath::join("d1","d2"),"d1/d2","ofFilePath::join",ofFilePath::join("d1","d2"));
#endif

		test(std::filesystem::exists(ofFile("test.txt")), "ofFile cast to filesystem::path");
		test(std::filesystem::exists(ofDirectory("d1")), "ofDirectory cast to filesystem::path");





		//========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4285";
        test(!ofDirectory::doesDirectoryExist("d5/d1"),"!ofDirectory::doesDirectoryExist");
        test(ofDirectory::createDirectory("d5/d1",true,true),"ofDirectory::create recursive");
        test(ofDirectory::createDirectory(ofToDataPath("d5/d2",true),false,true),"ofDirectory::create recursive");
        test(ofDirectory::createDirectory(ofToDataPath("d5/d3"),false,true),"ofDirectory::create recursive");


        //========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4299";
        test_eq(std::filesystem::path(ofFilePath::getCurrentWorkingDirectory()), initial_cwd, "ofFilePath::getCurrentWorkingDirectory()");
		if(ofGetTargetPlatform()==OF_TARGET_OSX){
			test_eq(ofToDataPath("",false),"../../../data/","ofToDataPath relative");
		}else if(ofGetTargetPlatform()==OF_TARGET_WINVS || ofGetTargetPlatform()==OF_TARGET_MINGW){
			test_eq(ofToDataPath("",false),"data\\","ofToDataPath relative");
		}else{
			test_eq(ofToDataPath("",false),"data/","ofToDataPath relative");
		}


        //========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4462";
		if(ofGetTargetPlatform()==OF_TARGET_WINVS || ofGetTargetPlatform()==OF_TARGET_MINGW){
			test_eq(ofToDataPath("movies\\",true).back(), '\\', "absolute ofToDataPath with \\ should end in \\");
			test_eq(ofToDataPath("movies",true).back(), 's', "absolute ofToDataPath without \\ should not end in \\");
			ofDirectory("movies").create();
			test_eq(ofToDataPath("movies\\",true).back(), '\\', "absolute ofToDataPath with \\ should end in \\");
			test_eq(ofToDataPath("movies",true).back(), 's', "absolute ofToDataPath without \\ should not end in \\");
		}else{
			test_eq(ofToDataPath("movies/",true).back(), '/', "absolute ofToDataPath with / should end in /");
			test_eq(ofToDataPath("movies",true).back(), 's', "absolute ofToDataPath without / should not end in /");
			ofDirectory("movies").create();
			test_eq(ofToDataPath("movies/",true).back(), '/', "absolute ofToDataPath with / should end in /");
			test_eq(ofToDataPath("movies",true).back(), 's', "absolute ofToDataPath without / should not end in /");
		}


        //========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4598";
		test_eq(ofToDataPath("").back(), std::filesystem::path::preferred_separator, "ofToDataPath with empty string shouldn't crash");

        //========================================================================
        ofLogNotice() << "";
        ofLogNotice() << "tests #4563";
#ifdef TARGET_LINUX 
        test_eq(ofToDataPath("a.txt"), "data/a.txt","#4563 test1");
        test_eq(ofToDataPath("data.txt"), "data/data.txt", "#4563 test2");
        test_eq(ofToDataPath(""), "data/", "#4563 test3");
#elif defined(TARGET_OSX)
        test_eq(ofToDataPath("a.txt"), "../../../data/a.txt","#4563 test1");
        test_eq(ofToDataPath("data.txt"), "../../../data/data.txt", "#4563 test2");
        test_eq(ofToDataPath(""), "../../../data/", "#4563 test3");
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
			test_eq(path.substr(path.size()-pathEnd.size()), pathEnd, "#4564");
		}else{
			ofDirectory currentVideoDirectory(ofToDataPath("../../../video", true));
			auto path = currentVideoDirectory.path();
			std::string pathEnd("data/../../../video/");
			test_eq(path.substr(path.size()-pathEnd.size()), pathEnd, "#4564");
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
