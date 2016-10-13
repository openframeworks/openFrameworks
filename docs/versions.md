#Table of versions of libraries used by OF

| Library    | default version | Win   | Mac | Linux | Android | iOS | Emscripten  | 
| ---------- |:---------------:|:-----:|:---:|:-----:|:-------:|:---:|------------:|
| Assimp     | 2.0             |       |     | _1    | -       | -   | -           |
| Cairo      | 1.10.2          |       |     | _1    | -       | -   | -           |
| Fmodex     | 4.22.0          |       |     |       |         |     | -           |
| FreeImage  | 3.15.3          |       |     |       |         |     | _2          |
| FreeType   | 2.4.4           |       |     | _1    |         |     | _2          |
| GLEW       | 1.7??           |       |     | _3    | -       | -   | -           |  
| GLFW       | 3.0.3           |       |     | _4    | -       | -   | -           |
| GLU        |                 |       |     | _7    | _6      | _5  | -           |
| GLUT       |                 | 3.7   | _8  | _9    | -       | -   | -           |
| Kiss FFT   |                 |       |     | ?libs/kiss/src | -       | -   | ?           |
| openssl    | 1.0.0a          |       |     | _1    |         |     | -           |   
| POCO       | 1.4.3p1         |       |     | _10   |         |     | 1.4.6p2 _11 |
| quicktime  |                 | ?     | ?   |       |         |     | -           |
| rtAudio    | 4.0.12          |       |     |       | -       | -   | -           |
| tess2      | latest version available |       |     | _12      |         |     | -           |
| videoInput | latest version available |       |     | ?        |         |     |             | 
| sndfile    |                 |       |     | ?     |         |     | -           |     

_1: System on linux

_2: Available via packages

_3: _1, Not used on arm linux

_4: Not used on arm linuxv6 + drag and drop patch from https://github.com/arturoc/glfw/tree/feature-dnd

_5: iphone-glu 1.0.0 http://code.google.com/p/iphone-glu/

_6: iphone-glu 1.0.0 patched to work on android, no sources available

_7: _1? Not used on arm linux

_8: OS version modified by theo to add drag and drop, no code available publicly?

_9: system but uses freeglut instead of glut

_10: patch for events which only affects headers so binaries can be compiled from original sources

_11: modified oF headers so patch is no longer necessary

_12: (not maintained anymore) patched to remove asserts, source in libs/tess2/Sources
