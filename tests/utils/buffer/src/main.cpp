#include "ofFileUtils.h"
#include "ofUtils.h"
#include "ofxUnitTests.h"

class ofApp: public ofxUnitTestsApp{
	void run(){
		std::vector<char> src(2048);
		for(auto & c: src){
			c = ofRandom(255);
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "constructor allocator";
			ofBuffer buffer(src.data(), src.size());
			test_eq(buffer.size(), src.size(), "constructor does correct allocation");
			test(buffer.end() == buffer.begin() + src.size(), "correct boundaries");
			bool bufferEqual = true;
			auto srcIt = src.begin();
			for(auto & c: buffer){
				bufferEqual &= c == *srcIt++;
			}
			test(bufferEqual, "data is correct");
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "text constructor allocator";
			std::string text("This is a text test");
			ofBuffer buffer;
			buffer.set(text);
			test_eq(buffer.size(), text.size(), "constructor does correct allocation");
			test(buffer.end() == buffer.begin() + text.size(), "correct boundaries");
			bool bufferEqual = true;
			auto srcIt = text.begin();
			for(auto & c: buffer){
				bufferEqual &= c == *srcIt++;
			}
			test(bufferEqual, "data is correct");
			test_eq(text, buffer.getText(), "getText");
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "set allocator";
			ofBuffer buffer;
			buffer.set(src.data(), src.size());
			test_eq(buffer.size(), src.size(), "set does correct allocation");
			test(buffer.end() == buffer.begin() + src.size(), "correct boundaries");
			bool bufferEqual = true;
			auto srcIt = src.begin();
			for(auto & c: buffer){
				bufferEqual &= c == *srcIt++;
			}
			test(bufferEqual, "data is correct");
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "text set allocator";
			std::string text("This is a text test");
			ofBuffer buffer;
			buffer.set(text);
			test_eq(buffer.size(), text.size(), "text set does correct allocation");
			test(buffer.end() == buffer.begin() + text.size(), "correct boundaries");
			bool bufferEqual = true;
			auto srcIt = text.begin();
			for(auto & c: buffer){
				bufferEqual &= c == *srcIt++;
			}
			test(bufferEqual, "data is correct");
			test_eq(text, buffer.getText(), "getText");
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "allocate & setall";
			ofBuffer buffer;
			auto bufferSize = 2048;
			buffer.allocate(bufferSize);
			test_eq(buffer.size(), bufferSize, "allocate does correct allocation");
			test(buffer.end() == buffer.begin() + bufferSize, "correct boundaries");
			bool bufferEqual = true;
			buffer.setall(5);
			for(auto & c: buffer){
				bufferEqual &= c == 5;
			}
			test(bufferEqual, "data is correct");
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "append text";
			std::string text("This is a text test");
			ofBuffer buffer;
			for(int i=0;i<5;i++){
				buffer.append(text);
			}
			test_eq(buffer.size(), text.size() * 5, "text append does correct allocation");
			test(buffer.end() == buffer.begin() + text.size() * 5, "correct boundaries");
			bool bufferEqual = true;
			auto bufferIt = buffer.begin();
			for(int i=0;i<5;i++){
				for(auto & c: text){
					bufferEqual &= c == *bufferIt++;
				}
			}
			test(bufferEqual, "data is correct");
			test_eq(text+text+text+text+text, buffer.getText(), "getText");
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "append raw data";
			ofBuffer buffer;
			for(int i=0;i<5;i++){
				buffer.append(src.data(), src.size());
			}
			test_eq(buffer.size(), src.size() * 5, "text append does correct allocation");
			test(buffer.end() == buffer.begin() + src.size() * 5, "correct boundaries");
			bool bufferEqual = true;
			auto bufferIt = buffer.begin();
			for(int i=0;i<5;i++){
				for(auto & c: src){
					bufferEqual &= c == *bufferIt++;
				}
			}
			test(bufferEqual, "data is correct");
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "append raw data";
			ofBuffer buffer;
			test_eq((uint64_t)buffer.getData(),(uint64_t)nullptr,"unallocated buffer getData");
			test(buffer.begin() == buffer.end(),"unallocated buffer begin");
			buffer.set(src.data(), src.size());
			test(buffer.getData() == &*buffer.begin(),"getData == begin");
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "lines iterator";
			std::vector<std::string> lines;
			lines.push_back("Lorem ipsum dolor sit amet,");
			lines.push_back("consectetur adipiscing elit.");
			lines.push_back("Vivamus viverra tortor ut condimentum");
			lines.push_back("condimentum. Vestibulum id luctus lectus.");
			lines.push_back("Duis porttitor turpis orci, eget pellentesque");
			lines.push_back("enim varius eu. Integer lectus urna,");
			lines.push_back("auctor in tincidunt nec, porta vitae tellus.");
			lines.push_back("Integer ac blandit felis, ullamcorper iaculis felis.");
			lines.push_back("Fusce eget sollicitudin purus, sed porttitor ante.");
			lines.push_back("Integer mattis tortor at lectus venenatis laoreet.");
			lines.push_back("Phasellus nibh massa, pellentesque non consequat vitae,");
			lines.push_back("volutpat ac quam. Cras ac mauris in justo");
			lines.push_back("hendrerit tincidunt eget a magna. Nullam volutpat,");
			lines.push_back("erat sit amet facilisis tempor,");
			lines.push_back("tellus urna volutpat dolor,");
			lines.push_back("a ornare nisl libero quis orci.");
			lines.push_back("Fusce in nunc id orci lobortis semper.");
			ofBuffer buffer;
			for(auto & line: lines){
				buffer.append(line + "\n"); // This should append one more line but the result should be correct
			}

			auto numLines = 0;
			auto linesIt = lines.begin();
			auto allLinesEqual = true;
			for(auto line: buffer.getLines()){
				allLinesEqual &= line == *linesIt++;
				++numLines;
			}
			test(allLinesEqual, "all lines are correct");
			test_eq(numLines,lines.size(),"lines iterator correct numLines");
		}

		{
			ofLogNotice() << "-------------------";
			ofLogNotice() << "lines reverse iterator";
			std::vector<std::string> lines;
			lines.push_back("Lorem ipsum dolor sit amet,");
			lines.push_back("consectetur adipiscing elit.");
			lines.push_back("Vivamus viverra tortor ut condimentum");
			lines.push_back("condimentum. Vestibulum id luctus lectus.");
			lines.push_back("Duis porttitor turpis orci, eget pellentesque");
			lines.push_back("enim varius eu. Integer lectus urna,");
			lines.push_back("auctor in tincidunt nec, porta vitae tellus.");
			lines.push_back("Integer ac blandit felis, ullamcorper iaculis felis.");
			lines.push_back("Fusce eget sollicitudin purus, sed porttitor ante.");
			lines.push_back("Integer mattis tortor at lectus venenatis laoreet.");
			lines.push_back("Phasellus nibh massa, pellentesque non consequat vitae,");
			lines.push_back("volutpat ac quam. Cras ac mauris in justo");
			lines.push_back("hendrerit tincidunt eget a magna. Nullam volutpat,");
			lines.push_back("erat sit amet facilisis tempor,");
			lines.push_back("tellus urna volutpat dolor,");
			lines.push_back("a ornare nisl libero quis orci.");
			lines.push_back("Fusce in nunc id orci lobortis semper.");
			ofBuffer buffer;
			for(auto & line: lines){
				buffer.append(line + "\n"); // This should append one more line but the result should be correct
			}

			auto numLines = 0;
			auto linesIt = lines.rbegin();
			auto allLinesEqual = true;
			for(auto rline : buffer.getReverseLines()){
				allLinesEqual &= rline == *linesIt++;
				++numLines;
			}
			test(allLinesEqual, "all lines are correct");
			test_eq(numLines,lines.size(),"lines iterator correct numLines");
		}
	}
};


#include "ofAppNoWindow.h"
#include "ofAppRunner.h"
//========================================================================
int main( ){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
