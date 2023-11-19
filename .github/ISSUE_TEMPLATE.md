Hi there!

### The issue tracker is for "issues", the openFrameworks forum is for "questions".

What is the difference? A obvious question might be "How do I get openFrameworks to compile on Android?" An obvious issue might be "The function `ofRandom()` always returns the number 42 on Linux Ubuntu 16.04".

Before you submit an issue, make sure:

* You are certain that the expected result and the actual result _are different_.
* You have code to demonstrate it. If you aren't certain what the "expected" result is (due to poor documentation), then please first ask about it on the forum https://forum.openframeworks.cc. There's a good chance there's a real issue, but please post it on the forum if you are in doubt.
* You searched the issue tracker and no one submitted this issue before.
* For issues with the openFrameworks website, go to https://github.com/openframeworks/ofSite/issues
* For issues with the ProjectGenerator, go to https://github.com/openframeworks/projectGenerator/issues

If you still want to continue, delete this and the previous lines, and provide
the following information so it's easier to help you:

### Environment

* openFrameworks version:  [0.9.8] [master from Git] [nightly v20170517] [?]
* OS & version:            [Ubuntu 16.04] [macOS 10.12] [Windows 10] [Android 7.0] [iOS 10] [Raspbian April 2017] [?]
* Computer:                [PC] [Macbook] [iPad] [Samsung Galaxy X] [RaspberryPi] [?]

### Expected result of my program

I expected that by doing ... then ... would happen because ...

### Actual result of my program

Instead of ... what actually happened was ...

### If the issue involves graphics

* Attach screenshots (find Attach at the bottom of this text)
* Graphics card:           [Intel XXXX] [GeForce GTX XXX] [?]
* Graphics card drivers:   [Linux: the output of `glxinfo -B`] [Windows: `Start > Run > dxdiag`]

### A minimal program showing the issue: 

_Please keep ```cpp AND ``` around your code below so it looks beautiful and readable_

```cpp

// <<< YOUR CODE STARTS HERE

// .h
int x;

// .cpp
void ofApp::setup() {
  x = 42;
}
void ofApp::draw() {
  cout << x << endl;
}

// YOUR CODE ENDS HERE >>>

```

### Proposed Solution

(describe your proposed solution if you have one)
