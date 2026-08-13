# GL render smoke test

Exercises the code paths touched by the OpenGL ES 3.0/3.1 support work:
an indexed `ofVboMesh` drawn both directly (`drawElements`) and instanced
(`drawElementsInstanced`), a tessellated `ofPath` (tess2 / `ofIndexType`),
and an `ofFbo` round trip with a pixel readback to catch garbage/blank
triangles caused by an index-type/size mismatch.

The actual test logic lives in `src/GLSmokeTestCore.h` and is shared,
unmodified, by every platform target (desktop here, `tests/emscripten/glRenderSmokeTest`,
`tests/ios/glRenderSmokeTest`) — only the platform bridge (`ofApp`/`main`)
differs.

On desktop this is a normal windowed openFrameworks app picked up
automatically by the existing `tests/` CI harness (`scripts/ci/*/run_tests.sh`):
it runs a fixed number of frames, logs `GL_SMOKE_TEST RESULT=PASS` or
`RESULT=FAIL reason=...`, and exits with a matching process exit code.

To run manually:

```sh
cd tests/gl/glRenderSmokeTest
cp ../../../scripts/templates/osx/Makefile .      # or linux/linux64 template
cp ../../../scripts/templates/osx/config.make .
make Debug
make RunDebug
```
