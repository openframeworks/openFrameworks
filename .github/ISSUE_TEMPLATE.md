Nice! You found an issue! :) Get help faster by providing the following details:

### Environment

* OpenFrameworks version:  [0.9.8, master from Git, nightly v20170517, ?]
* OS & version:            [Ubuntu 16.04, macOS 10.12, Windows 10, Android 7.0, iOS 10, Raspbian April 2017, ?]
* Computer:                [PC, Macbook, iPad, Samsung Galaxy Tab E, RaspberryPi, ?]

If graphics are involved:
* Graphics card:           [Intel XXXX, GeForce GTX XXX, ?]
* Graphics card drivers:   [Linux: the output of `glxinfo -B`, in Mac ..., Windows: start > run > dxdiag ]

### Expected result of my program

(explain here)

### Actual result of my program

(explain here)

### A minimal program showing the issue: 

```cpp
// YOUR PROGRAM GOES HERE
int x;
void ofApp::setup() {
  x = 42;
}
void ofApp::draw() {
  cout << x << endl;
}
```
