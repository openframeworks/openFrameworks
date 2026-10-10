#pragma once

// Internal: framebuffer / renderbuffer entry points for ofFbo, ofGLRenderer and
// ofTexture.
//
// On Android each GLES library only works with its own context type: an ES 1.1
// context provides GL_OES_framebuffer_object through libGLESv1_CM
// (glGenFramebuffersOES, ...), while the core names (glGenFramebuffers, ...)
// resolve to libGLESv2 and are null in an ES 1 context. The enum values are the
// same in both, so only the function names differ. Everywhere else (desktop,
// iOS, emscripten, and Android ES 2+) the core names are used unchanged.

#include "ofGLUtils.h"

#if defined(TARGET_ANDROID)
	#define OF_GL_FBO_OES_SWITCH(oesCall, coreCall) \
		if (!ofIsGLProgrammableRenderer()) { oesCall; } else { coreCall; }
#else
	#define OF_GL_FBO_OES_SWITCH(oesCall, coreCall) coreCall;
#endif

inline void ofGLGenFramebuffers(GLsizei n, GLuint * ids) {
	OF_GL_FBO_OES_SWITCH(glGenFramebuffersOES(n, ids), glGenFramebuffers(n, ids))
}
inline void ofGLDeleteFramebuffers(GLsizei n, const GLuint * ids) {
	OF_GL_FBO_OES_SWITCH(glDeleteFramebuffersOES(n, ids), glDeleteFramebuffers(n, ids))
}
inline void ofGLBindFramebuffer(GLenum target, GLuint id) {
	OF_GL_FBO_OES_SWITCH(glBindFramebufferOES(target, id), glBindFramebuffer(target, id))
}
inline void ofGLFramebufferTexture2D(GLenum target, GLenum attachment, GLenum texTarget, GLuint texture, GLint level) {
	OF_GL_FBO_OES_SWITCH(glFramebufferTexture2DOES(target, attachment, texTarget, texture, level),
						 glFramebufferTexture2D(target, attachment, texTarget, texture, level))
}
inline void ofGLFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum rbTarget, GLuint renderbuffer) {
	OF_GL_FBO_OES_SWITCH(glFramebufferRenderbufferOES(target, attachment, rbTarget, renderbuffer),
						 glFramebufferRenderbuffer(target, attachment, rbTarget, renderbuffer))
}
inline GLenum ofGLCheckFramebufferStatus(GLenum target) {
#if defined(TARGET_ANDROID)
	if (!ofIsGLProgrammableRenderer()) return glCheckFramebufferStatusOES(target);
#endif
	return glCheckFramebufferStatus(target);
}
inline void ofGLGenRenderbuffers(GLsizei n, GLuint * ids) {
	OF_GL_FBO_OES_SWITCH(glGenRenderbuffersOES(n, ids), glGenRenderbuffers(n, ids))
}
inline void ofGLDeleteRenderbuffers(GLsizei n, const GLuint * ids) {
	OF_GL_FBO_OES_SWITCH(glDeleteRenderbuffersOES(n, ids), glDeleteRenderbuffers(n, ids))
}
inline void ofGLBindRenderbuffer(GLenum target, GLuint id) {
	OF_GL_FBO_OES_SWITCH(glBindRenderbufferOES(target, id), glBindRenderbuffer(target, id))
}
inline void ofGLRenderbufferStorage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height) {
	OF_GL_FBO_OES_SWITCH(glRenderbufferStorageOES(target, internalFormat, width, height),
						 glRenderbufferStorage(target, internalFormat, width, height))
}
inline void ofGLGenerateMipmap(GLenum target) {
	OF_GL_FBO_OES_SWITCH(glGenerateMipmapOES(target), glGenerateMipmap(target))
}

#undef OF_GL_FBO_OES_SWITCH

/// \brief True on the Android emulator with an ES 1.x (fixed function) context.
///
/// The emulator's ES 1 driver (goldfish GLESv1_enc) advertises
/// GL_OES_framebuffer_object but can't be used for FBOs: glDeleteFramebuffersOES
/// crashes (GLClientState::removeFramebuffers locks renderbuffer state that is
/// only set up for ES 2+), and rebinding framebuffer 0 doesn't return drawing to
/// the window. ES 1.1 FBOs work on real devices, and ES 2+ works on the emulator.
inline bool ofIsAndroidEmulatorGLES1() {
#if defined(TARGET_ANDROID)
	if (ofIsGLProgrammableRenderer()) return false;
	static int cached = -1;
	if (cached < 0) {
		const GLubyte * renderer = glGetString(GL_RENDERER);
		if (renderer == nullptr) return false; // no context yet: ask again later
		cached = std::string(reinterpret_cast<const char *>(renderer)).find("Android Emulator") != std::string::npos ? 1 : 0;
	}
	return cached == 1;
#else
	return false;
#endif
}
