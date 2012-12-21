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
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aliceblue(0.941176*limit(),0.972549*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::antiquewhite(0.980392*limit(),0.921569*limit(),0.843137*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::antiquewhite1(1*limit(),0.937255*limit(),0.858824*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::antiquewhite2(0.933333*limit(),0.87451*limit(),0.8*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::antiquewhite3(0.803922*limit(),0.752941*limit(),0.690196*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::antiquewhite4(0.545098*limit(),0.513726*limit(),0.470588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aquamarine(0.498039*limit(),1*limit(),0.831373*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aquamarine1(0.498039*limit(),1*limit(),0.831373*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aquamarine2(0.462745*limit(),0.933333*limit(),0.776471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aquamarine3(0.4*limit(),0.803922*limit(),0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aquamarine4(0.270588*limit(),0.545098*limit(),0.454902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::azure(0.941176*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::azure1(0.941176*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::azure2(0.878431*limit(),0.933333*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::azure3(0.756863*limit(),0.803922*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::azure4(0.513726*limit(),0.545098*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::beige(0.960784*limit(),0.960784*limit(),0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::bisque(1*limit(),0.894118*limit(),0.768627*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::bisque1(1*limit(),0.894118*limit(),0.768627*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::bisque2(0.933333*limit(),0.835294*limit(),0.717647*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::bisque3(0.803922*limit(),0.717647*limit(),0.619608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::bisque4(0.545098*limit(),0.490196*limit(),0.419608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blanchedalmond(1*limit(),0.921569*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blue1(0*limit(),0*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blue2(0*limit(),0*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blue3(0*limit(),0*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blue4(0*limit(),0*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blueviolet(0.541176*limit(),0.168627*limit(),0.886275*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::brown(0.647059*limit(),0.164706*limit(),0.164706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::brown1(1*limit(),0.25098*limit(),0.25098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::brown2(0.933333*limit(),0.231373*limit(),0.231373*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::brown3(0.803922*limit(),0.2*limit(),0.2*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::brown4(0.545098*limit(),0.137255*limit(),0.137255*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::burlywood(0.870588*limit(),0.721569*limit(),0.529412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::burlywood1(1*limit(),0.827451*limit(),0.607843*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::burlywood2(0.933333*limit(),0.772549*limit(),0.568627*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::burlywood3(0.803922*limit(),0.666667*limit(),0.490196*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::burlywood4(0.545098*limit(),0.45098*limit(),0.333333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cadetblue(0.372549*limit(),0.619608*limit(),0.627451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cadetblue1(0.596078*limit(),0.960784*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cadetblue2(0.556863*limit(),0.898039*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cadetblue3(0.478431*limit(),0.772549*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cadetblue4(0.32549*limit(),0.52549*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chartreuse(0.498039*limit(),1*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chartreuse1(0.498039*limit(),1*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chartreuse2(0.462745*limit(),0.933333*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chartreuse3(0.4*limit(),0.803922*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chartreuse4(0.270588*limit(),0.545098*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chocolate(0.823529*limit(),0.411765*limit(),0.117647*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chocolate1(1*limit(),0.498039*limit(),0.141176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chocolate2(0.933333*limit(),0.462745*limit(),0.129412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chocolate3(0.803922*limit(),0.4*limit(),0.113725*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chocolate4(0.545098*limit(),0.270588*limit(),0.0745098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::coral(1*limit(),0.498039*limit(),0.313726*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::coral1(1*limit(),0.447059*limit(),0.337255*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::coral2(0.933333*limit(),0.415686*limit(),0.313726*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::coral3(0.803922*limit(),0.356863*limit(),0.270588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::coral4(0.545098*limit(),0.243137*limit(),0.184314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornflowerblue(0.392157*limit(),0.584314*limit(),0.929412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornsilk(1*limit(),0.972549*limit(),0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornsilk1(1*limit(),0.972549*limit(),0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornsilk2(0.933333*limit(),0.909804*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornsilk3(0.803922*limit(),0.784314*limit(),0.694118*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornsilk4(0.545098*limit(),0.533333*limit(),0.470588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cyan1(0*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cyan2(0*limit(),0.933333*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cyan3(0*limit(),0.803922*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cyan4(0*limit(),0.545098*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkblue(0*limit(),0*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkcyan(0*limit(),0.545098*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkgoldenrod(0.721569*limit(),0.52549*limit(),0.0431373*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkgoldenrod1(1*limit(),0.72549*limit(),0.0588235*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkgoldenrod2(0.933333*limit(),0.678431*limit(),0.054902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkgoldenrod3(0.803922*limit(),0.584314*limit(),0.0470588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkgoldenrod4(0.545098*limit(),0.396078*limit(),0.0313726*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkgray(0.662745*limit(),0.662745*limit(),0.662745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkgreen(0*limit(),0.392157*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkgrey(0.662745*limit(),0.662745*limit(),0.662745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkkhaki(0.741176*limit(),0.717647*limit(),0.419608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkmagenta(0.545098*limit(),0*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkolivegreen(0.333333*limit(),0.419608*limit(),0.184314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkolivegreen1(0.792157*limit(),1*limit(),0.439216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkolivegreen2(0.737255*limit(),0.933333*limit(),0.407843*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkolivegreen3(0.635294*limit(),0.803922*limit(),0.352941*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkolivegreen4(0.431373*limit(),0.545098*limit(),0.239216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorange(1*limit(),0.54902*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorange1(1*limit(),0.498039*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorange2(0.933333*limit(),0.462745*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorange3(0.803922*limit(),0.4*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorange4(0.545098*limit(),0.270588*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorchid(0.6*limit(),0.196078*limit(),0.8*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorchid1(0.74902*limit(),0.243137*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorchid2(0.698039*limit(),0.227451*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorchid3(0.603922*limit(),0.196078*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorchid4(0.407843*limit(),0.133333*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkred(0.545098*limit(),0*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darksalmon(0.913725*limit(),0.588235*limit(),0.478431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkseagreen(0.560784*limit(),0.737255*limit(),0.560784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkseagreen1(0.756863*limit(),1*limit(),0.756863*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkseagreen2(0.705882*limit(),0.933333*limit(),0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkseagreen3(0.607843*limit(),0.803922*limit(),0.607843*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkseagreen4(0.411765*limit(),0.545098*limit(),0.411765*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkslateblue(0.282353*limit(),0.239216*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkslategray(0.184314*limit(),0.309804*limit(),0.309804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkslategray1(0.592157*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkslategray2(0.552941*limit(),0.933333*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkslategray3(0.47451*limit(),0.803922*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkslategray4(0.321569*limit(),0.545098*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkslategrey(0.184314*limit(),0.309804*limit(),0.309804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkturquoise(0*limit(),0.807843*limit(),0.819608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkviolet(0.580392*limit(),0*limit(),0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deeppink(1*limit(),0.0784314*limit(),0.576471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deeppink1(1*limit(),0.0784314*limit(),0.576471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deeppink2(0.933333*limit(),0.0705882*limit(),0.537255*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deeppink3(0.803922*limit(),0.0627451*limit(),0.462745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deeppink4(0.545098*limit(),0.0392157*limit(),0.313726*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deepskyblue(0*limit(),0.74902*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deepskyblue1(0*limit(),0.74902*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deepskyblue2(0*limit(),0.698039*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deepskyblue3(0*limit(),0.603922*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deepskyblue4(0*limit(),0.407843*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dimgray(0.411765*limit(),0.411765*limit(),0.411765*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dimgrey(0.411765*limit(),0.411765*limit(),0.411765*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dodgerblue(0.117647*limit(),0.564706*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dodgerblue1(0.117647*limit(),0.564706*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dodgerblue2(0.109804*limit(),0.52549*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dodgerblue3(0.0941176*limit(),0.454902*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dodgerblue4(0.0627451*limit(),0.305882*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::firebrick(0.698039*limit(),0.133333*limit(),0.133333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::firebrick1(1*limit(),0.188235*limit(),0.188235*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::firebrick2(0.933333*limit(),0.172549*limit(),0.172549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::firebrick3(0.803922*limit(),0.14902*limit(),0.14902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::firebrick4(0.545098*limit(),0.101961*limit(),0.101961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::floralwhite(1*limit(),0.980392*limit(),0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::forestgreen(0.133333*limit(),0.545098*limit(),0.133333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gainsboro(0.862745*limit(),0.862745*limit(),0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ghostwhite(0.972549*limit(),0.972549*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gold(1*limit(),0.843137*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gold1(1*limit(),0.843137*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gold2(0.933333*limit(),0.788235*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gold3(0.803922*limit(),0.678431*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gold4(0.545098*limit(),0.458824*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::goldenrod(0.854902*limit(),0.647059*limit(),0.12549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::goldenrod1(1*limit(),0.756863*limit(),0.145098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::goldenrod2(0.933333*limit(),0.705882*limit(),0.133333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::goldenrod3(0.803922*limit(),0.607843*limit(),0.113725*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::goldenrod4(0.545098*limit(),0.411765*limit(),0.0784314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::green1(0*limit(),1*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::green2(0*limit(),0.933333*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::green3(0*limit(),0.803922*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::green4(0*limit(),0.545098*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::greenyellow(0.678431*limit(),1*limit(),0.184314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::grey(0.745098*limit(),0.745098*limit(),0.745098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::honeydew(0.941176*limit(),1*limit(),0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::honeydew1(0.941176*limit(),1*limit(),0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::honeydew2(0.878431*limit(),0.933333*limit(),0.878431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::honeydew3(0.756863*limit(),0.803922*limit(),0.756863*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::honeydew4(0.513726*limit(),0.545098*limit(),0.513726*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::hotpink(1*limit(),0.411765*limit(),0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::hotpink1(1*limit(),0.431373*limit(),0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::hotpink2(0.933333*limit(),0.415686*limit(),0.654902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::hotpink3(0.803922*limit(),0.376471*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::hotpink4(0.545098*limit(),0.227451*limit(),0.384314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::indianred(0.803922*limit(),0.360784*limit(),0.360784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::indianred1(1*limit(),0.415686*limit(),0.415686*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::indianred2(0.933333*limit(),0.388235*limit(),0.388235*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::indianred3(0.803922*limit(),0.333333*limit(),0.333333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::indianred4(0.545098*limit(),0.227451*limit(),0.227451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ivory(1*limit(),1*limit(),0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ivory1(1*limit(),1*limit(),0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ivory2(0.933333*limit(),0.933333*limit(),0.878431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ivory3(0.803922*limit(),0.803922*limit(),0.756863*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ivory4(0.545098*limit(),0.545098*limit(),0.513726*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::khaki(0.941176*limit(),0.901961*limit(),0.54902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::khaki1(1*limit(),0.964706*limit(),0.560784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::khaki2(0.933333*limit(),0.901961*limit(),0.521569*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::khaki3(0.803922*limit(),0.776471*limit(),0.45098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::khaki4(0.545098*limit(),0.52549*limit(),0.305882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavender(0.901961*limit(),0.901961*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavenderblush(1*limit(),0.941176*limit(),0.960784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavenderblush1(1*limit(),0.941176*limit(),0.960784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavenderblush2(0.933333*limit(),0.878431*limit(),0.898039*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavenderblush3(0.803922*limit(),0.756863*limit(),0.772549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavenderblush4(0.545098*limit(),0.513726*limit(),0.52549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lawngreen(0.486275*limit(),0.988235*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lemonchiffon(1*limit(),0.980392*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lemonchiffon1(1*limit(),0.980392*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lemonchiffon2(0.933333*limit(),0.913725*limit(),0.74902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lemonchiffon3(0.803922*limit(),0.788235*limit(),0.647059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lemonchiffon4(0.545098*limit(),0.537255*limit(),0.439216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightblue(0.678431*limit(),0.847059*limit(),0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightblue1(0.74902*limit(),0.937255*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightblue2(0.698039*limit(),0.87451*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightblue3(0.603922*limit(),0.752941*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightblue4(0.407843*limit(),0.513726*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightcoral(0.941176*limit(),0.501961*limit(),0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightcyan(0.878431*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightcyan1(0.878431*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightcyan2(0.819608*limit(),0.933333*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightcyan3(0.705882*limit(),0.803922*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightcyan4(0.478431*limit(),0.545098*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightgoldenrod(0.933333*limit(),0.866667*limit(),0.509804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightgoldenrod1(1*limit(),0.92549*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightgoldenrod2(0.933333*limit(),0.862745*limit(),0.509804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightgoldenrod3(0.803922*limit(),0.745098*limit(),0.439216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightgoldenrod4(0.545098*limit(),0.505882*limit(),0.298039*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightgoldenrodyellow(0.980392*limit(),0.980392*limit(),0.823529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightgray(0.827451*limit(),0.827451*limit(),0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightgreen(0.564706*limit(),0.933333*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightgrey(0.827451*limit(),0.827451*limit(),0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightpink(1*limit(),0.713726*limit(),0.756863*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightpink1(1*limit(),0.682353*limit(),0.72549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightpink2(0.933333*limit(),0.635294*limit(),0.678431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightpink3(0.803922*limit(),0.54902*limit(),0.584314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightpink4(0.545098*limit(),0.372549*limit(),0.396078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsalmon(1*limit(),0.627451*limit(),0.478431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsalmon1(1*limit(),0.627451*limit(),0.478431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsalmon2(0.933333*limit(),0.584314*limit(),0.447059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsalmon3(0.803922*limit(),0.505882*limit(),0.384314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsalmon4(0.545098*limit(),0.341176*limit(),0.258824*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightseagreen(0.12549*limit(),0.698039*limit(),0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightskyblue(0.529412*limit(),0.807843*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightskyblue1(0.690196*limit(),0.886275*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightskyblue2(0.643137*limit(),0.827451*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightskyblue3(0.552941*limit(),0.713726*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightskyblue4(0.376471*limit(),0.482353*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightslateblue(0.517647*limit(),0.439216*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightslategray(0.466667*limit(),0.533333*limit(),0.6*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightslategrey(0.466667*limit(),0.533333*limit(),0.6*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsteelblue(0.690196*limit(),0.768627*limit(),0.870588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsteelblue1(0.792157*limit(),0.882353*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsteelblue2(0.737255*limit(),0.823529*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsteelblue3(0.635294*limit(),0.709804*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightsteelblue4(0.431373*limit(),0.482353*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightyellow(1*limit(),1*limit(),0.878431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightyellow1(1*limit(),1*limit(),0.878431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightyellow2(0.933333*limit(),0.933333*limit(),0.819608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightyellow3(0.803922*limit(),0.803922*limit(),0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightyellow4(0.545098*limit(),0.545098*limit(),0.478431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::limegreen(0.196078*limit(),0.803922*limit(),0.196078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::linen(0.980392*limit(),0.941176*limit(),0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::magenta1(1*limit(),0*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::magenta2(0.933333*limit(),0*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::magenta3(0.803922*limit(),0*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::magenta4(0.545098*limit(),0*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::maroon(0.690196*limit(),0.188235*limit(),0.376471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::maroon1(1*limit(),0.203922*limit(),0.701961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::maroon2(0.933333*limit(),0.188235*limit(),0.654902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::maroon3(0.803922*limit(),0.160784*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::maroon4(0.545098*limit(),0.109804*limit(),0.384314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumaquamarine(0.4*limit(),0.803922*limit(),0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumblue(0*limit(),0*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumorchid(0.729412*limit(),0.333333*limit(),0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumorchid1(0.878431*limit(),0.4*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumorchid2(0.819608*limit(),0.372549*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumorchid3(0.705882*limit(),0.321569*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumorchid4(0.478431*limit(),0.215686*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumpurple(0.576471*limit(),0.439216*limit(),0.858824*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumpurple1(0.670588*limit(),0.509804*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumpurple2(0.623529*limit(),0.47451*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumpurple3(0.537255*limit(),0.407843*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumpurple4(0.364706*limit(),0.278431*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumseagreen(0.235294*limit(),0.701961*limit(),0.443137*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumslateblue(0.482353*limit(),0.407843*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumspringgreen(0*limit(),0.980392*limit(),0.603922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumturquoise(0.282353*limit(),0.819608*limit(),0.8*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumvioletred(0.780392*limit(),0.0823529*limit(),0.521569*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::midnightblue(0.0980392*limit(),0.0980392*limit(),0.439216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mintcream(0.960784*limit(),1*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mistyrose(1*limit(),0.894118*limit(),0.882353*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mistyrose1(1*limit(),0.894118*limit(),0.882353*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mistyrose2(0.933333*limit(),0.835294*limit(),0.823529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mistyrose3(0.803922*limit(),0.717647*limit(),0.709804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mistyrose4(0.545098*limit(),0.490196*limit(),0.482353*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::moccasin(1*limit(),0.894118*limit(),0.709804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navajowhite(1*limit(),0.870588*limit(),0.678431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navajowhite1(1*limit(),0.870588*limit(),0.678431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navajowhite2(0.933333*limit(),0.811765*limit(),0.631373*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navajowhite3(0.803922*limit(),0.701961*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navajowhite4(0.545098*limit(),0.47451*limit(),0.368627*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navy(0*limit(),0*limit(),0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navyblue(0*limit(),0*limit(),0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::oldlace(0.992157*limit(),0.960784*limit(),0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::olivedrab(0.419608*limit(),0.556863*limit(),0.137255*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::olivedrab1(0.752941*limit(),1*limit(),0.243137*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::olivedrab2(0.701961*limit(),0.933333*limit(),0.227451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::olivedrab3(0.603922*limit(),0.803922*limit(),0.196078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::olivedrab4(0.411765*limit(),0.545098*limit(),0.133333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orange(1*limit(),0.647059*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orange1(1*limit(),0.647059*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orange2(0.933333*limit(),0.603922*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orange3(0.803922*limit(),0.521569*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orange4(0.545098*limit(),0.352941*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orangered(1*limit(),0.270588*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orangered1(1*limit(),0.270588*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orangered2(0.933333*limit(),0.25098*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orangered3(0.803922*limit(),0.215686*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orangered4(0.545098*limit(),0.145098*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orchid(0.854902*limit(),0.439216*limit(),0.839216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orchid1(1*limit(),0.513726*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orchid2(0.933333*limit(),0.478431*limit(),0.913725*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orchid3(0.803922*limit(),0.411765*limit(),0.788235*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orchid4(0.545098*limit(),0.278431*limit(),0.537255*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palegoldenrod(0.933333*limit(),0.909804*limit(),0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palegreen(0.596078*limit(),0.984314*limit(),0.596078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palegreen1(0.603922*limit(),1*limit(),0.603922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palegreen2(0.564706*limit(),0.933333*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palegreen3(0.486275*limit(),0.803922*limit(),0.486275*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palegreen4(0.329412*limit(),0.545098*limit(),0.329412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleturquoise(0.686275*limit(),0.933333*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleturquoise1(0.733333*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleturquoise2(0.682353*limit(),0.933333*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleturquoise3(0.588235*limit(),0.803922*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleturquoise4(0.4*limit(),0.545098*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palevioletred(0.858824*limit(),0.439216*limit(),0.576471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palevioletred1(1*limit(),0.509804*limit(),0.670588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palevioletred2(0.933333*limit(),0.47451*limit(),0.623529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palevioletred3(0.803922*limit(),0.407843*limit(),0.537255*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::palevioletred4(0.545098*limit(),0.278431*limit(),0.364706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::papayawhip(1*limit(),0.937255*limit(),0.835294*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peachpuff(1*limit(),0.854902*limit(),0.72549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peachpuff1(1*limit(),0.854902*limit(),0.72549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peachpuff2(0.933333*limit(),0.796078*limit(),0.678431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peachpuff3(0.803922*limit(),0.686275*limit(),0.584314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peachpuff4(0.545098*limit(),0.466667*limit(),0.396078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peru(0.803922*limit(),0.521569*limit(),0.247059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::pink(1*limit(),0.752941*limit(),0.796078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::pink1(1*limit(),0.709804*limit(),0.772549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::pink2(0.933333*limit(),0.662745*limit(),0.721569*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::pink3(0.803922*limit(),0.568627*limit(),0.619608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::pink4(0.545098*limit(),0.388235*limit(),0.423529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::plum(0.866667*limit(),0.627451*limit(),0.866667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::plum1(1*limit(),0.733333*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::plum2(0.933333*limit(),0.682353*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::plum3(0.803922*limit(),0.588235*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::plum4(0.545098*limit(),0.4*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::powderblue(0.690196*limit(),0.878431*limit(),0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::purple(0.627451*limit(),0.12549*limit(),0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::purple1(0.607843*limit(),0.188235*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::purple2(0.568627*limit(),0.172549*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::purple3(0.490196*limit(),0.14902*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::purple4(0.333333*limit(),0.101961*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::red1(1*limit(),0*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::red2(0.933333*limit(),0*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::red3(0.803922*limit(),0*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::red4(0.545098*limit(),0*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::rosybrown(0.737255*limit(),0.560784*limit(),0.560784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::rosybrown1(1*limit(),0.756863*limit(),0.756863*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::rosybrown2(0.933333*limit(),0.705882*limit(),0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::rosybrown3(0.803922*limit(),0.607843*limit(),0.607843*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::rosybrown4(0.545098*limit(),0.411765*limit(),0.411765*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::royalblue(0.254902*limit(),0.411765*limit(),0.882353*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::royalblue1(0.282353*limit(),0.462745*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::royalblue2(0.262745*limit(),0.431373*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::royalblue3(0.227451*limit(),0.372549*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::royalblue4(0.152941*limit(),0.25098*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::saddlebrown(0.545098*limit(),0.270588*limit(),0.0745098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::salmon(0.980392*limit(),0.501961*limit(),0.447059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::salmon1(1*limit(),0.54902*limit(),0.411765*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::salmon2(0.933333*limit(),0.509804*limit(),0.384314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::salmon3(0.803922*limit(),0.439216*limit(),0.329412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::salmon4(0.545098*limit(),0.298039*limit(),0.223529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sandybrown(0.956863*limit(),0.643137*limit(),0.376471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seagreen(0.180392*limit(),0.545098*limit(),0.341176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seagreen1(0.329412*limit(),1*limit(),0.623529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seagreen2(0.305882*limit(),0.933333*limit(),0.580392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seagreen3(0.262745*limit(),0.803922*limit(),0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seagreen4(0.180392*limit(),0.545098*limit(),0.341176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seashell(1*limit(),0.960784*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seashell1(1*limit(),0.960784*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seashell2(0.933333*limit(),0.898039*limit(),0.870588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seashell3(0.803922*limit(),0.772549*limit(),0.74902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seashell4(0.545098*limit(),0.52549*limit(),0.509804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sienna(0.627451*limit(),0.321569*limit(),0.176471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sienna1(1*limit(),0.509804*limit(),0.278431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sienna2(0.933333*limit(),0.47451*limit(),0.258824*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sienna3(0.803922*limit(),0.407843*limit(),0.223529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sienna4(0.545098*limit(),0.278431*limit(),0.14902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::skyblue(0.529412*limit(),0.807843*limit(),0.921569*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::skyblue1(0.529412*limit(),0.807843*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::skyblue2(0.494118*limit(),0.752941*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::skyblue3(0.423529*limit(),0.65098*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::skyblue4(0.290196*limit(),0.439216*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateblue(0.415686*limit(),0.352941*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateblue1(0.513726*limit(),0.435294*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateblue2(0.478431*limit(),0.403922*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateblue3(0.411765*limit(),0.34902*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateblue4(0.278431*limit(),0.235294*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slategray(0.439216*limit(),0.501961*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slategray1(0.776471*limit(),0.886275*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slategray2(0.72549*limit(),0.827451*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slategray3(0.623529*limit(),0.713726*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slategray4(0.423529*limit(),0.482353*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slategrey(0.439216*limit(),0.501961*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::snow(1*limit(),0.980392*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::snow1(1*limit(),0.980392*limit(),0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::snow2(0.933333*limit(),0.913725*limit(),0.913725*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::snow3(0.803922*limit(),0.788235*limit(),0.788235*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::snow4(0.545098*limit(),0.537255*limit(),0.537255*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::springgreen(0*limit(),1*limit(),0.498039*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::springgreen1(0*limit(),1*limit(),0.498039*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::springgreen2(0*limit(),0.933333*limit(),0.462745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::springgreen3(0*limit(),0.803922*limit(),0.4*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::springgreen4(0*limit(),0.545098*limit(),0.270588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::steelblue(0.27451*limit(),0.509804*limit(),0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::steelblue1(0.388235*limit(),0.721569*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::steelblue2(0.360784*limit(),0.67451*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::steelblue3(0.309804*limit(),0.580392*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::steelblue4(0.211765*limit(),0.392157*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tan(0.823529*limit(),0.705882*limit(),0.54902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tan1(1*limit(),0.647059*limit(),0.309804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tan2(0.933333*limit(),0.603922*limit(),0.286275*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tan3(0.803922*limit(),0.521569*limit(),0.247059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tan4(0.545098*limit(),0.352941*limit(),0.168627*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::thistle(0.847059*limit(),0.74902*limit(),0.847059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::thistle1(1*limit(),0.882353*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::thistle2(0.933333*limit(),0.823529*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::thistle3(0.803922*limit(),0.709804*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::thistle4(0.545098*limit(),0.482353*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tomato(1*limit(),0.388235*limit(),0.278431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tomato1(1*limit(),0.388235*limit(),0.278431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tomato2(0.933333*limit(),0.360784*limit(),0.258824*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tomato3(0.803922*limit(),0.309804*limit(),0.223529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tomato4(0.545098*limit(),0.211765*limit(),0.14902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::turquoise(0.25098*limit(),0.878431*limit(),0.815686*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::turquoise1(0*limit(),0.960784*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::turquoise2(0*limit(),0.898039*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::turquoise3(0*limit(),0.772549*limit(),0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::turquoise4(0*limit(),0.52549*limit(),0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::violet(0.933333*limit(),0.509804*limit(),0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::violetred(0.815686*limit(),0.12549*limit(),0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::violetred1(1*limit(),0.243137*limit(),0.588235*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::violetred2(0.933333*limit(),0.227451*limit(),0.54902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::violetred3(0.803922*limit(),0.196078*limit(),0.470588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::violetred4(0.545098*limit(),0.133333*limit(),0.321569*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::wheat(0.960784*limit(),0.870588*limit(),0.701961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::wheat1(1*limit(),0.905882*limit(),0.729412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::wheat2(0.933333*limit(),0.847059*limit(),0.682353*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::wheat3(0.803922*limit(),0.729412*limit(),0.588235*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::wheat4(0.545098*limit(),0.494118*limit(),0.4*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::whitesmoke(0.960784*limit(),0.960784*limit(),0.960784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellow1(1*limit(),1*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellow2(0.933333*limit(),0.933333*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellow3(0.803922*limit(),0.803922*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellow4(0.545098*limit(),0.545098*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellowgreen(0.603922*limit(),0.803922*limit(),0.196078*limit());



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