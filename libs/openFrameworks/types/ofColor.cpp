#include "ofColor.h"
#include "ofConstants.h"

template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gray(limit() / 2, limit() / 2, limit() / 2);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::white(limit(), limit(), limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::red(limit(), 0, 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::green(0, limit(), 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blue(0, 0, limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cyan(0, limit(), limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::magenta(limit(), 0, limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellow(limit(), limit(), 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::black(0, 0, 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aliceBlue(0.941176*limit(),0.972549*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::antiqueWhite(0.980392*limit(),0.921569*limit(),0.843137*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aqua(0*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aquamarine(0.498039*limit(),1*limit(),0.831373*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::azure(0.941176*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::beige(0.960784*limit(),0.960784*limit(),0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::bisque(1*limit(),0.894118*limit(),0.768627*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blanchedAlmond(1*limit(),0.921569*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blueViolet(0.541176*limit(),0.168627*limit(),0.886275*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::brown(0.647059*limit(),0.164706*limit(),0.164706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::burlyWood(0.870588*limit(),0.721569*limit(),0.529412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cadetBlue(0.372549*limit(),0.619608*limit(),0.627451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chartreuse(0.498039*limit(),1*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chocolate(0.823529*limit(),0.411765*limit(),0.117647*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::coral(1*limit(),0.498039*limit(),0.313726*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornflowerBlue(0.392157*limit(),0.584314*limit(),0.929412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornsilk(1*limit(),0.972549*limit(),0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::crimson(0.862745*limit(),0.0784314*limit(),0.235294*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkBlue(0*limit(),0*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkCyan(0*limit(),0.545098*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkGoldenRod(0.721569*limit(),0.52549*limit(),0.0431373*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkGray(0.662745*limit(),0.662745*limit(),0.662745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkGrey(0.662745*limit(),0.662745*limit(),0.662745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkGreen(0*limit(),0.392157*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkKhaki(0.741176*limit(),0.717647*limit(),0.419608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkMagenta(0.545098*limit(),0*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkOliveGreen(0.333333*limit(),0.419608*limit(),0.184314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorange(1*limit(),0.54902*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkOrchid(0.6*limit(),0.196078*limit(),0.8*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkRed(0.545098*limit(),0*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSalmon(0.913725*limit(),0.588235*limit(),0.478431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSeaGreen(0.560784*limit(),0.737255*limit(),0.560784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSlateBlue(0.282353*limit(),0.239216*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSlateGray(0.184314*limit(),0.309804*limit(),0.309804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSlateGrey(0.184314*limit(),0.309804*limit(),0.309804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkTurquoise(0*limit(),0.807843*limit(),0.819608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkViolet(0.580392*limit(),0*limit(),0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deepPink(1*limit(),0.0784314*limit(),0.576471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deepSkyBlue(0*limit(),0.74902*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dimGray(0.411765*limit(),0.411765*limit(),0.411765*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dimGrey(0.411765*limit(),0.411765*limit(),0.411765*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dodgerBlue(0.117647*limit(),0.564706*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::fireBrick(0.698039*limit(),0.133333*limit(),0.133333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::floralWhite(1*limit(),0.980392*limit(),0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::forestGreen(0.133333*limit(),0.545098*limit(),0.133333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::fuchsia(1*limit(),0*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gainsboro(0.862745*limit(),0.862745*limit(),0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ghostWhite(0.972549*limit(),0.972549*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gold(1*limit(),0.843137*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::goldenRod(0.854902*limit(),0.647059*limit(),0.12549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::grey(0.501961*limit(),0.501961*limit(),0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::greenYellow(0.678431*limit(),1*limit(),0.184314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::honeyDew(0.941176*limit(),1*limit(),0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::hotPink(1*limit(),0.411765*limit(),0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::indianRed (0.803922*limit(),0.360784*limit(),0.360784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::indigo (0.294118*limit(),0*limit(),0.509804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ivory(1*limit(),1*limit(),0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::khaki(0.941176*limit(),0.901961*limit(),0.54902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavender(0.901961*limit(),0.901961*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavenderBlush(1*limit(),0.941176*limit(),0.960784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lawnGreen(0.486275*limit(),0.988235*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lemonChiffon(1*limit(),0.980392*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightBlue(0.678431*limit(),0.847059*limit(),0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightCoral(0.941176*limit(),0.501961*limit(),0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightCyan(0.878431*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightGoldenRodYellow(0.980392*limit(),0.980392*limit(),0.823529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightGray(0.827451*limit(),0.827451*limit(),0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightGrey(0.827451*limit(),0.827451*limit(),0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightGreen(0.564706*limit(),0.933333*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightPink(1*limit(),0.713726*limit(),0.756863*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSalmon(1*limit(),0.627451*limit(),0.478431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSeaGreen(0.12549*limit(),0.698039*limit(),0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSkyBlue(0.529412*limit(),0.807843*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSlateGray(0.466667*limit(),0.533333*limit(),0.6*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSlateGrey(0.466667*limit(),0.533333*limit(),0.6*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSteelBlue(0.690196*limit(),0.768627*limit(),0.870588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightYellow(1*limit(),1*limit(),0.878431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lime(0*limit(),1*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::limeGreen(0.196078*limit(),0.803922*limit(),0.196078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::linen(0.980392*limit(),0.941176*limit(),0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::maroon(0.501961*limit(),0*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumAquaMarine(0.4*limit(),0.803922*limit(),0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumBlue(0*limit(),0*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumOrchid(0.729412*limit(),0.333333*limit(),0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumPurple(0.576471*limit(),0.439216*limit(),0.858824*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumSeaGreen(0.235294*limit(),0.701961*limit(),0.443137*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumSlateBlue(0.482353*limit(),0.407843*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumSpringGreen(0*limit(),0.980392*limit(),0.603922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumTurquoise(0.282353*limit(),0.819608*limit(),0.8*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumVioletRed(0.780392*limit(),0.0823529*limit(),0.521569*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::midnightBlue(0.0980392*limit(),0.0980392*limit(),0.439216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mintCream(0.960784*limit(),1*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mistyRose(1*limit(),0.894118*limit(),0.882353*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::moccasin(1*limit(),0.894118*limit(),0.709804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navajoWhite(1*limit(),0.870588*limit(),0.678431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navy(0*limit(),0*limit(),0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::oldLace(0.992157*limit(),0.960784*limit(),0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::olive(0.501961*limit(),0.501961*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::oliveDrab(0.419608*limit(),0.556863*limit(),0.137255*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orange(1*limit(),0.647059*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orangeRed(1*limit(),0.270588*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orchid(0.854902*limit(),0.439216*limit(),0.839216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleGoldenRod(0.933333*limit(),0.909804*limit(),0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleGreen(0.596078*limit(),0.984314*limit(),0.596078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleTurquoise(0.686275*limit(),0.933333*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleVioletRed(0.858824*limit(),0.439216*limit(),0.576471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::papayaWhip(1*limit(),0.937255*limit(),0.835294*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peachPuff(1*limit(),0.854902*limit(),0.72549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peru(0.803922*limit(),0.521569*limit(),0.247059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::pink(1*limit(),0.752941*limit(),0.796078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::plum(0.866667*limit(),0.627451*limit(),0.866667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::powderBlue(0.690196*limit(),0.878431*limit(),0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::purple(0.501961*limit(),0*limit(),0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::rosyBrown(0.737255*limit(),0.560784*limit(),0.560784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::royalBlue(0.254902*limit(),0.411765*limit(),0.882353*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::saddleBrown(0.545098*limit(),0.270588*limit(),0.0745098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::salmon(0.980392*limit(),0.501961*limit(),0.447059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sandyBrown(0.956863*limit(),0.643137*limit(),0.376471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seaGreen(0.180392*limit(),0.545098*limit(),0.341176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seaShell(1*limit(),0.960784*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sienna(0.627451*limit(),0.321569*limit(),0.176471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::silver(0.752941*limit(),0.752941*limit(),0.752941*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::skyBlue(0.529412*limit(),0.807843*limit(),0.921569*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateBlue(0.415686*limit(),0.352941*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateGray(0.439216*limit(),0.501961*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateGrey(0.439216*limit(),0.501961*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::snow(1*limit(),0.980392*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::springGreen(0*limit(),1*limit(),0.498039*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::steelBlue(0.27451*limit(),0.509804*limit(),0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tan(0.823529*limit(),0.705882*limit(),0.54902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::teal(0*limit(),0.501961*limit(),0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::thistle(0.847059*limit(),0.74902*limit(),0.847059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tomato(1*limit(),0.388235*limit(),0.278431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::turquoise(0.25098*limit(),0.878431*limit(),0.815686*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::violet(0.933333*limit(),0.509804*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::wheat(0.960784*limit(),0.870588*limit(),0.701961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::whiteSmoke(0.960784*limit(),0.960784*limit(),0.960784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellowGreen(0.603922*limit(),0.803922*limit(),0.196078*limit());



template<typename PixelType>
float ofColor_<PixelType>::limit() {
	return numeric_limits<PixelType>::max();
}

template<>
float ofColor_<float>::limit() {
	return 1.f;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (){
	r = limit();
	g = limit();
	b = limit();
	a = limit();
}

template<typename PixelType>
ofColor_<PixelType>::~ofColor_ (){}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (float _r, float _g, float _b, float _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (const ofColor_<PixelType> & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (const ofColor_<PixelType> & color, float _a){
	r = color.r;
	g = color.g;
	b = color.b;
	a = _a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (float gray, float _a){
	r = g = b = gray;
	a = _a;
}

template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHsb (float hue, float saturation, float brightness, float alpha) {
	ofColor_<PixelType> cur;
	cur.setHsb(hue, saturation, brightness, alpha);
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHex(int hexColor, float alpha) {
	ofColor_<PixelType> cur;
	cur.setHex(hexColor, alpha);
	return cur;
}


template<typename PixelType>
void ofColor_<PixelType>::set (float _r, float _g, float _b, float _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}


template<typename PixelType>
void ofColor_<PixelType>::set (float _gray, float _a){
	r = g = b = _gray;
	a = _a;
}


template<typename PixelType>
void ofColor_<PixelType>::set (ofColor_<PixelType> const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}


template<typename PixelType>
void ofColor_<PixelType>::setHex (int hexColor, float alpha){
	r = (hexColor >> 16) & 0xff;
	g = (hexColor >> 8) & 0xff;
	b = (hexColor >> 0) & 0xff;
	a = alpha;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::clamp (){
	r = CLAMP(r, 0.0f, limit());
	g = CLAMP(g, 0.0f, limit());
	b = CLAMP(b, 0.0f, limit());
	a = CLAMP(a, 0.0f, limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::invert() {
	r = limit() - r;
	g = limit() - g;
	b = limit() - b;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::normalize() {
	float brightness = getBrightness(); 
    // avoid division by 0
    if ( brightness > 0 ) 
    {
        r = limit() * (r / brightness);
        g = limit() * (g / brightness);
        b = limit() * (b / brightness);
    }
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::lerp(const ofColor_<PixelType>& target, float amount) {
	float invAmount = 1.f - amount;
	r = invAmount * r + amount * target.r;
	g = invAmount * g + amount * target.g;
	b = invAmount * b + amount * target.b;
	a = invAmount * a + amount * target.a;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getClamped () const {
	ofColor_<PixelType> cur(*this);
	cur.clamp();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getInverted () const {
	ofColor_<PixelType> cur(*this);
	cur.invert();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getNormalized () const {
	ofColor_<PixelType> cur(*this);
	cur.normalize();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getLerped(const ofColor_<PixelType>& target, float amount) const {
	ofColor_<PixelType> cur(*this);
	cur.lerp(target, amount);
	return cur;
}



template<typename PixelType>
float ofColor_<PixelType>::getHue() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return hue;
}


template<typename PixelType>
float ofColor_<PixelType>::getSaturation() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return saturation;
}

/*
	Brightness is simply the maximum of the three color components.
	This is used by Photoshop (HSB) and Processing (HSB).
	Brightness is also called "value".
*/

template<typename PixelType>
float ofColor_<PixelType>::getBrightness() const {
	float max = r;
	if(g > max) {
		max = g;
	}
	if(b > max) {
		max = b;
	}
	return max;
}

/*
	Lightness is the average of the three color components.
	This is used by the Lab and HSL color spaces.
*/

template<typename PixelType>
float ofColor_<PixelType>::getLightness() const {
	return (r + g + b) / 3.f;
}



template<typename PixelType>
void ofColor_<PixelType>::getHsb(float& hue, float& saturation, float& brightness) const {
	float max = getBrightness();
	
	float min = r;
	if(g < min) {
		min = g;
	}
	if(b < min) {
		min = b;
	}
	
	if(max == min) { // grays
		hue = 0.f;
		saturation = 0.f;
		brightness = max;
		return;
	}
	
	float hueSixth;
	if(r == max) {
		hueSixth = (g - b) / (max - min);
		if(hueSixth < 0.f)
			hueSixth += 6.f;
	} else if (g == max) {
		hueSixth = 2.f + (b - r) / (max - min);
	} else {
		hueSixth = 4.f + (r - g) / (max - min);
	}
	hue = limit() * hueSixth / 6.f;
	saturation = limit() * (max - min) / max;
	brightness = max;
}


template<typename PixelType>
void ofColor_<PixelType>::setHue (float hue) {
	float oldHue, saturation, brightness;
	getHsb(oldHue, saturation, brightness);
	setHsb(hue, saturation, brightness, a );
}


template<typename PixelType>
void ofColor_<PixelType>::setSaturation (float saturation) {
	float hue, oldSaturation, brightness;
	getHsb(hue, oldSaturation, brightness);
	setHsb(hue, saturation, brightness, a );
}


template<typename PixelType>
void ofColor_<PixelType>::setBrightness (float brightness) {
	float hue, saturation, oldBrightness;
	getHsb(hue, saturation, oldBrightness);
	setHsb(hue, saturation, brightness, a );
}


template<typename PixelType>
void ofColor_<PixelType>::setHsb(float hue, float saturation, float brightness, float alpha ) {
	saturation = ofClamp(saturation, 0, limit());
	brightness = ofClamp(brightness, 0, limit());
	if(brightness == 0) { // black
		set(0);
	} else if(saturation == 0) { // grays
		set(brightness);
	} else {
		float hueSix = hue * 6. / limit();
		float saturationNorm = saturation / limit();
		int hueSixCategory = (int) floorf(hueSix);
		float hueSixRemainder = hueSix - hueSixCategory;
		PixelType pv = (PixelType) ((1.f - saturationNorm) * brightness);
		PixelType qv = (PixelType) ((1.f - saturationNorm * hueSixRemainder) * brightness);
		PixelType tv = (PixelType) ((1.f - saturationNorm * (1.f - hueSixRemainder)) * brightness);
		switch(hueSixCategory) {
			case 0: // r
				r = brightness;
				g = tv;
				b = pv;
				break;
			case 1: // g
				r = qv;
				g = brightness;
				b = pv;
				break;
			case 2:
				r = pv;
				g = brightness;
				b = tv;
				break;
			case 3: // b
				r = pv;
				g = qv;
				b = brightness;
				break;
			case 4:
				r = tv;
				g = pv;
				b = brightness;
				break;
			case 5: // back to r
				r = brightness;
				g = pv;
				b = qv;
				break;
		}
	}
    
    // finally assign the alpha
    a = alpha;
}


template<typename PixelType>
int ofColor_<PixelType>::getHex () const {
	return
		((0xff & (unsigned char) r) << 16) |
		((0xff & (unsigned char) g) << 8) |
		((0xff & (unsigned char) b));
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator = (ofColor_<PixelType> const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator = (float const & val){
	r = val;
	g = val;
	b = val;
	a = limit();
	return *this;
}


template<typename PixelType>
bool ofColor_<PixelType>::operator == (ofColor_<PixelType> const & color){
	return (r == color.r) && (g == color.g) && (b == color.b);
}


template<typename PixelType>
bool ofColor_<PixelType>::operator != (ofColor_<PixelType> const & color){
	return (r != color.r) || (g != color.g) || (b != color.b);
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( r+color.r, g+color.g, b+color.b ).clamp();
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (float const & val) const{
	return ofColor_<PixelType>( r+val, g+val, b+val ).clamp();
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (ofColor_<PixelType> const & color){
	r += color.r;
	g += color.g;
	b += color.b;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (float const & val){
	r += val;
	g += val;
	b += val;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( r-color.r, g-color.g, b-color.b ).clamp();
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (float const & val) const{
	return ofColor_<PixelType>( r-val, g-val, b-val).clamp();
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (ofColor_<PixelType> const & color){
	r -= color.r;
	g -= color.g;
	b -= color.b;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (float const & val){
	r -= val;
	g -= val;
	b -= val;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( r*(color.r/limit()), g*(color.g/limit()), b*(color.b/limit()));
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (float const & val) const{
	float v = CLAMP(val,0.0,1.0);
	return ofColor_<PixelType>( r*v, g*v, b*v).clamp();
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (ofColor_<PixelType> const & color){
	r *= (color.r/limit());
	g *= (color.g/limit());
	b *= (color.b/limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (float const & val){
	float v = CLAMP(val,0.0,1.0);
	r *= v;
	g *= v;
	b *= v;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( color.r!=0.0f ? r/(color.r/limit()) : r , color.g!=0.0f ? g/(color.g/limit()) : g, color.b!=0.0f ? b/(color.b/limit()) : b );
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (float const & val) const{
	if( val != 0.0f)
		return ofColor_<PixelType>( r/val, g/val, b/val ).clamp();
	else
		return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (ofColor_<PixelType> const & color){
	if (color.r!=0.0f) r /= (color.r/limit());
	if (color.g!=0.0f) g /= (color.g/limit());
	if (color.b!=0.0f) b /= (color.b/limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (float const & val){
	if( val != 0.0f ){
		r /= val;
		g /= val;
		b /= val;
		clamp();
		return *this;
	}
	else
		return *this;
}


template<typename PixelType>
const PixelType & ofColor_<PixelType>::operator [] (int n) const{
	switch( n ){
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3: 
			return a;
		default:
			return r;
			break;
	}
}

template<typename PixelType>
PixelType & ofColor_<PixelType>::operator [] (int n){
	switch( n ){
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3:
			return a;
		default:
			return r;
			break;
	}
}

template class ofColor_<char>;
template class ofColor_<unsigned char>;
template class ofColor_<short>;
template class ofColor_<unsigned short>;
template class ofColor_<int>;
template class ofColor_<unsigned int>;
template class ofColor_<long>;
template class ofColor_<unsigned long>;
template class ofColor_<float>;
template class ofColor_<double>;