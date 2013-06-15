#include "testApp.h"
#include "glewOutput.h"

#include <stdio.h>
#include <stdlib.h>





// this example uses code from glew and from Brian Paul
//
/*
 * Copyright (C) 1999-2002  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


struct token_name {
    GLuint count;
    GLenum token;
    const char *name;
};


void
print_extension_list(char *ext)
{
    const char *indentString = "    ";
    const int indent = 4;
    const int max = 79;
    int width, i, j;

    if (!ext || !ext[0])
        return;

    width = indent;
    printf("%s", indentString);
    i = j = 0;
    while (1) {
        if (ext[j] == ' ' || ext[j] == 0) {
            /* found end of an extension name */
            const int len = j - i;
            if (width + len > max) {
                /* start a new line */
                printf("\n");
                width = indent;
                printf("%s", indentString);
            }
            /* print the extension name between ext[i] and ext[j] */
            while (i < j) {
                printf("%c", ext[i]);
                i++;
            }
            /* either we're all done, or we'll continue with next extension */
            width += len + 1;
            if (ext[j] == 0) {
                break;
            }
            else {
                i++;
                j++;
                if (ext[j] == 0)
                    break;
                printf(", ");
                width += 2;
            }
        }
        j++;
    }
    printf("\n");
}

static void
print_limits(void)
{

    static const struct token_name openglLimits[] = {
        { 1, GL_MAX_ATTRIB_STACK_DEPTH, "GL_MAX_ATTRIB_STACK_DEPTH" },
        { 1, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, "GL_MAX_CLIENT_ATTRIB_STACK_DEPTH" },
        { 1, GL_MAX_CLIP_PLANES, "GL_MAX_CLIP_PLANES" },
        { 1, GL_MAX_COLOR_MATRIX_STACK_DEPTH, "GL_MAX_COLOR_MATRIX_STACK_DEPTH" },
        { 1, GL_MAX_ELEMENTS_VERTICES, "GL_MAX_ELEMENTS_VERTICES" },
        { 1, GL_MAX_ELEMENTS_INDICES, "GL_MAX_ELEMENTS_INDICES" },
        { 1, GL_MAX_EVAL_ORDER, "GL_MAX_EVAL_ORDER" },
        { 1, GL_MAX_LIGHTS, "GL_MAX_LIGHTS" },
        { 1, GL_MAX_LIST_NESTING, "GL_MAX_LIST_NESTING" },
        { 1, GL_MAX_MODELVIEW_STACK_DEPTH, "GL_MAX_MODELVIEW_STACK_DEPTH" },
        { 1, GL_MAX_NAME_STACK_DEPTH, "GL_MAX_NAME_STACK_DEPTH" },
        { 1, GL_MAX_PIXEL_MAP_TABLE, "GL_MAX_PIXEL_MAP_TABLE" },
        { 1, GL_MAX_PROJECTION_STACK_DEPTH, "GL_MAX_PROJECTION_STACK_DEPTH" },
        { 1, GL_MAX_TEXTURE_STACK_DEPTH, "GL_MAX_TEXTURE_STACK_DEPTH" },
        { 1, GL_MAX_TEXTURE_SIZE, "GL_MAX_TEXTURE_SIZE" },
        { 1, GL_MAX_3D_TEXTURE_SIZE, "GL_MAX_3D_TEXTURE_SIZE" },
        { 1, GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB, "GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB" },
        { 1, GL_MAX_RECTANGLE_TEXTURE_SIZE_NV, "GL_MAX_RECTANGLE_TEXTURE_SIZE_NV" },
        { 1, GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, "GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB" },
        { 1, GL_MAX_TEXTURE_UNITS_ARB, "GL_MAX_TEXTURE_UNITS_ARB" },
        { 1, GL_MAX_TEXTURE_LOD_BIAS_EXT, "GL_MAX_TEXTURE_LOD_BIAS_EXT" },
        { 1, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, "GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT" },
        { 2, GL_MAX_VIEWPORT_DIMS, "GL_MAX_VIEWPORT_DIMS" },
        { 2, GL_ALIASED_LINE_WIDTH_RANGE, "GL_ALIASED_LINE_WIDTH_RANGE" },
        { 2, GL_SMOOTH_LINE_WIDTH_RANGE, "GL_SMOOTH_LINE_WIDTH_RANGE" },
        { 2, GL_ALIASED_POINT_SIZE_RANGE, "GL_ALIASED_POINT_SIZE_RANGE" },
        { 2, GL_SMOOTH_POINT_SIZE_RANGE, "GL_SMOOTH_POINT_SIZE_RANGE" },
        { 0, (GLenum) 0, NULL }
    };
    GLint i, max[2];
    printf("OpenGL limits:\n");
    for (i = 0; openglLimits[i].count; i++) {
        glGetIntegerv(openglLimits[i].token, max);
        if (glGetError() == GL_NONE) {
            if (openglLimits[i].count == 1)
                printf("    %s = %d\n", openglLimits[i].name, max[0]);
            else /* XXX fix if we ever query something with more than 2 values */
                printf("    %s = %d, %d\n", openglLimits[i].name, max[0], max[1]);
        }
    }
}

void printShaderLimits(){

    static const struct token_name lll[] = {
        { 1, GL_MAX_VERTEX_ATTRIBS, "GL_MAX_VERTEX_ATTRIBS" },
        { 1, GL_MAX_VERTEX_UNIFORM_COMPONENTS, "GL_MAX_VERTEX_UNIFORM_COMPONENTS" },
        { 1, GL_MAX_VARYING_FLOATS, "GL_MAX_VARYING_FLOATS" },
        { 1, GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS" },
        { 1, GL_MAX_TEXTURE_IMAGE_UNITS, "GL_MAX_TEXTURE_IMAGE_UNITS" },
        { 0, (GLenum) 0, NULL }
    };

    GLint i, max[2];
    printf("Shader limits:\n");
    for (i = 0; lll[i].count; i++) {
        glGetIntegerv(lll[i].token, max);
        if (glGetError() == GL_NONE) {
            if (lll[i].count == 1)
                printf("    %s = %d\n", lll[i].name, max[0]);
            else /* XXX fix if we ever query something with more than 2 values */
                printf("    %s = %d, %d\n", lll[i].name, max[0], max[1]);
        }
    }
}


void printGLInfo(){

    char *version = NULL;
    char *vendor = NULL;
    char *renderer = NULL;
//    char *extensions = NULL;
//    int   glutVersion;

    //glutVersion = glutGet(0x01FC);
    version =     (char*)glGetString(GL_VERSION);
    vendor =      (char*)glGetString(GL_VENDOR);
    renderer =    (char*)glGetString(GL_RENDERER);

    printf("version=%s\nvendor=%s\nrenderer=%s\n",
           version,vendor,renderer);

}


//--------------------------------------------------------------
void testApp::setup(){


    info.version = (char*)glGetString(GL_VERSION);
    info.vendor = (char*)glGetString(GL_VENDOR);
    info.renderer = (char*)glGetString(GL_RENDERER);
    info.bVboSupported = false;
    info.bShadersSupported = false;
    info.bPointSpritesSupported = false;


    if(glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite")) {
        info.bPointSpritesSupported = true;
    }

    if(glewIsSupported("GL_ARB_vertex_buffer_object")) {
        info.bVboSupported = true;
    }

    if(glewIsSupported("GL_ARB_vertex_shader")) {
        info.bShadersSupported = true;
    }

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &info.maxTextureSize);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, info.maxDimensions);
    glGetIntegerv(GL_MAX_LIGHTS, &info.maxLights);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

    string output = "";

    string pointSprites = ((info.bPointSpritesSupported == true) ? "yes" : "no");
    string shaders = ((info.bShadersSupported == true) ? "yes" : "no");
    string vbo = ((info.bVboSupported == true) ? "yes" : "no");


    output += "opengl version: " + info.version + "\n";
    output += "vendor: " + info.vendor + "\n";
    output += "renderer: " + info.renderer + "\n";
    output += "\n";
    output += "point sprites support: " + pointSprites + "\n";
    output += "shader support: " + shaders + "\n";
    output += "vbo support: " + vbo + "\n";
    output += "\n";
    output += "max texture size: " + ofToString(info.maxTextureSize) + "\n";
    output += "max viewport dimensions: " + ofToString(info.maxDimensions[0]) + "," +  ofToString(info.maxDimensions[1]) + "\n";
    output += "max lights: " + ofToString(info.maxLights) + "\n";



    ofDrawBitmapStringHighlight(output, ofPoint(20,20));
    ofDrawBitmapStringHighlight("press ' ' to load full report", ofPoint(20,220), ofColor::magenta, ofColor::white);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if (key == ' '){

        // todo: rewrite this with ofLog:

        FILE *fp;


        if((fp=freopen(ofToDataPath("openglReport.txt").c_str(), "w" ,stdout))==NULL) {
            printf("Cannot open file.\n");
            return;
        }


        printf("-------------------------------------------------\n");
        printf("opengl info\n");
        printf("-------------------------------------------------\n");

        printGLInfo();

        printf("-------------------------------------------------\n");
        printf("opengl limits\n");
        printf("-------------------------------------------------\n");


        print_limits();

        printf("-------------------------------------------------\n");
        printf("shader limits\n");
        printf("-------------------------------------------------\n");

        printShaderLimits();


        printf("-------------------------------------------------\n");
        printf("available extensions\n");
        printf("-------------------------------------------------\n");

        const GLubyte * strExt;
        strExt = glGetString (GL_EXTENSIONS);

        //cout << "extensions: " << strExt << endl;
        print_extension_list((char *)strExt);


        //isShade = gluCheckExtension ((const GLubyte*)"GL_ARB_shading_language_100", strExt);

        printf("-------------------------------------------------\n");
        printf("opengl calls available\n");
        printf("-------------------------------------------------\n");


        printGlewInfo();

        fclose(fp);

        #ifdef TARGET_WIN32
        string command = "start " + ofToString(ofToDataPath("openglReport.txt").c_str());
        #elif defined(TARGET_LINUX)
        string command = "xdg-open " + ofToString(ofToDataPath("openglReport.txt").c_str());
        #else
        string command = "open " + ofToString(ofToDataPath("openglReport.txt").c_str());
        #endif

        if (0 != system(command.c_str())){
			ofLogWarning() << "Command " << command.c_str() << " did not return 0. Something may have gone wrong.";
		}
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
