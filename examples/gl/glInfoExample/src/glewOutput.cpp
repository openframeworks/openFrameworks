
#include "glewOutput.h"


void glewPrintExt (const char* name, GLboolean def1, GLboolean def2, GLboolean def3){
    unsigned int i;
    printf( "\n%s:", name);
    for (i=0; i<62-strlen(name); i++) printf( " ");
    printf( "%s ", def1 ? "OK" : "MISSING");
    if (def1 != def2)
        printf( "[%s] ", def2 ? "OK" : "MISSING");
    if (def1 != def3)
        printf( "[%s]\n", def3 ? "OK" : "MISSING");
    else
        printf( "\n");
    for (i=0; i<strlen(name)+1; i++) printf( "-");
    printf( "\n");
    
}

void glewInfoFunc (const char* name, GLint undefined){
    unsigned int i;
    printf( "  %s:", name);
    for (i=0; i<60-strlen(name); i++) printf( " ");
    printf( "%s\n", undefined ? "MISSING" : "OK");
   
}


#ifdef GL_VERSION_1_1

void glewInfo_GL_VERSION_1_1 (void)
{
    glewPrintExt("GL_VERSION_1_1", GLEW_VERSION_1_1, GLEW_VERSION_1_1, GLEW_VERSION_1_1);
}

#endif /* GL_VERSION_1_1 */

#ifdef GL_VERSION_1_2

void glewInfo_GL_VERSION_1_2 (void)
{
    glewPrintExt("GL_VERSION_1_2", GLEW_VERSION_1_2, GLEW_VERSION_1_2, GLEW_VERSION_1_2);
    
    glewInfoFunc("glCopyTexSubImage3D", glCopyTexSubImage3D == NULL);
    glewInfoFunc("glDrawRangeElements", glDrawRangeElements == NULL);
    glewInfoFunc("glTexImage3D", glTexImage3D == NULL);
    glewInfoFunc("glTexSubImage3D", glTexSubImage3D == NULL);
}

#endif /* GL_VERSION_1_2 */

#ifdef GL_VERSION_1_2_1

void glewInfo_GL_VERSION_1_2_1 (void)
{
    glewPrintExt("GL_VERSION_1_2_1", GLEW_VERSION_1_2_1, GLEW_VERSION_1_2_1, GLEW_VERSION_1_2_1);
}

#endif /* GL_VERSION_1_2_1 */

#ifdef GL_VERSION_1_3

void glewInfo_GL_VERSION_1_3 (void)
{
    glewPrintExt("GL_VERSION_1_3", GLEW_VERSION_1_3, GLEW_VERSION_1_3, GLEW_VERSION_1_3);
    
    glewInfoFunc("glActiveTexture", glActiveTexture == NULL);
    glewInfoFunc("glClientActiveTexture", glClientActiveTexture == NULL);
    glewInfoFunc("glCompressedTexImage1D", glCompressedTexImage1D == NULL);
    glewInfoFunc("glCompressedTexImage2D", glCompressedTexImage2D == NULL);
    glewInfoFunc("glCompressedTexImage3D", glCompressedTexImage3D == NULL);
    glewInfoFunc("glCompressedTexSubImage1D", glCompressedTexSubImage1D == NULL);
    glewInfoFunc("glCompressedTexSubImage2D", glCompressedTexSubImage2D == NULL);
    glewInfoFunc("glCompressedTexSubImage3D", glCompressedTexSubImage3D == NULL);
    glewInfoFunc("glGetCompressedTexImage", glGetCompressedTexImage == NULL);
    glewInfoFunc("glLoadTransposeMatrixd", glLoadTransposeMatrixd == NULL);
    glewInfoFunc("glLoadTransposeMatrixf", glLoadTransposeMatrixf == NULL);
    glewInfoFunc("glMultTransposeMatrixd", glMultTransposeMatrixd == NULL);
    glewInfoFunc("glMultTransposeMatrixf", glMultTransposeMatrixf == NULL);
    glewInfoFunc("glMultiTexCoord1d", glMultiTexCoord1d == NULL);
    glewInfoFunc("glMultiTexCoord1dv", glMultiTexCoord1dv == NULL);
    glewInfoFunc("glMultiTexCoord1f", glMultiTexCoord1f == NULL);
    glewInfoFunc("glMultiTexCoord1fv", glMultiTexCoord1fv == NULL);
    glewInfoFunc("glMultiTexCoord1i", glMultiTexCoord1i == NULL);
    glewInfoFunc("glMultiTexCoord1iv", glMultiTexCoord1iv == NULL);
    glewInfoFunc("glMultiTexCoord1s", glMultiTexCoord1s == NULL);
    glewInfoFunc("glMultiTexCoord1sv", glMultiTexCoord1sv == NULL);
    glewInfoFunc("glMultiTexCoord2d", glMultiTexCoord2d == NULL);
    glewInfoFunc("glMultiTexCoord2dv", glMultiTexCoord2dv == NULL);
    glewInfoFunc("glMultiTexCoord2f", glMultiTexCoord2f == NULL);
    glewInfoFunc("glMultiTexCoord2fv", glMultiTexCoord2fv == NULL);
    glewInfoFunc("glMultiTexCoord2i", glMultiTexCoord2i == NULL);
    glewInfoFunc("glMultiTexCoord2iv", glMultiTexCoord2iv == NULL);
    glewInfoFunc("glMultiTexCoord2s", glMultiTexCoord2s == NULL);
    glewInfoFunc("glMultiTexCoord2sv", glMultiTexCoord2sv == NULL);
    glewInfoFunc("glMultiTexCoord3d", glMultiTexCoord3d == NULL);
    glewInfoFunc("glMultiTexCoord3dv", glMultiTexCoord3dv == NULL);
    glewInfoFunc("glMultiTexCoord3f", glMultiTexCoord3f == NULL);
    glewInfoFunc("glMultiTexCoord3fv", glMultiTexCoord3fv == NULL);
    glewInfoFunc("glMultiTexCoord3i", glMultiTexCoord3i == NULL);
    glewInfoFunc("glMultiTexCoord3iv", glMultiTexCoord3iv == NULL);
    glewInfoFunc("glMultiTexCoord3s", glMultiTexCoord3s == NULL);
    glewInfoFunc("glMultiTexCoord3sv", glMultiTexCoord3sv == NULL);
    glewInfoFunc("glMultiTexCoord4d", glMultiTexCoord4d == NULL);
    glewInfoFunc("glMultiTexCoord4dv", glMultiTexCoord4dv == NULL);
    glewInfoFunc("glMultiTexCoord4f", glMultiTexCoord4f == NULL);
    glewInfoFunc("glMultiTexCoord4fv", glMultiTexCoord4fv == NULL);
    glewInfoFunc("glMultiTexCoord4i", glMultiTexCoord4i == NULL);
    glewInfoFunc("glMultiTexCoord4iv", glMultiTexCoord4iv == NULL);
    glewInfoFunc("glMultiTexCoord4s", glMultiTexCoord4s == NULL);
    glewInfoFunc("glMultiTexCoord4sv", glMultiTexCoord4sv == NULL);
    glewInfoFunc("glSampleCoverage", glSampleCoverage == NULL);
}

#endif /* GL_VERSION_1_3 */

#ifdef GL_VERSION_1_4

void glewInfo_GL_VERSION_1_4 (void)
{
    glewPrintExt("GL_VERSION_1_4", GLEW_VERSION_1_4, GLEW_VERSION_1_4, GLEW_VERSION_1_4);
    
    glewInfoFunc("glBlendColor", glBlendColor == NULL);
    glewInfoFunc("glBlendEquation", glBlendEquation == NULL);
    glewInfoFunc("glBlendFuncSeparate", glBlendFuncSeparate == NULL);
    glewInfoFunc("glFogCoordPointer", glFogCoordPointer == NULL);
    glewInfoFunc("glFogCoordd", glFogCoordd == NULL);
    glewInfoFunc("glFogCoorddv", glFogCoorddv == NULL);
    glewInfoFunc("glFogCoordf", glFogCoordf == NULL);
    glewInfoFunc("glFogCoordfv", glFogCoordfv == NULL);
    glewInfoFunc("glMultiDrawArrays", glMultiDrawArrays == NULL);
    glewInfoFunc("glMultiDrawElements", glMultiDrawElements == NULL);
    glewInfoFunc("glPointParameterf", glPointParameterf == NULL);
    glewInfoFunc("glPointParameterfv", glPointParameterfv == NULL);
    glewInfoFunc("glPointParameteri", glPointParameteri == NULL);
    glewInfoFunc("glPointParameteriv", glPointParameteriv == NULL);
    glewInfoFunc("glSecondaryColor3b", glSecondaryColor3b == NULL);
    glewInfoFunc("glSecondaryColor3bv", glSecondaryColor3bv == NULL);
    glewInfoFunc("glSecondaryColor3d", glSecondaryColor3d == NULL);
    glewInfoFunc("glSecondaryColor3dv", glSecondaryColor3dv == NULL);
    glewInfoFunc("glSecondaryColor3f", glSecondaryColor3f == NULL);
    glewInfoFunc("glSecondaryColor3fv", glSecondaryColor3fv == NULL);
    glewInfoFunc("glSecondaryColor3i", glSecondaryColor3i == NULL);
    glewInfoFunc("glSecondaryColor3iv", glSecondaryColor3iv == NULL);
    glewInfoFunc("glSecondaryColor3s", glSecondaryColor3s == NULL);
    glewInfoFunc("glSecondaryColor3sv", glSecondaryColor3sv == NULL);
    glewInfoFunc("glSecondaryColor3ub", glSecondaryColor3ub == NULL);
    glewInfoFunc("glSecondaryColor3ubv", glSecondaryColor3ubv == NULL);
    glewInfoFunc("glSecondaryColor3ui", glSecondaryColor3ui == NULL);
    glewInfoFunc("glSecondaryColor3uiv", glSecondaryColor3uiv == NULL);
    glewInfoFunc("glSecondaryColor3us", glSecondaryColor3us == NULL);
    glewInfoFunc("glSecondaryColor3usv", glSecondaryColor3usv == NULL);
    glewInfoFunc("glSecondaryColorPointer", glSecondaryColorPointer == NULL);
    glewInfoFunc("glWindowPos2d", glWindowPos2d == NULL);
    glewInfoFunc("glWindowPos2dv", glWindowPos2dv == NULL);
    glewInfoFunc("glWindowPos2f", glWindowPos2f == NULL);
    glewInfoFunc("glWindowPos2fv", glWindowPos2fv == NULL);
    glewInfoFunc("glWindowPos2i", glWindowPos2i == NULL);
    glewInfoFunc("glWindowPos2iv", glWindowPos2iv == NULL);
    glewInfoFunc("glWindowPos2s", glWindowPos2s == NULL);
    glewInfoFunc("glWindowPos2sv", glWindowPos2sv == NULL);
    glewInfoFunc("glWindowPos3d", glWindowPos3d == NULL);
    glewInfoFunc("glWindowPos3dv", glWindowPos3dv == NULL);
    glewInfoFunc("glWindowPos3f", glWindowPos3f == NULL);
    glewInfoFunc("glWindowPos3fv", glWindowPos3fv == NULL);
    glewInfoFunc("glWindowPos3i", glWindowPos3i == NULL);
    glewInfoFunc("glWindowPos3iv", glWindowPos3iv == NULL);
    glewInfoFunc("glWindowPos3s", glWindowPos3s == NULL);
    glewInfoFunc("glWindowPos3sv", glWindowPos3sv == NULL);
}

#endif /* GL_VERSION_1_4 */

#ifdef GL_VERSION_1_5

void glewInfo_GL_VERSION_1_5 (void)
{
    glewPrintExt("GL_VERSION_1_5", GLEW_VERSION_1_5, GLEW_VERSION_1_5, GLEW_VERSION_1_5);
    
    glewInfoFunc("glBeginQuery", glBeginQuery == NULL);
    glewInfoFunc("glBindBuffer", glBindBuffer == NULL);
    glewInfoFunc("glBufferData", glBufferData == NULL);
    glewInfoFunc("glBufferSubData", glBufferSubData == NULL);
    glewInfoFunc("glDeleteBuffers", glDeleteBuffers == NULL);
    glewInfoFunc("glDeleteQueries", glDeleteQueries == NULL);
    glewInfoFunc("glEndQuery", glEndQuery == NULL);
    glewInfoFunc("glGenBuffers", glGenBuffers == NULL);
    glewInfoFunc("glGenQueries", glGenQueries == NULL);
    glewInfoFunc("glGetBufferParameteriv", glGetBufferParameteriv == NULL);
    glewInfoFunc("glGetBufferPointerv", glGetBufferPointerv == NULL);
    glewInfoFunc("glGetBufferSubData", glGetBufferSubData == NULL);
    glewInfoFunc("glGetQueryObjectiv", glGetQueryObjectiv == NULL);
    glewInfoFunc("glGetQueryObjectuiv", glGetQueryObjectuiv == NULL);
    glewInfoFunc("glGetQueryiv", glGetQueryiv == NULL);
    glewInfoFunc("glIsBuffer", glIsBuffer == NULL);
    glewInfoFunc("glIsQuery", glIsQuery == NULL);
    glewInfoFunc("glMapBuffer", glMapBuffer == NULL);
    glewInfoFunc("glUnmapBuffer", glUnmapBuffer == NULL);
}

#endif /* GL_VERSION_1_5 */

#ifdef GL_VERSION_2_0

void glewInfo_GL_VERSION_2_0 (void)
{
    glewPrintExt("GL_VERSION_2_0", GLEW_VERSION_2_0, GLEW_VERSION_2_0, GLEW_VERSION_2_0);
    
    glewInfoFunc("glAttachShader", glAttachShader == NULL);
    glewInfoFunc("glBindAttribLocation", glBindAttribLocation == NULL);
    glewInfoFunc("glBlendEquationSeparate", glBlendEquationSeparate == NULL);
    glewInfoFunc("glCompileShader", glCompileShader == NULL);
    glewInfoFunc("glCreateProgram", glCreateProgram == NULL);
    glewInfoFunc("glCreateShader", glCreateShader == NULL);
    glewInfoFunc("glDeleteProgram", glDeleteProgram == NULL);
    glewInfoFunc("glDeleteShader", glDeleteShader == NULL);
    glewInfoFunc("glDetachShader", glDetachShader == NULL);
    glewInfoFunc("glDisableVertexAttribArray", glDisableVertexAttribArray == NULL);
    glewInfoFunc("glDrawBuffers", glDrawBuffers == NULL);
    glewInfoFunc("glEnableVertexAttribArray", glEnableVertexAttribArray == NULL);
    glewInfoFunc("glGetActiveAttrib", glGetActiveAttrib == NULL);
    glewInfoFunc("glGetActiveUniform", glGetActiveUniform == NULL);
    glewInfoFunc("glGetAttachedShaders", glGetAttachedShaders == NULL);
    glewInfoFunc("glGetAttribLocation", glGetAttribLocation == NULL);
    glewInfoFunc("glGetProgramInfoLog", glGetProgramInfoLog == NULL);
    glewInfoFunc("glGetProgramiv", glGetProgramiv == NULL);
    glewInfoFunc("glGetShaderInfoLog", glGetShaderInfoLog == NULL);
    glewInfoFunc("glGetShaderSource", glGetShaderSource == NULL);
    glewInfoFunc("glGetShaderiv", glGetShaderiv == NULL);
    glewInfoFunc("glGetUniformLocation", glGetUniformLocation == NULL);
    glewInfoFunc("glGetUniformfv", glGetUniformfv == NULL);
    glewInfoFunc("glGetUniformiv", glGetUniformiv == NULL);
    glewInfoFunc("glGetVertexAttribPointerv", glGetVertexAttribPointerv == NULL);
    glewInfoFunc("glGetVertexAttribdv", glGetVertexAttribdv == NULL);
    glewInfoFunc("glGetVertexAttribfv", glGetVertexAttribfv == NULL);
    glewInfoFunc("glGetVertexAttribiv", glGetVertexAttribiv == NULL);
    glewInfoFunc("glIsProgram", glIsProgram == NULL);
    glewInfoFunc("glIsShader", glIsShader == NULL);
    glewInfoFunc("glLinkProgram", glLinkProgram == NULL);
    glewInfoFunc("glShaderSource", glShaderSource == NULL);
    glewInfoFunc("glStencilFuncSeparate", glStencilFuncSeparate == NULL);
    glewInfoFunc("glStencilMaskSeparate", glStencilMaskSeparate == NULL);
    glewInfoFunc("glStencilOpSeparate", glStencilOpSeparate == NULL);
    glewInfoFunc("glUniform1f", glUniform1f == NULL);
    glewInfoFunc("glUniform1fv", glUniform1fv == NULL);
    glewInfoFunc("glUniform1i", glUniform1i == NULL);
    glewInfoFunc("glUniform1iv", glUniform1iv == NULL);
    glewInfoFunc("glUniform2f", glUniform2f == NULL);
    glewInfoFunc("glUniform2fv", glUniform2fv == NULL);
    glewInfoFunc("glUniform2i", glUniform2i == NULL);
    glewInfoFunc("glUniform2iv", glUniform2iv == NULL);
    glewInfoFunc("glUniform3f", glUniform3f == NULL);
    glewInfoFunc("glUniform3fv", glUniform3fv == NULL);
    glewInfoFunc("glUniform3i", glUniform3i == NULL);
    glewInfoFunc("glUniform3iv", glUniform3iv == NULL);
    glewInfoFunc("glUniform4f", glUniform4f == NULL);
    glewInfoFunc("glUniform4fv", glUniform4fv == NULL);
    glewInfoFunc("glUniform4i", glUniform4i == NULL);
    glewInfoFunc("glUniform4iv", glUniform4iv == NULL);
    glewInfoFunc("glUniformMatrix2fv", glUniformMatrix2fv == NULL);
    glewInfoFunc("glUniformMatrix3fv", glUniformMatrix3fv == NULL);
    glewInfoFunc("glUniformMatrix4fv", glUniformMatrix4fv == NULL);
    glewInfoFunc("glUseProgram", glUseProgram == NULL);
    glewInfoFunc("glValidateProgram", glValidateProgram == NULL);
    glewInfoFunc("glVertexAttrib1d", glVertexAttrib1d == NULL);
    glewInfoFunc("glVertexAttrib1dv", glVertexAttrib1dv == NULL);
    glewInfoFunc("glVertexAttrib1f", glVertexAttrib1f == NULL);
    glewInfoFunc("glVertexAttrib1fv", glVertexAttrib1fv == NULL);
    glewInfoFunc("glVertexAttrib1s", glVertexAttrib1s == NULL);
    glewInfoFunc("glVertexAttrib1sv", glVertexAttrib1sv == NULL);
    glewInfoFunc("glVertexAttrib2d", glVertexAttrib2d == NULL);
    glewInfoFunc("glVertexAttrib2dv", glVertexAttrib2dv == NULL);
    glewInfoFunc("glVertexAttrib2f", glVertexAttrib2f == NULL);
    glewInfoFunc("glVertexAttrib2fv", glVertexAttrib2fv == NULL);
    glewInfoFunc("glVertexAttrib2s", glVertexAttrib2s == NULL);
    glewInfoFunc("glVertexAttrib2sv", glVertexAttrib2sv == NULL);
    glewInfoFunc("glVertexAttrib3d", glVertexAttrib3d == NULL);
    glewInfoFunc("glVertexAttrib3dv", glVertexAttrib3dv == NULL);
    glewInfoFunc("glVertexAttrib3f", glVertexAttrib3f == NULL);
    glewInfoFunc("glVertexAttrib3fv", glVertexAttrib3fv == NULL);
    glewInfoFunc("glVertexAttrib3s", glVertexAttrib3s == NULL);
    glewInfoFunc("glVertexAttrib3sv", glVertexAttrib3sv == NULL);
    glewInfoFunc("glVertexAttrib4Nbv", glVertexAttrib4Nbv == NULL);
    glewInfoFunc("glVertexAttrib4Niv", glVertexAttrib4Niv == NULL);
    glewInfoFunc("glVertexAttrib4Nsv", glVertexAttrib4Nsv == NULL);
    glewInfoFunc("glVertexAttrib4Nub", glVertexAttrib4Nub == NULL);
    glewInfoFunc("glVertexAttrib4Nubv", glVertexAttrib4Nubv == NULL);
    glewInfoFunc("glVertexAttrib4Nuiv", glVertexAttrib4Nuiv == NULL);
    glewInfoFunc("glVertexAttrib4Nusv", glVertexAttrib4Nusv == NULL);
    glewInfoFunc("glVertexAttrib4bv", glVertexAttrib4bv == NULL);
    glewInfoFunc("glVertexAttrib4d", glVertexAttrib4d == NULL);
    glewInfoFunc("glVertexAttrib4dv", glVertexAttrib4dv == NULL);
    glewInfoFunc("glVertexAttrib4f", glVertexAttrib4f == NULL);
    glewInfoFunc("glVertexAttrib4fv", glVertexAttrib4fv == NULL);
    glewInfoFunc("glVertexAttrib4iv", glVertexAttrib4iv == NULL);
    glewInfoFunc("glVertexAttrib4s", glVertexAttrib4s == NULL);
    glewInfoFunc("glVertexAttrib4sv", glVertexAttrib4sv == NULL);
    glewInfoFunc("glVertexAttrib4ubv", glVertexAttrib4ubv == NULL);
    glewInfoFunc("glVertexAttrib4uiv", glVertexAttrib4uiv == NULL);
    glewInfoFunc("glVertexAttrib4usv", glVertexAttrib4usv == NULL);
    glewInfoFunc("glVertexAttribPointer", glVertexAttribPointer == NULL);
}

#endif /* GL_VERSION_2_0 */

#ifdef GL_VERSION_2_1

void glewInfo_GL_VERSION_2_1 (void)
{
    glewPrintExt("GL_VERSION_2_1", GLEW_VERSION_2_1, GLEW_VERSION_2_1, GLEW_VERSION_2_1);
    
    glewInfoFunc("glUniformMatrix2x3fv", glUniformMatrix2x3fv == NULL);
    glewInfoFunc("glUniformMatrix2x4fv", glUniformMatrix2x4fv == NULL);
    glewInfoFunc("glUniformMatrix3x2fv", glUniformMatrix3x2fv == NULL);
    glewInfoFunc("glUniformMatrix3x4fv", glUniformMatrix3x4fv == NULL);
    glewInfoFunc("glUniformMatrix4x2fv", glUniformMatrix4x2fv == NULL);
    glewInfoFunc("glUniformMatrix4x3fv", glUniformMatrix4x3fv == NULL);
}

#endif /* GL_VERSION_2_1 */

#ifdef GL_VERSION_3_0

void glewInfo_GL_VERSION_3_0 (void)
{
    glewPrintExt("GL_VERSION_3_0", GLEW_VERSION_3_0, GLEW_VERSION_3_0, GLEW_VERSION_3_0);
    
    glewInfoFunc("glBeginConditionalRender", glBeginConditionalRender == NULL);
    glewInfoFunc("glBeginTransformFeedback", glBeginTransformFeedback == NULL);
    glewInfoFunc("glBindFragDataLocation", glBindFragDataLocation == NULL);
    glewInfoFunc("glClampColor", glClampColor == NULL);
    glewInfoFunc("glClearBufferfi", glClearBufferfi == NULL);
    glewInfoFunc("glClearBufferfv", glClearBufferfv == NULL);
    glewInfoFunc("glClearBufferiv", glClearBufferiv == NULL);
    glewInfoFunc("glClearBufferuiv", glClearBufferuiv == NULL);
    glewInfoFunc("glColorMaski", glColorMaski == NULL);
    glewInfoFunc("glDisablei", glDisablei == NULL);
    glewInfoFunc("glEnablei", glEnablei == NULL);
    glewInfoFunc("glEndConditionalRender", glEndConditionalRender == NULL);
    glewInfoFunc("glEndTransformFeedback", glEndTransformFeedback == NULL);
    glewInfoFunc("glGetBooleani_v", glGetBooleani_v == NULL);
    glewInfoFunc("glGetFragDataLocation", glGetFragDataLocation == NULL);
    glewInfoFunc("glGetStringi", glGetStringi == NULL);
    glewInfoFunc("glGetTexParameterIiv", glGetTexParameterIiv == NULL);
    glewInfoFunc("glGetTexParameterIuiv", glGetTexParameterIuiv == NULL);
    glewInfoFunc("glGetTransformFeedbackVarying", glGetTransformFeedbackVarying == NULL);
    glewInfoFunc("glGetUniformuiv", glGetUniformuiv == NULL);
    glewInfoFunc("glGetVertexAttribIiv", glGetVertexAttribIiv == NULL);
    glewInfoFunc("glGetVertexAttribIuiv", glGetVertexAttribIuiv == NULL);
    glewInfoFunc("glIsEnabledi", glIsEnabledi == NULL);
    glewInfoFunc("glTexParameterIiv", glTexParameterIiv == NULL);
    glewInfoFunc("glTexParameterIuiv", glTexParameterIuiv == NULL);
    glewInfoFunc("glTransformFeedbackVaryings", glTransformFeedbackVaryings == NULL);
    glewInfoFunc("glUniform1ui", glUniform1ui == NULL);
    glewInfoFunc("glUniform1uiv", glUniform1uiv == NULL);
    glewInfoFunc("glUniform2ui", glUniform2ui == NULL);
    glewInfoFunc("glUniform2uiv", glUniform2uiv == NULL);
    glewInfoFunc("glUniform3ui", glUniform3ui == NULL);
    glewInfoFunc("glUniform3uiv", glUniform3uiv == NULL);
    glewInfoFunc("glUniform4ui", glUniform4ui == NULL);
    glewInfoFunc("glUniform4uiv", glUniform4uiv == NULL);
    glewInfoFunc("glVertexAttribI1i", glVertexAttribI1i == NULL);
    glewInfoFunc("glVertexAttribI1iv", glVertexAttribI1iv == NULL);
    glewInfoFunc("glVertexAttribI1ui", glVertexAttribI1ui == NULL);
    glewInfoFunc("glVertexAttribI1uiv", glVertexAttribI1uiv == NULL);
    glewInfoFunc("glVertexAttribI2i", glVertexAttribI2i == NULL);
    glewInfoFunc("glVertexAttribI2iv", glVertexAttribI2iv == NULL);
    glewInfoFunc("glVertexAttribI2ui", glVertexAttribI2ui == NULL);
    glewInfoFunc("glVertexAttribI2uiv", glVertexAttribI2uiv == NULL);
    glewInfoFunc("glVertexAttribI3i", glVertexAttribI3i == NULL);
    glewInfoFunc("glVertexAttribI3iv", glVertexAttribI3iv == NULL);
    glewInfoFunc("glVertexAttribI3ui", glVertexAttribI3ui == NULL);
    glewInfoFunc("glVertexAttribI3uiv", glVertexAttribI3uiv == NULL);
    glewInfoFunc("glVertexAttribI4bv", glVertexAttribI4bv == NULL);
    glewInfoFunc("glVertexAttribI4i", glVertexAttribI4i == NULL);
    glewInfoFunc("glVertexAttribI4iv", glVertexAttribI4iv == NULL);
    glewInfoFunc("glVertexAttribI4sv", glVertexAttribI4sv == NULL);
    glewInfoFunc("glVertexAttribI4ubv", glVertexAttribI4ubv == NULL);
    glewInfoFunc("glVertexAttribI4ui", glVertexAttribI4ui == NULL);
    glewInfoFunc("glVertexAttribI4uiv", glVertexAttribI4uiv == NULL);
    glewInfoFunc("glVertexAttribI4usv", glVertexAttribI4usv == NULL);
    glewInfoFunc("glVertexAttribIPointer", glVertexAttribIPointer == NULL);
}

#endif /* GL_VERSION_3_0 */

#ifdef GL_VERSION_3_1

void glewInfo_GL_VERSION_3_1 (void)
{
    glewPrintExt("GL_VERSION_3_1", GLEW_VERSION_3_1, GLEW_VERSION_3_1, GLEW_VERSION_3_1);
    
    glewInfoFunc("glDrawArraysInstanced", glDrawArraysInstanced == NULL);
    glewInfoFunc("glDrawElementsInstanced", glDrawElementsInstanced == NULL);
    glewInfoFunc("glPrimitiveRestartIndex", glPrimitiveRestartIndex == NULL);
    glewInfoFunc("glTexBuffer", glTexBuffer == NULL);
}

#endif /* GL_VERSION_3_1 */

#ifdef GL_VERSION_3_2

void glewInfo_GL_VERSION_3_2 (void)
{
    glewPrintExt("GL_VERSION_3_2", GLEW_VERSION_3_2, GLEW_VERSION_3_2, GLEW_VERSION_3_2);
    
    glewInfoFunc("glFramebufferTexture", glFramebufferTexture == NULL);
    glewInfoFunc("glGetBufferParameteri64v", glGetBufferParameteri64v == NULL);
    glewInfoFunc("glGetInteger64i_v", glGetInteger64i_v == NULL);
}

#endif /* GL_VERSION_3_2 */

#ifdef GL_VERSION_3_3

void glewInfo_GL_VERSION_3_3 (void)
{
    glewPrintExt("GL_VERSION_3_3", GLEW_VERSION_3_3, GLEW_VERSION_3_3, GLEW_VERSION_3_3);
    
    glewInfoFunc("glVertexAttribDivisor", glVertexAttribDivisor == NULL);
}

#endif /* GL_VERSION_3_3 */

#ifdef GL_VERSION_4_0

void glewInfo_GL_VERSION_4_0 (void)
{
    glewPrintExt("GL_VERSION_4_0", GLEW_VERSION_4_0, GLEW_VERSION_4_0, GLEW_VERSION_4_0);
    
    glewInfoFunc("glBlendEquationSeparatei", glBlendEquationSeparatei == NULL);
    glewInfoFunc("glBlendEquationi", glBlendEquationi == NULL);
    glewInfoFunc("glBlendFuncSeparatei", glBlendFuncSeparatei == NULL);
    glewInfoFunc("glBlendFunci", glBlendFunci == NULL);
    glewInfoFunc("glMinSampleShading", glMinSampleShading == NULL);
}

#endif /* GL_VERSION_4_0 */

#ifdef GL_VERSION_4_1

void glewInfo_GL_VERSION_4_1 (void)
{
    glewPrintExt("GL_VERSION_4_1", GLEW_VERSION_4_1, GLEW_VERSION_4_1, GLEW_VERSION_4_1);
}

#endif /* GL_VERSION_4_1 */

#ifdef GL_VERSION_4_2

void glewInfo_GL_VERSION_4_2 (void)
{
    glewPrintExt("GL_VERSION_4_2", GLEW_VERSION_4_2, GLEW_VERSION_4_2, GLEW_VERSION_4_2);
}
#endif

void printGlewInfo(){
    
    #ifdef GL_VERSION_1_1
        glewInfo_GL_VERSION_1_1();
    #endif /* GL_VERSION_1_1 */
    #ifdef GL_VERSION_1_2
        glewInfo_GL_VERSION_1_2();
    #endif /* GL_VERSION_1_2 */
    #ifdef GL_VERSION_1_2_1
        glewInfo_GL_VERSION_1_2_1();
    #endif /* GL_VERSION_1_2_1 */
    #ifdef GL_VERSION_1_3
        glewInfo_GL_VERSION_1_3();
    #endif /* GL_VERSION_1_3 */
    #ifdef GL_VERSION_1_4
        glewInfo_GL_VERSION_1_4();
    #endif /* GL_VERSION_1_4 */
    #ifdef GL_VERSION_1_5
        glewInfo_GL_VERSION_1_5();
    #endif /* GL_VERSION_1_5 */
    #ifdef GL_VERSION_2_0
        glewInfo_GL_VERSION_2_0();
    #endif /* GL_VERSION_2_0 */
    #ifdef GL_VERSION_2_1
        glewInfo_GL_VERSION_2_1();
    #endif /* GL_VERSION_2_1 */
    #ifdef GL_VERSION_3_0
        glewInfo_GL_VERSION_3_0();
    #endif /* GL_VERSION_3_0 */
    #ifdef GL_VERSION_3_1
        glewInfo_GL_VERSION_3_1();
    #endif /* GL_VERSION_3_1 */
    #ifdef GL_VERSION_3_2
        glewInfo_GL_VERSION_3_2();
    #endif /* GL_VERSION_3_2 */
    #ifdef GL_VERSION_3_3
        glewInfo_GL_VERSION_3_3();
    #endif /* GL_VERSION_3_3 */
    #ifdef GL_VERSION_4_0
        glewInfo_GL_VERSION_4_0();
    #endif /* GL_VERSION_4_0 */
    #ifdef GL_VERSION_4_1
        glewInfo_GL_VERSION_4_1();
    #endif /* GL_VERSION_4_1 */
    #ifdef GL_VERSION_4_2
        glewInfo_GL_VERSION_4_2();
    #endif /* GL_VERSION_4_2 */
}
