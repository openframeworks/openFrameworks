#List of versions of libraries used by OF

- Assimp 2.0
- Cairo 1.10.2  (System on linux, not used on android and iOS)
- Fmodex 4.22.0
- FreeImage 3.15.3 (System on linux)
- FreeType 2.4.4 (System on linux)
- GLEW 1.7?? (System on linux) (Not used on iOS, android or arm linux)
- GLFW 3.0.3 + drag and drop patch from https://github.com/arturoc/glfw/tree/feature-dnd (Not used on iOS, android or arm linux v6)
- GLU  desktop: no version?
       iOS: iphone-glu 1.0.0 http://code.google.com/p/iphone-glu/
       android: iphone-glu 1.0.0 patched to work on android, no sources available
       linux: system
       arm linux: not used
- GLUT osx: OS version modified by theo to add drag and drop, no code available publicly?
       win: 3.7
       linux: system but uses freeglut instead of glut
       others: not used
- kiss only used on linux src inside libs/kiss/src
- openssl 1.0.0a (System on linux)
- poco 1.4.3p1 + patch for events which only affects headers so binaries can be compiled from original sources
- quicktime only osx and windows, no available version
- rtAudio 4.0.12 (not used on iOS and Android)
- tess2 latest version available (not mantained anymore) patched to remove asserts, source in libs/tess2/Sources
- videoInput latest version available always (only used on windows)
