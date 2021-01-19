#include "utils/ofXml.h"
#include "ofxUnitTests.h"
#include <locale>

using namespace std;


class ofApp: public ofxUnitTestsApp{
	void run(){
		
		// Create an ofXml object - it should be empty
		ofXml xml;
		ofxTest(xml.getChildren().begin()==xml.getChildren().end(), "a newly created ofXml object should be empty");

		// Create the root element
		ofLogNotice() << "Building a SVG document...";
		auto svg = xml.appendChild("svg");

		ofLogNotice() << "Testing attributes";
		ofLogVerbose() << "\tSetting '@width' to 500";
		svg.setAttribute("width",500);
		ofLogVerbose() << "\tAdd '@height' and set it to 200";
		svg.appendAttribute("height");
		svg.getAttribute("height").set(200);
		ofxTestEq(svg.getFirstAttribute().getName(),"width", "\tfirst attribute should be 'width'");
		ofxTestEq(svg.getLastAttribute().getName(),"height", "\tlast attribute should be 'height'");
		
		ofLogVerbose() << "\tAdding svg namespace in first position";
		svg.prependAttribute("xmlns");
		svg.setAttribute("xmlns","http://www.w3.org/2000/svg");
		ofxTestEq(svg.getFirstAttribute().getName(),"xmlns", "\tfirst attribute should now be 'xmlns'");

		ofLogVerbose() << "\tAdding more attributes";
		svg.appendAttribute("unexpected");
		svg.appendAttribute("version");
		ofxTestEq(svg.getLastAttribute().getName(),"version", "\tlast attribute should now be 'version'");
		ofLogVerbose() << "\tand remove some them";
		svg.removeAttribute( svg.getLastAttribute() );	//remove @version
		svg.removeAttribute("unexpected");				//remove @unexpected
		{	// Add and remove attribute
			auto a = svg.appendAttribute("todelete");
			svg.removeAttribute( a );
		}
		ofxTestEq(svg.getLastAttribute().getName(),"height", "\tlast attribute should noww be 'height'");
		{	// Iterate over attributes
			std::string attrs("");
			for (auto it=svg.getAttributes().begin(); it!=svg.getAttributes().end(); ++it){
				attrs += it->getName() + ":" + it->getValue() + " ";
			}
			ofxTestEq(attrs,"xmlns:http://www.w3.org/2000/svg width:500 height:200 ", "\titerating over attributes to get their values");
		}
		ofLogNotice()<<"\n";

		ofLogNotice() << "Creating elements";
		{
			auto el = svg.appendChild("circle");
			el.setAttribute("id","circle1");
			el.setAttribute<float>("r",10.5);
			el = svg.appendChild("circle");
			el.setAttribute("id","circle2");
			el.setAttribute("r",int(21));
			el = svg.appendChild("rect");
			el.setAttribute("id","rect1");
			el.setAttribute("width",200);
			el.setAttribute("height",100);
			
			
			// add a text element first
			el = svg.prependChild("text");
			el.setAttribute("id","text1");
			// add an extra circle by adding an ellipse and changing its tag name
			el = svg.appendChild("ellipse");
			el.setName("circle");
			el.setAttribute("id","circle3");

			// build 2 other XML tree - SVG  groups
			// and add them at begin and end
			ofXml otherxml;
			el = otherxml.appendChild("g");
			el.setAttribute("id","group1");
			el.appendChild("line").setAttribute("id","line1");
			el.appendChild("image").setAttribute("id","image1");
			svg.prependChild(el);

			el = otherxml.appendChild("g");
			el.setAttribute("id","group2");
			el.appendChild("ellipse").setAttribute("id","ellipse1");
			el.appendChild("path").setAttribute("id","path1");
			svg.appendChild(el);


			/*At this time, the XML should look like
			<svg>
				<g id="group1"><line id="line1"/><image id="image1"/></g>
				<text id="text1"/>
				<circle id="circle1" r="10.5"/>
				<circle id="circle2" r="21"/>
				<rect id="rect1" width="200" height="100"/>
				<circle id="circle3"/>
				<g id="group2"><ellipse id="ellipse1"/><path id="path1"/></g>
			</svg>
			*/
			auto child = svg.getFirstChild();
			ofxTestEq(child.getName(), "g", "\tfirst child should be <g>");
			child = child.getNextSibling();
			ofxTestEq(child.getAttribute("id").getValue(), "text1", "\tfollowed by <text>");
			child = child.getNextSibling("rect");
			ofxTestEq(child.getAttribute("id").getValue(), "rect1", "\twhich has a following-sibling <rect>'");
			child = child.getPreviousSibling();
			ofxTestEq(child.getAttribute("id").getValue(), "circle2", "\tprevious child should be 'circle2'");
			child = child.getPreviousSibling("g");
			ofxTestEq(child.getAttribute("id").getValue(), "group1", "\tprevious 'g' child should be 'group1'");
			child = child.getPreviousSibling("g");
			ofxTest(!child, "\tthere should be no previous sibling 'g'");
			child = svg.getLastChild();
			ofxTestEq(child.getAttribute("id").getValue(), "group2", "\tlast child should be 'group2'");
			ofxTest(!child.getPreviousSibling("ellipse"), "\tthere should be no previous 'ellipse' child");
			if( (child = svg.getNextSibling("rect").getNextSibling("circle")) ) {
				ofxTest(child.getNextSibling("circle"), "\tthere should be no other sibling 'circle'");
			}
			ofLogNotice()<<"\n";

			ofLogNotice()<<"Iterating over children";
			std::string ids("");
			auto children = svg.getChildren();
			for (auto it=children.begin(); it!=children.end(); it++){
				ids += it->getAttribute("id").getValue();
			}
			ofxTestEq(ids,"group1text1circle1circle2rect1circle3group2", "\telements are in expected order with no reference to grandchildren");
			ids="";
			for (auto& c : svg.getChildren("circle") ){
				ids += c.getAttribute("id").getValue();
			}
			ofxTestEq(ids,"circle1circle2circle3", "\tcircle elements are the expected ones");
			ids="";
			auto children3 = svg.getChildren("ellipse");
			for (auto it=children3.begin(); it!=children3.end(); ++it){
				ids += it->getAttribute("id").getValue();
			}
			ofxTestEq(ids,"", "\tthere is no ellipse direct children in svg");
			ofLogNotice()<<"\n";

			// Testing search 
			ofLogNotice()<<"Searching for elements";
			el = svg.findFirst("g");
			ofxTestEq(el.getAttribute("id").getValue(), "group1", "\tThe first 'g' found should be 'group1'");
			ofLogNotice()<<"\n";
		}

		
		ofLogNotice() << "Testing issue #6111";
		{
			ofXml xml1, xml2;

			xml1.parse("<tag float_period='3.5' float_comma='2,5'/>");

			std::locale saved_loc;
			// Building a locale with comma as decimal point - inspired from https://stackoverflow.com/a/15221403
			class decimmal_comma: public std::numpunct<char> {
				protected:
    			char do_decimal_point() const { return ','; }
			};
			std::locale decimal_comma_locale( std::locale("C"), new decimmal_comma() );
			std::locale decimal_point_locale( std::locale("C") );
			std::locale current_locale;
			
			ofLogNotice()<<" - using locale where ',' is the decimal separator";
			current_locale.global( decimal_comma_locale );
			
			ofxTestEq(xml1.getFirstChild().getAttribute("float_period").getFloatValue(), 3.5, "\tnumbers with period are float");
			ofxTestEq(xml1.getFirstChild().getAttribute("float_comma").getFloatValue(), 2.0, "\tnumbers with comma are not float");

			ofLogNotice()<<" - using locale where '.' is the decimal separator";
			current_locale.global( decimal_point_locale );
			xml2.parse("<tag float_period='3.5' float_comma='2,5'/>");
			ofxTestEq(xml1.getFirstChild().getAttribute("float_period").getFloatValue(), 3.5, "\tnumbers with period are float");
			ofxTestEq(xml1.getFirstChild().getAttribute("float_comma").getFloatValue(), 2.0, "\tnumbers with comma are not float");
			// set back locale
			current_locale.global( saved_loc );
		}
		ofLogNotice()<<"\n";
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
