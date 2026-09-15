# Android lifecycle stress test

This GLES 3 example repeatedly renders indexed VBO geometry, a tessellated path,
an FBO, and a custom shader. It deliberately disables context preservation so
background/foreground cycles exercise `unloadGL` and `reloadGL`.

Generate the Android Studio project with the openFrameworks Project Generator,
build and install it, then run:

```sh
tests/android/androidLifecycleStressTest/test_hardware.sh \
  cc.openframeworks.androidLifecycleStressTest \
  cc.openframeworks.android.OFActivity \
  25
```

The runner performs repeated Home/resume, rotation, and process-recreation
cycles. It exits nonzero if logcat contains a Java fatal exception, native crash,
ANR, or the test application's GL error marker.
