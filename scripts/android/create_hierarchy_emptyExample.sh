
DEST_DIR="apps/android/newApp/"

SRC_DIR="examples/android/androidEmptyExample"

rm -r $DEST_DIR

mkdir -p $DEST_DIR/{gradle/wrapper,ofApp/{gradle/wrapper,src/main/{cpp,bin/data,java/cc/openframeworks/android,res/{drawable-hdpi,drawable-mdpi,drawable-xhdpi,drawable,layout,menu,mipmap-anydpi-v26,mipmap-hdpi,mipmap-mdpi,mipmap-xhdpi,mipmap-xxhdpi,mipmap-xxxhdpi,values-v11,values-v14,values}}}}

tree $DEST_DIR


cp $SRC_DIR/addons.make $DEST_DIR
cp $SRC_DIR/build.gradle $DEST_DIR
cp $SRC_DIR/gradle.properties $DEST_DIR
cp $SRC_DIR/proguard.cfg $DEST_DIR
cp $SRC_DIR/settings.gradle $DEST_DIR
cp $SRC_DIR/gradlew $DEST_DIR
cp $SRC_DIR/gradlew.bat $DEST_DIR

cp $SRC_DIR/gradle/wrapper/gradle-wrapper.jar $DEST_DIR/gradle/wrapper
cp $SRC_DIR/gradle/wrapper/gradle-wrapper.properties $DEST_DIR/gradle/wrapper

cp $SRC_DIR/ofApp/build.gradle $DEST_DIR/ofApp
cp $SRC_DIR/ofApp/gradle.properties $DEST_DIR/ofApp
cp $SRC_DIR/ofApp/proguard-rules.pro $DEST_DIR/ofApp
cp $SRC_DIR/ofApp/gradlew $DEST_DIR/ofApp
cp $SRC_DIR/ofApp/gradlew.bat $DEST_DIR/ofApp

cp $SRC_DIR/ofApp/gradle/wrapper/* $DEST_DIR/ofApp/gradle/wrapper

cp $SRC_DIR/ofApp/src/main/AndroidManifest.xml $DEST_DIR/ofApp/src/main
cp $SRC_DIR/ofApp/src/main/ic_launcher-playstore.png $DEST_DIR/ofApp/src/main

cp $SRC_DIR/ofApp/src/main/cpp/CMakeLists.txt $DEST_DIR/ofApp/src/main/cpp
cp $SRC_DIR/ofApp/src/main/cpp/main.cpp $DEST_DIR/ofApp/src/main/cpp
cp $SRC_DIR/ofApp/src/main/cpp/ofApp.cpp $DEST_DIR/ofApp/src/main/cpp
cp $SRC_DIR/ofApp/src/main/cpp/ofApp.h $DEST_DIR/ofApp/src/main/cpp

cp $SRC_DIR/ofApp/src/main/java/cc/openframeworks/android/OFActivity.java $DEST_DIR/ofApp/src/main/java/cc/openframeworks/android

cp $SRC_DIR/ofApp/src/main/res/drawable-hdpi/icon.png $DEST_DIR/ofApp/src/main/res/drawable-hdpi
cp $SRC_DIR/ofApp/src/main/res/drawable-mdpi/icon.png $DEST_DIR/ofApp/src/main/res/drawable-mdpi
cp $SRC_DIR/ofApp/src/main/res/drawable-xhdpi/icon.png $DEST_DIR/ofApp/src/main/res/drawable-xhdpi

cp $SRC_DIR/ofApp/src/main/res/drawable/icon.png $DEST_DIR/ofApp/src/main/res/drawable

cp $SRC_DIR/ofApp/src/main/res/layout/main_layout.xml $DEST_DIR/ofApp/src/main/res/layout

cp $SRC_DIR/ofApp/src/main/res/menu/main_layout.xml $DEST_DIR/ofApp/src/main/res/menu

cp $SRC_DIR/ofApp/src/main/res/mipmap-anydpi-v26/* $DEST_DIR/ofApp/src/main/res/mipmap-anydpi-v26

cp $SRC_DIR/ofApp/src/main/res/mipmap-hdpi/* $DEST_DIR/ofApp/src/main/res/mipmap-hdpi
cp $SRC_DIR/ofApp/src/main/res/mipmap-mdpi/* $DEST_DIR/ofApp/src/main/res/mipmap-mdpi
cp $SRC_DIR/ofApp/src/main/res/mipmap-xhdpi/* $DEST_DIR/ofApp/src/main/res/mipmap-xhdpi
cp $SRC_DIR/ofApp/src/main/res/mipmap-xxhdpi/* $DEST_DIR/ofApp/src/main/res/mipmap-xxhdpi
cp $SRC_DIR/ofApp/src/main/res/mipmap-xxxhdpi/* $DEST_DIR/ofApp/src/main/res/mipmap-xxxhdpi

cp $SRC_DIR/ofApp/src/main/res/values-v11/* $DEST_DIR/ofApp/src/main/res/values-v11
cp $SRC_DIR/ofApp/src/main/res/values-v14/* $DEST_DIR/ofApp/src/main/res/values-v14
cp $SRC_DIR/ofApp/src/main/res/values/* $DEST_DIR/ofApp/src/main/res/values


tree $DEST_DIR

