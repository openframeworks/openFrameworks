| Key     | Meaning                                   
|---------|:--------
| ✗       | requires build, not written
| ☁       | written but doesn't build, etc          
| ✓?      | builds but not tested
| ✓       | builds and tested in an OF project
| ✗C      | copy only, doesn't need build, not written
| ✓C      | copy only, written & tested                
| N/A     | not applicable

| Lib                             | osx | linux | linux64 | linuxarmv6l | linuxarmv7l | vs | msys2 | ios | android |
|---------------------------------|-----|-------|---------|-------------|-------------|----|--------|-----|---------|
| FreeImage                       | ✓   | N/A   | N/A     | N/A         | N/A         | ✗  | ✗      | ✓   | ✗       |
| FreeType                        | ✓   | N/A   | N/A     | N/A         | N/A         | ?  | ?      | ✓   | ?       |
| tess2                           | ✓   | ✗     | ✗       | ✗           | ✗           | ✓? | ✗      | ✓   | ✗       |
| Poco                            | ✓   | ✗     | ✗       | ✗           | ✗           | ✓? | ✗      | ✓   | ✗       |
| OpenSSL                         | ✓   | ✗     | ✗       | ✗           | ✗           | ✗  | ✗      | ✓   | N/A     |
| cairo                           | ✓   | N/A   | N/A     | N/A         | N/A         | ✗  | ✗      | N/A | N/A     |
| fmodex                          | ✓C  | ✗     | ✗       | ✗           | ✗           | ✗  | ✗      | N/A | N/A     |
| glew                            | ✓   | ✗     | ✗       | ✗           | ✗           | ✓  | ✗      | N/A | N/A     |
| glfw                            | ✓   | ✗     | ✗       | ✗           | ✗           | ✓? | ✗      | N/A | N/A     |
| glu                             | N/A | N/A   | N/A     | N/A         | N/A         | ✗  | ✗      | N/A | N/A     |
| glut                            | ✗   | N/A   | N/A     | N/A         | N/A         | ✗  | ✗      | N/A | N/A     |
| kiss                            | ✓C  | ✗     | ✗       | ✗           | ✗           | ✓C | ✓C     | ✓   | ✓C      |
| portaudio                       | ✓C  | ✓C    | ✓C      | ✗           | ✗           | ✓C | ✓C     | N/A | N/A     |
| quicktime                       | N/A | N/A   | N/A     | N/A         | N/A         | ✗  | ✗      | N/A | N/A     |
| rtAudio                         | ✓?  | ✗     | ✗       | ✗           | ✗           | ✗  | ✗      | N/A | N/A     |
| videoInput                      | N/A | N/A   | N/A     | N/A         | N/A         | ✓  | ✗      | N/A | N/A     |
| ofxAssimpModelLoader -> assimp  | ☁   | ✗     | ✗       | ✗           | ✗           | ✗  | ✗      | ✓?  | ✗       |
| ofxOpenCV -> opencv             | ✓?  | ✗     | ✗       | ✗           | ✗           | ✗  | ✗      | ✓?  | ✗       |
| ofxOsc -> oscpack               | ✗C  | ✗C    | ✗C      | ✗C          | ✗C          | ✗C | ✗C     | ✗C  | ✗C      |
| ofxSvg -> svgTiny               | ✗C  | ✗C    | ✗C      | ✗C          | ✗C          | ✗C | ✗C     | ✗C  | ✗C      |
| ofxXmlSettings -> tinyxml       | ✗C  | ✗C    | ✗C      | ✗C          | ✗C          | ✗C | ✗C     | ✗C  | ✗C      |
