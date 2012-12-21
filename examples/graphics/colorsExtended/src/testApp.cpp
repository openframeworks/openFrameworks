#include "testApp.h"

bool compareName( const colorNameMapping& s1, const colorNameMapping& s2 ) {
    return strcasecmp( s1.name.c_str(), s2.name.c_str() ) <= 0;
}

bool compareBrightness( const colorNameMapping& s1, const colorNameMapping& s2 ) {
    return s1.color.getBrightness() < s2.color.getBrightness();
}

bool compareHue( const colorNameMapping& s1, const colorNameMapping& s2 ) {
    return s1.color.getHue() < s2.color.getHue();
}

bool compareSaturation( const colorNameMapping& s1, const colorNameMapping& s2 ) {
    return s1.color.getSaturation() < s2.color.getSaturation();
}

//--------------------------------------------------------------
void testApp::setup(){
    
    
    // build a map from name to ofColor of all the named OF colors;

    colorNameMap["white"] = ofColor::white;
    colorNameMap["gray"] = ofColor::gray;
    colorNameMap["black"] = ofColor::black;
    colorNameMap["red"] = ofColor::red;
    colorNameMap["green"] = ofColor::green;
    colorNameMap["blue"] = ofColor::blue;
    colorNameMap["cyan"] = ofColor::cyan;
    colorNameMap["magenta"] = ofColor::magenta;
    colorNameMap["yellow"] = ofColor::yellow;
    colorNameMap["aliceblue"] = ofColor::aliceblue;
    colorNameMap["antiquewhite"] = ofColor::antiquewhite;
    colorNameMap["antiquewhite1"] = ofColor::antiquewhite1;
    colorNameMap["antiquewhite2"] = ofColor::antiquewhite2;
    colorNameMap["antiquewhite3"] = ofColor::antiquewhite3;
    colorNameMap["antiquewhite4"] = ofColor::antiquewhite4;
    colorNameMap["aquamarine"] = ofColor::aquamarine;
    colorNameMap["aquamarine1"] = ofColor::aquamarine1;
    colorNameMap["aquamarine2"] = ofColor::aquamarine2;
    colorNameMap["aquamarine3"] = ofColor::aquamarine3;
    colorNameMap["aquamarine4"] = ofColor::aquamarine4;
    colorNameMap["azure"] = ofColor::azure;
    colorNameMap["azure1"] = ofColor::azure1;
    colorNameMap["azure2"] = ofColor::azure2;
    colorNameMap["azure3"] = ofColor::azure3;
    colorNameMap["azure4"] = ofColor::azure4;
    colorNameMap["beige"] = ofColor::beige;
    colorNameMap["bisque"] = ofColor::bisque;
    colorNameMap["bisque1"] = ofColor::bisque1;
    colorNameMap["bisque2"] = ofColor::bisque2;
    colorNameMap["bisque3"] = ofColor::bisque3;
    colorNameMap["bisque4"] = ofColor::bisque4;
    colorNameMap["blanchedalmond"] = ofColor::blanchedalmond;
    colorNameMap["blue1"] = ofColor::blue1;
    colorNameMap["blue2"] = ofColor::blue2;
    colorNameMap["blue3"] = ofColor::blue3;
    colorNameMap["blue4"] = ofColor::blue4;
    colorNameMap["blueviolet"] = ofColor::blueviolet;
    colorNameMap["brown"] = ofColor::brown;
    colorNameMap["brown1"] = ofColor::brown1;
    colorNameMap["brown2"] = ofColor::brown2;
    colorNameMap["brown3"] = ofColor::brown3;
    colorNameMap["brown4"] = ofColor::brown4;
    colorNameMap["burlywood"] = ofColor::burlywood;
    colorNameMap["burlywood1"] = ofColor::burlywood1;
    colorNameMap["burlywood2"] = ofColor::burlywood2;
    colorNameMap["burlywood3"] = ofColor::burlywood3;
    colorNameMap["burlywood4"] = ofColor::burlywood4;
    colorNameMap["cadetblue"] = ofColor::cadetblue;
    colorNameMap["cadetblue1"] = ofColor::cadetblue1;
    colorNameMap["cadetblue2"] = ofColor::cadetblue2;
    colorNameMap["cadetblue3"] = ofColor::cadetblue3;
    colorNameMap["cadetblue4"] = ofColor::cadetblue4;
    colorNameMap["chartreuse"] = ofColor::chartreuse;
    colorNameMap["chartreuse1"] = ofColor::chartreuse1;
    colorNameMap["chartreuse2"] = ofColor::chartreuse2;
    colorNameMap["chartreuse3"] = ofColor::chartreuse3;
    colorNameMap["chartreuse4"] = ofColor::chartreuse4;
    colorNameMap["chocolate"] = ofColor::chocolate;
    colorNameMap["chocolate1"] = ofColor::chocolate1;
    colorNameMap["chocolate2"] = ofColor::chocolate2;
    colorNameMap["chocolate3"] = ofColor::chocolate3;
    colorNameMap["chocolate4"] = ofColor::chocolate4;
    colorNameMap["coral"] = ofColor::coral;
    colorNameMap["coral1"] = ofColor::coral1;
    colorNameMap["coral2"] = ofColor::coral2;
    colorNameMap["coral3"] = ofColor::coral3;
    colorNameMap["coral4"] = ofColor::coral4;
    colorNameMap["cornflowerblue"] = ofColor::cornflowerblue;
    colorNameMap["cornsilk"] = ofColor::cornsilk;
    colorNameMap["cornsilk1"] = ofColor::cornsilk1;
    colorNameMap["cornsilk2"] = ofColor::cornsilk2;
    colorNameMap["cornsilk3"] = ofColor::cornsilk3;
    colorNameMap["cornsilk4"] = ofColor::cornsilk4;
    colorNameMap["cyan1"] = ofColor::cyan1;
    colorNameMap["cyan2"] = ofColor::cyan2;
    colorNameMap["cyan3"] = ofColor::cyan3;
    colorNameMap["cyan4"] = ofColor::cyan4;
    colorNameMap["darkblue"] = ofColor::darkblue;
    colorNameMap["darkcyan"] = ofColor::darkcyan;
    colorNameMap["darkgoldenrod"] = ofColor::darkgoldenrod;
    colorNameMap["darkgoldenrod1"] = ofColor::darkgoldenrod1;
    colorNameMap["darkgoldenrod2"] = ofColor::darkgoldenrod2;
    colorNameMap["darkgoldenrod3"] = ofColor::darkgoldenrod3;
    colorNameMap["darkgoldenrod4"] = ofColor::darkgoldenrod4;
    colorNameMap["darkgray"] = ofColor::darkgray;
    colorNameMap["darkgreen"] = ofColor::darkgreen;
    colorNameMap["darkgrey"] = ofColor::darkgrey;
    colorNameMap["darkkhaki"] = ofColor::darkkhaki;
    colorNameMap["darkmagenta"] = ofColor::darkmagenta;
    colorNameMap["darkolivegreen"] = ofColor::darkolivegreen;
    colorNameMap["darkolivegreen1"] = ofColor::darkolivegreen1;
    colorNameMap["darkolivegreen2"] = ofColor::darkolivegreen2;
    colorNameMap["darkolivegreen3"] = ofColor::darkolivegreen3;
    colorNameMap["darkolivegreen4"] = ofColor::darkolivegreen4;
    colorNameMap["darkorange"] = ofColor::darkorange;
    colorNameMap["darkorange1"] = ofColor::darkorange1;
    colorNameMap["darkorange2"] = ofColor::darkorange2;
    colorNameMap["darkorange3"] = ofColor::darkorange3;
    colorNameMap["darkorange4"] = ofColor::darkorange4;
    colorNameMap["darkorchid"] = ofColor::darkorchid;
    colorNameMap["darkorchid1"] = ofColor::darkorchid1;
    colorNameMap["darkorchid2"] = ofColor::darkorchid2;
    colorNameMap["darkorchid3"] = ofColor::darkorchid3;
    colorNameMap["darkorchid4"] = ofColor::darkorchid4;
    colorNameMap["darkred"] = ofColor::darkred;
    colorNameMap["darksalmon"] = ofColor::darksalmon;
    colorNameMap["darkseagreen"] = ofColor::darkseagreen;
    colorNameMap["darkseagreen1"] = ofColor::darkseagreen1;
    colorNameMap["darkseagreen2"] = ofColor::darkseagreen2;
    colorNameMap["darkseagreen3"] = ofColor::darkseagreen3;
    colorNameMap["darkseagreen4"] = ofColor::darkseagreen4;
    colorNameMap["darkslateblue"] = ofColor::darkslateblue;
    colorNameMap["darkslategray"] = ofColor::darkslategray;
    colorNameMap["darkslategray1"] = ofColor::darkslategray1;
    colorNameMap["darkslategray2"] = ofColor::darkslategray2;
    colorNameMap["darkslategray3"] = ofColor::darkslategray3;
    colorNameMap["darkslategray4"] = ofColor::darkslategray4;
    colorNameMap["darkslategrey"] = ofColor::darkslategrey;
    colorNameMap["darkturquoise"] = ofColor::darkturquoise;
    colorNameMap["darkviolet"] = ofColor::darkviolet;
    colorNameMap["deeppink"] = ofColor::deeppink;
    colorNameMap["deeppink1"] = ofColor::deeppink1;
    colorNameMap["deeppink2"] = ofColor::deeppink2;
    colorNameMap["deeppink3"] = ofColor::deeppink3;
    colorNameMap["deeppink4"] = ofColor::deeppink4;
    colorNameMap["deepskyblue"] = ofColor::deepskyblue;
    colorNameMap["deepskyblue1"] = ofColor::deepskyblue1;
    colorNameMap["deepskyblue2"] = ofColor::deepskyblue2;
    colorNameMap["deepskyblue3"] = ofColor::deepskyblue3;
    colorNameMap["deepskyblue4"] = ofColor::deepskyblue4;
    colorNameMap["dimgray"] = ofColor::dimgray;
    colorNameMap["dimgrey"] = ofColor::dimgrey;
    colorNameMap["dodgerblue"] = ofColor::dodgerblue;
    colorNameMap["dodgerblue1"] = ofColor::dodgerblue1;
    colorNameMap["dodgerblue2"] = ofColor::dodgerblue2;
    colorNameMap["dodgerblue3"] = ofColor::dodgerblue3;
    colorNameMap["dodgerblue4"] = ofColor::dodgerblue4;
    colorNameMap["firebrick"] = ofColor::firebrick;
    colorNameMap["firebrick1"] = ofColor::firebrick1;
    colorNameMap["firebrick2"] = ofColor::firebrick2;
    colorNameMap["firebrick3"] = ofColor::firebrick3;
    colorNameMap["firebrick4"] = ofColor::firebrick4;
    colorNameMap["floralwhite"] = ofColor::floralwhite;
    colorNameMap["forestgreen"] = ofColor::forestgreen;
    colorNameMap["gainsboro"] = ofColor::gainsboro;
    colorNameMap["ghostwhite"] = ofColor::ghostwhite;
    colorNameMap["gold"] = ofColor::gold;
    colorNameMap["gold1"] = ofColor::gold1;
    colorNameMap["gold2"] = ofColor::gold2;
    colorNameMap["gold3"] = ofColor::gold3;
    colorNameMap["gold4"] = ofColor::gold4;
    colorNameMap["goldenrod"] = ofColor::goldenrod;
    colorNameMap["goldenrod1"] = ofColor::goldenrod1;
    colorNameMap["goldenrod2"] = ofColor::goldenrod2;
    colorNameMap["goldenrod3"] = ofColor::goldenrod3;
    colorNameMap["goldenrod4"] = ofColor::goldenrod4;
    colorNameMap["green1"] = ofColor::green1;
    colorNameMap["green2"] = ofColor::green2;
    colorNameMap["green3"] = ofColor::green3;
    colorNameMap["green4"] = ofColor::green4;
    colorNameMap["greenyellow"] = ofColor::greenyellow;
    colorNameMap["grey"] = ofColor::grey;
    colorNameMap["honeydew"] = ofColor::honeydew;
    colorNameMap["honeydew1"] = ofColor::honeydew1;
    colorNameMap["honeydew2"] = ofColor::honeydew2;
    colorNameMap["honeydew3"] = ofColor::honeydew3;
    colorNameMap["honeydew4"] = ofColor::honeydew4;
    colorNameMap["hotpink"] = ofColor::hotpink;
    colorNameMap["hotpink1"] = ofColor::hotpink1;
    colorNameMap["hotpink2"] = ofColor::hotpink2;
    colorNameMap["hotpink3"] = ofColor::hotpink3;
    colorNameMap["hotpink4"] = ofColor::hotpink4;
    colorNameMap["indianred"] = ofColor::indianred;
    colorNameMap["indianred1"] = ofColor::indianred1;
    colorNameMap["indianred2"] = ofColor::indianred2;
    colorNameMap["indianred3"] = ofColor::indianred3;
    colorNameMap["indianred4"] = ofColor::indianred4;
    colorNameMap["ivory"] = ofColor::ivory;
    colorNameMap["ivory1"] = ofColor::ivory1;
    colorNameMap["ivory2"] = ofColor::ivory2;
    colorNameMap["ivory3"] = ofColor::ivory3;
    colorNameMap["ivory4"] = ofColor::ivory4;
    colorNameMap["khaki"] = ofColor::khaki;
    colorNameMap["khaki1"] = ofColor::khaki1;
    colorNameMap["khaki2"] = ofColor::khaki2;
    colorNameMap["khaki3"] = ofColor::khaki3;
    colorNameMap["khaki4"] = ofColor::khaki4;
    colorNameMap["lavender"] = ofColor::lavender;
    colorNameMap["lavenderblush"] = ofColor::lavenderblush;
    colorNameMap["lavenderblush1"] = ofColor::lavenderblush1;
    colorNameMap["lavenderblush2"] = ofColor::lavenderblush2;
    colorNameMap["lavenderblush3"] = ofColor::lavenderblush3;
    colorNameMap["lavenderblush4"] = ofColor::lavenderblush4;
    colorNameMap["lawngreen"] = ofColor::lawngreen;
    colorNameMap["lemonchiffon"] = ofColor::lemonchiffon;
    colorNameMap["lemonchiffon1"] = ofColor::lemonchiffon1;
    colorNameMap["lemonchiffon2"] = ofColor::lemonchiffon2;
    colorNameMap["lemonchiffon3"] = ofColor::lemonchiffon3;
    colorNameMap["lemonchiffon4"] = ofColor::lemonchiffon4;
    colorNameMap["lightblue"] = ofColor::lightblue;
    colorNameMap["lightblue1"] = ofColor::lightblue1;
    colorNameMap["lightblue2"] = ofColor::lightblue2;
    colorNameMap["lightblue3"] = ofColor::lightblue3;
    colorNameMap["lightblue4"] = ofColor::lightblue4;
    colorNameMap["lightcoral"] = ofColor::lightcoral;
    colorNameMap["lightcyan"] = ofColor::lightcyan;
    colorNameMap["lightcyan1"] = ofColor::lightcyan1;
    colorNameMap["lightcyan2"] = ofColor::lightcyan2;
    colorNameMap["lightcyan3"] = ofColor::lightcyan3;
    colorNameMap["lightcyan4"] = ofColor::lightcyan4;
    colorNameMap["lightgoldenrod"] = ofColor::lightgoldenrod;
    colorNameMap["lightgoldenrod1"] = ofColor::lightgoldenrod1;
    colorNameMap["lightgoldenrod2"] = ofColor::lightgoldenrod2;
    colorNameMap["lightgoldenrod3"] = ofColor::lightgoldenrod3;
    colorNameMap["lightgoldenrod4"] = ofColor::lightgoldenrod4;
    colorNameMap["lightgoldenrodyellow"] = ofColor::lightgoldenrodyellow;
    colorNameMap["lightgray"] = ofColor::lightgray;
    colorNameMap["lightgreen"] = ofColor::lightgreen;
    colorNameMap["lightgrey"] = ofColor::lightgrey;
    colorNameMap["lightpink"] = ofColor::lightpink;
    colorNameMap["lightpink1"] = ofColor::lightpink1;
    colorNameMap["lightpink2"] = ofColor::lightpink2;
    colorNameMap["lightpink3"] = ofColor::lightpink3;
    colorNameMap["lightpink4"] = ofColor::lightpink4;
    colorNameMap["lightsalmon"] = ofColor::lightsalmon;
    colorNameMap["lightsalmon1"] = ofColor::lightsalmon1;
    colorNameMap["lightsalmon2"] = ofColor::lightsalmon2;
    colorNameMap["lightsalmon3"] = ofColor::lightsalmon3;
    colorNameMap["lightsalmon4"] = ofColor::lightsalmon4;
    colorNameMap["lightseagreen"] = ofColor::lightseagreen;
    colorNameMap["lightskyblue"] = ofColor::lightskyblue;
    colorNameMap["lightskyblue1"] = ofColor::lightskyblue1;
    colorNameMap["lightskyblue2"] = ofColor::lightskyblue2;
    colorNameMap["lightskyblue3"] = ofColor::lightskyblue3;
    colorNameMap["lightskyblue4"] = ofColor::lightskyblue4;
    colorNameMap["lightslateblue"] = ofColor::lightslateblue;
    colorNameMap["lightslategray"] = ofColor::lightslategray;
    colorNameMap["lightslategrey"] = ofColor::lightslategrey;
    colorNameMap["lightsteelblue"] = ofColor::lightsteelblue;
    colorNameMap["lightsteelblue1"] = ofColor::lightsteelblue1;
    colorNameMap["lightsteelblue2"] = ofColor::lightsteelblue2;
    colorNameMap["lightsteelblue3"] = ofColor::lightsteelblue3;
    colorNameMap["lightsteelblue4"] = ofColor::lightsteelblue4;
    colorNameMap["lightyellow"] = ofColor::lightyellow;
    colorNameMap["lightyellow1"] = ofColor::lightyellow1;
    colorNameMap["lightyellow2"] = ofColor::lightyellow2;
    colorNameMap["lightyellow3"] = ofColor::lightyellow3;
    colorNameMap["lightyellow4"] = ofColor::lightyellow4;
    colorNameMap["limegreen"] = ofColor::limegreen;
    colorNameMap["linen"] = ofColor::linen;
    colorNameMap["magenta1"] = ofColor::magenta1;
    colorNameMap["magenta2"] = ofColor::magenta2;
    colorNameMap["magenta3"] = ofColor::magenta3;
    colorNameMap["magenta4"] = ofColor::magenta4;
    colorNameMap["maroon"] = ofColor::maroon;
    colorNameMap["maroon1"] = ofColor::maroon1;
    colorNameMap["maroon2"] = ofColor::maroon2;
    colorNameMap["maroon3"] = ofColor::maroon3;
    colorNameMap["maroon4"] = ofColor::maroon4;
    colorNameMap["mediumaquamarine"] = ofColor::mediumaquamarine;
    colorNameMap["mediumblue"] = ofColor::mediumblue;
    colorNameMap["mediumorchid"] = ofColor::mediumorchid;
    colorNameMap["mediumorchid1"] = ofColor::mediumorchid1;
    colorNameMap["mediumorchid2"] = ofColor::mediumorchid2;
    colorNameMap["mediumorchid3"] = ofColor::mediumorchid3;
    colorNameMap["mediumorchid4"] = ofColor::mediumorchid4;
    colorNameMap["mediumpurple"] = ofColor::mediumpurple;
    colorNameMap["mediumpurple1"] = ofColor::mediumpurple1;
    colorNameMap["mediumpurple2"] = ofColor::mediumpurple2;
    colorNameMap["mediumpurple3"] = ofColor::mediumpurple3;
    colorNameMap["mediumpurple4"] = ofColor::mediumpurple4;
    colorNameMap["mediumseagreen"] = ofColor::mediumseagreen;
    colorNameMap["mediumslateblue"] = ofColor::mediumslateblue;
    colorNameMap["mediumspringgreen"] = ofColor::mediumspringgreen;
    colorNameMap["mediumturquoise"] = ofColor::mediumturquoise;
    colorNameMap["mediumvioletred"] = ofColor::mediumvioletred;
    colorNameMap["midnightblue"] = ofColor::midnightblue;
    colorNameMap["mintcream"] = ofColor::mintcream;
    colorNameMap["mistyrose"] = ofColor::mistyrose;
    colorNameMap["mistyrose1"] = ofColor::mistyrose1;
    colorNameMap["mistyrose2"] = ofColor::mistyrose2;
    colorNameMap["mistyrose3"] = ofColor::mistyrose3;
    colorNameMap["mistyrose4"] = ofColor::mistyrose4;
    colorNameMap["moccasin"] = ofColor::moccasin;
    colorNameMap["navajowhite"] = ofColor::navajowhite;
    colorNameMap["navajowhite1"] = ofColor::navajowhite1;
    colorNameMap["navajowhite2"] = ofColor::navajowhite2;
    colorNameMap["navajowhite3"] = ofColor::navajowhite3;
    colorNameMap["navajowhite4"] = ofColor::navajowhite4;
    colorNameMap["navy"] = ofColor::navy;
    colorNameMap["navyblue"] = ofColor::navyblue;
    colorNameMap["oldlace"] = ofColor::oldlace;
    colorNameMap["olivedrab"] = ofColor::olivedrab;
    colorNameMap["olivedrab1"] = ofColor::olivedrab1;
    colorNameMap["olivedrab2"] = ofColor::olivedrab2;
    colorNameMap["olivedrab3"] = ofColor::olivedrab3;
    colorNameMap["olivedrab4"] = ofColor::olivedrab4;
    colorNameMap["orange"] = ofColor::orange;
    colorNameMap["orange1"] = ofColor::orange1;
    colorNameMap["orange2"] = ofColor::orange2;
    colorNameMap["orange3"] = ofColor::orange3;
    colorNameMap["orange4"] = ofColor::orange4;
    colorNameMap["orangered"] = ofColor::orangered;
    colorNameMap["orangered1"] = ofColor::orangered1;
    colorNameMap["orangered2"] = ofColor::orangered2;
    colorNameMap["orangered3"] = ofColor::orangered3;
    colorNameMap["orangered4"] = ofColor::orangered4;
    colorNameMap["orchid"] = ofColor::orchid;
    colorNameMap["orchid1"] = ofColor::orchid1;
    colorNameMap["orchid2"] = ofColor::orchid2;
    colorNameMap["orchid3"] = ofColor::orchid3;
    colorNameMap["orchid4"] = ofColor::orchid4;
    colorNameMap["palegoldenrod"] = ofColor::palegoldenrod;
    colorNameMap["palegreen"] = ofColor::palegreen;
    colorNameMap["palegreen1"] = ofColor::palegreen1;
    colorNameMap["palegreen2"] = ofColor::palegreen2;
    colorNameMap["palegreen3"] = ofColor::palegreen3;
    colorNameMap["palegreen4"] = ofColor::palegreen4;
    colorNameMap["paleturquoise"] = ofColor::paleturquoise;
    colorNameMap["paleturquoise1"] = ofColor::paleturquoise1;
    colorNameMap["paleturquoise2"] = ofColor::paleturquoise2;
    colorNameMap["paleturquoise3"] = ofColor::paleturquoise3;
    colorNameMap["paleturquoise4"] = ofColor::paleturquoise4;
    colorNameMap["palevioletred"] = ofColor::palevioletred;
    colorNameMap["palevioletred1"] = ofColor::palevioletred1;
    colorNameMap["palevioletred2"] = ofColor::palevioletred2;
    colorNameMap["palevioletred3"] = ofColor::palevioletred3;
    colorNameMap["palevioletred4"] = ofColor::palevioletred4;
    colorNameMap["papayawhip"] = ofColor::papayawhip;
    colorNameMap["peachpuff"] = ofColor::peachpuff;
    colorNameMap["peachpuff1"] = ofColor::peachpuff1;
    colorNameMap["peachpuff2"] = ofColor::peachpuff2;
    colorNameMap["peachpuff3"] = ofColor::peachpuff3;
    colorNameMap["peachpuff4"] = ofColor::peachpuff4;
    colorNameMap["peru"] = ofColor::peru;
    colorNameMap["pink"] = ofColor::pink;
    colorNameMap["pink1"] = ofColor::pink1;
    colorNameMap["pink2"] = ofColor::pink2;
    colorNameMap["pink3"] = ofColor::pink3;
    colorNameMap["pink4"] = ofColor::pink4;
    colorNameMap["plum"] = ofColor::plum;
    colorNameMap["plum1"] = ofColor::plum1;
    colorNameMap["plum2"] = ofColor::plum2;
    colorNameMap["plum3"] = ofColor::plum3;
    colorNameMap["plum4"] = ofColor::plum4;
    colorNameMap["powderblue"] = ofColor::powderblue;
    colorNameMap["purple"] = ofColor::purple;
    colorNameMap["purple1"] = ofColor::purple1;
    colorNameMap["purple2"] = ofColor::purple2;
    colorNameMap["purple3"] = ofColor::purple3;
    colorNameMap["purple4"] = ofColor::purple4;
    colorNameMap["red1"] = ofColor::red1;
    colorNameMap["red2"] = ofColor::red2;
    colorNameMap["red3"] = ofColor::red3;
    colorNameMap["red4"] = ofColor::red4;
    colorNameMap["rosybrown"] = ofColor::rosybrown;
    colorNameMap["rosybrown1"] = ofColor::rosybrown1;
    colorNameMap["rosybrown2"] = ofColor::rosybrown2;
    colorNameMap["rosybrown3"] = ofColor::rosybrown3;
    colorNameMap["rosybrown4"] = ofColor::rosybrown4;
    colorNameMap["royalblue"] = ofColor::royalblue;
    colorNameMap["royalblue1"] = ofColor::royalblue1;
    colorNameMap["royalblue2"] = ofColor::royalblue2;
    colorNameMap["royalblue3"] = ofColor::royalblue3;
    colorNameMap["royalblue4"] = ofColor::royalblue4;
    colorNameMap["saddlebrown"] = ofColor::saddlebrown;
    colorNameMap["salmon"] = ofColor::salmon;
    colorNameMap["salmon1"] = ofColor::salmon1;
    colorNameMap["salmon2"] = ofColor::salmon2;
    colorNameMap["salmon3"] = ofColor::salmon3;
    colorNameMap["salmon4"] = ofColor::salmon4;
    colorNameMap["sandybrown"] = ofColor::sandybrown;
    colorNameMap["seagreen"] = ofColor::seagreen;
    colorNameMap["seagreen1"] = ofColor::seagreen1;
    colorNameMap["seagreen2"] = ofColor::seagreen2;
    colorNameMap["seagreen3"] = ofColor::seagreen3;
    colorNameMap["seagreen4"] = ofColor::seagreen4;
    colorNameMap["seashell"] = ofColor::seashell;
    colorNameMap["seashell1"] = ofColor::seashell1;
    colorNameMap["seashell2"] = ofColor::seashell2;
    colorNameMap["seashell3"] = ofColor::seashell3;
    colorNameMap["seashell4"] = ofColor::seashell4;
    colorNameMap["sienna"] = ofColor::sienna;
    colorNameMap["sienna1"] = ofColor::sienna1;
    colorNameMap["sienna2"] = ofColor::sienna2;
    colorNameMap["sienna3"] = ofColor::sienna3;
    colorNameMap["sienna4"] = ofColor::sienna4;
    colorNameMap["skyblue"] = ofColor::skyblue;
    colorNameMap["skyblue1"] = ofColor::skyblue1;
    colorNameMap["skyblue2"] = ofColor::skyblue2;
    colorNameMap["skyblue3"] = ofColor::skyblue3;
    colorNameMap["skyblue4"] = ofColor::skyblue4;
    colorNameMap["slateblue"] = ofColor::slateblue;
    colorNameMap["slateblue1"] = ofColor::slateblue1;
    colorNameMap["slateblue2"] = ofColor::slateblue2;
    colorNameMap["slateblue3"] = ofColor::slateblue3;
    colorNameMap["slateblue4"] = ofColor::slateblue4;
    colorNameMap["slategray"] = ofColor::slategray;
    colorNameMap["slategray1"] = ofColor::slategray1;
    colorNameMap["slategray2"] = ofColor::slategray2;
    colorNameMap["slategray3"] = ofColor::slategray3;
    colorNameMap["slategray4"] = ofColor::slategray4;
    colorNameMap["slategrey"] = ofColor::slategrey;
    colorNameMap["snow"] = ofColor::snow;
    colorNameMap["snow1"] = ofColor::snow1;
    colorNameMap["snow2"] = ofColor::snow2;
    colorNameMap["snow3"] = ofColor::snow3;
    colorNameMap["snow4"] = ofColor::snow4;
    colorNameMap["springgreen"] = ofColor::springgreen;
    colorNameMap["springgreen1"] = ofColor::springgreen1;
    colorNameMap["springgreen2"] = ofColor::springgreen2;
    colorNameMap["springgreen3"] = ofColor::springgreen3;
    colorNameMap["springgreen4"] = ofColor::springgreen4;
    colorNameMap["steelblue"] = ofColor::steelblue;
    colorNameMap["steelblue1"] = ofColor::steelblue1;
    colorNameMap["steelblue2"] = ofColor::steelblue2;
    colorNameMap["steelblue3"] = ofColor::steelblue3;
    colorNameMap["steelblue4"] = ofColor::steelblue4;
    colorNameMap["tan"] = ofColor::tan;
    colorNameMap["tan1"] = ofColor::tan1;
    colorNameMap["tan2"] = ofColor::tan2;
    colorNameMap["tan3"] = ofColor::tan3;
    colorNameMap["tan4"] = ofColor::tan4;
    colorNameMap["thistle"] = ofColor::thistle;
    colorNameMap["thistle1"] = ofColor::thistle1;
    colorNameMap["thistle2"] = ofColor::thistle2;
    colorNameMap["thistle3"] = ofColor::thistle3;
    colorNameMap["thistle4"] = ofColor::thistle4;
    colorNameMap["tomato"] = ofColor::tomato;
    colorNameMap["tomato1"] = ofColor::tomato1;
    colorNameMap["tomato2"] = ofColor::tomato2;
    colorNameMap["tomato3"] = ofColor::tomato3;
    colorNameMap["tomato4"] = ofColor::tomato4;
    colorNameMap["turquoise"] = ofColor::turquoise;
    colorNameMap["turquoise1"] = ofColor::turquoise1;
    colorNameMap["turquoise2"] = ofColor::turquoise2;
    colorNameMap["turquoise3"] = ofColor::turquoise3;
    colorNameMap["turquoise4"] = ofColor::turquoise4;
    colorNameMap["violet"] = ofColor::violet;
    colorNameMap["violetred"] = ofColor::violetred;
    colorNameMap["violetred1"] = ofColor::violetred1;
    colorNameMap["violetred2"] = ofColor::violetred2;
    colorNameMap["violetred3"] = ofColor::violetred3;
    colorNameMap["violetred4"] = ofColor::violetred4;
    colorNameMap["wheat"] = ofColor::wheat;
    colorNameMap["wheat1"] = ofColor::wheat1;
    colorNameMap["wheat2"] = ofColor::wheat2;
    colorNameMap["wheat3"] = ofColor::wheat3;
    colorNameMap["wheat4"] = ofColor::wheat4;
    colorNameMap["whitesmoke"] = ofColor::whitesmoke;
    colorNameMap["yellow1"] = ofColor::yellow1;
    colorNameMap["yellow2"] = ofColor::yellow2;
    colorNameMap["yellow3"] = ofColor::yellow3;
    colorNameMap["yellow4"] = ofColor::yellow4;
    colorNameMap["yellowgreen"] = ofColor::yellowgreen;
    

    // this map is useful if we want to address the colors by string.
    // since we might want to sort this, we can put them in a vector also
    
    for (int i = 0; i < colorNameMap.size(); i++){
        
        map<string, ofColor>::iterator mapEntry = colorNameMap.begin();
        std::advance( mapEntry, i );
        
        colorNameMapping mapping;
        mapping.name = mapEntry->first;
        mapping.color = mapEntry->second;
        colorNames.push_back(mapping);
    
    }
    
    ofBackground(255);

    ofSetVerticalSync(true);
    
    ofEnableAlphaBlending();
    
    
}

//--------------------------------------------------------------
void testApp::update(){

    // smoothing the mouse a bit over time

    mouseSmoothed = 0.95 * mouseSmoothed + 0.05 * ofPoint(mouseX, mouseY);

}

//--------------------------------------------------------------
void testApp::draw(){
    
    // calculate the total size needed to display all the colors
    
    float totalSize = (ceil(colorNameMap.size()/3.0)) * 50 - ofGetHeight() + 60;
    
    // map the smoothed mouse to this: 
    
    float offset = ofMap(mouseSmoothed.y, 0, ofGetHeight(), 0, totalSize, true);
    
    // draw all the colors
    // note this could be optimized, since we're drawing plenty that's offscreen here.
    
  
    
    for (int i = 0; i < colorNames.size(); i++){
    
        int x = (i % 3) * ofGetWidth()/3.0;
        int y = (floor(i / 3)) * 50;
        
        ofSetColor( colorNames[i].color );
        ofRect(0 + x, y - offset, (i%3 == 2) ? ofGetWidth() - x : ofGetWidth()/3.0, 50);
        
        ofDrawBitmapStringHighlight(colorNames[i].name, 20 + x, y -offset+30, ofColor::white, ofColor::black);
        
    }

    
    
    ofSetColor(0);
    ofRect(0, ofGetHeight()-60, ofGetWidth(), 60);
    ofDrawBitmapStringHighlight("press '1' to sort by name, '2' to sort by hue,\n'3' to sort by brightness, '4' to sort by saturation", 20, ofGetHeight()-60 + 30, ofColor::black, ofColor::white);
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == '1'){
        std::sort(colorNames.begin(), colorNames.end(), compareName);
    } else if (key == '2'){
        std::sort(colorNames.begin(), colorNames.end(), compareHue);
    } else if (key == '3'){
        std::sort(colorNames.begin(), colorNames.end(), compareBrightness);
    } else if (key == '4'){
        std::sort(colorNames.begin(), colorNames.end(), compareSaturation);
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}