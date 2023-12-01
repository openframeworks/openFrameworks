                                                     
```                                                     
    ,----..        ,---,      ,----,      ,----..    
   /   /   \    ,`--.' |    .'   .' \    /   /   \   
  /   .     :  /    /  :  ,----,'    |  /   .     :  
 .   /   ;.  \:    |.' '  |    :  .  ; .   /   ;.  \ 
.   ;   /  ` ;`----':  |  ;    |.'  / .   ;   /  ` ; 
;   |  ; \ ; |   '   ' ;  `----'/  ;  ;   |  ; \ ; | 
|   :  | ; | '   |   | |    /  ;  /   |   :  | ; | ' 
.   |  ' ' ' :   '   : ;   ;  /  /-,  .   |  ' ' ' : 
'   ;  \; /  |   |   | '  /  /  /.`|  '   ;  \; /  | 
 \   \  ',  /__  '   : |./__;      :___\   \  ',  /  
  ;   :    /  .\ ;   |.'|   :    .'/  .\;   :    /   
   \   \ .'\  ; |'---'  ;   | .'   \  ; |\   \ .'    
    `---`   `--"        `---'       `--"  `---`      
                                                     
```
### All 0.12.0 changes newest to oldest ### 
Grabbed via: `git log --oneline  --ancestry-path ac69b2f..34e8eac --pretty=format:'- %s [commit](https://github.com/openframeworks/openFrameworks/commit/%H)' > changes.txt` 

Major Changes: 
- all: Physical Based Rendering (PBR) now supported in ofxAssimpModelLoader and ofMaterial
- all: Built in shadow support
- all: C++17 default
- arm: aarch64 / 64bit support for Rapsberry Pi and other arm64 devices
- Windows: ofMediaFoundationSoundPlayer can be used instead of fmod
- macOS: ofAVEngineSoundPlayer can be used instead of fmod
- Windows ofMediaFoundationPlayer for video playback. No need for codecs!!! 
- iOS: ARC is now enabled for all projects by default. Some addons might require -fno-objc-arc for files which aren't ARC compatible. [More info](https://stackoverflow.com/a/6658549)
- emscripten: Loads of emscripten fixes to make more examples compatible.
- all: new computer_vision / OpenCV examples
- msys2: UCRT64 support

Full list: 
- Update manual-nightly.yml [commit](https://github.com/openframeworks/openFrameworks/commit/42b6dff3e77aa49425dffc2902d32ca86c818a59)
- update submodules [commit](https://github.com/openframeworks/openFrameworks/commit/93d0efda1413471e6f50761f73b1e72f04f6ba42)
- removal of some iOS examples (#7601) [commit](https://github.com/openframeworks/openFrameworks/commit/478620f212ec09b48abfd99e8e285c09b97ca921)
- Emscripten: wrap generated html into a directory (#7617) [commit](https://github.com/openframeworks/openFrameworks/commit/f68ff52821533080c56f8342301ef0c4cb8bdc12)
- ofRandomDistributions: more explicit comparison/precedence for glm::vec elements (#7618) [commit](https://github.com/openframeworks/openFrameworks/commit/9660d022e4135a8ad58f99abc510d1cf1e4505fb)
- update submodule pg (#7619) [commit](https://github.com/openframeworks/openFrameworks/commit/e4c8585af90ea325ea5662fc8253568f33540652)
- ofSeedRandom: remove deprecation (#7616) [commit](https://github.com/openframeworks/openFrameworks/commit/3d05b012b34682dc4dea01655f20f806afb70b0f)
- Update manual-nightly.yml [commit](https://github.com/openframeworks/openFrameworks/commit/bbfd761597e472b1ac32ae5fcb4cfb28409db217)
- Update build-linux-nightly.yml [commit](https://github.com/openframeworks/openFrameworks/commit/8d313d8274b50753ae3dc4db93438cbc01bfd4b3)
- iOS: simpler movieplayer example + retain/transfer fix (#7608) [commit](https://github.com/openframeworks/openFrameworks/commit/4c1b27b2700b699e06a2ece7c6527fc0e1f19b13)
- Custom shaders during a shadow pass. (#7613) [commit](https://github.com/openframeworks/openFrameworks/commit/1837cd083752f6d14c4ca00917dcab0aaa989b8d)
- Ci aarch64 (#7612) [commit](https://github.com/openframeworks/openFrameworks/commit/aef438b3b31a3f08a5236c52afc3ac81fa4bd379)
- ofRandom (#7598) [commit](https://github.com/openframeworks/openFrameworks/commit/16d7da194005fb79ea13977efc2609b6f46df084)
- Upgrade arm - closes #7573 (#7611) [commit](https://github.com/openframeworks/openFrameworks/commit/af602342ecb86ba4f6a91bea6fadcfdce7bdb3a2)
- macOS GLFW - no shadow when fullscreen (#7605) [commit](https://github.com/openframeworks/openFrameworks/commit/11cdc79a94d3a795020ca7a26b082b95e5989a29)
- initGrabber -> setup (#7604) [commit](https://github.com/openframeworks/openFrameworks/commit/900f23951dd19481d5404b76df2e3faa536243e5)
- Emscripten player update (#7561) [commit](https://github.com/openframeworks/openFrameworks/commit/e005b572d005a7b1164b84f5d166072dce73f4dd)
- Improve ofGetEnv with default value. (#7587) [commit](https://github.com/openframeworks/openFrameworks/commit/2237665c62af25faaf27eea1355280b9d8b3d122)
- fix: ofLogError message typo (#7599) [commit](https://github.com/openframeworks/openFrameworks/commit/fba7db2f63347eee9fd3df8879b3c92bb835e26e)
- Osc swap (#7600) [commit](https://github.com/openframeworks/openFrameworks/commit/04ce6a3ed126515b43eb13c65781c0624b1d49c3)
- ofParameter: reinit mechanism: use setMethod() instead of set()  (#7564) [commit](https://github.com/openframeworks/openFrameworks/commit/6add96bb4a012a70d15d36acbfb6430f2e67f93f)
- add of.entitlements to Examples/.gitignore (#7583) [commit](https://github.com/openframeworks/openFrameworks/commit/42e5dd259b465131b6a7b835455e197697299fda)
- bundle identifier fix (rfc1034 to allow spaces in names) (#7597) [commit](https://github.com/openframeworks/openFrameworks/commit/0db74fa88209ef27da75a5d9ea6d79e1824c7bfe)
- WIP - more .h cleanup (#7596) [commit](https://github.com/openframeworks/openFrameworks/commit/ef0a3be5e5f78d502fa1b27ded61aeea383d941f)
- update pg submodule PG v.17 (#7580) [commit](https://github.com/openframeworks/openFrameworks/commit/8a014cdad15415a7314639efb44de080c73560ee)
- bugfix random_shuffle is removed in C++17 - replace with std::shuffle (#7575) [commit](https://github.com/openframeworks/openFrameworks/commit/cce8428e1b6754f0457b14a81aa19d7434be06a3)
- add setup argument to listen on specific ip (#7574) [commit](https://github.com/openframeworks/openFrameworks/commit/4b5632acfc4ad42639b29a79f2f63d935925a565)
- Ubuntu : add make to packages to install (#7571) [commit](https://github.com/openframeworks/openFrameworks/commit/da1dc6b915506467913ca3ea596c3447745e6019)
- projectGenerator update (#7567) [commit](https://github.com/openframeworks/openFrameworks/commit/2adc7639e4b9953b73419cdaeda1acf46aea9215)
- loadModel -> load (#7566) [commit](https://github.com/openframeworks/openFrameworks/commit/a903caa97c355bdc82e2cbaf0b76cef970413dcc)
- osx template and xcode_project.sh edit (#7551) [commit](https://github.com/openframeworks/openFrameworks/commit/b2eb11ff5ac8551c65d6bb5d53bd77eec5102b1a)
- ofGetUnixTimeMillis() (#7549) [commit](https://github.com/openframeworks/openFrameworks/commit/e5fb71638048a908af705d541515140bf91a6bd8)
- More .h work (#7545) [commit](https://github.com/openframeworks/openFrameworks/commit/cdbc3a5aa1366b6c4c2cc39888ef7c92e1ba7bff)
- ofIsCurrentThreadTheMainThread (#7548) [commit](https://github.com/openframeworks/openFrameworks/commit/8804b19c39302e6fcb9f4edc0f18a5b562214c7e)
- pg submodule update (#7544) [commit](https://github.com/openframeworks/openFrameworks/commit/67e0b87c72539b1473eafa3031f70569c3273e4b)
- Bugfix nightly wine (#7541) [commit](https://github.com/openframeworks/openFrameworks/commit/f838e32e93c963429cc2f9378dcd39f3e337da0d)
- switch package gen to use PG from github (#7540) [commit](https://github.com/openframeworks/openFrameworks/commit/515b768b1d9cc133d651d045c64a14c09dd254ba)
- fix macOS issue where fullscreen activation is broken after maximizing with green button (#7538) [commit](https://github.com/openframeworks/openFrameworks/commit/515ece5538078ca9963b1c3648f8e51871dbb739)
- make MF foundation default for visual studio (#7534) [commit](https://github.com/openframeworks/openFrameworks/commit/9ce1204c86a5409dcd7f97c5cc2de9f325f66e5d)
- move serialTest (#7536) [commit](https://github.com/openframeworks/openFrameworks/commit/dd07c5ca3cc4fe602190773972ee7dd3de8f624b)
- map (#7535) [commit](https://github.com/openframeworks/openFrameworks/commit/189fbb66d93657eba49198f9f2b72d78a1d1c64d)
- Fixing GLFW issue in Ubuntu 16.04 - GLFW_TRANSPARENT_FRAMEBUFFER (#7532) [commit](https://github.com/openframeworks/openFrameworks/commit/c3cd02b1535ce317841578e144308b46d729efda)
- update linux64 (#7531) [commit](https://github.com/openframeworks/openFrameworks/commit/8d78e0edc7ac4549b821e79666052a3eca3fbcd8)
- add linuxaarch64 to ofxKinect to support kinect on raspberrypi 64 bit (#7530) [commit](https://github.com/openframeworks/openFrameworks/commit/8c7739f50765aadc2219f7c24e9cbfbfab9ccf95)
- add missing gstreamer packages for Ubuntu 22.04 and newer closes #7516 (#7528) [commit](https://github.com/openframeworks/openFrameworks/commit/17542e12a0d77f648095e9063a4c5008e5434e4f)
- RPI - Linux aarch64 minor fixes (#7527) [commit](https://github.com/openframeworks/openFrameworks/commit/29c99edb8a434ff430be354f99d14f54595ce424)
- bump version (#7526) [commit](https://github.com/openframeworks/openFrameworks/commit/84c0cd0f3125c98d2ce5fa3e4f381603ce82aa9b)
- Windows Media Foundation and XAudio Sound Player (#7523) [commit](https://github.com/openframeworks/openFrameworks/commit/0e37607aca920e7671826d1b26ca6fbc62099c43)
- Bugfix vs pg (#7525) [commit](https://github.com/openframeworks/openFrameworks/commit/b49ac59b3d22d18a9922c57fc3e1b445f4e627b9)
- use AV_Engine if OF_NO_FMOD is defined (#7512) [commit](https://github.com/openframeworks/openFrameworks/commit/c8195102e99b1491369772426eb2975f75a20151)
- manual run nightly [commit](https://github.com/openframeworks/openFrameworks/commit/15c54714bef5054fa2b541b6a37391f6cb0f6eac)
- Proposal: a way of not bundling FMOD in XCode (#7507) [commit](https://github.com/openframeworks/openFrameworks/commit/af09bd77bcd753159868eaf6881f459d0b82cbb9)
- add linuxaarch64 to nightly builds and create_package (#7511) [commit](https://github.com/openframeworks/openFrameworks/commit/8cf28e2f55665fdd82c6de745cd814086828cf99)
- Rename assimpExample remove 3DModelLoaderExample (#7510) [commit](https://github.com/openframeworks/openFrameworks/commit/7367cf87257d4153246f3a030d348945858b9348)
- faster sort and option for natural, fast and by date (#7509) [commit](https://github.com/openframeworks/openFrameworks/commit/6ad07c350b7af1a7974966af7368e5f7027bdb74)
- Media Foundation Video Player fix pixels (#7506) [commit](https://github.com/openframeworks/openFrameworks/commit/e1ed2a43bcc10f2463f966f16cfaed276df97116)
- OF_OS_WINDOWS (#7495) [commit](https://github.com/openframeworks/openFrameworks/commit/85fedb9277cd562464615e7e7ad07adea0c9fef8)
- Github Actions only runs on forked repo if it is not a pull request (#7508) [commit](https://github.com/openframeworks/openFrameworks/commit/26d4ae89c0e3501e32da5f67a9d936e230a022cc)
- Raspberry Pi aarch64 — Debian GNU/Linux 11 (bullseye) (#7069) [commit](https://github.com/openframeworks/openFrameworks/commit/f7e0a56134cb58d7068e4c6e86eb4e745ed3ee1d)
- Add mediafoundation to OF VS project and switch to 142 ( VS 2019 )  (#7505) [commit](https://github.com/openframeworks/openFrameworks/commit/54751a642154ffb730851c0eaec7ebab7fbb23f0)
- switch to cron [commit](https://github.com/openframeworks/openFrameworks/commit/ffb36ceaef8c4fcfdcd914cb324a9ae61a627f87)
- Fix nightly (#7503) [commit](https://github.com/openframeworks/openFrameworks/commit/01da3d427e65ac0817183ef512cc6c17f421e56b)
- Update create_package.sh [commit](https://github.com/openframeworks/openFrameworks/commit/a8b9fe0c5afd6a0060701016937afb83d5bf7867)
- Make platform exclusions (#7426) [commit](https://github.com/openframeworks/openFrameworks/commit/f3b3f55da4d75a0ed38a84bbeea6a6bf1550d71d)
- update pg to latest [commit](https://github.com/openframeworks/openFrameworks/commit/58d5fda70b756c2dd413711504a1e16a0ceb3952)
- Emscripten for msys2 and vs  (#7499) [commit](https://github.com/openframeworks/openFrameworks/commit/f028d484b17dd2c590ecd2a056d72d6b245d330e)
- Cpp17 fix for msys2 and linux arm (#7498) [commit](https://github.com/openframeworks/openFrameworks/commit/3818728c8ce8a0143baaf5d9e9fcd3a060e4bc06)
- Cairo update to Filesystem (#7438) [commit](https://github.com/openframeworks/openFrameworks/commit/9943d4bfc234dc06f2901152d06eedb44da29f13)
- Switch CI to Ubuntu-Latest and fix install script on 22.04 (#7496) [commit](https://github.com/openframeworks/openFrameworks/commit/46d1e1669e8d9f715766343cfcb02eaf8f740cc5)
- temp fix for brokwen emscripten window (#7490) [commit](https://github.com/openframeworks/openFrameworks/commit/102986e2bc5154ad16e6258c659ae62633cbc01d)
- Update README.md [commit](https://github.com/openframeworks/openFrameworks/commit/59301f752b1e085080ad6245c818bfb1f3e01ef4)
- local_addons removal (#7485) [commit](https://github.com/openframeworks/openFrameworks/commit/69c24da2c935fb8b478687142ce92710907037ba)
- GLFW Window Enable Transparency with settings variable. Start Draw wi… (#7488) [commit](https://github.com/openframeworks/openFrameworks/commit/dac8f6592307ade71b333ccac3b6a8ce9e3f04c6)
- fix warning with replace instance variable to @property (#7487) [commit](https://github.com/openframeworks/openFrameworks/commit/7a7a251f445fb97ec0fdddbb8d86a60375210523)
- Av engine sound player -redo (#7293) [commit](https://github.com/openframeworks/openFrameworks/commit/26490c51d4f545064667244d7372a29ce997cefb)
- prepare for ofxProjectGenerator folder integration to PG [commit](https://github.com/openframeworks/openFrameworks/commit/a76ef83f0a6eaf1079fbe35a444ae721225e5fee)
- Msys2 fix warnings (#7434) [commit](https://github.com/openframeworks/openFrameworks/commit/239e6b30cb675537cfb3095f663f687f313c0a1f)
- Media Foundation Player for Windows (#7477) [commit](https://github.com/openframeworks/openFrameworks/commit/b9f765639660872cc39c45e406564e45a0d8669c)
- XCode project template - run script phases outside xcode project (#7482) [commit](https://github.com/openframeworks/openFrameworks/commit/6b11b7a70bef4c08493debd3aa9b3dd56fd3668b)
- Assimp example update for Emscripten (#7428) [commit](https://github.com/openframeworks/openFrameworks/commit/d7da2ebfcc1e8847882723f485dc6e4788ce3388)
- emscripten defaults to passing mouse events for first touch and preventing default touch event listener in document. (#7479) [commit](https://github.com/openframeworks/openFrameworks/commit/2c2ab22864bd6fadb42bb8e5a415a1ccf79722a4)
- bugfix ofOSC receiver segfault index out of bounds (#7474) [commit](https://github.com/openframeworks/openFrameworks/commit/8041df2a8e6dfc4783bf6be133182abfd0ce12cb)
- qtcreator qbs: allow project-based override of cxxLanguageVersion (#7475) [commit](https://github.com/openframeworks/openFrameworks/commit/2daca0e265fa4aab1ba5f88fe359b34fbf4cc756)
- Update README.md [commit](https://github.com/openframeworks/openFrameworks/commit/da5f12874dd0ddbb85f10b08abda4c89c4f29fb9)
- Update README.md [commit](https://github.com/openframeworks/openFrameworks/commit/09f00e0b0fc676cc5019ca6cbef9951e110f1715)
- Update README.md [commit](https://github.com/openframeworks/openFrameworks/commit/ddfb2edec9bc9db4b851ea689b4a82e6424bd8d3)
- Update README.md [commit](https://github.com/openframeworks/openFrameworks/commit/faa01b2fbea70f3a35fce7b33774a0cb8689293b)
- fix for include lib in direct show player (#7472) [commit](https://github.com/openframeworks/openFrameworks/commit/507e25a01f5c693cf03ab1474b95cd2bb7b9fb5a)
- update template (#7468) [commit](https://github.com/openframeworks/openFrameworks/commit/92900c79612059b33491c38363b5efd45a30719a)
- Nightly release via Github Actions (#7469) [commit](https://github.com/openframeworks/openFrameworks/commit/1b6af2762101e9f20960c53d8f474e59606bfbb4)
- Fix defines in ofGLUtils (#7467) [commit](https://github.com/openframeworks/openFrameworks/commit/2f1cb0596f818c4cab182e2043e93610aaeab24b)
- exclude fmod from linux qbs (#7465) [commit](https://github.com/openframeworks/openFrameworks/commit/68b6699b2762029ea2be4f9c2484d359779555ce)
- cubemap fs (#7439) [commit](https://github.com/openframeworks/openFrameworks/commit/a3370755a2be618cbb26f5e74b42f6af09a397d6)
- .h cleanup, more simplification (#7459) [commit](https://github.com/openframeworks/openFrameworks/commit/cb10151d790428911cbb26622c12fe0c0cc52226)
- update qtcreator to C++17 (#7461) [commit](https://github.com/openframeworks/openFrameworks/commit/52c92d86e305b37857ea74482b0679e3cf02a9dd)
- Update QtCreator wizard template (override, ofRunApp and some events) (#7463) [commit](https://github.com/openframeworks/openFrameworks/commit/10a8d06f336daf033eade32617e1c097b011651f)
- Fix plist issues with make on osx. Updates #7455 closes #7437  (#7460) [commit](https://github.com/openframeworks/openFrameworks/commit/8c1a6e3c47248e0f782fbddad8c0e7433c338b9f)
- XCode - openFrameworks files by reference (blue folder) (#7456) [commit](https://github.com/openframeworks/openFrameworks/commit/24746f0c644579783d0d06adb6148cb999157752)
- std::map to unordered_map (#7444) [commit](https://github.com/openframeworks/openFrameworks/commit/477f711e8a370d0cb695a43927a14cf377aa5e35)
- MSYS2: add UCRT64 to CI (#7433) [commit](https://github.com/openframeworks/openFrameworks/commit/2a6cdded6723fc11b147a84634a44e8fb6c51799)
- remove eclipse settings (#7454) [commit](https://github.com/openframeworks/openFrameworks/commit/023d955a35067571e098281cc2736d5a5b262225)
- darkorange details... (#7451) [commit](https://github.com/openframeworks/openFrameworks/commit/b369a1861ee0bd82973fe85adafc93054fbd21dd)
- darkOrange typo (#7447) [commit](https://github.com/openframeworks/openFrameworks/commit/b018f42d34f8bd8265ad008f2ce71b7e2a2c48d8)
- fix conditional for GL drop (#7448) [commit](https://github.com/openframeworks/openFrameworks/commit/ad111265c26f0e11077a7a9e836756845549f89c)
- ofxAssimpModelLoader : unused col warning fix (#7445) [commit](https://github.com/openframeworks/openFrameworks/commit/1e77cec5465446b91ff21cc915aa8d646de98099)
- .h simplification (#7442) [commit](https://github.com/openframeworks/openFrameworks/commit/7614d9369d53eb0d96395e7b47f89475ebe5caaa)
- Actions macOS - more folders on cache (#7443) [commit](https://github.com/openframeworks/openFrameworks/commit/fed7f3418df7628c85568dcd7c6e35392f41ad7f)
- moving aside android action for now (#7440) [commit](https://github.com/openframeworks/openFrameworks/commit/0d29c5e9fa60f1cac85f94cac2df2dcb48cbdf69)
- Msys2: fix pkgconf (#7432) [commit](https://github.com/openframeworks/openFrameworks/commit/42d31d25b3e844f866e992fd75d5dfefab139c51)
- Create FUNDING.yml (#7431) [commit](https://github.com/openframeworks/openFrameworks/commit/ca746901d6851fbeb0fe2c2bd566ff79e2ca9b75)
- texture unordered_map (#7388) [commit](https://github.com/openframeworks/openFrameworks/commit/79431f1f35655853b23bc2f14cad5bef2fee70eb)
- soft fullscreen for iOS and option to resize via checkbox in html template. closes #7423 (#7424) [commit](https://github.com/openframeworks/openFrameworks/commit/1f8934b1d8d7f9da4cb7e6685a627b69b8868049)
- make pthreads not on by default for emscripten. add notes in config.make on how to enable. closes #7421 (#7425) [commit](https://github.com/openframeworks/openFrameworks/commit/a6906aeaceadc6dd2fa19912479f5e9e4337f2f4)
- Emscripten Soft Fullscreen (#7418) [commit](https://github.com/openframeworks/openFrameworks/commit/f8da29f3cf7463386eb627049e3d82f4cf7809a1)
- revert c++20 in make osx (#7420) [commit](https://github.com/openframeworks/openFrameworks/commit/c2007a84bca1ae3c1cb9b06f6b33d7a883d8e1de)
- Substitute defines DEG_TO_RAD / RAD_TO_DEG with ofDegToRad / ofRadToDeg (#7413) [commit](https://github.com/openframeworks/openFrameworks/commit/b734917b2bd1080c26e61a12fa6b2a72c290c283)
- update osx actions (#7410) [commit](https://github.com/openframeworks/openFrameworks/commit/2290da40b2b6abcb8a5471e9e1041e69a4cefb62)
- revert addWindow template (#7415) [commit](https://github.com/openframeworks/openFrameworks/commit/6ba9ee396f778075274fba3079f12f121a57018d)
- actions cache to v3 in android (#7417) [commit](https://github.com/openframeworks/openFrameworks/commit/97a16f972cefcf6eebfe61d72ea637787347b98c)
- maintained C++20 compatibility for Linux (#7268) (#7269) [commit](https://github.com/openframeworks/openFrameworks/commit/18f0d80f80a95a9451ba6b1254c88e9f92c7482d)
- fix ctor (#7409) [commit](https://github.com/openframeworks/openFrameworks/commit/312ad3cee094a6d689591dd3385267b6c2fbd1a7)
- More .h work (#7403) [commit](https://github.com/openframeworks/openFrameworks/commit/11366f3b56589b267ed5abf0ee7ad26ea377e488)
- Fixes for some model rotations and cache camera orbit on mouse press … (#7382) [commit](https://github.com/openframeworks/openFrameworks/commit/8a20605bd86dc3b929635af900a126d90e94e540)
- Linux - enable gcc 5.4 to use c++17 (#7406) [commit](https://github.com/openframeworks/openFrameworks/commit/17b46af205b804680523ae9750245d56e892d068)
- remove pragma (#7393) [commit](https://github.com/openframeworks/openFrameworks/commit/95c075ea933ea15bf06cda811e1d42e3007c434d)
- rm boost (#7397) [commit](https://github.com/openframeworks/openFrameworks/commit/aba12c08634d2dd702235d5edc7a1a1de107538d)
- Fixes for programmable renderer and open gl es (#7323) [commit](https://github.com/openframeworks/openFrameworks/commit/4912466c49ee4a325fb1f339618e924223760cf6)
- Updates to shaderGL2 to work with non arb tex (#7402) [commit](https://github.com/openframeworks/openFrameworks/commit/46c625f2a0846206a88868511305f689b4d9c52e)
- replaces all GLES shaders from shader examples with the OF_GLSL_SHADER_HEADER flag (#7341) [commit](https://github.com/openframeworks/openFrameworks/commit/49fadb2937bd86224ed0fa20be0ef661d3a4e0c4)
- added missing const (#7395) [commit](https://github.com/openframeworks/openFrameworks/commit/5f49beee84e0c5c1d691484f72de4fb747e3c852)
- osx make c++17 (#7386) [commit](https://github.com/openframeworks/openFrameworks/commit/6d579a1d4f7a7bbb8cabb8191f9fe1f3613a034e)
- fix for brush rendering (#7381) [commit](https://github.com/openframeworks/openFrameworks/commit/ddd3be8dd0745bda815e56f1861fc0b5b8bc57ce)
- fix for mouse position in the multiTexture3dExample (#7380) [commit](https://github.com/openframeworks/openFrameworks/commit/02c930817dc5d456670fa0d409b9cd39a274aa3d)
- msys2 nightly fix [commit](https://github.com/openframeworks/openFrameworks/commit/efbbdc6522331b0d9af4e9cc8f21d776f3ccc58a)
- fix msys2 downloader [commit](https://github.com/openframeworks/openFrameworks/commit/1a199f1b14723fa744902b76a31a388c301575ed)
- pg head [commit](https://github.com/openframeworks/openFrameworks/commit/7f1442dea7b438303173ee11bb85fc35400384ff)
- update submodules (that was way too hard) [commit](https://github.com/openframeworks/openFrameworks/commit/b644c924df6076509814aebdea32ffa0901af203)
- Assimp fix model scale issue - GLM_FORCE_CTOR_INIT (#7379) [commit](https://github.com/openframeworks/openFrameworks/commit/8ac6818e02ea69f2dcd64064abed001edfd40805)
- ofThreads Emscripten (#7328) [commit](https://github.com/openframeworks/openFrameworks/commit/b9f1f6c274227f1120d1bd9f321c6574a6f155f1)
- add copy build phase for dylibs etc (#7374) [commit](https://github.com/openframeworks/openFrameworks/commit/f79e48960cc88230ffd5126240db9ecc5f401956)
- Add functionality to Xcode template to bundle dylibs (#7358) [commit](https://github.com/openframeworks/openFrameworks/commit/edc7bb4162ec2e0367a1058124396a23e7024224)
- ofColor.h cleanup (#7366) [commit](https://github.com/openframeworks/openFrameworks/commit/ba8987551e1cd7c7900d81cd4a9747d42d69076e)
- ofMainloop cleanup (#7357) [commit](https://github.com/openframeworks/openFrameworks/commit/0c02995693c44439a635d085fd87f155f82bd832)
- update parameters (#7367) [commit](https://github.com/openframeworks/openFrameworks/commit/a0d405a4f18440318bb836a6c548feaa34af8c35)
- Tweaks to iOS examples to remove ofxAccelerometer and ofxiOSAccelerometer (#7371) [commit](https://github.com/openframeworks/openFrameworks/commit/f4193b8b03bf7198dc0b993906e55eafaca90f09)
- update addon_config to specify correct lib in iOS (#7372) [commit](https://github.com/openframeworks/openFrameworks/commit/bbd16d03508d9380b47b61a978aeb2a3b84993b8)
- batch of fixes to the iOS examples (#7373) [commit](https://github.com/openframeworks/openFrameworks/commit/cc5f9244bbf2e0c6a521b2d32c871701fcddd1b5)
- listDir change to of::filesystem::path (#7369) [commit](https://github.com/openframeworks/openFrameworks/commit/ae76a71054dbce78bf246cf20e7bff6090796c80)
- MSYS: Add support for UCRT64 and CLANG64 (#7360) [commit](https://github.com/openframeworks/openFrameworks/commit/f71884f12fde7dc6f9a3ea0fff2ebb33e3e60c4d)
- ofCubeMap.cpp - remove unused bAllPixelsCreated (#7365) [commit](https://github.com/openframeworks/openFrameworks/commit/9885c9eea59557d274afbb1238036c9feab1b1a8)
- changed ifdef to if defined (#7364) [commit](https://github.com/openframeworks/openFrameworks/commit/f52655b0b7a3004986c5f52fc78b1ec04818cd40)
- of::filesystem PR (#7110) [commit](https://github.com/openframeworks/openFrameworks/commit/5dd623d556fcda35496d80a2842731179beb46a9)
- orbitdeg (#7355) [commit](https://github.com/openframeworks/openFrameworks/commit/edb72bd71f3a91cf8833ffc0a5e39df9e2cdca6f)
- correct check for shadows support (#7354) [commit](https://github.com/openframeworks/openFrameworks/commit/c382ed2ba1bfb2c653c108b3d6a2979483af6d46)
- ofAppBaseWindow format (#7353) [commit](https://github.com/openframeworks/openFrameworks/commit/dd68420dc64f696afe9226605f0564c472cd3fc6)
- Fix StdFunctionId public constructor (#7356) [commit](https://github.com/openframeworks/openFrameworks/commit/1d6e2c18ba5634747c0523e2bf67946545f15aab)
- Project template: adds missing inherited boilerplate (and override) (#7349) [commit](https://github.com/openframeworks/openFrameworks/commit/b66d61ca49188745461c4b6774894a2ea4ea9897)
- General changes (namespace, deprecated loadFile function) (#7344) [commit](https://github.com/openframeworks/openFrameworks/commit/61a045c58820384f474442547fd4462244d20ddf)
- ofSoundBaseTypes unneeded include (#7345) [commit](https://github.com/openframeworks/openFrameworks/commit/f6091a03f6347b273bb3e0f7379b5abe7b15ed5f)
- don't initialize until needed (#7301) [commit](https://github.com/openframeworks/openFrameworks/commit/69af556612db2a1a6818ea0898de16b3cf8118b1)
- Cubemap support for Emscripten and pbr cube map caching (#7347) [commit](https://github.com/openframeworks/openFrameworks/commit/46f36d312487468c182a3e159b0225f87cd2ab65)
- ofxiOSCoreHaptics (#7343) [commit](https://github.com/openframeworks/openFrameworks/commit/571b1b0aef589c6d871eca04d33d66161f7f6450)
- Emscripten mouse / touch event improvement (#7342) [commit](https://github.com/openframeworks/openFrameworks/commit/c767ea4f637592f8ecc2da31eec5d2014900a600)
- ofxPoco - remove namespace std (#7337) [commit](https://github.com/openframeworks/openFrameworks/commit/d6784139021ccd2f582f59ef0374c8e142327394)
- remove using namespace std (#7336) [commit](https://github.com/openframeworks/openFrameworks/commit/febd8cd23a0b7ab9fdf9f1c7948e6d7d049bf3f4)
- undef NO (#7335) [commit](https://github.com/openframeworks/openFrameworks/commit/56364cd4f7bdb7b1710b1cf4ebfa2f9ce80a45c5)
- remove using namespace std (#7334) [commit](https://github.com/openframeworks/openFrameworks/commit/07c31b765e572a1a7d3b6d23b1d24b7574ca6865)
- ofxGui removing using namespace std (#7332) [commit](https://github.com/openframeworks/openFrameworks/commit/c5433845688c9c59cd073d9df667ceb11f3d11b0)
- fix android (#7333) [commit](https://github.com/openframeworks/openFrameworks/commit/110d6a4efc4b93598858f6d7a073d55b3cfa8199)
- TransformFeedbackAnimatedExample (#7331) [commit](https://github.com/openframeworks/openFrameworks/commit/a0354c387a25b0ea4a0f88683fd2114fab72f020)
- Add a default header token for glsl shaders closes #7242 (#7330) [commit](https://github.com/openframeworks/openFrameworks/commit/5241a3853f201e2e9e085a8b7c2fbdbe8cea9a5f)
- Actions - cancel-in-progress (#7134) [commit](https://github.com/openframeworks/openFrameworks/commit/affc2eb9ecd793a5d68ac4b341bf687e8d7f6696)
- parameterGroupExample Emscripten (#7327) [commit](https://github.com/openframeworks/openFrameworks/commit/0b969d539b793864741f5876b154d4012a6b419b)
- Github Actions : checkout v3, ubuntu 20.04 (#7304) [commit](https://github.com/openframeworks/openFrameworks/commit/82a28bbf63ea05d90e43b9f4f0c60bc5b950c1a1)
- fix phong shading on gles specular reflection (#7321) [commit](https://github.com/openframeworks/openFrameworks/commit/51ac47b72749a7a8fcda5587a07cef683476b521)
- clean up and more checks for shadow and material shader classes (#7322) [commit](https://github.com/openframeworks/openFrameworks/commit/02c65378a07538966832d211c6d4321cd6c64304)
- added support for ofMaterial shader (#7319) [commit](https://github.com/openframeworks/openFrameworks/commit/1134c3f38af715cfe4154c5ca4036b4bb432bab6)
- update to materialPBRAdvanced changed cube map image (#7318) [commit](https://github.com/openframeworks/openFrameworks/commit/b44b199dccbcf9d763bf8a6af9a5ab9c4f0b206f)
- update material pbr example and cube map image (#7317) [commit](https://github.com/openframeworks/openFrameworks/commit/8914d8ac538f53b4077b1edbe8e5528c57c340fb)
- cubemap support for emscripten (#7316) [commit](https://github.com/openframeworks/openFrameworks/commit/6dbe4891acadf87ae9a359478ce9d6987bb1f806)
- spaces to tabs (#7315) [commit](https://github.com/openframeworks/openFrameworks/commit/12555c41b53571d6f36cd2013c1742fa194dd6c1)
- ofxAssimpModelLoader - animation speed now working and OF_LOOP_PALINDROME (#7313) [commit](https://github.com/openframeworks/openFrameworks/commit/f87f44eb2eb8eb9c72d67aedcd94b0413101f741)
- Emscripten pointsAsTexturesExample 2 (#7300) [commit](https://github.com/openframeworks/openFrameworks/commit/8ee1df71523f733fecd9cf310d55a3cc5686d33c)
- Update to shadows example (#7308) [commit](https://github.com/openframeworks/openFrameworks/commit/67f582af9a48c8a88d4d5d93a51764576f8608bc)
- Shadow support for emscripten (#7307) [commit](https://github.com/openframeworks/openFrameworks/commit/18b44898c18fe9ac82cf0da3b8ca92ca6b748545)
- ofxAssimpModelLoader rm using namespace std and other changes (#7302) [commit](https://github.com/openframeworks/openFrameworks/commit/d7413009a87179107c9ba39f12971a1f2a9ba500)
- iOS examples - audioInputExample update (#7290) [commit](https://github.com/openframeworks/openFrameworks/commit/72a6ea3296b554093b6c168da4c1a7eb13d4e05d)
- Fix ARC problem of ofSoundStream  (#7283) [commit](https://github.com/openframeworks/openFrameworks/commit/ca50df85b96394cfc063940d02e7834dc9b81043)
- Emscripten float textures (#7275) [commit](https://github.com/openframeworks/openFrameworks/commit/4f70827cbb14b45c01ca9ad90c7c93523880ab01)
- Camera and Microphone entries (#7287) [commit](https://github.com/openframeworks/openFrameworks/commit/5e335aa73a3216c3b15f1cdd6047f9b0ff2ff31e)
- audioOutputExample changes (#7286) [commit](https://github.com/openframeworks/openFrameworks/commit/b3f1fce58d86f29d385b96062d2682f2dc00e260)
- inputchannels = 1 instead of 2 (#7288) [commit](https://github.com/openframeworks/openFrameworks/commit/a766c187121cb7bcc40b2c5d5272a6e3afccab80)
- remove using namespace std (#7289) [commit](https://github.com/openframeworks/openFrameworks/commit/191563d488992ecc3ff032f5ed2e12dfc137db29)
- removing release to work with ARC (#7285) [commit](https://github.com/openframeworks/openFrameworks/commit/d6b54dcc9e36a9d65031a67621f4cce864072a1e)
- fix macOS icon for tiny resolution and include iconset gen script (#7274) [commit](https://github.com/openframeworks/openFrameworks/commit/b98cdfd0165b41fcb1bee5b87266bb1197271316)
- maintain and recall the initial value (#7265) [commit](https://github.com/openframeworks/openFrameworks/commit/424b87890da3366d417508650fc10de5aff1af98)
- refine ofSystem() docs to raise awareness about stderr (#7262) [commit](https://github.com/openframeworks/openFrameworks/commit/0c18e80b5d4efa267acd654740102526a064ad99)
- docs: clarification of the meaning of System Time (#7263) [commit](https://github.com/openframeworks/openFrameworks/commit/28429dfc608cbe6efca667e7a4f2b77432ea813c)
- fix(msys2): icon compilation (#7266) [commit](https://github.com/openframeworks/openFrameworks/commit/d90529626fcba5837639728dded5d073756fc4c9)
- Emscripten replace allocate and some small fixes... (#7255) [commit](https://github.com/openframeworks/openFrameworks/commit/db9ec1ceeca1944ba0761ff9e58c58a8bf73efe7)
- Bugfix pbr lighting and light direction (#7257) [commit](https://github.com/openframeworks/openFrameworks/commit/513d61e53c3ca5824195b8a9fa7faff4439b80b9)
- Revert "macOS / iOS sound player. AVAudioEngine - WIP (#6728)" (#7264) [commit](https://github.com/openframeworks/openFrameworks/commit/6fbdfb64af2250e18ede8f292d80e479e5947b6b)
- template cleanup AppStore (#7253) [commit](https://github.com/openframeworks/openFrameworks/commit/a0bfae6633f550aaa2c5ef63f150cdae0a05765e)
- macOS / iOS sound player. AVAudioEngine - WIP (#6728) [commit](https://github.com/openframeworks/openFrameworks/commit/b3a609136d4f1dd8035a09bde977cfb7cfda82ae)
- Xcode template: fix $ITEMS split/iteration in codesign step (#7247) [commit](https://github.com/openframeworks/openFrameworks/commit/de21b76303afb80a313e85f0a8466667364afe24)
- Emscripten audio video revision (#7217) [commit](https://github.com/openframeworks/openFrameworks/commit/281b1b1f44ede7aad6174fd7726c64b6e3f2cdbd)
- macOS modern icons (#7245) [commit](https://github.com/openframeworks/openFrameworks/commit/714e8dffd07f4ccfd736e718c85a560b969bd975)
- Fixed typo in README (#7250) [commit](https://github.com/openframeworks/openFrameworks/commit/1582fd200b5a423ae9355185e5f3fee064d4d3f8)
- iOS template fix (#7252) [commit](https://github.com/openframeworks/openFrameworks/commit/7416a952d641fbea38ce8d31e56b08e9bb8a0451)
- add osx and ios section (#7251) [commit](https://github.com/openframeworks/openFrameworks/commit/3cb2003d79694a3d6c1d1dbfb171687065de2dc4)
- Emscripten isFrameNew() (#7215) [commit](https://github.com/openframeworks/openFrameworks/commit/8fc82635f12398a88fb32b0484fe58e1c3927843)
- Emscripten wheel event (#7241) [commit](https://github.com/openframeworks/openFrameworks/commit/644259d1257a0f95248b63eabe380488ec1a88f8)
- Update ofxAppEmscriptenWindow.cpp (#7238) [commit](https://github.com/openframeworks/openFrameworks/commit/62fc584c867d36406107e205214bb89f9efb739f)
- forward declaration ofShader (#7236) [commit](https://github.com/openframeworks/openFrameworks/commit/c270e476fd1c83c51fad6b3555b4110a7893da7d)
- remove sprintf (#7235) [commit](https://github.com/openframeworks/openFrameworks/commit/8df651735945612f368f0b18980a7a3bb5f08e68)
- Add ofCubeMap to ofMain.h (#7233) [commit](https://github.com/openframeworks/openFrameworks/commit/dedb73a611b463ad10b784401eb2fe32d6e2fdb4)
- ofMaterial PBR advanced example (#7232) [commit](https://github.com/openframeworks/openFrameworks/commit/460cbcca82f9bee7bb4ca646793b610ecf0c49a8)
- PBR fixes and improvements for materials and cubemaps (#7231) [commit](https://github.com/openframeworks/openFrameworks/commit/6bfeff40ad177bcde9c4d194886b82739d2d4be1)
- Emscripten 07_fboAlphaMask fix (#7204) [commit](https://github.com/openframeworks/openFrameworks/commit/1b24b7c517085d1f587b7568c5d93874a4390980)
- VS2019 Fixes : move some settings from emptyExample into props, add missing files (#7230) [commit](https://github.com/openframeworks/openFrameworks/commit/6b2dfca1804a74189bb36a7718d5b7f353fc0f71)
- Update ofxAppEmscriptenWindow.cpp (#7228) [commit](https://github.com/openframeworks/openFrameworks/commit/33370bf8831abf25301970328759e0f52be69c6c)
- convenience functions for cube maps (#7223) [commit](https://github.com/openframeworks/openFrameworks/commit/c256f7b2ec62980a7a6aed643ff15653caa9bc1e)
- PBR Material example (#7225) [commit](https://github.com/openframeworks/openFrameworks/commit/58e22fdf045b0b0a9d4bb61053c81cd5dd0358b3)
- Fix HAS_SHADOWS define (#7224) [commit](https://github.com/openframeworks/openFrameworks/commit/7f16da77d791a86ef20c41148953b682d8a5393d)
- Emscripten list devices (#7226) [commit](https://github.com/openframeworks/openFrameworks/commit/75f9d817447f42af9d89956fa5a6e0f801c76a95)
- Update ofxAppEmscriptenWindow.cpp (#7227) [commit](https://github.com/openframeworks/openFrameworks/commit/21bc5ffed0875bb4fd9517e1ff089b1697b1c56c)
- allow PBR renderer for models that have it (#7218) [commit](https://github.com/openframeworks/openFrameworks/commit/befb2b60bea0983d3c3fd25f59ca674946c1013e)
- Emscripten multiTexture3dExample fix (#7208) [commit](https://github.com/openframeworks/openFrameworks/commit/e564ee93e0667447e97444dd8ed128c42039e3e7)
- Emscripten slitscanRadialClockExample fix (#7201) [commit](https://github.com/openframeworks/openFrameworks/commit/b4466c98018503403a0160ef6ce7b4f7f8eb2554)
- Emscripten floatingPointImageExample fix (#7205) [commit](https://github.com/openframeworks/openFrameworks/commit/719f8fb1db3538a6fa65ce07dac3cd7ff044fd9a)
- Update ofApp.cpp (#7203) [commit](https://github.com/openframeworks/openFrameworks/commit/5367a9f9e525cff52bf70f65326040e61995bf09)
- modelNoiseExample fix (#7199) [commit](https://github.com/openframeworks/openFrameworks/commit/af771dd392e0bbd6b16dd9537a97cf56a39af2b3)
- Emscripten ofxVectorGraphics fix (#7206) [commit](https://github.com/openframeworks/openFrameworks/commit/390097d9eacf84fd1d4f9ec0f17ce67dc082dfa3)
- GLES includes for Emscripten (#7207) [commit](https://github.com/openframeworks/openFrameworks/commit/9082dac7073e803e61fc74ac7f350ad379803bfa)
- Emscripten cameraLensOffsetExample and quaternionLatLongExample fix (#7200) [commit](https://github.com/openframeworks/openFrameworks/commit/c10fbb9ea556f1a7d330ad9eeaaefd671560b76a)
- Emscripten slowFastRenderingExample fix (#7209) [commit](https://github.com/openframeworks/openFrameworks/commit/64484606ba4dd4392996a73954c3e38bdd3aceba)
- Emscripten special keys fix (#7198) [commit](https://github.com/openframeworks/openFrameworks/commit/703aaf8b712455a0e055b39bb9ee51677479f433)
- fix models with normal maps not working in opengles (#7196) [commit](https://github.com/openframeworks/openFrameworks/commit/c6abe2eb8977a4cc29c00cf9eff4251695ac8f9f)
- Core PBR Material (#7183) [commit](https://github.com/openframeworks/openFrameworks/commit/b6d0334adb9655350f51a1a028410337e45f65e0)
- Emscripten billboard examples fix (#7194) [commit](https://github.com/openframeworks/openFrameworks/commit/9d6f63204ea6dd59310a3c703f716005372531cd)
- Update ofApp.cpp (#7186) [commit](https://github.com/openframeworks/openFrameworks/commit/432bad5b0acae0abe10893f729b7ba51f07cf82b)
- fix for emscripten not loading the first model (#7195) [commit](https://github.com/openframeworks/openFrameworks/commit/c76044971fb3c796fe008d218ce02a5f589f22ed)
- Emscripten sortingExample fix (#7197) [commit](https://github.com/openframeworks/openFrameworks/commit/d54862543b6f79bf862b002f5ae69556399d4b43)
- Update template.html (#7193) [commit](https://github.com/openframeworks/openFrameworks/commit/ac0f7788f93f333e13af47b9b32daa2b4311d048)
- Update ofApp.cpp (#7192) [commit](https://github.com/openframeworks/openFrameworks/commit/e8aed3e8efba2738bf09215a873a5a2856ee2d08)
- Update ofApp.cpp (#7189) [commit](https://github.com/openframeworks/openFrameworks/commit/27108601540f6a9784d48457c80a8e376d32a09f)
- Update ofxAppEmscriptenWindow.cpp (#7190) [commit](https://github.com/openframeworks/openFrameworks/commit/4be8f3924feaf5e25262ecf247d8efec75f760e7)
- Update library_html5video.js (#7188) [commit](https://github.com/openframeworks/openFrameworks/commit/13f48cc3841e93a2ea3a1f9f923ccddec1f90aaa)
- Revert "Update ofApp.cpp (#7185)" (#7187) [commit](https://github.com/openframeworks/openFrameworks/commit/419b57cae1205b5d6bb39cf4fc80ae1438dd095e)
- Update ofApp.cpp (#7185) [commit](https://github.com/openframeworks/openFrameworks/commit/5e6d731543993f666be009d2f8af183c19c1f9ce)
- Update config.emscripten.default.mk (#7184) [commit](https://github.com/openframeworks/openFrameworks/commit/762a2baaf9faaf3b45c8cf877de2bafb253a18cd)
- Emscripten ofxgui fix (#7176) [commit](https://github.com/openframeworks/openFrameworks/commit/65fa677a79a5506294a2ae814d21d2504fd67bc1)
- ofNodeExample fix (for Emscripten) (#7178) [commit](https://github.com/openframeworks/openFrameworks/commit/caee5a69c2de257469685894540983204ef5f6a4)
- Emscripten_pointCloudExample fix (#7179) [commit](https://github.com/openframeworks/openFrameworks/commit/06f06c49bfa8fade950285a5b8987e12e81a1815)
- Emscripten pointsAsTexturesExample fix (#7181) [commit](https://github.com/openframeworks/openFrameworks/commit/3c0be3968f87e8fa5932682366b33abcb0151524)
- Emscripten video player fix (#7175) [commit](https://github.com/openframeworks/openFrameworks/commit/9fb210591de922551c40eafb5a0535e6fc0a9e32)
- Emscripten 3.1.19 (#7056) [commit](https://github.com/openframeworks/openFrameworks/commit/b49bf2821d3a18c8f0cffdc9c75909d039433f80)
- Fix for Header files missing in Windows for some first-party addons (#7169) (#7173) [commit](https://github.com/openframeworks/openFrameworks/commit/0a78ebd327875315f21d48866f89fe7021ae50c5)
- ofMesh added more utility in removal functions (start/end index) (#7164) [commit](https://github.com/openframeworks/openFrameworks/commit/cf8a31252ba18793198320977f21409f0b020b17)
- ofxAssimpModelLoader - fix ambiguous load (#7166) [commit](https://github.com/openframeworks/openFrameworks/commit/84ba6d92848c43bf2a4e858e434a8ae3f2484199)
- disable armv7 for now as its failing [commit](https://github.com/openframeworks/openFrameworks/commit/9f149005eb417503dab293295a3529274e58a4ad)
- bugfix with addons symbols using the same location in Visual Studio (#7171) [commit](https://github.com/openframeworks/openFrameworks/commit/8a3a7003f99f17c56ff7ee08d69cb12c55cad49f)
- fixes to windows exceptions in assimp model loader (#7170) [commit](https://github.com/openframeworks/openFrameworks/commit/3503c9c8de92f404cc6a7e52046417da9c4c17dc)
- Fix linux testallexamples (#7159) [commit](https://github.com/openframeworks/openFrameworks/commit/fc98419f9b133407349bd426263e4a8bfe9a8b6b)
- vscode templates includes for all platforms that can use them (#7162) [commit](https://github.com/openframeworks/openFrameworks/commit/af7c3eb1b3d022c09ea1e833b1aa62f4722f82e8)
- Fix model loading for obj and allow for more options for passing in flags to assimp (#7157) [commit](https://github.com/openframeworks/openFrameworks/commit/be89b0dd34d0e00dfcc1d24f9325094b6f9be749)
- Fix duplicate double quotes (#7158) [commit](https://github.com/openframeworks/openFrameworks/commit/49428f95ccbc740bbc96a868d6b56a1e0b97f481)
- swapping around to close the window before you delete it (#7150) [commit](https://github.com/openframeworks/openFrameworks/commit/9f680c4186532604e13839089142919b3e3ccfb0)
- Archlinux install script: use -S instead of -Sy (#7155) [commit](https://github.com/openframeworks/openFrameworks/commit/6e9f871f074ecc312251bc1bf6dae57842e4e53a)
- Bugfix assimp example (#7154) [commit](https://github.com/openframeworks/openFrameworks/commit/665b0f86d327412019ba9d86c88c8bdf5998dc3a)
- set shadow matrix (#7153) [commit](https://github.com/openframeworks/openFrameworks/commit/8ea79da3f153c31b86677c6e59583d046ff3c84e)
- support for include and pragma include shader macros (#7149) [commit](https://github.com/openframeworks/openFrameworks/commit/d04f836a2e04f6de7bd3d04365ea8b7b5dfabcc7)
- ios template fix (#7148) [commit](https://github.com/openframeworks/openFrameworks/commit/200e88947fbb6614db616b79e8815a2b09788ae8)
- fix semicolon [commit](https://github.com/openframeworks/openFrameworks/commit/f551f149097cf8cfe78bd5959fd5f0a7bb98aaf0)
- Add missing semicolon. (#7147) [commit](https://github.com/openframeworks/openFrameworks/commit/45150aeaa7d4be6b14c2070d0baccc0e55fd337d)
- Cairo functions from ofGraphics separated in different files. (#6985) [commit](https://github.com/openframeworks/openFrameworks/commit/64365d1ea21f79804e47396a10bc42f4e972b6c1)
- create ppackage fixes for nighty [commit](https://github.com/openframeworks/openFrameworks/commit/6fbef22403587c6b45a100907dcb3fe8af3a35f8)
- update assimp addon to support multiple textures, normal map,  new c++ api (#7127) [commit](https://github.com/openframeworks/openFrameworks/commit/96b01cd1ea42d884fe4f14d853614086abb47f3c)
- support for area lights (#7141) [commit](https://github.com/openframeworks/openFrameworks/commit/5caa613f4a99407cd888ea1bc705cc8736fe952e)
- initial commit slitscan example (#7142) [commit](https://github.com/openframeworks/openFrameworks/commit/b06095b84d4ca866a841278880d92d3fa043a25d)
- Shadows! (#7135) [commit](https://github.com/openframeworks/openFrameworks/commit/c1b3c81772750db84f03882eb2c2b4157ac2dcaa)
- Update build-macos.yml (#7139) [commit](https://github.com/openframeworks/openFrameworks/commit/a887d78f86130715f2edb2921190504ca1ff4ba8)
- remove appstore xcscheme (#7133) [commit](https://github.com/openframeworks/openFrameworks/commit/60b63b56e4142b78362cfe96bdd8a9b2a82894e2)
- macos action cache (#7137) [commit](https://github.com/openframeworks/openFrameworks/commit/de5b257622e2f8a087d4ce32e63e90c9649e56b2)
- Add transform feedback example to macos (osx) download. (#7124) [commit](https://github.com/openframeworks/openFrameworks/commit/b3e0128cf89516f86a531ea2c9ba47570ab2649a)
- Remove win32 and use apothecary release builds in windows (#7131) [commit](https://github.com/openframeworks/openFrameworks/commit/72faca1669a90b07146c52ad6f3a6839a12d759b)
- download_libs.sh from apothecary nightly (#7128) [commit](https://github.com/openframeworks/openFrameworks/commit/912ffaea6da96046ef0476355ea7a102f625e11f)
- better key for caching [commit](https://github.com/openframeworks/openFrameworks/commit/acf18e329711bfcc231af33d7383cf2611f937a6)
- ofMaterial set additional textures to support better model loading (#7126) [commit](https://github.com/openframeworks/openFrameworks/commit/7134e423c97b884b522786a0f95f9641358a43d8)
- update camera example (#7119) [commit](https://github.com/openframeworks/openFrameworks/commit/41691e90a56fb98e588efa8f34d58aae18196486)
- Removing Quicktime code - Updates on make template (#7118) [commit](https://github.com/openframeworks/openFrameworks/commit/a702b164103d8eeff8e35f0da54f029403e14fe8)
- log message whitespace fixes (#7114) [commit](https://github.com/openframeworks/openFrameworks/commit/977cbf3797a13a2ec39121405b2a82a35e418de2)
- Examples - spaces to tabs (#7115) [commit](https://github.com/openframeworks/openFrameworks/commit/42cc7623cd3e1b60ba6b1db1cd2f68f29f4ee9d1)
- Unneeded inheritance (#6829) [commit](https://github.com/openframeworks/openFrameworks/commit/291e64dba047e6e69c9ae77cdbb50e9bbe168228)
- Optical flow example (#7112) [commit](https://github.com/openframeworks/openFrameworks/commit/60e42a93a376d99243ed50ca307fba7edaff1d5f)
- check for attribute location and respond default corresponding attribute or custom attribute (#7096) [commit](https://github.com/openframeworks/openFrameworks/commit/b6e20bcec5d14291acc3c7c3f1962b94aed20c69)
- Github Actions updates - Caching libs and compiled code on macOs (#7053) [commit](https://github.com/openframeworks/openFrameworks/commit/33bbde4fa1037c3c8e3163f94d62015e759956b0)
- Feature opencv cvmat (#7107) [commit](https://github.com/openframeworks/openFrameworks/commit/ab5a7aea1959228638f013cdc135eb84f964535c)
- Paths to mesh example from font (#7109) [commit](https://github.com/openframeworks/openFrameworks/commit/c61719679d7338a2dd84b0bf4d9d182974b84e76)
- Example multi texture3d (#7108) [commit](https://github.com/openframeworks/openFrameworks/commit/d5ec1b744ba0abbf7cafd831b9d45f49cfe2b6fc)
- alternating deprecated devicesWithMediaType with AVCaptureDeviceDiscoverySession (#6935) [commit](https://github.com/openframeworks/openFrameworks/commit/5dd4b5c497c7fab46963648f91770c62dd29db3c)
- Moving lines from Release and Debug to Shared and selector [config=] (#6973) [commit](https://github.com/openframeworks/openFrameworks/commit/15e626bbc6b0a993d516876e64c47c367d59fec8)
- fix ofTrueTypeFont::stringWidth (#7065) [commit](https://github.com/openframeworks/openFrameworks/commit/c4922f7a454a3380fb1c610efe027eadce0d901b)
- image classification example using Yolo5 (#7099) [commit](https://github.com/openframeworks/openFrameworks/commit/4caad2a1ce9dd64a7c53130a3b2032a88d42c12c)
- Submodules to latest (#7105) [commit](https://github.com/openframeworks/openFrameworks/commit/7f3e1fe40bce777d1dd7414a36e1cfb5f32fa9aa)
- iOS updates  (#7033) [commit](https://github.com/openframeworks/openFrameworks/commit/4b51bd240de1a3adf5c6a76ff1c4a3cdb5ce98e8)
- Simple example of high resolution saving of an fbo (#7104) [commit](https://github.com/openframeworks/openFrameworks/commit/ec7a71c3065aec10e9c2e09cb5d8f4759a3d4f0b)
- Extend of material (#7102) [commit](https://github.com/openframeworks/openFrameworks/commit/3f32291f5119a5ceff44adf8546fc558c69f7ec9)
- opencv example for detecting people in an image (#7100) [commit](https://github.com/openframeworks/openFrameworks/commit/54294b5b05f9e0af6f2ece8d21aa5e797f31f6d4)
- Update camera ribbon example to use radians and polylines for smoothing (#7103) [commit](https://github.com/openframeworks/openFrameworks/commit/98e3742fce275d491a25ad99c33ca3f3831730fe)
- update to the serial example to allow for reconnection and typing a message to send (#7101) [commit](https://github.com/openframeworks/openFrameworks/commit/e5e54691a454899e9dfa1e2f1a4ae81ea9ea43e1)
- fix algeterror (#7098) [commit](https://github.com/openframeworks/openFrameworks/commit/240e8b140dbddb5952224b826b101f3e31c69972)
- Transform Feedback animated example (#7094) [commit](https://github.com/openframeworks/openFrameworks/commit/f9fcaded42bf3916505a3f535e536c021d9530e5)
- Point sprite render shader (#7095) [commit](https://github.com/openframeworks/openFrameworks/commit/feeea94555427a5dc0bde97ba88db2414110d9f9)
- changes to support frame by frame in videoPlayerExample. use newer setSpeed api for Linux (#7066) [commit](https://github.com/openframeworks/openFrameworks/commit/02c3333be25390e03c8a5246b971491f9396fc5b)
- fix fedora install script closes #7088 [commit](https://github.com/openframeworks/openFrameworks/commit/e21ee1050ce2c1b08e25d2cf3f392b1b1f372009)
- ios json pretty (#7093) [commit](https://github.com/openframeworks/openFrameworks/commit/f250bce0633c86e6ebcbf8d1e698d67e8006ba78)
- remove code that is never executed (#7083) [commit](https://github.com/openframeworks/openFrameworks/commit/51935432645546b1b6997c4041983d0f5cc8c841)
- template (#7084) [commit](https://github.com/openframeworks/openFrameworks/commit/6a1de9561ae245821c1a4c6510d24e8c4337ff26)
- Quaternion Arcball example update with multiple methods for deriving position (#7090) [commit](https://github.com/openframeworks/openFrameworks/commit/b9317f53c92eb7b3914a5a6fd3fe532c42531f8c)
- map degrees to radians for glm conversion (#7091) [commit](https://github.com/openframeworks/openFrameworks/commit/7565b867d4ee19412602c7d0be5a66eff5b545f6)
- Example quaternion lat long example (#7089) [commit](https://github.com/openframeworks/openFrameworks/commit/a67930003a08addcf7e1d0bd0b1853a0a266b793)
- removed because it was a duplicate of shader / 07_fboAlphaMask (#7087) [commit](https://github.com/openframeworks/openFrameworks/commit/bec015610d23fa5cc18529c606bf6b42af1eb2ad)
- Truck with OF logo in the bed moving around (#7086) [commit](https://github.com/openframeworks/openFrameworks/commit/f2d39834d7edfd83dde9afbcd39912c9d812e837)
- geometry example update with openGL3 shader support (#7085) [commit](https://github.com/openframeworks/openFrameworks/commit/8cfa8513ee2d319d6c72ab2b13aca34e1e3522df)
- Update main.cpp to use more modern setup in examples/  (#7082) [commit](https://github.com/openframeworks/openFrameworks/commit/b051ff3f8c5caea01dd8dfb28c4a64885d63c62c)
- Changed singleLightExample to pointLightsExample and update (#7080) [commit](https://github.com/openframeworks/openFrameworks/commit/4a039d01acbaab8ddd6745bc09820fa3daef55eb)
- Update area light example with of logo (#7078) [commit](https://github.com/openframeworks/openFrameworks/commit/959b1b6574122dfd09e39303ca0e816e94e97691)
- update to examples/shader/07_fboAlphaMask (#7081) [commit](https://github.com/openframeworks/openFrameworks/commit/7b73201cd2c0084f620157ba59ffb93036c822c0)
- ignore example folders in CI (#7079) [commit](https://github.com/openframeworks/openFrameworks/commit/4ccc0b20a6b138dc94bc719bc308e29d518a8e67)
- No ARC enforcing (#7051) [commit](https://github.com/openframeworks/openFrameworks/commit/4989126f4c509a4a9ffbe8c5680296ec24abb577)
- update macos to 12.0 in CI closes #7036 (#7072) [commit](https://github.com/openframeworks/openFrameworks/commit/12aed8b724e2c963de4a6baeb19ce6841733b7bb)
- ofOpenALSoundPlayer load check (#7074) [commit](https://github.com/openframeworks/openFrameworks/commit/d87e8ca0b1048dcecab585a74b9b23941ee4d4e2)
- fix ofMatrixStack error message spacing (#7071) [commit](https://github.com/openframeworks/openFrameworks/commit/bd869a6715e154c8af41e6152be4c86587689373)
- WIP: Fixes for gstreamer 1.20 broken video playback. closes #6871 (#7050) [commit](https://github.com/openframeworks/openFrameworks/commit/bd4042344dc9670770754374607d2ca8190f9476)
- json format (#7052) [commit](https://github.com/openframeworks/openFrameworks/commit/fa662f328c81b03beba1aa428a7158f117065984)
- ofAVFoundationMediaPlayer - fix 'self' warning (#7026) [commit](https://github.com/openframeworks/openFrameworks/commit/9aab53df0dee93852c2dcb12168a8ecaded8844e)
- Adding packages now not included on Ubuntu 22.04 (#7035) [commit](https://github.com/openframeworks/openFrameworks/commit/3a96e0355233a157cb2e5d273ff5a84a3ca273b1)
- update subdmodules [commit](https://github.com/openframeworks/openFrameworks/commit/25ea209c74e943de007688b70e789a87e06f6c1c)
- pbxproj json conversion (#7028) [commit](https://github.com/openframeworks/openFrameworks/commit/dbba158c1c66513978f8dbcff8716afd524ed7f5)
- pragma ignored (#7027) [commit](https://github.com/openframeworks/openFrameworks/commit/3d0074fe19d5743fc3bf2b1eab16cfd198a6e614)
- XCode preparing for AppStore (#7003) [commit](https://github.com/openframeworks/openFrameworks/commit/a89f676523367ed477614d83a5ae63899adad386)
- Yet another std::Iterator replacement (#7018) [commit](https://github.com/openframeworks/openFrameworks/commit/7c67accdea5bfe4888569d941b759af451951d1e)
- use of Variadic templates instead of variable arguments (#7012) [commit](https://github.com/openframeworks/openFrameworks/commit/3774e728adb16b84107d48d45b1edac6abc21153)
- Msys2 : use libs from pkg-config (#7014) [commit](https://github.com/openframeworks/openFrameworks/commit/b136ba1762072d846b11f5c5c8aad7bcb923443c)
- Bugfix linux cpp17 (#7013) [commit](https://github.com/openframeworks/openFrameworks/commit/5263fddaa34062dd116a6fd5f0609c252a3cf4c6)
- Unicode changes ofTrueTypeFont (#6971) [commit](https://github.com/openframeworks/openFrameworks/commit/6a7f7457c52695c6c23ea110be96b014201bdda5)
- removing unneeded .h (#7009) [commit](https://github.com/openframeworks/openFrameworks/commit/bb9852dcf2812a04efd2961d93fc4b79352b4c6c)
- using namespace std, removing (#7005) [commit](https://github.com/openframeworks/openFrameworks/commit/7a4d2412cac84b1d3f803fe854bc77b5e324e403)
- ofxSVG to ofxSvg (#6906) [commit](https://github.com/openframeworks/openFrameworks/commit/4c29c11c218d83c3a77d9b47b4bbf2c0dcddfa7c)
- Revert "CoreOF.xcconfig boostless (#6994)" [commit](https://github.com/openframeworks/openFrameworks/commit/0fb6e8cb02982c7ecc78bae76b0fd027aa0f115d)
- ofCairoRenderer fix scale=0 / matrix transformation issues (#6991) [commit](https://github.com/openframeworks/openFrameworks/commit/fb54ddb00debed892aa6e538849ebfd8fcc9ff61)
- CoreOF.xcconfig boostless (#6994) [commit](https://github.com/openframeworks/openFrameworks/commit/812c7a3a9f954891446df00d40c8f96e47bcd9dc)
- Silence OpenGL deprecation warnings (#6995) [commit](https://github.com/openframeworks/openFrameworks/commit/a15d7dd28ed83bebe97fec3be4c0bad95274b201)
- migrating settings from pbxproj to xcconfig (#6937) [commit](https://github.com/openframeworks/openFrameworks/commit/69e75aea5b238f69004ea3d7b394ce6699c25697)
- Removing unneeded .h files (#7002) [commit](https://github.com/openframeworks/openFrameworks/commit/463131aba35e8906256d8b13889f396a86be24cc)
- Revert "remove duplicate "using namespace std;"" [commit](https://github.com/openframeworks/openFrameworks/commit/7509b54c5fda28b46d2fb9f3d01d5cc3f98ff94e)
- remove duplicate "using namespace std;" [commit](https://github.com/openframeworks/openFrameworks/commit/d2ec1c766321cbf8dc367891d66e5e9798bab1a8)
- ofSetupOpenGL - const & for shared_ptr in function parameter (#6996) [commit](https://github.com/openframeworks/openFrameworks/commit/6047035c52c65aca9b5e5e7677a27b71637992b4)
- xcodebuild - less verbose (#6992) [commit](https://github.com/openframeworks/openFrameworks/commit/7dd84cb8ce079328898731707a765141940ebaac)
- auto window - const and & (#6913) [commit](https://github.com/openframeworks/openFrameworks/commit/39166af8b8505dd685beb23585743d810f931ba7)
- ofMainLoop::addWindow, avoid temporary copy (#6867) [commit](https://github.com/openframeworks/openFrameworks/commit/1b6830aee5a8dd6557a33041abc34f5c251aa15c)
- shared_ptr as const in function parameter to avoid temporary copies (#6839) [commit](https://github.com/openframeworks/openFrameworks/commit/eb830256f69e66fdc8b0b62f56f709c1f1a746ea)
- remove ofRectangle include (#6982) [commit](https://github.com/openframeworks/openFrameworks/commit/3f4444b1bc9c27a8b6fa2c57ce52560292596aa5)
- ofAppRunner unneeded .h files (#6908) [commit](https://github.com/openframeworks/openFrameworks/commit/ce8e0d41ee13c09d0932baf753cf341ab87b1835)
- ofAppRunner::ofSetMainLoop with const and & (#6910) [commit](https://github.com/openframeworks/openFrameworks/commit/21af940112db5a7c087f6f503c62818aa63e24ce)
- ofPolyline - Removing unneded includes (#6963) [commit](https://github.com/openframeworks/openFrameworks/commit/2f24d353b2fbfc900e8a6d128bc57bf22c3de0e2)
- 3dModelLoader example - Ident fix (#6876) [commit](https://github.com/openframeworks/openFrameworks/commit/34b83df85b50fbd9818a7d7ea92fded160c98c46)
- iosOrientationExample - ofRotate to ofRotateDeg (#6976) [commit](https://github.com/openframeworks/openFrameworks/commit/13a1df19f6e54c9a47c4bc4eb7f85f9d8d26951e)
- videoPlayer GST isPaused isPlaying fix (#6987) [commit](https://github.com/openframeworks/openFrameworks/commit/d67b04ab4281b035d8ee40bff8bf6ef429a778ba)
- ofMatrixStack.h - fix [-Wreorder-ctor] warning (#6877) [commit](https://github.com/openframeworks/openFrameworks/commit/9e8d3f52665de6198910929a735d6bb47930d002)
- Remove redundant serialTest.ino and .cpp (#6951) [commit](https://github.com/openframeworks/openFrameworks/commit/a07ad81b5ec4dd7c765945b573140bcf4adf3d6d)
- ofVideoPlayer removing includes (#6978) [commit](https://github.com/openframeworks/openFrameworks/commit/a2e8eedaf54d356871004c6b47155591a565556f)
- Adding iOS icon 1024x1024 (#6993) [commit](https://github.com/openframeworks/openFrameworks/commit/46f91a4bfd77060b1ea14bf3c1774890bd66095b)
- Revert "Refactoring ofBuffer::Lines/RLines (#6945)" (#6988) [commit](https://github.com/openframeworks/openFrameworks/commit/840c149b06b0d97b352422a70217be1e2e569e40)
- supress warning unused needsResizeCheck (#6878) [commit](https://github.com/openframeworks/openFrameworks/commit/8274161a5c7eedca239391461a145aef68ea53f1)
- ofVideoPlayer isPaused fix for macOS (#6986) [commit](https://github.com/openframeworks/openFrameworks/commit/ff380467109eb1babe3fdfb20f608cff26568c2a)
- Refactoring ofBuffer::Lines/RLines (#6945) [commit](https://github.com/openframeworks/openFrameworks/commit/11653065df8da29827cbd57ad97bf0d166b9c63c)
- ofAppRunner - removing unneeded includes (#6972) [commit](https://github.com/openframeworks/openFrameworks/commit/4da76771859215b40ba73b3b70a1e2ea120d41ec)
- Skip warning lose precision for now (#6974) [commit](https://github.com/openframeworks/openFrameworks/commit/ec03a0dc7e3eea55e2df9f7d88695f26e67d485e)
- remove unused numEncoders (#6880) [commit](https://github.com/openframeworks/openFrameworks/commit/664be94b9bad8094c31da754ffd29f0bd3c09b0d)
- Cleanup unused code - remove warning (#6916) [commit](https://github.com/openframeworks/openFrameworks/commit/3090304b111e455aa00df545a68748a0f50e5057)
- iOS template update - Launch Screen (#6983) [commit](https://github.com/openframeworks/openFrameworks/commit/0251a370236abb30ca013158b2ee5143ba578975)
- Fix Objective-C/C++ codes to supporting ARC (#6889) [commit](https://github.com/openframeworks/openFrameworks/commit/625df269a462dcbc988ca0bcd90aafd0b66767bf)
- Commented out Boost in Project.xcconfig (#6969) [commit](https://github.com/openframeworks/openFrameworks/commit/284060876925bec8e2c615207c0074a1cdbc9234)
- XCode project ios : remove outdated localization settings (#6958) [commit](https://github.com/openframeworks/openFrameworks/commit/056cc6497b0c2c9d4690ce291e6148d39e61d4ef)
- Enable Testability on Debug (#6961) [commit](https://github.com/openframeworks/openFrameworks/commit/d91ff019c41f8814ab868182de830d527fc1a92c)
- move common lines from release/debug to shared (#6956) [commit](https://github.com/openframeworks/openFrameworks/commit/58d74952a299a88cb06e4a42ee71f17f6f076322)
- $(PRODUCT_BUNDLE_IDENTIFIER) in info.plist (#6959) [commit](https://github.com/openframeworks/openFrameworks/commit/640e85c0bb3a1c36285d2cceab6fddd9b3f232f6)
- Fix missing include. [commit](https://github.com/openframeworks/openFrameworks/commit/f90003b4a5cc54531f02682d74309188cd3da6b1)
- Missing return now is an error, not warning (#6962) [commit](https://github.com/openframeworks/openFrameworks/commit/50bd1999bd8332db0e0a6d27c6362dc486ad45a2)
- ofxOpenCV - ofPoint to glm::vec2 (#6882) [commit](https://github.com/openframeworks/openFrameworks/commit/ecefc082077fece4228c2ab4f9408bb03a21773f)
- Removing QTKit from Frameworks (#6960) [commit](https://github.com/openframeworks/openFrameworks/commit/f67aa4c5576841db6f6d65ca9157a61329bdb561)
- ofxXmlSettings loadFile / saveFile deprecation message (#6944) [commit](https://github.com/openframeworks/openFrameworks/commit/6538a9ffa959832f7d638270f392aef1a2dcf0dc)
- Xcconfig - Moving Release and Debug common to Shared (#6941) [commit](https://github.com/openframeworks/openFrameworks/commit/938936355db0224c1748b0c7136babf03a637f3e)
- Icon simplification (#6946) [commit](https://github.com/openframeworks/openFrameworks/commit/cd317b6001ae6370510e7e2738af3d9e6f3c46bf)
- Arduino binaries removal (#6915) [commit](https://github.com/openframeworks/openFrameworks/commit/2d8ce23aa309ae5d83dd647fd6d4df885a191b0d)
- XCode - Enable Testability on Debug warning (#6927) [commit](https://github.com/openframeworks/openFrameworks/commit/073f17d1bc8cf77407234cabf58fa37ad0b22b53)
- Supress localization update warnings (#6921) [commit](https://github.com/openframeworks/openFrameworks/commit/a27a790a2e44284e5530cf80d01a3ac9fc344292)
- Supress XCode warnings (#6924) [commit](https://github.com/openframeworks/openFrameworks/commit/4dc148b778cb291a84d8f277f1f6c5823b326cbe)
- remove unused lambda capture this (#6917) [commit](https://github.com/openframeworks/openFrameworks/commit/44937b967e4cc28445afff7e2fdca2a1642a4e9a)
- Replacing deprecated NSFileHandlingPanelOKButton (#6918) [commit](https://github.com/openframeworks/openFrameworks/commit/66c4b61a6b5caf426b82349ac23a2d887472bbe9)
- change CFBundleIdentifier (#6922) [commit](https://github.com/openframeworks/openFrameworks/commit/b959fa52f33ef753f9dbddf12b1c9a35b1e48c09)
- Update ofAppGLFWWindow.cpp (#6934) [commit](https://github.com/openframeworks/openFrameworks/commit/12d769f4fba067857c18a08216ca6f2b51db4638)
- Bugfix OpenAL libsndfile error on linux and msys2  (#6932) [commit](https://github.com/openframeworks/openFrameworks/commit/290dbfc23930425024c9456b5c5519611fb1990d)
- Fix windows ( VS ) ci (#6928) [commit](https://github.com/openframeworks/openFrameworks/commit/95ed6598a272cdcdab7a129bfe76c28efac99d22)
- ofAppRunner::ofRunApp const and & (#6914) [commit](https://github.com/openframeworks/openFrameworks/commit/bc294119135e62eeb07b6845a01990820c2ec0e5)
- switch default value of wrapInBundle from true to false. (#6901) [commit](https://github.com/openframeworks/openFrameworks/commit/176d96c2221739c6e428a96660c4790f0236db0a)
- update deprecated cocoaWindow setStyleMask options (#6912) [commit](https://github.com/openframeworks/openFrameworks/commit/967f59a4064b30cc43d7fbf3f15a37ad83a58304)
- update deprecated function NSBorderlessWindowMask (#6911) [commit](https://github.com/openframeworks/openFrameworks/commit/6c2a423eea2031308df04e6536cc8114438abd72)
- Fix ofLineSegmentIntersection test between line and rectangle. (#6586) [commit](https://github.com/openframeworks/openFrameworks/commit/2b8a581b77b6c658f6794b2df10d429f6493591e)
- unused variable bytesleft removal (#6904) [commit](https://github.com/openframeworks/openFrameworks/commit/2457a2ec02f142310865f11986d02e23c04910ad)
- Move functions about data path from ofUtils to ofFileUtils. (#6893) [commit](https://github.com/openframeworks/openFrameworks/commit/9e4a859f735a4bf35852454de1f11c40eaa8fd27)
- Revert "ofUtils.cpp warning  [-Wmisleading-indentation] (#6891)" (#6897) [commit](https://github.com/openframeworks/openFrameworks/commit/7784734d69917c42ba197565a6d9698059d47a98)
- ofUtils.cpp warning  [-Wmisleading-indentation] (#6891) [commit](https://github.com/openframeworks/openFrameworks/commit/3fbddb17931975d3d72d587591485b3e7f4695e2)
- Enabled SSL certificate verification (#6894) [commit](https://github.com/openframeworks/openFrameworks/commit/b4dacc8f8e6d3c1f63d50706920ded58da686bc8)
- fix secure git url (#6896) [commit](https://github.com/openframeworks/openFrameworks/commit/b935d52308ed1b052a9ec310dc8f6b7bec6cf521)
- Create SECURITY.md [commit](https://github.com/openframeworks/openFrameworks/commit/42fa5579d0bc8cf934e15ce772bc608f411ffe95)
- cpp 17 / std::filesystem  (#6844) [commit](https://github.com/openframeworks/openFrameworks/commit/fab44829713fe167451f056e8570a379fbe97409)
- Suggestion: shallow clone (#6841) [commit](https://github.com/openframeworks/openFrameworks/commit/5e46cfe20bf00ef9f85e92ccebc4a4e3eae172ef)
- remove std::move which prevents copy elision (#6879) [commit](https://github.com/openframeworks/openFrameworks/commit/f54536fcdf8b79558907f636b2101371d641b04d)
- lambda capture 'this' is not used (#6881) [commit](https://github.com/openframeworks/openFrameworks/commit/4ce0abc593f8aeb6b0e9afd2dced076229e17d28)
- ofCairoRenderer reordering fields (#6883) [commit](https://github.com/openframeworks/openFrameworks/commit/c5ef11916839a2e857d0f8a75abb408bf25cb0cb)
- ofxTruetypeFont: Added index property to settings so fonts with several styles can be loaded. This shouldnt break anything . It is just exposing this feature (#6875) [commit](https://github.com/openframeworks/openFrameworks/commit/ce35be666681c68a00fbce5e4562b6983da69a16)
- Bugfix CI armv7 (#6886) [commit](https://github.com/openframeworks/openFrameworks/commit/fd96adf6a608477fe66d58b8ce32cfe4a67fe002)
- ofxOsc - Excluding a include folder for macOs (#6843) [commit](https://github.com/openframeworks/openFrameworks/commit/bd5eda3863edc58dd22cba45d4dfb8c4d1fcf9a5)
- ofVec3f to glm::vec3 in doxygen comments (#6853) [commit](https://github.com/openframeworks/openFrameworks/commit/169aac1790aa271e580c9e274f55fb30f9363003)
- Removing unneeded includes (#6852) [commit](https://github.com/openframeworks/openFrameworks/commit/241079e33fde08bb1f8a387d10a8e33fdd99e547)
- replace loadModel with load (#6873) [commit](https://github.com/openframeworks/openFrameworks/commit/3b91655b945049f7aab54c29d9f697a35812dd99)
- replace loadModel with load (#6874) [commit](https://github.com/openframeworks/openFrameworks/commit/025ffcfe2022ed530072af44614f1d8c98608cd8)
- fix doxygen comment from ofMatrix4x4 to glm::mat4 (#6857) [commit](https://github.com/openframeworks/openFrameworks/commit/6f685a2d991b7cc5d91b8dc54142793ea23e3a60)
- add type alias of template arguments for get size of type strictly (#6856) [commit](https://github.com/openframeworks/openFrameworks/commit/c9f61bcff7d4f67a95f1a109b37275005c3874e2)
- ofCamera - removing ofVec2f from doxygen (#6859) [commit](https://github.com/openframeworks/openFrameworks/commit/dd86bd02ccd482b002d400b43e33dd02b300ec74)
- fix spell from extention to extension (#6861) [commit](https://github.com/openframeworks/openFrameworks/commit/b6b640d96e57165ca07f3ace2b01532d9fe8d81d)
- fix documents about replacing ofVecNf to glm::vecN (#6855) [commit](https://github.com/openframeworks/openFrameworks/commit/31bfec1bae34dc0733e2fc17534330b18457c0b6)
- remove ofVec2f (#6854) [commit](https://github.com/openframeworks/openFrameworks/commit/6e4b81963484a1a042967e161f4b379a21218ed1)
- ofVec3f removal (#6851) [commit](https://github.com/openframeworks/openFrameworks/commit/e194964654d54a464212764c77a670fc9ee19eb3)
- make cwd free (#6847) [commit](https://github.com/openframeworks/openFrameworks/commit/ea57a8d5bd9e248b2fd7046a37498ea1448035c8)
- remove duplicate include ofMesh.h (#6850) [commit](https://github.com/openframeworks/openFrameworks/commit/5d4990903c502e49787bc41b1870e2f95bf910da)
- Removing duplicate include ofMesh.h (#6849) [commit](https://github.com/openframeworks/openFrameworks/commit/bf7b415cde4829d766fef244773796ba3e922525)
- ios sound player to allow bluetooth audio (#6840) [commit](https://github.com/openframeworks/openFrameworks/commit/2940d8f1070a131c4eed5e5899d19c175ed82617)
- std::move here (#6846) [commit](https://github.com/openframeworks/openFrameworks/commit/e3be41027cffc751cd5f18bc6b843fd00126068b)
- Merge pull request #6845 from 2bbb/PR/fix_bugs_on_tvos [commit](https://github.com/openframeworks/openFrameworks/commit/7d7993550c2f9b00a985769dfaab3825abb975e5)
- fix glViewController to uiViewController [commit](https://github.com/openframeworks/openFrameworks/commit/578c3a93e924d487e9c419476fba64fb324c41d6)
- fix include guard is same name... [commit](https://github.com/openframeworks/openFrameworks/commit/95a5e01933acd0f0c3908519ea793e0e9abcd29d)
- fix: ambiguous byte conflict with windows header (#6783) [commit](https://github.com/openframeworks/openFrameworks/commit/9973bd9a589eb300aaeb3e46cd2d57b880f3af38)
- Freeimage flags (#6741) [commit](https://github.com/openframeworks/openFrameworks/commit/e0418318e157f0725adb06859c128421b447005d)
- Fix issue in ofSerial during Win32 enumeration (#6777) [commit](https://github.com/openframeworks/openFrameworks/commit/65c350dc031716865f3398b704585c7079466bf0)
- remove unused variables (#6817) [commit](https://github.com/openframeworks/openFrameworks/commit/e9af13d388c06bd7e703adf2be491846030f9bec)
- ofColor::getLightness() - fix overflow error (#6731) [commit](https://github.com/openframeworks/openFrameworks/commit/3cf8ff4d551e98ae978ac23b8823431c3cf2501f)
- added range for CJKSymbolAndPunctuation and added to CJK ranges (#6756) [commit](https://github.com/openframeworks/openFrameworks/commit/0d33ad37c40e2f842624d0f910cb10126354ca13)
- Updating old defines - TARGET_OS_IPHONE_SIMULATOR (#6836) [commit](https://github.com/openframeworks/openFrameworks/commit/1f4aa5905324d688c5b868ff21586c9a7342d76c)
- fixed duplicate ofBaseApp.cpp in openFramworksLib.xcodeproj (#6796) [commit](https://github.com/openframeworks/openFrameworks/commit/17775f29527d5b2bfd9d6c935ecd6b8be4ccdb7e)
- alertwindow - Focus on textfield (#6820) [commit](https://github.com/openframeworks/openFrameworks/commit/562a08c9bb0170a27452aa4cf64a16b105d9f3b2)
- fixed addTrailingSlash on std filesystem (#6826) [commit](https://github.com/openframeworks/openFrameworks/commit/75ca933305966ede0e3ce4dce7d960df9c01d26f)
- new url for armv7 download (#6834) [commit](https://github.com/openframeworks/openFrameworks/commit/af04c60d43fd252ba4cdf956264485ee6d125da8)
- redundant code removal (#6824) [commit](https://github.com/openframeworks/openFrameworks/commit/670e5fb3515dcd3f1f9bec53f9bf69160126c43e)
- fixed argument name of ofRotate*Rad from degress and radians, updated comments (#6825) [commit](https://github.com/openframeworks/openFrameworks/commit/5296a63696abedda2202f5c5be8aab519fd9cba7)
- ofVideoGrabber setGrabber std::move shared_ptr (#6830) [commit](https://github.com/openframeworks/openFrameworks/commit/438dd524d94be075d80b0bb895faa1fa3f41b0d2)
- std::move shared_ptr optimization (#6821) [commit](https://github.com/openframeworks/openFrameworks/commit/e71175c606c5b32269687f830a13a596abbb6b97)
- ofBaseHasPixels is already present (#6823) [commit](https://github.com/openframeworks/openFrameworks/commit/3d5a914118f36f1c234c6bc60568242fe0e623b2)
- PROJECT_CC and PROJECT_CXX fix (#6787) [commit](https://github.com/openframeworks/openFrameworks/commit/a188a734c17ae4e639a365a8718a7577c1de596c)
- AVFoundationVideoPlayer fix - set video position to a previous position than the playhead (#6797) [commit](https://github.com/openframeworks/openFrameworks/commit/9bc3120a00d420af9b74fb78d999a4b86a74ad48)
- Add autorelease to dispatch_async (#6791) [commit](https://github.com/openframeworks/openFrameworks/commit/ce1bbc751595881398934e82029d74fe099899c5)
- ofAVFoundationPlayer.mm Minor tweaks, avoid texture allocation every frame (#6788) [commit](https://github.com/openframeworks/openFrameworks/commit/cb12deeef6300298313c3d622d1394656f7fd993)
- fix to beginshape endshape not using actual color (#6786) [commit](https://github.com/openframeworks/openFrameworks/commit/5b4bbfd2c8c4049d41d2f1579ed84971b84bd205)
- XCode template changes to new build system (#6792) [commit](https://github.com/openframeworks/openFrameworks/commit/28e99be5b57097f277e91063800a4762a1764a57)
- Fix XCode release schemes (#6785) [commit](https://github.com/openframeworks/openFrameworks/commit/2b0fbfac2e8090b4eeff511dc9c6ae09c60fcf39)
- ofURLFileLoader: don't reuse CURL* handle (#6778) [commit](https://github.com/openframeworks/openFrameworks/commit/6526e705ade6a1432f2c34c04ab53119b5647fd4)
- Merge pull request #6775 from 2bbb/PR/fixed_ofxiOS_DuplicatedOverlayView [commit](https://github.com/openframeworks/openFrameworks/commit/19e79bfee3942e1059fa966eaf60981a29229571)
- fixed indentation [commit](https://github.com/openframeworks/openFrameworks/commit/31416b0e001479f660069b8f6f552c4ca79cc71e)
- renamed OverlayView to ofxiOSImagePickerOverlyaView to fixed warning: OverlayView class is implemented in MapKit and ofxiOSImagePicker [commit](https://github.com/openframeworks/openFrameworks/commit/fa630c3506a34a132e6f0c37a8cd573cd7de995a)
- bugfix fixes broken renderer for newer codec tools. closes #6765 (#6772) [commit](https://github.com/openframeworks/openFrameworks/commit/c1cd2528c70d9acfee7a97c226c30aa7e794e417)
- Linux ci upgrade (#6774) [commit](https://github.com/openframeworks/openFrameworks/commit/217c270fd8f866db4535a99c926afe5881668e6b)
- Allow custom icon for Linux. Fixes #6702 (#6759) [commit](https://github.com/openframeworks/openFrameworks/commit/8569142e8850862da3701d8723da92fdfebc25bb)
- Update README.md [commit](https://github.com/openframeworks/openFrameworks/commit/271863cd2d6ef3901048d9b6aafdd24e360cecfc)
- Fixed blur shader on RPI, needed to use normalised texcoords (#6593) [commit](https://github.com/openframeworks/openFrameworks/commit/7c46c8a9415703c1cf7ce1bb66c91cc97851fa19)
- Turn off software flow control in serial device (#6749) [commit](https://github.com/openframeworks/openFrameworks/commit/33f7e49a74899a9aeda17806804fe7bb4dc7b033)
- loadModel deprecated, using load instead (#6732) [commit](https://github.com/openframeworks/openFrameworks/commit/fb4b8af2bbaab638b92e8d8152f80d5ae7807fb0)
- Take2 RPi Multitouch Support (#6737) [commit](https://github.com/openframeworks/openFrameworks/commit/1c47ff59eb79a3cd52fb54d0fc41fc9b1e0197e8)
- ofGetUnixTime() returns 64bit uint now #6738 (#6744) [commit](https://github.com/openframeworks/openFrameworks/commit/0b41399b0479e5a246176b6ee1faf4aeeb280a19)
- Add size() to ofThreadChannel re #4809 (#6730) [commit](https://github.com/openframeworks/openFrameworks/commit/e06be9a4c61a0bb7298e17bf36f9e4f4954e76c8)
- Fix GStreamer DLL paths for newer versions of the library. (#6617) [commit](https://github.com/openframeworks/openFrameworks/commit/e20a5cd4d55da341379d15160c96efc5be69ecf0)
- remove unused i (#6651) [commit](https://github.com/openframeworks/openFrameworks/commit/fe49d24bb05994b55ffa879dd762615894e05f26)
- Fixed bug: crash when call ofVAArgsToString("%d", 0); (#6725) [commit](https://github.com/openframeworks/openFrameworks/commit/1ab9ad04e1d31e735c8a5ca2afce1b2aeaaa97b3)
- make fs utils work with boost::filesystem, std::experimetal::filesystem and std::filesystem [commit](https://github.com/openframeworks/openFrameworks/commit/bf3364ec0c132fdc22eb0fb9e5c26e7e3c0ddaad)



```
 ________    _____    _____       _______     
|\   __  \  / __  \  / __  \     /  ___  \    
\ \  \|\  \|\/_|\  \|\/_|\  \   /__/|_/  /|   
 \ \  \\\  \|/ \ \  \|/ \ \  \  |__|//  / /   
  \ \  \\\  \ __\ \  \   \ \  \ ___ /  /_/__  
   \ \_______\\__\ \__\   \ \__\\__\\________\
    \|_______\|__|\|__|    \|__\|__|\|_______|                                              
                                              
```

CORE
----
### core
- bugfix ofAppGLFWWindow requested monitor wasn't being clamped (#6718) [commit](https://github.com/openframeworks/openFrameworks/commit/ee30e43df10f78ed5b054b2aae17381e1bddab02)

PLATFORM/IDE SPECIFIC
-----------------
### macos 
- Update Info.plist with retina entry (#6701) [commit](https://github.com/openframeworks/openFrameworks/commit/46483211a0faf34cf5d06c050e911630a02083ef)
- bugfix fmod sound player for macos. closes #6714 #6708 (#6717) [commit](https://github.com/openframeworks/openFrameworks/commit/23f9bfd394ca91316257f3d2c9d597a7ac7ff71f)
- add support for Objective C and Objective C++ source in addon Make builds (#6652) [commit](https://github.com/openframeworks/openFrameworks/commit/8ca678228e589c9c8ae0a644dfb8f79ec1ca7e80)


```
      _           _           _               _              
    / /\         / /\        / /\            / /\            
   / /  \       / /  \      / /  \          / /  \           
  / / /\ \     /_/ /\ \    /_/ /\ \        /_/ /\ \          
 / / /\ \ \    \_\/\ \ \   \_\/\ \ \       \_\/\ \ \         
/_/ /  \ \ \        \ \ \       \ \ \           \ \ \        
\ \ \   \ \ \        \ \ \       \ \ \           \ \ \       
 \ \ \   \ \ \        \ \ \       \ \ \           \ \ \      
  \ \ \___\ \ \  _   __\ \ \___  __\ \ \___  _   __\ \ \___  
   \ \/____\ \ \/\_\/___\_\/__/\/___\_\/__/\/\_\/___\_\/__/\ 
    \_________\/\/_/\_________\/\_________\/\/_/\_________\/ 
                                                             
       												
```

CORE
----
### 3d
- glm::vec3 parameter to drawPlane() functions should be const. (#6516) [commit](https://github.com/openframeworks/openFrameworks/commit/0d3ee1cbead31c7fff4636c82e55ddab9851f0f7)
- bugfix fixes incorrect pixelScreenCoordScale calc in ofAppGLFWWindow::setup. closes #6487 (#6546) [commit](https://github.com/openframeworks/openFrameworks/commit/c2e3a9b078b5bf26a18f6a2abcdbc0dceb5a80b1)
- Update ofMesh.inl (#6604) [commit](https://github.com/openframeworks/openFrameworks/commit/b7b7d40ae68587201d1b10ea97f29afb956f8a70)

### core
- fix error log (#6480) [commit](https://github.com/openframeworks/openFrameworks/commit/dd972c6d595d8c8894db9065fcac18c1dc8bfa02)
- Add notification to parents for parameterChanged when ofParameter<void> gets triggered. (#6471) [commit](https://github.com/openframeworks/openFrameworks/commit/0e0a59fcc35bcbb5e2078163c1b607c57afd351f)
- ofEventListeners: added size() method. (#6022) [commit](https://github.com/openframeworks/openFrameworks/commit/4d341aca7079cb4f6dc6668ca62b08f906d9d726)

### examples
- regexExample - removed closing parenthesis (#6693) [commit](https://github.com/openframeworks/openFrameworks/commit/3a187e7032586f75566e21c2312a3c41f4a811ae)
- videoGrabberExample - set the inverted pixels to actual grabber dimensions (#6698) [commit](https://github.com/openframeworks/openFrameworks/commit/a6e565809d53834c23112be0b816e9243fb4429d)
- imageLoaderWebExample - fix the async request on key press (#6698) [commit](https://github.com/openframeworks/openFrameworks/commit/a6e565809d53834c23112be0b816e9243fb4429d)

### gl
- Allow hex format int defines in ofShader code. (#6515) [commit](https://github.com/openframeworks/openFrameworks/commit/c4f90bcb51405905ec0713835f6f57531927b7a6)
- Fix regex when ofShader int define is set to 0. (#6608) [commit](https://github.com/openframeworks/openFrameworks/commit/a52280c252997e379ca1f8da2dde68302baeaabc)

### graphics
- fixes ofImage grabScreen and resize getting swapped R and B. (#6670) [commit](https://github.com/openframeworks/openFrameworks/commit/ea66f8de812b62ab4f21696b2a1cb3ef8bc5779f)

### math 
- core: remove glm/ext.hpp (#6673) [commit](https://github.com/openframeworks/openFrameworks/commit/1d20db946af541faeda94a82f633ef6a8848cef1)
- bugfix ofNode::setOrientation euler angles. closes #6589 (#6674) [commit](https://github.com/openframeworks/openFrameworks/commit/c828d528ae528571e262d5df88794adf0e0a7135)

### sound
- ofOpenALSoundPlayer: fix conflicting forward declaration in newer versions (#6498) [commit](https://github.com/openframeworks/openFrameworks/commit/2bee6a395dba12839a5043e3a1f282e3fd7b24dd)
- changed glGetError() to alGetError() (#6525) [commit](https://github.com/openframeworks/openFrameworks/commit/1afca9f5925ffd0484405bd015181fa8b02bfa09)
- improve load -> unload (#6580) [commit](https://github.com/openframeworks/openFrameworks/commit/15997a0ff43ab9c80020c64a2a9b4512a9bccedb)
- fix undesired play after load (#6582) [commit](https://github.com/openframeworks/openFrameworks/commit/6c6ea54f2d429851960fed6617b7664fa0e666b6)
- bugfix replace deprecated fmod system fft call with future compatible calls (#6639) [commit](https://github.com/openframeworks/openFrameworks/commit/dc17c4329db72c2f44e057c5d882597c3e685ce9)

### utils
- ofXML fixes (#6678) [commit](https://github.com/openframeworks/openFrameworks/commit/be009f6d2c68f26113832343db72a27d16481cb2)

PLATFORM/IDE SPECIFIC
-----------------
### emscripten
- Fix deprecations emscripten (#6502) [commit](https://github.com/openframeworks/openFrameworks/commit/857df55bb0b2c7e61c2e7eea24a7bcda45b2fd55)
- Ensures depth buffer for ofxAppEmscriptenWindow. Closes #6484 (#6485) [commit](https://github.com/openframeworks/openFrameworks/commit/06a9366d93d8cea83fbf7efe3819e2bd928627d3)
- emscripten  DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR (#6536) [commit](https://github.com/openframeworks/openFrameworks/commit/0f03916a4636a7f55b58240fe7c36ebb0c9776f1)
- Fix emscripten to use non deprecated emsdk docker image (#6665) [commit](https://github.com/openframeworks/openFrameworks/commit/2013d179bd355cf60c9f87acb318cd9fafb125eb)
- fix for emscripten and ofFbo (#6697) [commit](https://github.com/openframeworks/openFrameworks/commit/0ccbc1852a1e56e586fc330a00edc2ac1ecea546)

### ios 
- Fixes for drawing BGRA textures on iOS. (#5909) [commit](https://github.com/openframeworks/openFrameworks/commit/0818683e6729db33f5701766bb96b2c1c4c723a4)

### linux 
- Add libgconf-2-4 to debian/install_dependencies.sh (#6495) [commit](https://github.com/openframeworks/openFrameworks/commit/63916a8ee71d92c15964d34577644602486d5336)
- Remove gcc4 / gcc5 from nightly builds (#6659) [commit](https://github.com/openframeworks/openFrameworks/commit/6ca04beb025a9f0ae8fb2c3725e9838953672d94)
- Build gcc6 (#6660) [commit](https://github.com/openframeworks/openFrameworks/commit/b2c18fe22505dc97d83e7d70973f6af41c57c020)
- set ROOT environment variable (#6668) [commit](https://github.com/openframeworks/openFrameworks/commit/b4ea0d9f1804fb218b0702ea79426ff3b65b1335)
- This bugfix fixes a segmentation fault when one tries to read the video stream from a device that only provides metadata information (but no video). (#6688) [commit](https://github.com/openframeworks/openFrameworks/commit/ff22d9e254a512094f21107784444c01c5a89876)

### linuxarm 
- set default AA numSamples to 0 for GLFW window for RPi. closes #6497 (#6503) [commit](https://github.com/openframeworks/openFrameworks/commit/cf8c5005d1e431ede7094d46b8b10c4e5f22100b)
- add latomic linker for legacy Rpi builds in Stretch and newer (#6508) [commit](https://github.com/openframeworks/openFrameworks/commit/bb3451dc5ec6c180303dcf1101ad118645dddb0f)
- remove flags (#6629) [commit](https://github.com/openframeworks/openFrameworks/commit/7e942993fc0cbdf0a61bfcd4fc4e5a66328c389d)

### makefiles
- split PLATFORM_CFLAGS into PLATFORM_CFLAGS and PLATFORM_CXXFLAGS to avoid err when compiling .c files (#6625) [commit](https://github.com/openframeworks/openFrameworks/commit/9eabc1c0048fc3cf1ece8aea26e4931341d0c7d2)
- fixes wrong OF_ROOT path for make files. closes #6650 (#6661) [commit](https://github.com/openframeworks/openFrameworks/commit/fd102cc4b648b7d63466ad808c7856d48f1c23c3)
- better fix for resetting OF_ROOT in config.make [commit](https://github.com/openframeworks/openFrameworks/commit/76d738c33783461e9456a76ff633817b85dadf4b)

### macos
- Fix for transform feedback on macOS (#6473) [commit](https://github.com/openframeworks/openFrameworks/commit/565f9635bdd494a37ee8fb6dd2fa59ae5ae5edcd)
- remove LDAP from core frameworks (#6519) [commit](https://github.com/openframeworks/openFrameworks/commit/16e9ff41f202285646f664e2c3cadbe85ccf4cc7)
- bugfix for removing 32bit archs for appstore. stops lipo error (#6520) [commit](https://github.com/openframeworks/openFrameworks/commit/2d2b88f446f60b06df1ebe0610edd7b09a961c7d)
- Fixed title not being shown when calling ofSystemLoadDialog in macos (#6590) [commit](https://github.com/openframeworks/openFrameworks/commit/29f5edecff90c12ef69e9f79467840af9cdf0ffc)
- Macos xcode fixes (#6640) [commit](https://github.com/openframeworks/openFrameworks/commit/fe591d17e95218569cc2426d1d8f4f646f75fa00)
- bugfix make non-double buffer work for macos. closes #6612 (#6663) [commit](https://github.com/openframeworks/openFrameworks/commit/6a59943a518b6ca30673bd7b420503eb4ceed00b)
- removed install_name_tool fmod post build step as dylib now has correct path. update qbs template to look for dylibs in Contents/Frameworks/ (#6679) [commit](https://github.com/openframeworks/openFrameworks/commit/d95ec462bae8a3e460851454d68721b7bbb35f72)
- disabled debugging when browsing versions, fixes #6687 (#6689) [commit](https://github.com/openframeworks/openFrameworks/commit/b402c5f7cbb0a98bc262bb13adc8df7492aeb47f)
- bugfix macos arm fullscreen crash. closes #6685 (#6691) [commit](https://github.com/openframeworks/openFrameworks/commit/5dbfbe951bf6ce300af77cbebeb69ba8a16f9443)
- Support for Big Sur and Apple M1 Silicon chip / arm64 
- macos - added fix for apps being half sized on retina in Big Sur (#6694) [commit](https://github.com/openframeworks/openFrameworks/commit/845fc8507f48998cb8b0c09cc1f3d68c8394cf61)

### msys2
- msys2: Make OpenAL the default soundplayer (#6664) [commit](https://github.com/openframeworks/openFrameworks/commit/3b790d2c50e24e64d18602502ea62824b2e44888)
- Bugfix msys2 Debug builds. Closes #6683 (#6690) [commit](https://github.com/openframeworks/openFrameworks/commit/43b3e94f0ce6680e2ae9adfd0cc67fe67349b9bb)
- msys2: Added 64bit release. 
- added msys2 projectGenerator packaging. (#6696) [commit](https://github.com/openframeworks/openFrameworks/commit/119adffb39928856064a380938dd99a661ca87c4)

### vs 
- vs bugfix fixes issue where two different cpp files with same name produce same .o file (#6620) [commit](https://github.com/openframeworks/openFrameworks/commit/e5835eab336a865657f0e0d929c268a8fa084636)


LIBS
----
- removed ldap dependency on private LDAP.framework [commit](https://github.com/openframeworks/apothecary/commit/70b695b327dd975f85593b83a64a619e9fd45376)
- Emscripten: machine type must be wasm32 or wasm64 [commit](https://github.com/openframeworks/apothecary/commit/966abe0c45810a89b3d4be9c6dd9ce169e1f19cf)
- switched macos to build arm64 and x86_64 closes #161 [commit](https://github.com/openframeworks/apothecary/commit/128943ab2107246c8a3daa11ebd608154b3c3e82)
- Bugfix linux armv7 closes #160 [commit](https://github.com/openframeworks/apothecary/commit/14c7a412386fc6b7f1a78d4fa8e2cf8ad2867dee)
- Update README.md [commit](https://github.com/openframeworks/apothecary/commit/e5d8cf4ee750f5856b0e61611fa1de97fcaa3003)
- use newer msys build environment by chaning image to VS 2019 (#166) [commit](https://github.com/openframeworks/apothecary/commit/b5d89d7372d68485d1e08e83ce5ce48909e95a73)
- Msys2 svgtiny (#169) [commit](https://github.com/openframeworks/apothecary/commit/f6ac96b9ee9f73d2622f14685e85da004c3e7f2e)
- Add support for GitHub actions  (#170) [commit](https://github.com/openframeworks/apothecary/commit/f80cd91dd4967c66ea86eba3d9090a9d6b09ba53)
- Update README.md [commit](https://github.com/openframeworks/apothecary/commit/216e77195689de59a13c8edf1da601f66d673b85)
- Fix upload (#171) [commit](https://github.com/openframeworks/apothecary/commit/19c1b46509211e13fdb407958337b89a35fa7685)
- Fix uploads  (#172) [commit](https://github.com/openframeworks/apothecary/commit/4521e4acafe1d37d9a780b9ddb114a62c9a40a58)
- tiny fix for macOS builds (#173) [commit](https://github.com/openframeworks/apothecary/commit/18549991659e20841c317fc6371988b279dea8f2)
- disable appveyor for markdown changes and GitHub actions [commit](https://github.com/openframeworks/apothecary/commit/f31b646abd0b125080ca2461ed3db2365a3ddeb9)
- Update README.md [commit](https://github.com/openframeworks/apothecary/commit/342016d2a1b2803e1f4df8a95ca7c64128d0737a)
- small fixes for libaries to make compatible with M1 machines (#174) [commit](https://github.com/openframeworks/apothecary/commit/5284c1d2017b9882d15fc1807c61ace18f7d1880)
- Bugfix glfw macOS (#175) [commit](https://github.com/openframeworks/apothecary/commit/09b9853c19f2b54c214c87ecb8bf90b415a8ca2a)
- Curl macos fix legacy (#177) [commit](https://github.com/openframeworks/apothecary/commit/a64ff27cc90d7eb30b35d938e549cd2ac57b4af5)
- bugfix for min osx sdk being set incorrectly for glfw / cmake [commit](https://github.com/openframeworks/apothecary/commit/44d2d1e94da3a25c6c02a2b945d95ce0bbe2acea)
- adding fmod script for downloading newer fmod libs. (#168) [commit](https://github.com/openframeworks/apothecary/commit/aee22b638b29eedcb821a0747238471f08e6022b)
- revert back min osx to 10.9 as the rest of OF uses that. (#179) [commit](https://github.com/openframeworks/apothecary/commit/f08347fa56603c3796e8a711bdf251f98a81fc68)
- Explicit linux gcc4 (#180) [commit](https://github.com/openframeworks/apothecary/commit/60be0b1f2e821909629e66c00feaede51bf6a454)
- Bugfix upload (#181) [commit](https://github.com/openframeworks/apothecary/commit/356ef458324cbbceb2c13cd0642a8c202665ddaa)
- removes CI as an environment which should stop annoying deployment list showing up (#183) [commit](https://github.com/openframeworks/apothecary/commit/a846d2e6186b03dfa64c663578c0fdccc68c7af8)
- move glm 0.9.9.6 to 0.9.9.7 (#182) [commit](https://github.com/openframeworks/apothecary/commit/79101376435edf6ca8ac47d5be57b3611180576b)
- try and set default gcc and g++ to 4.9 not 5 (#184) [commit](https://github.com/openframeworks/apothecary/commit/99d57381d1db93489044eb8bf3483371e4f6c056)
- fix emscripten to use offical docker and move from deprecated image (#186) [commit](https://github.com/openframeworks/apothecary/commit/31609e50ced95a54192c68792ac713d5c6c742a8)
- Update build-emscripten.yml [commit](https://github.com/openframeworks/apothecary/commit/784426c5c50579b6e4893f1f3aa4652f4be3e259)
- glfw switch to 3.3 stable (#187) [commit](https://github.com/openframeworks/apothecary/commit/68a0ec866341a8487d5c555311f3d5975bd62436)
- add fmod download for msys2 64bit (#188) [commit](https://github.com/openframeworks/apothecary/commit/f03420688ba10a577439a918b6a57dbfb42eb202)
- change branch to stable (#189) [commit](https://github.com/openframeworks/apothecary/commit/30a276b2ffd2fbc137b994ca71a769f10de2b960)
- fix videoinput.sh to use the branch name (#190) [commit](https://github.com/openframeworks/apothecary/commit/c321bf99a0c6c1926cc71bec4c1a123ccf7ba31a)
- fix fmod formula to set correct loader path for fmod (#191) [commit](https://github.com/openframeworks/apothecary/commit/d1273ba869c0e36a7c92fbde7ba76fe707a8da23)
- Fix opencv arm64 (#193) [commit](https://github.com/openframeworks/apothecary/commit/e1b06ea14ae533841b748c99190fc858571d10eb)
- Bugfix arm64 opencv (#196) [commit](https://github.com/openframeworks/apothecary/commit/7b1205bee13f98ce3a2eefc2012f1bada6f5aed1)
- added cmake flags for min osx target to fix warnings (#195) [commit](https://github.com/openframeworks/apothecary/commit/f11e77c2327e6e50d49ab5ef685775224b2e08aa)

ADDONS
------
### ofxSVG
- switch to C locale to parse SVG in svgtiny (#6657) [commit](https://github.com/openframeworks/openFrameworks/commit/ea4c2c3154654172d2bb7a77b3344856108070f7)

### ofxGui
- ofxGui: Updating events to allow multiwindow functioning (#6539) [commit](https://github.com/openframeworks/openFrameworks/commit/ce27971aab60f5c8c0009db6b82df73de20b6938)
- Fix ofxPanel disable header (#6479) [commit](https://github.com/openframeworks/openFrameworks/commit/b9612146dcb1bb74b2cfa95613ef175632119e70)
- Revert "ofxGui: Updating events to allow multiwindow functioning (#6539)" (#6544) [commit](https://github.com/openframeworks/openFrameworks/commit/065ed29aabf048d71acf5dab5e293cfe1ee02d7b)
- ofxGui: Updating events to allow multiwindow functioning (#6429) [commit](https://github.com/openframeworks/openFrameworks/commit/c0a25003942dc8c9331008d2c9d5ce6db3e46ce1)
- fixed ofxGuiSlider text input not positioned correctly as a side efect of commit b9612146dcb1bb74b2cfa95613ef175632119e70 (#6567) [commit](https://github.com/openframeworks/openFrameworks/commit/07e47f745b6793f3a319d49e470f44b04cbcda07)
- ofxGui: Fix possible memory leak, because keyword "new" was being used but not deleted. Now using only unique_ptr (#6570) [commit](https://github.com/openframeworks/openFrameworks/commit/8946a080d847887b23c07c0c4e08c33680c296ae)
- ofxGui:: ofxSliderGroup: properly removing listeners which could lead to a crash when dynamically creating ofxGui objects (#6610) [commit](https://github.com/openframeworks/openFrameworks/commit/f9cb24b3f58d7f8b6aca54dcf4ec57e672202a0d)

### ofxNetwork
- Fix ofx network (#6675) [commit](https://github.com/openframeworks/openFrameworks/commit/0f4a40358279f3c6868fa2943c3820c4516fc43d)


CI 
----
- add msys2 64bit builds to nightly and release scripts. (#6486) [commit](https://github.com/openframeworks/openFrameworks/commit/8eabc2ae8859f5ceb187dde3e8870b885acc064b)
- Add master vs patch-release branches (#6523) [commit](https://github.com/openframeworks/openFrameworks/commit/e6ed59359e23c8a4a34b799c259a200155716caa)
- Merge make43 msys2ccache (#6566) [commit](https://github.com/openframeworks/openFrameworks/commit/f23c71da938452cb42d4c0bfaef087afc26db82e)
- MSYS2 : (re)enable ccache in appveyor (#6572) [commit](https://github.com/openframeworks/openFrameworks/commit/6c9654bd9e0f31ab96162097155aa58e84bb6ed5)
- [CI] Optimize MSYS2 on Appveyor (#6573) [commit](https://github.com/openframeworks/openFrameworks/commit/5453d35ae88022d2c09f47d3beb9a57c1e87a65d)
- update travis.yml to fix linux build (#6636) [commit](https://github.com/openframeworks/openFrameworks/commit/7edcdec7f3af3a5aeaec2928ab5804576377d4fd)
- Issue6605 msys2 ci fails (#6645) [commit](https://github.com/openframeworks/openFrameworks/commit/d3562149cbd0b9a02b9a9bda8f8b3942a6d49175)
- WIP PR for Github Actions (#6649) [commit](https://github.com/openframeworks/openFrameworks/commit/69b671ad39df56fff1bdb293571177431f9b057d)


PROJECT GENERATOR 
----
- feature  ADDON_FRAMEWORKS_EXCLUDE to exclude frameworks from libs folder [commit](https://github.com/openframeworks/projectGenerator/commit/d459eec68c14e9741110059dc6da845e6d4f0736)
- added version option [commit](https://github.com/openframeworks/projectGenerator/commit/8c01f1507616c039997619c9ea26f3e38b33736d)
- feature -  ability to specify additional source/include folders to add to project (#248) [commit](https://github.com/openframeworks/projectGenerator/commit/90a1f0a20c2d31d8d2a020c04d5768a3a3f7ac09)
- Fix nested dependencies (#252) [commit](https://github.com/openframeworks/projectGenerator/commit/e35cc07893c1319a3f49d3da1a02b95434876121)
- switched fmodex to fmod (#254) [commit](https://github.com/openframeworks/projectGenerator/commit/f46c32da75ac9516de4553e69d3d42570200ab01)
- Moving current CI builds to Github Actions (#255) [commit](https://github.com/openframeworks/projectGenerator/commit/c09de46323bce32ba5545fe3413575fd4cb12157)
- add .metal ext for ios (#256) [commit](https://github.com/openframeworks/projectGenerator/commit/b2c28be317dffb5cf6c6363adad4b88a627998a6)
- fix due to bug in current electron-packager [commit](https://github.com/openframeworks/projectGenerator/commit/fa7ff49951911a5bd6021361fea15c445f11ecad)

DOCUMENTATION 
----
- Update Raspberry-Pi-Getting-Started.md (#719) [commit](https://github.com/openframeworks/projectGenerator/commit/e923ec0d460bc5177461948a8ef6ec818eb4cfc1)
- Update download.mako (#720) [commit](https://github.com/openframeworks/projectGenerator/commit/a3876be3ddb94d8145f4425ece00824cbaf92d90)
- update raspberrypi.md.ko for oF 0.11.0 [commit](https://github.com/openframeworks/projectGenerator/commit/f7bd9739a8bef9da6ce7dea7c038422bf7c07153)
- Raspberry-Pi-Getting-Started.ko.md update for oF 0.11.0 [commit](https://github.com/openframeworks/projectGenerator/commit/93f08120a1cd4fbae5fee26c9bff52c1acba07a3)
- fix conflicts from KavinHan [commit](https://github.com/openframeworks/projectGenerator/commit/65171cc4fcd123f944d7f901ccb8d572b69656ef)
- cleaned multilanguage section [commit](https://github.com/openframeworks/projectGenerator/commit/106279d78dea1f4dfee2c5bf08b691f7765cea21)
- update README-ko.md [commit](https://github.com/openframeworks/projectGenerator/commit/9c06fc053f8053451d4c0d4642b895656d4b43a1)
- removed new line [commit](https://github.com/openframeworks/projectGenerator/commit/ccc44fb00ffff000ad14ecd7a384bf02d3801436)
- Update MSYS2 setup guide (#700) [commit](https://github.com/openframeworks/projectGenerator/commit/473a7547b95529feae3540f21ae9cd3493d2da21)
- add 64bit instructions for msys2 (#721) [commit](https://github.com/openframeworks/projectGenerator/commit/ac0485a431b9703a0fe3bf86f67882cab6ef9a4e)
- Fix conf.py [commit](https://github.com/openframeworks/projectGenerator/commit/7fedd2a5b75ff3d45539e1d0a87ab0b2fa553362)
- fix space for the download boxes [commit](https://github.com/openframeworks/projectGenerator/commit/18f8176c52a9c5fa3792f7e7daae286218937b14)
- update msys2.ko.md [commit](https://github.com/openframeworks/projectGenerator/commit/42bfa561137f57904d4c6cf4573e0211c109bf2d)
- fix mistype [commit](https://github.com/openframeworks/projectGenerator/commit/fa9921af79628c1516a193ed0f057936399fcf0f)
- suggest [commit](https://github.com/openframeworks/projectGenerator/commit/d134afe5ffa77c03570609c3f5d037cc5bc2f8c2)
- remov [commit](https://github.com/openframeworks/projectGenerator/commit/23279c1f88db8a1a7111b881b02c86c245220d03)
- fix some duplicate and errors (#723) [commit](https://github.com/openframeworks/projectGenerator/commit/2b17b0fca6d2bcd50662f4452c9df673cb17f477)
- Update emscripten guide (#722) [commit](https://github.com/openframeworks/projectGenerator/commit/65fcd3664201291fb32f142c63bf105809900a6b)
- Update play_a_sound.markdown (#724) [commit](https://github.com/openframeworks/projectGenerator/commit/dd39680ad25349f3fdb408f3254143bd5a36d1b7)
- emscripten.ko.md update [commit](https://github.com/openframeworks/projectGenerator/commit/18a013c76f5aa32c682e66df9ad4a8951e0707b2)
- Update older.md [commit](https://github.com/openframeworks/projectGenerator/commit/0c00220d4665443f652a7d688e060406efca3af3)
- Update Raspberry-Pi-Getting-Started.md [commit](https://github.com/openframeworks/projectGenerator/commit/408870879898ee40bd3b56b60ada5d34e6bce536)
- Fix a file path. / ファイルパスの記載を修正 (#727) [commit](https://github.com/openframeworks/projectGenerator/commit/7c1bcee69213ee5fcd839501c473d26729518b5d)
- ofScale function description update (#733) [commit](https://github.com/openframeworks/projectGenerator/commit/91c05aa76af58f9a096e8eb16afe32b231dd0e5a)
- Update vs.md [commit](https://github.com/openframeworks/projectGenerator/commit/b4d58a18e426b29c11bf492b69399e5ef2070668)
- Update vs.ko.md [commit](https://github.com/openframeworks/projectGenerator/commit/270fd52fa10ed22c2425706b6442ad732fdf4af7)
- Update vs.ja.md [commit](https://github.com/openframeworks/projectGenerator/commit/8d8881ba11ce22dd46f9cf49b285ab39fb957c2c)
- Update Raspberry-Pi-Getting-Started.md [commit](https://github.com/openframeworks/projectGenerator/commit/2a218598508c0b92afea336a57659304a6718e53)
- Update Raspberry-Pi-Getting-Started.md [commit](https://github.com/openframeworks/projectGenerator/commit/c6700820be66245158a0dd6dcf6bc8f897dd682f)
- small syntax fix (#736) [commit](https://github.com/openframeworks/projectGenerator/commit/8a9ada826712bd9fdd7c6d2e14b8f781d31e5474)
- Update 3d_example_how_to.markdown (#738) [commit](https://github.com/openframeworks/projectGenerator/commit/a2f9200b4b80fefa8c925875042ea37d38c1f8ff)
- Fix typo in ofRectangle.markdown (#737) [commit](https://github.com/openframeworks/projectGenerator/commit/28ce405a3790b73dac6d31c1f0ede08f89eccbde)
- Fixed path of where to find example (#730) [commit](https://github.com/openframeworks/projectGenerator/commit/01ad6280e4d65d7306d4aa84440388775c0fba89)
- Fixed Rectangle coordinate suggestion. (#702) [commit](https://github.com/openframeworks/projectGenerator/commit/7be08964f957d7f0c513299c56baf094e07eba39)
- Update introduction.markdown (#739) [commit](https://github.com/openframeworks/projectGenerator/commit/5dfed24a04106c50add816c962d03c7f0baee713)
- Update qtcreator.md to increase readability (#740) [commit](https://github.com/openframeworks/projectGenerator/commit/621e29805d0497ec2586f2fb382330d5f8a4e73c)
- Update emscripten.md [commit](https://github.com/openframeworks/projectGenerator/commit/8b8d4e2e60c60ec2d64c300962f857ec8eda5d8b)
- Update download.mako [commit](https://github.com/openframeworks/projectGenerator/commit/840a39522176314e4ed8618a9e2a175851b88345)




```
     __                  _     _                 __     
   /'__`\              /' \  /' \              /'__`\   
  /\ \/\ \            /\_, \/\_, \            /\ \/\ \  
  \ \ \ \ \           \/_/\ \/_/\ \           \ \ \ \ \ 
   \ \ \_\ \      __     \ \ \ \ \ \      __   \ \ \_\ \
    \ \____/     /\_\     \ \_\ \ \_\    /\_\   \ \____/
     \/___/      \/_/      \/_/  \/_/    \/_/    \/___/    
```

CORE
----
### 3d
- Add handling of double precision position data in PLY model loader (#6292) [commit](https://github.com/openframeworks/openFrameworks/commit/860f2658cd33edad44d3cfbdb23d344e8912b295)
- Bugfix ofEasyCam (#6284) [commit](https://github.com/openframeworks/openFrameworks/commit/29b4d63875bb30fe14b51a8afd7a37ad4cabb6a5)
- ofEasyCam: setPosition actually sets the position no matter what other settings (#6265) [commit](https://github.com/openframeworks/openFrameworks/commit/2b2cc1330d7c49def93a86a2624ed87ace4df77b)
- Implement ofMesh .save() of triangle strips [commit](https://github.com/openframeworks/openFrameworks/commit/45d7055c4afabacbead35e924ebb07894ff8a833)

### core
- Make ofSystemTextBoxDialog return empty string on cancel. (#6155) [commit](https://github.com/openframeworks/openFrameworks/commit/9fc64fda9ca4be0e994230cd0242ddff7cef84b7)
- fix `conflicting declaration` OpenGL and just include glew (#6187) [commit](https://github.com/openframeworks/openFrameworks/commit/fb0ec3c13a81c227da417699fcf8854d34fcd0c2)
- remove line break from ofGetVersionInfo() end (#6107) [commit](https://github.com/openframeworks/openFrameworks/commit/4d86cc7d4ef7b2a21c9771d2c7169829f0ca6f18)
- fix `conflicting declaration` OpenGL and just include glew (#6187) [commit](https://github.com/openframeworks/openFrameworks/commit/c23f11738fbdbe051b3e3e184d002e28f5111ed0)
- replace std::ptr_fun to std::function (#6243) [commit](https://github.com/openframeworks/openFrameworks/commit/0f07e94b2b830efc41c529e7cc7ce08258f30666)
- making the default background color 60 (#6339) [commit](https://github.com/openframeworks/openFrameworks/commit/7773c0267d84d0ae208fb6eb1db27273f2281c56)

### examples
- Fixed OpenGL ES2 shader example 08 (#6439) [commit](https://github.com/openframeworks/openFrameworks/commit/54ce0e2d10337480b2d3d512c9e5953fccda1527)
- update multi window example to draw w/2, h/2 instead of w/2, w/2. (#6442) [commit](https://github.com/openframeworks/openFrameworks/commit/939c823000fa00715f7924ec5f0c88c349523ca8)
- Updated examples to glm (#6389) [commit](https://github.com/openframeworks/openFrameworks/commit/b8473052bdaf49f8cb362ae1fcfdc887ab988105)
- Example fixes (#6431) [commit](https://github.com/openframeworks/openFrameworks/commit/177c1f4292677e37faaf76892117e16ec4a36c06)
- Example fixes2 (#6433) [commit](https://github.com/openframeworks/openFrameworks/commit/5f4bb35f16aa07cc36f1bfd5d924796675c2b771)

### gl
- fix data type on texture load from int32_t* and uint32_t* (#6257) [commit](https://github.com/openframeworks/openFrameworks/commit/eb1f22f876fc033daad908562a78a5b383ee29d4)
- phong.frag: v_transformedNormal and eye should be re-normalized (#6221) (#6225) [commit](https://github.com/openframeworks/openFrameworks/commit/38b0e32d72aa88f4a41d7048c130f000894906e4)


### graphics
- Remove radial line from ofpath::arc and ofPath::arcNegative (#6224) [commit](https://github.com/openframeworks/openFrameworks/commit/68fdf1a87cf949d69ea612fe31da52ef7fa17673)
- fixes ofPolyline to prepare oF for glm 0.9.9.5 (#6366) [commit](https://github.com/openframeworks/openFrameworks/commit/bf2401186f8dc8ad9897e59a67d04473ebd83059)
- ofTruetypeFont: fix kerning calculation (#6464) [commit](https://github.com/openframeworks/openFrameworks/commit/49942e9c53bd154866eee574e76f3a201d99bb6e)
- ofPixels.h/cpp remove move on return to allow copy ellision (#6407) [commit](https://github.com/openframeworks/openFrameworks/commit/951d1e0846ab9fdff4a02b8e3b99dcc1e802db66)
- ofPolyline::removeVertex( ... ) added.
- fix setSpaceSize and getStringTexture (#6282) [commit](https://github.com/openframeworks/openFrameworks/commit/2c036d023e7d6552afd7438a2fd33c87c5a47461)
- ofColor: fix float scaling when copying different types [commit](https://github.com/openframeworks/openFrameworks/commit/2ecfa59110b97b8657643a72fca1fa2ab8c01a38)
- Fix missing const in ofPath (#6341) [commit](https://github.com/openframeworks/openFrameworks/commit/eb444a58a435ac2045fd87985f951665deadff6e)
- Included missing header in ofImage.h required for instantination of ofImage_::clone() (#6314) [commit](https://github.com/openframeworks/openFrameworks/commit/97074921a6c6c346fa9fea425ad6c57366f2b6f7)
- Add default values for ofPath::Command members. (#6344) [commit](https://github.com/openframeworks/openFrameworks/commit/575d5bcc016c8712b59b048625de325f1053f8b8)
- small improovement for getResampledBySpacing (#6361) [commit](https://github.com/openframeworks/openFrameworks/commit/459145efb24896af25b10c3f65380b5777c7acdb)
- Add ofPolyline::removeVertex(...) (#6400) [commit](https://github.com/openframeworks/openFrameworks/commit/d8398128cff3e113c0aca0f1e42ee566012f8a88)
- bugfix ofPolyline::getSmoothed missing glm initialization. closes #6414 (#6421) [commit](https://github.com/openframeworks/openFrameworks/commit/c21aba181f5180a8f4c2e0bcbde541a643abecec)

### math 
- single pr for glm fixes. closes #6397 (#6402) [commit](https://github.com/openframeworks/openFrameworks/commit/97213fbf18e4c12e2f5c5da2487b8d95c0e7fc8f)
- bugfix glm regression when not passing argument to constructor. closes #6397 (#6409) [commit](https://github.com/openframeworks/openFrameworks/commit/04d05af01c650041c13e0eb95439b5914ad063ea)
- bugfix:missing glm initialization (#6430) [commit](https://github.com/openframeworks/openFrameworks/commit/5134ed813d3dc400fd937ced9b66d56d26268605)
- add default member initialiser for position (#6444) [commit](https://github.com/openframeworks/openFrameworks/commit/b1c6ab88b3d937cf836f26ee7dc3c40dd69d32f5)
- bugfix. fixes errors if legacy vector math is enabled. closes #6463 (#6468) [commit](https://github.com/openframeworks/openFrameworks/commit/241131dbf0445b87e0253586d9e840de0518488b)
- added GLM define to default to intiailizing objects with good values (#6450) [commit](https://github.com/openframeworks/openFrameworks/commit/a9e962c6d8ba9496784596e4c84038351145b798)

### sound
- ofSoundBuffer: copying tick count and device id when calling copyTo (#6321) [commit](https://github.com/openframeworks/openFrameworks/commit/79dbcfb014e2f6584e5118ddecc4c6521ea9b2fc)
- fix buffer overflow in linear interpolation of audio buffer with loop (#6217) [commit](https://github.com/openframeworks/openFrameworks/commit/037ee2fa8c3348105d6435b263e5ae0f462a440c)
- ofSoundBuffer getChannel fix (#6117) [commit](https://github.com/openframeworks/openFrameworks/commit/5a2cf1e3012aa8d756a8d427eb87c01a28ab0639)
- ofSoundBuffer::copy from was not being set properly (#6312) [commit](https://github.com/openframeworks/openFrameworks/commit/88d2f9e0d7fd7f088545146c49c37b7bccca8e30)

### utils
- ofXML - provided access to the underlying pugi::xml_node method "getParent()"
- ofTime: fix overflow when adding more than 2 seconds [commit](https://github.com/openframeworks/openFrameworks/commit/f7889ef087f629b77165b5d15f6600a6b58422b0)

### video 
- set speed, frame/position when beeing ready to play [commit](https://github.com/openframeworks/openFrameworks/commit/b206dddc41ea62691d1f5c38551685ea515188d7)
- set frame/positionBeforeRead to 0 on load [commit](https://github.com/openframeworks/openFrameworks/commit/acfe5243cb7f2007dfe3a2fb8aa7a2754b30b3a6)
- add init for grabber was not init. (#6405) [commit](https://github.com/openframeworks/openFrameworks/commit/ab3a139e7bac7a976109e4c8063ad69f762b6abb)

PLATFORM/IDE SPECIFIC
-----------------
### android
- Improvements in life cycle of Android Activity. (#6291) [commit](https://github.com/openframeworks/openFrameworks/commit/9af30346120a10c6de2644873da6f8bb3afeb963)
- Android fixes (#6446) [commit](https://github.com/openframeworks/openFrameworks/commit/5f191ccefcedfbb0992d582cffb5772de4e3f6da)
- Update the android ndk version to 21. (#6318) [commit](https://github.com/openframeworks/openFrameworks/commit/97b291e7ae07c1ca96e34976f5b94e90413eab67)

### emscripten
- emscripten: fix problems with latest versions [commit](https://github.com/openframeworks/openFrameworks/commit/c070afcb319565fb2fb630a89362702d39af5cf3)
- ofxEmscripten: audio context start after reloading page (#6233) [commit](https://github.com/openframeworks/openFrameworks/commit/b3ddff73dd3882066a9c5fa22a776a0196a4d06b)
- ofVbo: emscripten: just set vao support to false instead of trying to dynamic load [commit](https://github.com/openframeworks/openFrameworks/commit/bc87525feedde92dae5f27909f7fe7316f9d26a0)
- Emscripten ci docker (#6260) [commit](https://github.com/openframeworks/openFrameworks/commit/e6e4ee1da88f507f36bad503cd329b31eaadccac)
- Fix Uncaught TypeError: Runtime.dynCall (#6347) [commit](https://github.com/openframeworks/openFrameworks/commit/01c24ccbf793bcd0a0e24bbcfda5f0a7e5b407b7)


### ios 
- add multitouch settings to iOS template (#6210) [commit](https://github.com/openframeworks/openFrameworks/commit/bd6eb1dcd35fe246ebf1314d81a86ff46e465494)
- ofxiOS: fix FBO for GLK view (#6248) [commit](https://github.com/openframeworks/openFrameworks/commit/4ca4f6e47c725d79b9e461e9fdd99dc843612145)
- bugfix fixes reversed touches when not using hardware orientation. closes #6317 (#6363) [commit](https://github.com/openframeworks/openFrameworks/commit/95ccc98e52feea267f0c43229e969158687fedbc)
- bugfix changes iOS template to allow simulator/device linking.  (#6364) [commit](https://github.com/openframeworks/openFrameworks/commit/04579e74e17fe27b95356fb99f77a7cfca0cb64c)
- fix: can not make fullscreen on external display. (#6406) [commit](https://github.com/openframeworks/openFrameworks/commit/6079447ba2ca3508ca47214019fa5b6bdb80738f)


### linux 
- better install_dependencies support for linuxmint. (#6263) [commit](https://github.com/openframeworks/openFrameworks/commit/048c18827b37201f93c7f9897b2b86c4bf7f31e6)
- Add jack-audio-connection-kit-dbus as Fedora dep (#6213) [commit](https://github.com/openframeworks/openFrameworks/commit/9fb1b833a0a9647c5150c5883c48792fbf3be8d0)
- Linux: Default to poco from system [commit](https://github.com/openframeworks/openFrameworks/commit/e157173464d79cd604b0edb41764074461734190)
- Added missing ROOT variable to fedora install_dependencies.sh (#6286) [commit](https://github.com/openframeworks/openFrameworks/commit/e559f15d878ae9e79f19bc52074e2be4c3f008b0)
- Update install_dependencies.sh Ubuntu libgconf-2-4 (#6477) [commit](https://github.com/openframeworks/openFrameworks/commit/684fb37c6446f02d0289aa91da791c6836286550)

### linuxarm 
- ofAppEGLWindow: Add control of udev input device attached/detached (#6215) [commit](https://github.com/openframeworks/openFrameworks/commit/0a834f7163f16a910616db65959805e8d98f895e)
- Update buildAllRPIExamples.sh [commit](https://github.com/openframeworks/openFrameworks/commit/5d43b23a4453ab92fb5adeedb3387f28b64b7282)
- supports the Rpi 4 with EGL and GLFW. Closes #6436 (#6438) [commit](https://github.com/openframeworks/openFrameworks/commit/261af064d764e4138bfb0aead93196d7ffc5c4b6)
- fix tabs and spaces with rpi makefiles. change permissions for install_dependcies.sh (#6455) [commit](https://github.com/openframeworks/openFrameworks/commit/20cb26d2d285923ef7045188adfc7840971e6c5b)
- bugfix fixes fullscreen issues on RPI. closes #6445 (#6456) [commit](https://github.com/openframeworks/openFrameworks/commit/637dbe8a0ab569d3f9b13cd2acb2fe245d09c489)

### makefiles
- makefiles: addon data not being copied(#2903).
- bugfix #2903 Addon data copy (#6244) [commit](https://github.com/openframeworks/openFrameworks/commit/9baea23a9a635270ff4e08282dfdcb4b62175e31)
- Fix makefile errors when MAKEFILE_DEBUG is defined. (#6304) [commit](https://github.com/openframeworks/openFrameworks/commit/e71fc7449c3efe7aa2c4a692c99a19679f43cfaf)
- Fix Makefile Verbosity level (#6307) [commit](https://github.com/openframeworks/openFrameworks/commit/a7564f242a62cf62bb691be5d64f075d2d092e1e)
- replace erroneous tabs in makefiles (#6229) [commit](https://github.com/openframeworks/openFrameworks/commit/da2b6a8bb219e9df8be37b5830634fc7f779a4fb)

### macos
- bugfix. adds 32bit stripping to AppStore configuration for libfmodex.dylib (#6475) [commit](https://github.com/openframeworks/openFrameworks/commit/d443178e27ea56ec41fd984ca277b045b7e57dbb)
- Fix case of rtaudio.a (#6184) [commit](https://github.com/openframeworks/openFrameworks/commit/83c54daf20506177c26f8ac38ece7c2a84a41991)
- bugfix for mojave permissions issues with camera. closes #6193 [commit](https://github.com/openframeworks/openFrameworks/commit/b54280745941cc394bd5edcb9fcda92be66439ac)
- bugfix AppStore configuration for osx - strips 32bit from dylibs and frameworks. closes #6358 (#6365) [commit](https://github.com/openframeworks/openFrameworks/commit/b9a4881ab5008f4896342167a7c7a9b49220357e)
- added flag to xcode template to fix code signing errors. Closes #6383 (#6390) [commit](https://github.com/openframeworks/openFrameworks/commit/fa63aa0cb8889d71c51cb157456bb99091b6ac73)
- bugfix. fixes error with Xcode build due to depracted function error. (#6394) [commit](https://github.com/openframeworks/openFrameworks/commit/2cae97c104b8e34c145e3d075312d89d1394f097)


### msys2
- Mingw64 support (#6413) [commit](https://github.com/openframeworks/openFrameworks/commit/a9eadffb5d28191e99411823bbbb08306584c8b9)
- MSYS2 installation and compilation enhancements (#6408) [commit](https://github.com/openframeworks/openFrameworks/commit/3884d4931d15919573fc46077a4f6807552fdc6b)
- add 64 bits support.
- setup : removed automatic setup of PATH environment variable(#5740). Move instructions to setup PATH to documentation.
- documentation : fix typo (#6211) and insist on the use of MINGW32 shell. 
- fixed 'copy_dlls' target to work woth both Debug and Release executables.
- define APPNAME earlier to fix msys2 copy dlls failing (#6166) [commit](https://github.com/openframeworks/openFrameworks/commit/751842afec6b871bea8d75c52faa12c6bfee5d57)
- MSYS2 : Remove setup of PATH env var in install_dependencies (#6349) [commit](https://github.com/openframeworks/openFrameworks/commit/ec9ba1508debe3872173446b06d6adb2a3f67e35)
- Msys2 icon (#6415) [commit](https://github.com/openframeworks/openFrameworks/commit/2da5c6723fdcdfccf2c3a6391badb8f7557c8076)
- improved msys2 install detection (#6417) [commit](https://github.com/openframeworks/openFrameworks/commit/d48d1422750751ece906880be85de1ba155f85cd)
- MSYS2 : Copy dlls (#6416) [commit](https://github.com/openframeworks/openFrameworks/commit/73cb6bfa7bf60a414087feaeacadf12e6a826425)

### opengles
- fix shaders examples for OpenGL ES (#6223) [commit](https://github.com/openframeworks/openFrameworks/commit/8db8e45fccbc886f6749d45c5021f129d350f232)

### qtcreator
- Correctly parse escaped linker flags in addons for QTCreator. (#6323) [commit](https://github.com/openframeworks/openFrameworks/commit/76deadbae534d7d573318866922310cfae07da53)
- bugfix for qtcreator wizard. closes #6461 (#6462) [commit](https://github.com/openframeworks/openFrameworks/commit/8d8c8ff5280f7140d5bf2cdfda08ad90f552383d)
- bugfix fixes dylibs not working in qtcreator if consoleApplication is true.  (#6458) [commit](https://github.com/openframeworks/openFrameworks/commit/2e573e1a73cf3e26c2a0051627dfae4c82393797)
- not prefixing frameworks with the global addons path (#6424) [commit](https://github.com/openframeworks/openFrameworks/commit/fe1f85b3a6b5536e4f24e9c132f9a878c8aedf9e)

### vs 
- no console on windows for release targets (#6242) [commit](https://github.com/openframeworks/openFrameworks/commit/1e81b3f92ac080445fb9a6295259b791d75872b3)

### vscode 
- add Makefile to /templates/vscode (Windows PG does not copy Makefile by default) (#6369) [commit](https://github.com/openframeworks/openFrameworks/commit/88fe84c753854ab3a1f5004eb87071812df58cac)


LIBS
----
- glm updated to 0.9.9.6. NOTE: all glm types are not intitialized by default now. Do glm::vec3(0) instead of glm::vec3() or set this define in ofConstants.h to default to the old initializtion method: `#define GLM_FORCE_CTOR_INIT 1`
- Fix. Added openCv C++ headers. (#6222) [commit](https://github.com/openframeworks/openFrameworks/commit/eaf2883f81f972a12214d74807e2f63c6a64fc1b)
- Opencv4. windows, osx, ios, android and emscripten (#6256) [commit](https://github.com/openframeworks/openFrameworks/commit/021e1a236a98d0b289c57b8262eb5bbda36e6a38)
- prepare glm 0.9.9 branch (#6338) [commit](https://github.com/openframeworks/openFrameworks/commit/5fa86b5ca63119e8ae239208bcf7ff67069c48a4)

ADDONS
------
### ofxOpenCv
- added support for OpenCV4 : deprecated C functions replaced by their C++ counterpart. Also fix issue due to incorrect pkg-config package [commit](https://github.com/openframeworks/openFrameworks/commit/)
- Check for !bAllocated on assignment of ofxCvImage (fixes #6115) (#6116) [commit](https://github.com/openframeworks/openFrameworks/commit/cfc7d2dca79d5f037cc046423d31be81dcf4e963)
- opencv4 support for msys2 and all linux platforms where available (#6252) [commit](https://github.com/openframeworks/openFrameworks/commit/529ffd82b50bf7f94440de0806cc10d53678a487)
- bugfix findHaarObjects. fixes constant push back to blobs every frame. (#6432) [commit](https://github.com/openframeworks/openFrameworks/commit/b479caefd79b7e79104a3cd78429143ec8666678)

### ofxSVG
- added support for the SVG "use" command, a common feature that allows multiple instances of the same graphic elements. Also fixed an issue where line weights less that 1 unit would not render. This increased compatibility can be disabled if required by using the ofxSVG::setImprovedCompatibilityMode(bool mode) method. [commit](https://github.com/openframeworks/openFrameworks/commit/)
- ofxSvg improved compatibility (#6303) [commit](https://github.com/openframeworks/openFrameworks/commit/1c1e15a5ecdfe9e7179ee349fbf1f59f227af31d)

### ofxOsc
- ofxOscSender.setup() fix (#6287) [commit](https://github.com/openframeworks/openFrameworks/commit/8e70657729071c20b32dec480b6d16b96158a4f3)

### ofxGui
- Added check to update header color when color is changed from external source (fixes #6381)
- update text color to changing bg colour to ofxGui as well (#6440) [commit](https://github.com/openframeworks/openFrameworks/commit/6a3c359d5820eea2e40843796494a1135ac37f89)
- ofxGui: Changed instances of ofVec*f and ofPoint into GLM (#6207) [commit](https://github.com/openframeworks/openFrameworks/commit/1ad102264926a70cae18e6152106bf689c5c000d)
- Fix ofx gui events propagation (#6203) [commit](https://github.com/openframeworks/openFrameworks/commit/9e8f1499cc86e86104ccae29a8b17c8867c5a795)
- update method for drawing slider bars (#6297) [commit](https://github.com/openframeworks/openFrameworks/commit/35ff092d643948c204600c112061fc215f0fa0e0)
- Feature ofxRectangleSlider (#5793) [commit](https://github.com/openframeworks/openFrameworks/commit/332c926808380d904eba79d3ad19af11cc6a3be0)
- ofxGui: slider scrolling (#6144) [commit](https://github.com/openframeworks/openFrameworks/commit/9dd4249eb4419d8d9ea2715453aca07d4169f906)
- ofxGui scaled for retina displays + group header enable/disable feature  (#6179) [commit](https://github.com/openframeworks/openFrameworks/commit/8ad1fbb26bc88df40f0769830a92baaa0d2b30dd)
- fixed ofxColorPicker glitch (#6426) [commit](https://github.com/openframeworks/openFrameworks/commit/627b9aa2c09dc884ae60752e6069073fc6eee85b)
- Fixes #6381 based on @LYHSH code snippet (#6423) [commit](https://github.com/openframeworks/openFrameworks/commit/e911fdcdceba52282522bbc9db50fae73d45b99d)

### ofxNetwork
- Bugfix ofx tcp manager (#6201) [commit](https://github.com/openframeworks/openFrameworks/commit/0bf04a4833cc0e9480d2d8fa2580c451a8a81075)
- Add SO_REUSEADDR option to ofxTCPManager and ofxTCPServer. (#5011) [commit](https://github.com/openframeworks/openFrameworks/commit/d5678cd38962c7f285f20bcabc9b2c4e82b926a8)
- add missing explicit cast [commit](https://github.com/openframeworks/openFrameworks/commit/09ccd34d4007c51b9f08d85ff472227887e1d060)


CI 
----
- appveyor: fix ssl dependencies (#6170) [commit](https://github.com/openframeworks/openFrameworks/commit/eacb4a33446c9e1d070100b987feac96b2cdec0a)
- create_package: pull OF from github instead of local [commit](https://github.com/openframeworks/openFrameworks/commit/6257aa613988de87486cf0eb04102aa911f8f558)
- linuxarmv7: fix ci install phase [commit](https://github.com/openframeworks/openFrameworks/commit/a21b974d25e05cce51787d8e5ed66f07891d753b)
- linuxarmv7: fix install ci [commit](https://github.com/openframeworks/openFrameworks/commit/7554926d375e35d86dc84b46d6d26e7b48364601)
- travis: fix ccache install in linux tests [commit](https://github.com/openframeworks/openFrameworks/commit/24046a448488e5cb3e88d7e6b949170054da52c5)
- fix cache install in ubuntu [commit](https://github.com/openframeworks/openFrameworks/commit/64bc349bac691013e5260cb4d435f8714552482a)
- travis: fixing ccache install in linux [commit](https://github.com/openframeworks/openFrameworks/commit/33f16b0d2511532588fce1df322c64138bef70e2)
- travis: fixing ccache [commit](https://github.com/openframeworks/openFrameworks/commit/67076ab191a95162cdb882a94fdbe2355fa37e19)
- linuxarmv7: add -lm since tests are now failing on some missing math symbols [commit](https://github.com/openframeworks/openFrameworks/commit/19c454ad6b7fd3e3e3746d9665e86b0ed2b73e5a)
- Trying to fix linuxarmv7 ci [commit](https://github.com/openframeworks/openFrameworks/commit/84a7146ac5a085c66bd205d61da1d82ffbb1fa18)
- Upgrade MSYS2 packages in appveyor (including GCC) (#6331) [commit](https://github.com/openframeworks/openFrameworks/commit/de51b0ef21d56604fe1b09271e48f679c9f298f5)
- Bugfix - last commit hash written before build successful. (#6355) [commit](https://github.com/openframeworks/openFrameworks/commit/99e2741acc978e3642135ef6a251ad83271b71f1)
- bugfix changes the create_package script to call the project generator executable generated by DebugNoOF [commit](https://github.com/openframeworks/openFrameworks/commit/18ef244a5e023bfee3cb572fe3067f3ba1e0eb07)
- bugfix needs to cd back to the dev/scripts folder to write lasthash.txt to the correct location (#6359) [commit](https://github.com/openframeworks/openFrameworks/commit/561a298524de15261d77d966edd7ef7be53e0fa4)
- add maven url to comply with v4:25 (#6373) [commit](https://github.com/openframeworks/openFrameworks/commit/b34f9a936badb4b9667daaa90d16e719fab8eb03)
- download_libs: add double quotes to allow for folder names with spaces (#6374) [commit](https://github.com/openframeworks/openFrameworks/commit/63411dbbdf3b6e32b43fa02a60068f343a3ad991)
- Readme ci status per platform (#6391) [commit](https://github.com/openframeworks/openFrameworks/commit/34a96307dbabdc2b3d6e7a7fc7f56f67fce24d50)
- fixes junest for linuxarmv7 -u argument isnt valid anymore (#6422) [commit](https://github.com/openframeworks/openFrameworks/commit/ce68a112ab8b2c96a1ffdb95124f5042e82fc1ba)


PROJECT GENERATOR 
----
- update template.config files (#6368) [commit](https://github.com/openframeworks/openFrameworks/commit/25aa922ca107b912500d3a8a74d1ef24a7770b86)
- Feature add vscode template (#6298) [commit](https://github.com/openframeworks/openFrameworks/commit/63de87fbbc616a3c549b76d8c0851fbb26a3d0a0)
- add props support (#196) [commit](https://github.com/openframeworks/projectGenerator/commit/31a0b6fca8807bd7fb4165bf758f6ba4583ed087)
- ignores comments in addons.make after an addon, [commit](https://github.com/openframeworks/projectGenerator/commit/c8ddfe81542a5d50853b8694adde3c033f36a17d)
- Typo on string #31? (#200) [commit](https://github.com/openframeworks/projectGenerator/commit/7ed23dccf1d44aa3f90539e49581e3d441aca831)
- Search Addons for partial match & update semantic.min.js (#201) [commit](https://github.com/openframeworks/projectGenerator/commit/3e6223b4971847cb39e7cdc2800c5297da0ee03f)
- fix Windows folder drop feature (#207) [commit](https://github.com/openframeworks/projectGenerator/commit/6b36107cd55046478a8cae60d362b6bb0d5d48c8)
- Support ADDON_DATA in addon_config.mk (#190) [commit](https://github.com/openframeworks/projectGenerator/commit/96d97bbfb38b8b532582f1459ea4762ca7a87ffe)
- fix verbose logging about defines on xcodeProject (#179) [commit](https://github.com/openframeworks/projectGenerator/commit/e11781ecd6af97f37187a39017587cb01c8280bd)
- fix: call ofGetElapsedTimef after initutils() (#178) [commit](https://github.com/openframeworks/projectGenerator/commit/f6ba22506d03230e2f3270a431a0262bea968020)
- Feature show hidden folder (#205) [commit](https://github.com/openframeworks/projectGenerator/commit/50a0ee2aaf70199c80e75c5e3b67501e55579367)
- Feature add template dropdown to frontend PG (#206) [commit](https://github.com/openframeworks/projectGenerator/commit/550d7467b6391361b30cd549ce5daf1b486c807f)
- update shrinkwrap from Windows (#219) [commit](https://github.com/openframeworks/projectGenerator/commit/fe3a2b8a26a36718009a6014d7b52e55e312e2d8)
- adds _runMeFirst.command to macOS packages to escape translocation. closes #220 [commit](https://github.com/openframeworks/projectGenerator/commit/672bb754c352de8b307d0661281d13e140e37e9a)
- 10.12 translocation message instructions [commit](https://github.com/openframeworks/projectGenerator/commit/6b47f0869e012b19fd1d40c4b8fa1684141541b4)
- added the -r flag for xattr quarantine closes #220 [commit](https://github.com/openframeworks/projectGenerator/commit/0c4d02f932807639b1b0fd379b87067817304d82)
- change node version in package.json to 6.11.0 [commit](https://github.com/openframeworks/projectGenerator/commit/efd18d7fe4b7d849331a34b3fbf17c34a982c6d9)
- fix for linux not finding pg binary. closes #209 (#227) [commit](https://github.com/openframeworks/projectGenerator/commit/5f9f6255e1ef8933ccb213d5f039dc15eb2a937c)

DOCUMENTATION 
----
- fixed documentation [commit](https://github.com/openframeworks/openFrameworks/commit/e7abdd8757603dcc27d4eb9dcd8fa359a4a40c57)
- Add inline documentation for ofBaseGLRenderer [commit](https://github.com/openframeworks/openFrameworks/commit/0ec7384239e90ee024aab23cb44c039c06edb0e0)
- Fix params doxygen error [commit](https://github.com/openframeworks/openFrameworks/commit/2659dca26faa607484aaf76617a326f2f4ce7f0f)
- THANKS.md [commit](https://github.com/openframeworks/openFrameworks/commit/0607a720c846dd912f09035b1e5358a4e9397014)
- fixed thanks format [commit](https://github.com/openframeworks/openFrameworks/commit/01e2e2fafed89be7b18b120abb6c5c5dcd30ba8a)
- changelog and thanks scripts [commit](https://github.com/openframeworks/openFrameworks/commit/faeb05d54647915464dcf2bf23ee9da03d8abcd5)
- ubuntu install dependencies: add libraries needed by the project generator [commit](https://github.com/openframeworks/openFrameworks/commit/39938489c3d2511f781ff386282d1deb90fa7029)
- [Docs] - openframeworks/openframeworks - openframeworks/CONTRIBUTING.md - minor tweaks [grammar, etc al] (#5846) [commit](https://github.com/openframeworks/openFrameworks/commit/2cee86a1e0ead1328dd509e75d5f2df327e64b73)
- A collection of fixes for invalid doxygen comments. (#5799) [commit](https://github.com/openframeworks/openFrameworks/commit/27c513c3fcd7d25294a25a20b806152f987d3fdf)
- Update CONTRIBUTING.md [commit](https://github.com/openframeworks/openFrameworks/commit/60281fbe0b4672025a4c54b0c8e3f1242a7c806a)
- Broken Link Fix (#6261) [commit](https://github.com/openframeworks/openFrameworks/commit/042352b8d9995b58ad546f0f11584393564cbde9)
- Fix url for issue tracker (#6275) [commit](https://github.com/openframeworks/openFrameworks/commit/35657c2a8c4715e1121dbe40df7ffc88785d828a)
- Fix typos (#6324) [commit](https://github.com/openframeworks/openFrameworks/commit/68eb8a7333f2d301268a41028ceafdd2f0d7d894)
- Fix doc (#6371) [commit](https://github.com/openframeworks/openFrameworks/commit/fa81f54a7e42369beccedd0dc8efc361e15af03d)
- Re-merge spaces. [commit](https://github.com/openframeworks/openFrameworks/commit/9245e4ea81f3d903780d64ac4fd8281210d6eae7)
- @digitalcoleman 's work! [commit](https://github.com/openframeworks/openFrameworks/commit/eed001c5f2033debb68359c1f2205285c06c6d0b)
- Whitespace. [commit](https://github.com/openframeworks/openFrameworks/commit/39a4ba0b3838c4350b676364a1234c6bb73c989d)
- Whitespace, punctuation and removing docs on deprecated functions. [commit](https://github.com/openframeworks/openFrameworks/commit/3ef22bb3429c24ae246036fc9127f4bf91af405e)
- Capitalization. [commit](https://github.com/openframeworks/openFrameworks/commit/28cbbab2167a269d3bd320c37543ccfb73b3c630)
- Whitespace. [commit](https://github.com/openframeworks/openFrameworks/commit/d57cd8046bb7f0360fb752657bbe32068402e0fe)
- Update main.cpp (#6037) [commit](https://github.com/openframeworks/openFrameworks/commit/b5a45ef4262094074acaaa800b8200e10539f862)
- fix: Fix the indent (#6281) [commit](https://github.com/openframeworks/openFrameworks/commit/6cce0616c9095de37408f05e1ba149d0246c083a)
- Docs of3d graphics (#6385) [commit](https://github.com/openframeworks/openFrameworks/commit/162795a1563c6c022e27c8a827d49ea5a7cb07aa)
- Add inline documentation to UTF8 string functions. [commit](https://github.com/openframeworks/openFrameworks/commit/7fff51bf799de8ca85100b4251af578ef0a8aba7)
- Add inline documentation of ofUTF8Iterator. [commit](https://github.com/openframeworks/openFrameworks/commit/fb4f33c2fd5b78577ee81dec64f9b4b673342bd1)
- Documentation spacing. [commit](https://github.com/openframeworks/openFrameworks/commit/4da8cd19e35df80b294dd3137466efc622a958a0)
- INSTALL_FROM_GITHUB: Include how-to-update section (#6474) [commit](https://github.com/openframeworks/openFrameworks/commit/fcad6b0fab790fbaa27353e858b84d0d4fff6b70)


--------------------------------------

```
  .----.                  .---.  .----.                  .---.
 /  ..  \                /_   | /  ..  \                /_   |
.  /  \  .                |   |.  /  \  .                |   |
|  |  '  |                |   ||  |  '  |                |   |
'  \  /  '                |   |'  \  /  '                |   |
 \  `'  /       .-.       |   | \  `'  /       .-.       |   |
  `---''        `-'       `---'  `---''        `-'       `---'
```

CORE
----

### 3d
- Fix order of quaternion multiplication in ofNode. [commit](https://github.com/openframeworks/openFrameworks/commit/e4e9bfca751de8960151faa182f070238ebaea5c)
- Fixing LookAt method on ofNode (#6069). [commit](https://github.com/openframeworks/openFrameworks/commit/62949aeb689b6859185f6318d45fcbff9214ffb3)
- Bugfix ofnode orientation with scale (#6093). [commit](https://github.com/openframeworks/openFrameworks/commit/17c224684d6395d54fc7c5e0238eee6a9333bc27)
- More efficient way for ofNode::getGlobalOrientation (3x~ faster) (#6094). [commit](https://github.com/openframeworks/openFrameworks/commit/a0c412aab740cfbd2f2221efe40e3bedd38cf6df)
- ofCamera with ortho enabled will respect a lensOffset by translating the projection matrix (#6138). [commit](https://github.com/openframeworks/openFrameworks/commit/c1545bc4dd5da0b0327a4c994ca1bc27294dbca8)
- fix drawArrow internal matrix calculation (#6164). [commit](https://github.com/openframeworks/openFrameworks/commit/cf08ca3857776f6e776fc83233202b2b05575865)

### gl
- Fix error message legibility. (#6013). [commit](https://github.com/openframeworks/openFrameworks/commit/58e47e329d4a2717b5211755a1cb2e65a1a5e1a9)
- ofGetImageTypeFromGLType: fix missing GL_R8. [commit](https://github.com/openframeworks/openFrameworks/commit/ec20a865c8d07ec473ff93a96659c2972ee1e5d8)

### types
- ofParameterGroup: fix some issues with parameter removal. [commit](https://github.com/openframeworks/openFrameworks/commit/bad923d37dc67fc97c47cca07f79d389833aaa35)
- ofParameterGroup: when removing use escaped name to lookup parameter. [commit](https://github.com/openframeworks/openFrameworks/commit/7a868842bf56730b9f3ff054704ff148523fb4c9)
- ofParameterGroup: used escaped name to delete. [commit](https://github.com/openframeworks/openFrameworks/commit/6f80d19d1ce563bca4786d14c801bcc02047ad09)

### app
- ofAppGLFWWindow: close as private. [commit](https://github.com/openframeworks/openFrameworks/commit/bca934a72a962930066f19b5d904ddbc159fc13a)
- ofAppGLFWWindow: query screen size to set w/h in setup on fullscreen. [commit](https://github.com/openframeworks/openFrameworks/commit/fea8b9a63dde8889a7f4236036777d4199c3f516)
- ofAppGLFWWindow: use ofGetWindowSize() when storing current window rect as windowW & windowH may already reflect the fullscreen size, fixes #6074 (#6092). [commit](https://github.com/openframeworks/openFrameworks/commit/2e7f9e99dcd97f4df4aa4e709374242395fd8b3f)
- ofAppGLFWWindow: refactor & fix size on fullscreen + position. [commit](https://github.com/openframeworks/openFrameworks/commit/6b4b68a14fe4bb5fae70ac575eef501eeb8f8def)

### graphics
- ofTTF: add latin and latinA ranges to latin alphabet. [commit](https://github.com/openframeworks/openFrameworks/commit/707541894ac1437cf73a4f4b5dc943a1e5beac59)
- ofPixels: Fixed int division that needs to be floating point (#6033). [commit](https://github.com/openframeworks/openFrameworks/commit/9d8bca997f74628c49f9b0170e5537480335d6c8)
- ofTTF: fix custom letter space. [commit](https://github.com/openframeworks/openFrameworks/commit/d9d7bc58ce6e882b1052b6090c438623c45d625b)
- ofPixels.h: CLAMP -> ofClamp and include ofMath. [commit](https://github.com/openframeworks/openFrameworks/commit/5b6dc65c851b4b8ca1f72a87206a575bc7954009)
- ofTtf: Fix some problems with outlines. [commit](https://github.com/openframeworks/openFrameworks/commit/1ddd2a01fd8b5732a3393a1233adf3ae68c8d3b5)
- Speed up image loading (#6035). [commit](https://github.com/openframeworks/openFrameworks/commit/4b03c1005cd77c0e19d555c41e9e4f364785e351)
- Fix ttf font bounding box calculations (#6109). [commit](https://github.com/openframeworks/openFrameworks/commit/12c99122a62a52929c10375450ca19332830e0fc)
- Fix ttf kerning factor (#6136). [commit](https://github.com/openframeworks/openFrameworks/commit/e192b470b5d0e0a929db348f800d5185e7cef9bb)

### sound
- ofSoundBuffer getChannel fix (#6117). [commit](https://github.com/openframeworks/openFrameworks/commit/d9e3a30715d15af2520d1467c9a4097249c1216b)
-
### video
- Fix for ofAVFoundationGrabber.mm setDevice(...) after close(). [commit](https://github.com/openframeworks/openFrameworks/commit/58ea2b8dd512d77ba15b147c227e24d6bd75aeb4)

### utils
- fix const about `ofSerialize` in ofJson.h (#6096). [commit](https://github.com/openframeworks/openFrameworks/commit/af3d07cd51a6bd4d647fc552a37f1f3f595d1b09)
- ofXml: add methods to remove child through a reference or move. [commit](https://github.com/openframeworks/openFrameworks/commit/a7b4b108031aaef09ab058577ca6036aa967a349)
- ofXml: add missing methods to remove attributes. [commit](https://github.com/openframeworks/openFrameworks/commit/e1cf97c2b8f8c5815c25b93b80044256713a1cbe)
- ofXml::Attribute: add getName. [commit](https://github.com/openframeworks/openFrameworks/commit/333ce0d359eb9b7f2f0e7c4790c2a2fb421a9c31)
- ofXml: fix attribute iterator. [commit](https://github.com/openframeworks/openFrameworks/commit/d06e4354dd3daaa3c1535fd72e63c7f9e5eb90b2)
- ofXml: add clear method which clears the xml doc. [commit](https://github.com/openframeworks/openFrameworks/commit/83dc9c5b495e265928aaed7454eca9826b1d4052)
- ofConstants: version to 0.10.1 stable

ADDONS
------
### ofxOpenCv
- ofxCvImage: remove old macro not present on some versions.. [commit](https://github.com/openframeworks/openFrameworks/commit/84a8328f1be1387289de6e029fd323ead9b7a445)

### ofxGui
- Bugfix ofxSliderGroup division by zero (#6034). [commit](https://github.com/openframeworks/openFrameworks/commit/659cc30ef21bc9b183b724214c8168ee1c2f2f7e)
- ofxGuiGroup: fix ofVec*f / glm::vec*. [commit](https://github.com/openframeworks/openFrameworks/commit/668c62ff2276681147804dd6f2a5b4d279ba2a65)
- ofxGui: slider scrolling (#6144). [commit](https://github.com/openframeworks/openFrameworks/commit/50faea3f9c0d4163b0b3c5778e2a36b4eee163a4)

### ofxKinect
- update addons/.gitignore to ignore libs in ofxKinect (#6030). [commit](https://github.com/openframeworks/openFrameworks/commit/678963e345d50dcc1f7a9928afaadde5009bb5b6)
- update libfreenect to fix ofxKinect on raspberry pi (#6123). [commit](https://github.com/openframeworks/openFrameworks/commit/1b71506ccd26eb91b3ac50f04217e77c50ac7420)

### ofxOsc
- ofxOsc: add const for getter method returns constant. (#6018). [commit](https://github.com/openframeworks/openFrameworks/commit/c7bb09cfeac2204a6aa1f897a23d7e64afefc28f)

PLATFORM/IDE SPECIFIC
-----------------

### qtcreator
- fix qbs project file for commandline PG (#6010). [commit](https://github.com/openframeworks/openFrameworks/commit/b5ee15a7a15e160ed633addebf157147a992b3e5)
- fix linux project templates for QtCreator (#6011). [commit](https://github.com/openframeworks/openFrameworks/commit/fbcf32bdde6e37da664abb3b482351ffef8c4377)
- Add option to create console window in QtCreator wizards (#6105). [commit](https://github.com/openframeworks/openFrameworks/commit/6127e9fb81520f27996265aea704255cc17748f8)

### linux
- linux: if ubuntu 18.04 or more install poco on the system. [commit](https://github.com/openframeworks/openFrameworks/commit/84077013d8a9c482696af18777a4e6976afe970c)
- Fix Arch Linux install_dependencies.sh (#6051). [commit](https://github.com/openframeworks/openFrameworks/commit/cd4529f1db013b627fa84602c9e274bcb1e98e58)

### makefiles
- makefiles: only add src as a source folder. [commit](https://github.com/openframeworks/openFrameworks/commit/ce77dae31ef9553f4b61602d5a41985ca1b70417)

### android
- Fixed selection of GLES version on android. (#6088). [commit](https://github.com/openframeworks/openFrameworks/commit/0f9cdc2b54a8c42f0a16632ca8c0a12aa6b9f880)
- android multi activity example: add manifest (#6036). [commit](https://github.com/openframeworks/openFrameworks/commit/31ca50677fe466178f92ed335acab8993506c8ba)
- ofxAndroidGrabber: when changing camera ID close device if it was open. [commit](https://github.com/openframeworks/openFrameworks/commit/347f43765ff37a044c0ec0f3cd1793433cbbba81)
- Android camera optimizations (#6097). [commit](https://github.com/openframeworks/openFrameworks/commit/4c2322187b646fd49352ca223ee8a249b9e7ec47)

### osx
- use correct macOS key event modifier masks, fixes #6070 (#6087). [commit](https://github.com/openframeworks/openFrameworks/commit/b3a250a75c446347cf9bf9ab6018c83c55b6ad58)
- Fix compilation on Xcode 10 (#6131). [commit](https://github.com/openframeworks/openFrameworks/commit/da6929bf6dc529a1082224a7db249e8a073b2361)
- fixes xcode template to not have OF as a dependcy but as a pre-build script. related to #6139. [commit](https://github.com/openframeworks/openFrameworks/commit/25e91b911cb7b36a57191f96ee717a15207d9c1c)
- remove i386 architecture from macos projects (#6159). [commit](https://github.com/openframeworks/openFrameworks/commit/647954e3ee4a3c1d75416d50eac5390d8541b51c)
- added camera and microphone permissions for mojave. [commit](https://github.com/openframeworks/openFrameworks/commit/d8b6287148d929f0c07f067a417596f2a38f0986)
- workspace files for xcode template to enable legacy build in Xcode 10. [commit](https://github.com/openframeworks/openFrameworks/commit/57f3ea2798057dee286cda89ba37372d8af2d3d5)
- fix for xcode template file to allow PG to add frameworks and fix broken projects. closes #6172. [commit](https://github.com/openframeworks/openFrameworks/commit/99b80ad9c77f9d1d4426cb96d504f9aaf6dc8c95)

### emscripten
- Fixes for html5 audio and video with emscripten (#6039). [commit](https://github.com/openframeworks/openFrameworks/commit/f09a37ddf9fbdd137782ec00463e5b8a7bc6fe9a)

### msys2
- fix copy_dlls make target when there's only a debug build, (#6046). [commit](https://github.com/openframeworks/openFrameworks/commit/861ac3607734461d25d6c9c6d614956fd6ca40ec)

### vs
- Bugfix VS project template (#6122). [commit](https://github.com/openframeworks/openFrameworks/commit/06bea481039ab244eded2fabc450ad4e30585d03)
- Bugfix vs project template (pt.2) (#6130). [commit](https://github.com/openframeworks/openFrameworks/commit/1241140f4d399f8e3f670691f4c274fdc814d8b6)
- Bugfix VS buildscript (#6121). [commit](https://github.com/openframeworks/openFrameworks/commit/e6386bbba4e0aeddacf3ffcdd4982eab6b499ab8)

EXAMPLES
--------
- fix gui parameter group example (#6079). [commit](https://github.com/openframeworks/openFrameworks/commit/6e465e149d725844af2a6697fdc82c4e992cb350)
- fix iOS / tvOS examples broken during docs sprint. [commit](https://github.com/openframeworks/openFrameworks/commit/cec70b09efbf81223e1b5c5d04737b4adaab33f3)
- parameter examples: binding the listener method to the vSync param (#6098). [commit](https://github.com/openframeworks/openFrameworks/commit/76031feebc9f42d366e6667ab657aa9c10107e3b)

DOCS
----
- `ofEnabledDepthTesting()` to `ofEnableDepthTest()` (#6040). [commit](https://github.com/openframeworks/openFrameworks/commit/888d20c70aa8e50bc2bb46bbfb65074e5286c983)


CONTINUOUS INTEGRATION
----------------------
- create_package: change osx -> ios for PG in the ios download. [commit](https://github.com/openframeworks/openFrameworks/commit/53d92ff8483a009e6352fbda77ebf39baceeace2)
- rename macros in ofxUnitTests (#6017). [commit](https://github.com/openframeworks/openFrameworks/commit/86c0e5e37537ce40bcd2ceb466b26cf2f7a68af6)
- parameters: add parameters tests. [commit](https://github.com/openframeworks/openFrameworks/commit/81245d23cdcbb4fba74b8ed16bd06cdc3aa9f239)
- don't cache firmware-master. [commit](https://github.com/openframeworks/openFrameworks/commit/254a22067ba3967816e3c8ed07bf5c83eaffcf7a)
- create_pacakge: remove projectGenerator from apps. [commit](https://github.com/openframeworks/openFrameworks/commit/23647c3fa1fc428d005170c43f87c882ee7f4db3)
- nightlies template: use https for css and fonts. [commit](https://github.com/openframeworks/openFrameworks/commit/66f68819dc7aa74441e5b89d8741ca8daeeb1af2)
- Xcode 9.4 Default Travis (#6104). [commit](https://github.com/openframeworks/openFrameworks/commit/b9f664cfe22d14dc3696177fa6a5a5ebfbb858f3)
- appveyor: fix ssl dependencies (#6170). [commit](https://github.com/openframeworks/openFrameworks/commit/e1aebaceac2d2fb2a6f427f274d9bf2c9ccff5c7)


--------------------------------------


```
  .----.        .---.   .----.         .----.
 /  ..  \      /_   |  /  ..  \       /  ..  \
.  /  \  .      |   | .  /  \  .     .  /  \  .
|  |  '  |      |   | |  |  '  |     |  |  '  |
'  \  /  '      |   | '  \  /  '     '  \  /  '
 \  `'  /  .-.  |   |  \  `'  /  .-.  \  `'  /
  `---''   `-'  `---'   `---''   `-'   `---''
```

CORE
----

### general
    / load* std::string -> std::fs::path
    / no return is an error instead of warning
    - external dependencies moved out of git repositiory
    + added download_libs scripts to download external dependencies
    / optimized includes for faster compilation & autocomplete
    - export folder with shared libs is gone, now live in libs
    / many functions and classes ported to use size_t instead of int for indices and sizes
    - completely removed Poco from core into it's own addon
    / Const corrections
    + implemented move constructor for classes that needed them
    + Add Rad/Deg versions for everything accepting angles
    - Deprecated all functions that accept angles without Deg/Rad suffix
    / remove some unnecessary casts, convert some casts to c++ style, convert some MAX & MIN to std::max & std::min
    / makefiles: filter repeated flags when parsing addons
    / makefiles: better addons parsing
    / qtcreator updated to work with at least 4.6.1
    / qtcreator uses probes to parse filesystem to avoid warnings and increase compile speed
    / qtcreator: option to precompile ofMain.h

### app
    / fixed teardown order
    + added some facilities to create new windows when the app is running
    / several fixes for glfw and main loop
    / main loop: fix logic for destroying app and windows in that order
    / Unregister GLFW callbacks before closing
    / handle negative framerate requests
    + ofMainLoop: notify exit before destroying applications
    + Global time modes (ofSetTimeModeFiltered, ofSetTimeModeSystem, ofSetTimeModeFixedRate)
    / ofAppGLFWWindow: the size reported on setup was wrong in some cases
    / ofAppGLFWWindow: calculate pixel scale even in fullscreen
    / ofAppGLFWWindow: don't reposition window if in fullscren or gamemode
    - ofWindowSettings width and height made private now settable through setSize

### communication
    / Update to newest firmata protocol
    / ofSerial: fixes for firmata servos in linux + better type handling
    + Add 12mb/s communication over ofSerial.cpp
    / ofSerial: only check filenames, not full paths, in buildDeviceList()
    / ofSerial: fix port name vs. path parsing on Windows

### utils
    / moved using std from ofConstants into ofMain so including it is optional
    + ofTime to work with unit independent time
    / ofXml ported to use pugixml instead of poco, old version lives as ofxPocoXml in the ofxPoco addon
    + utf8 utility functions
    / fix for crash on close due to ofThread::waitForThread - fixed by specifying a timeout value
    / several fixes for ofThreadChannel
    + ofThreadChannel: allow multiple consumers (not broadcasting)
    / catch exceptions in ofJsonLoad/Save
    / ofThread uses std::thread instead of Poco::Thread
    + ofURLFileLoader: add basic post support
    / ofURLFileLoader uses curl instead of Poco
    / ofBuffer: faster + reserve, resize and setall
    / ofBuffer: better implementation of ofBuffer without insert
    + templated ofTo that converts string to anything that implements >>
    + add ofGetLogLevel for specific module
    / ofThread: check if joinable before trying to join
    / ofFileUtils: set all binary defaults to true
    / ofFileUtils: setReadOnly deprecated
    + ofFileUtils added setReadable
    + ofFileUtils: added ofFile::create(path)
    / url loader: uniformize api for request handling
    / bugfix for static ofFile and ofDirectory calls not working if data path is set false
    + ofBuffer: add getReversedLines
    + ofURLFileLoader: done callback per request
    + ofURLFileLoader: add timeout to request
    / ofSystemUtils: allow to set the current forlder in linux
    / ofSystemUtils: fix locale changed when opening gtk dialog
    / ofSystemUtils: fix exclusive folder/files selection on osx
    / ofLog: use a function for static channel so it can be used from static variables

### 3d
    + ofCamera::drawFrustum
    / fix box primitive winding order
    / fix ofNode destructor when parented
    / fixed a regression issue in ofNode orbit
    / implement more robust loading of ply meshes
    / ofEasyCam: FIX. fixed drift when moving mouse
    / ofEasyCam: fix y axis for interative rotation for more stable movement
    + ofEasyCam: enable/disable inertia
    + ofEasyCam: up axis setter
    / ofEasyCam: fix unitialized values
    / ofEasyCam: disable inertia by default
    + ofEasyCam: Added custom control area. Defaults to viewport if not set.
    / ofEasyCam Ortho zoom via scroll
    / ofEasyCam: When ortho is enabled the far and near clip are set to -10000 and 10000
    / ofEasyCam: Added code that allows to set  different mouse and key interactions.
    / ofCamera: fixed coordinate system transform functions so these take into account the cameras V flip

### gl
    / fix issue with ofLight segfaulting during app exit
    + added support for transform feedback in ofShader
    + added support for custom behaviour to ofMaterial
    + GL debug logs
    / Make ofTexture::loadData with different glFormat/glType public
    + Added uniform block support to ofShader.
    / Swappd glewGetExtension() by glewIsSupported()
    - ofGLProgrammableRenderer: in GLES remove enable/disable client state
    + ofGLUtils: support for more GL formats and internals
    + ofTexture: load methods for more types
    + ofBufferObject: invalidate method
    / ofFbo: use enum to set begin mode
    + ofShaderSettings
    / ofMaterial: allow diffuse color from color coordinates in mesh
    / ofMaterial: better shader cache handling
    / ofMaterial: shader cache per renderer to allow multiwindow
    + ofShader: add methods to change constant and defines values
    / ofShader: report absolute path when not able to load
    +`ofFbo: add methods to clear the different buffers + allow no color
    + ofTexture: set r->rgb swizzles for depth formats
    + ofBufferObject: add copyTo with offset and size
    / ofBufferObject: fix DSA check
    + ofGLUtils: support for half floats
    / fix issue with ofLight segfaulting during app exit
    / Medium P to High P for Shader Floating Point Precision in default shaders

### graphics
    + ttf full support for utf8 glyphs and right to left text orientation
    / fix save screen
    / fix ortho projection matrix calc
    / fix ofPolyLine::calcData returning invalid values
    / fixes to ofBackgroundGradient
    + ofImageLoadSettings
    / Bug Fix for "ofPixels mirrorTo horizontal"
    / Return bounding box of size 0 at coordinates (x,y) when attempting to draw an empty text at (x,y)
    / Corrected the initial value of maxX/maxY in getStringBoundingBox()
    / ofPixels: make some utility functions public
    + ofTexture: Added convenience method to draw subsection using ofRectangle parameters.
    / ofTTF: fix winding mode.
    - deprecated ofSetBackground
    + ofSetBackgroundColor
    / Fixes for ofBitmapFont getBoundingBox()
    / ofPath: stroked circle started from center so it had a line
    - deprecated ofTtfCharacter and just use ofPath instead
    / ofImage: fix save image returning false after some correct cases
    / Added translate, scale and rotate to ofPolyline
    / ofMesh: have*Changed made private
    / ofTTF: check max supported texture size and report if bigger than needed

### events
    + key events with utf8 codepoints + modifiers
    + mouse events with modifiers
    + ofEventListener
    + ofEventListeners
    + ofEvent::newListener
    + ofParameter::newListener
    + lambda support as listener when using ofEventListener
    + events: return true in notify if event is attended
    + ofFastEvent

### types
    / fixed some problems with read only parameters
    + added ofParameterGroup::remove and ofParameter::isReferenceTo
    / compare Parameters by their internal objects
    + ofParameter: implement castGroup
    / ofParameterGroup_: preserve name on clear
    / ofParameter: escape # to _

### sound
    + ofSoundStreamSettings
    + ofSoundStream: ability to specify API
    / ofSoundStream: fix fmod buffer setting
    / rtaudio stream: fix problem with old linux versions
    / ofSoundBuffer: getChannel, setChannel weren't correct
    / ofSoundBuffer: fixed wrong interpolation that was only iterpolating the first channel

### video
    / ofGstUtils: don't use SKIP on setSpeed seems to slow down some videos a lot
    / ofAVFoundationGrabber was not unlocking image buffer causing logging issue.


LIBS
----
    + added glm and ported whole core to work with it
    + added https://github.com/nlohmann/json as default OF json
    / most libraries updated to their most recent versions

EXAMPLES
--------
    / reorganized
    + added readme explaining the example for a lot of them
    + json example
    + remove ofxVectorGraphics from all addons example, won't compile in GLES
    + threadExample: use locks and shared memory in a way that shows the effects of not doing so
    + threadExample: adds keyboard shortcuts to set or remove lock
    / advanced3dExample: fix Swarm so it works with programmable renderer too
    / cameraParenting  example: explicit up vector on lookAt
    / 3dprimitiveexample: fix wireframe offset
    + added an example using the regular expression library from std
    + transformFeedbackExample
    / fboAlphaMask example fix. -Fixed issue where blending gave darker edges. -Moved all the masking into the shader. -Removed one fbo so it is easier to understand.
    / glParticleSystemExample: fixed segfault at startup
    / regularExpressionExample: change from Poco::RegularExpression to std::regex
    / Updated ofEasyCam example. -Now it shows the ortho mode interaction
    / Fixed polylineBlobExample type mismatch
    / fonstShapesExample: remove deprecated ofTtfCharacter and just use ofPath instead
    / soundBufferExample: use a sound stream object instead of global
    / ofNodeExample: fix starting pov
    / openCV Example and common-functions cleanup
    / removed osxRecordingVideoExample not working anymore cause AVFoundation
    / modernise advanced3DExample
    / fix systemSpeakExample and add linux version
    / shader/gaussianBlueExample: fix gaussian kernel
    / fftExample: used fixed array instead of over allocating to 8192
    / networkTcpExample: use proper utf8 append + full key events to create the string to send
    / svgExample: some syntax fixes

ADDONS
------
### ofxGui
    + color picker
    + text field
    + sliders text field on right click
    + support for any numeric type on sliders
    + `ofParameterReadOnly<string>` adds a label
    + `ofParameter<void>` adds a button
    + ofxPanel: load save/load icons locally (wouldn't work on multiwindow)
    + ofxGui/ofxPanel: capture save/load events to allow to customize behaviour
    / ofxGui: refresh render on name change
    + ofxGui: allow to load fonts with ofTtfSettings
    / ofxGui: cut overflowing text
    + ofxGui: custom events priority

### ofxPoco
    / poco and ofXml moved from core to addon with ofXml becomming ofxXmlPoco

### ofxKinect
    / fix for tilting triggering reconnect.
    / bugfix for ofxKinect fixes reconnection timing issues and reopening wrong kinect id

### ofxNetwork
    + ofxTCPSettings
    + ofxUDPSettings
    / ofxTCPClient::receive: try to receive untill buffer is empty

### ofxOsc
    / catch unknown osc parameter addresses
    / ofxOscMessage: fix return value warning
    / ofxOscMessage: cleaned up argument getters with consistent type conversion
    + ofxOscSender & ofxOscReceiver: added hostname & port getters
    + ofxOscSender & ofxOscReceiver: added clear() functions
    / standardized host/port member function naming
    + added ofxOscArgNone for N nil type tag
    / ofxOscSender: sending message arguments to oscpack requires specific types or wrong typetags may be set
    + ofxOscReceiver: added isListening() and ability to restart with current port value;
    / sender & receiver no longer clear port & host when cleared
    / ofxOscReceiver: slightly improved setup error print
    + ofxOscMessage: added getTypeString()
    / changed midi message, timetag, and rgba color types to use unsigned values
    / fixed signed/unsigned conversion issues when passing between oscpack
    - ofxOsc: removed bad arg type asserts in favor of better error messages
    + ofxOscReceiverSettings
    + ofxOscSenderSettings
    + ofxOscSender: added setHost() & setPort()
    + added ostream operators for sender, receiver, & bundle
    + ofxOscSender: handle socket creation exceptions

### ofxSvg
    / use original svgtiny instead of patched poco version


PLATFORM SPECIFIC
-----------------

### linux
    - removed linux 32bits
    + added support for gcc4, 5 and 6 or greater
    + ubuntu install script: added support for ubuntu based OSes like Elementary.
    / use gold linker for faster linking

### android
    - removed support for eclipse
    + added full support for android studio
    + fixed ofxandroidsoundstream
    + ofxGPS - GPS addon for iOS & Android
    + full refactor of android lifecycle handling
    + refactor video grabber texture handling
    + Add support for OF_PIXELS_NV21 in android video grabber
    + Android orientation event
    + Update android to SDK 25 and NDK 15c
    + now includes project generator

### ios
    + added experimental and optional support for GLKit
    + ofxGPS - GPS addon for iOS & Android
    + support for tvOS
    + GameController Support for iOS / tvOS
    / added ofxiOSCoreMotion and removed ofxiOSAccelerometer
    / ofxiOS - reset touches map before becoming visible (to clean junk that may be there from last time)
    + ofxiOSEventAdapter
    / iOS EAGLView - prevent flicker by synchronously drawing the first frame when starting animation
    / Fixes Update and Draw being called before Setup

### vs
    - removed support for vs2015
    + added support for vs2017

### macos
    / fix for OS X OF_TTF_SANS default on 10.13 closes
    / macos PG is now signed
    / fix framebuffer/screen mouse mapping on HiDPi
    / fix dual-monitor-resolution issue and HiDpi
    / Fixes for OS X Template project with AppStore and Legacy Project Build Cycle.
    / Fix Xcode IEEE Fast Math Release Fast Math Error
    / MacOS: restore window title when restoring window
    / fix to make the post-compile script on Xcode not fail when copying libfmodex.dylib when the project is not in the standard path depth

### msys2
    / install_dependencies fix to add path to environment variables correctly

### emscripten
    / fix emscripten sound stream for the latest api changes

### linux arm
    / Fallback to /dev/input/event* for uinput mouse and keyboard
    / Fixed opening of mouse and keyboard on RPi
    / Settings for EGLWindow were getting lost.

CI TESTS
--------
    + added ccache on platforms that support it
    + all libraries / platforms building on ci servers through apothecary
    + testing all platforms
    + PG bulding on CI servers
    + added unit tests for certain functionalities

  ___         ___         ___
 / _ \       / _ \       / _ \
| | | |     | (_) |     | (_) |
| | | |      \__, |      > _ <
| |_| |  _     / /   _  | (_) |
 \___/  (_)   /_/   (_)  \___/


  PLATFORM SPECIFIC
  -----------------

  ### linux
      + add EL6 build support of install_dependencies.sh and install_codecs.sh

 OF 0.9.8
 ========

 #### change key
 + added
 - removed
 / modified

 ------------------------------------------------------------------------------

   ___         ___        ______
  / _ \       / _ \      |____  |
 | | | |     | (_) |         / /
 | | | |      \__, |        / /
 | |_| |  _     / /   _    / /
  \___/  (_)   /_/   (_)  /_/

  PLATFORM SPECIFIC
  -----------------

  ### linux
      / fixes for debian alpha testing install_dependencies.sh

  ### qtcreator
      / fix some problems in msys2


OF 0.9.7
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

PROJECT GENERATOR
=================

/ Fixed a problem where PG included wouldn't run correctly on OSX Sierra


CORE
----

### graphics
    / revert bug introduced in ofPixels getColor(int) in 0.9.5


PLATFORM SPECIFIC
-----------------

### linux
    / fixes for install_dependencies.sh

### qtcreator
    / better detection for some libraries


   ___         ___          __
  / _ \       / _ \        / /
 | | | |     | (_) |      / /_
 | | | |      \__, |     | '_ \
 | |_| |  _     / /   _  | (_) |
  \___/  (_)   /_/   (_)  \___/



OF 0.9.6
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

PROJECT GENERATOR
=================

/ Fixed a problem where PG included wasn't compatible with 0.9.x anymore


PLATFORM SPECIFIC
-----------------

### ios
    / Disable TLS to avoid access to non-public symbols in appstore

### linux
    / Fix some problems with install dependencies scripts in latest dsitributions


   ___         ___        _____
  / _ \       / _ \      | ____|
 | | | |     | (_) |     | |__
 | | | |      \__, |     |___ \
 | |_| |  _     / /   _   ___) |
  \___/  (_)   /_/   (_) |____/



OF 0.9.5
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

CORE
----

### graphcis
    / ofPixels ported to use size_t so it supports higher image sizes
    / ofPixels fix for getColor(int)


PLATFORM SPECIFIC
-----------------

### osx
    / qtcreator: fixes not finding addons libs

### linux
    / mk: fixes rtaudio in latest distributions
    / mk: show exact package if one is missing
    / ubuntu 16.10 requires recompiling all libraries
    / ttf: solve a crash when closing after using a file dialog

### windows
    / qtcreator: fixes rtaudio




   ___         ___        _  _
  / _ \       / _ \      | || |
 | | | |     | (_) |     | || |_
 | | | |      \__, |     |__   _|
 | |_| |  _     / /   _     | |
  \___/  (_)   /_/   (_)    |_|



OF 0.9.4
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

CORE
----

### types
   / ofParameterGroup: fix for clear not freeing the children weak_ptrs
   / ofParameter: clean invalid weak_ptr


PLATFORM SPECIFIC
-----------------

### osx
    / recompiled freeimage to fix support for 32bits
    / Change to not include QuickTime or QTKit files for 10.12
    + New AVFoundation grabber for OS X 10.12

### linux
    / install_dependencies: add check to see if all packages are up to date
    / fix compatibility with qtcreator 4.1.0
    / fixes for serial in msys2 and linux

### windows
    / fixes for msys2 with qtcreator
    / fixes for serial in msys2 and linux

### ios
    / Fix to iOS examples which had missing Icon and Splash screen files.



   ___         ___        ____
  / _ \       / _ \      |___ \
 | | | |     | (_) |       __) |
 | | | |      \__, |      |__ <
 | |_| |  _     / /   _   ___) |
  \___/  (_)   /_/   (_) |____/


OF 0.9.3
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

CORE
----

### graphics
    / fixes for ofPixels move constructor and operator

### gl
    / fix for ofSetColor as default material in fixed pipeline


PLATFORM SPECIFIC
-----------------

### msys2
    / FreeImage to freeimage dependencies fix

### osx
    / remove data copy to bundle in osx default target
    / AVFoundation player fix for getPixels after unsuccessful movie load
    / fixes for window size on osx retina
    / FreeImage static libs recompiled to fix SSL errors #4812


### linux
    / fixes for linux fullscreen in gnome shell, openbox and others



 _____     _____     _____
|  _  |   |  _  |   / __  \
| |/' |   | |_| |   `' / /'
|  /| |   \____ |     / /
\ |_/ / _ .___/ / _ ./ /___
 \___/ (_)\____/ (_)\_____/


OF 0.9.2
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

CORE
----

### graphics
    / ttf font size had wrong dpi



PLATFORM SPECIFIC
-----------------

### raspberry pi
   / fix makefiles

### msys2
   / use poco from repositories to fix DNS error problem

          _              _             _
        / /\           / /\           / /\
       / /  \         / /  \         / /  \
      / / /\ \       / / /\ \       /_/ /\ \
     / / /\ \ \     /_/ /\ \ \      \_\/\ \ \
    /_/ /  \ \ \    \ \ \_\ \ \          \ \ \
    \ \ \   \ \ \    \ \/__\ \ \          \ \ \
     \ \ \   \ \ \    \_____\ \ \          \ \ \
      \ \ \___\ \ \   _      \ \ \   _    __\ \ \___
       \ \/____\ \ \ /\_\     \ \ \ /\_\ /___\_\/__/\
        \_________\/ \/_/      \_\/ \/_/ \_________\/

OF 0.9.1
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

GENERAL
-------
    / fix ofToDataPath on empty string (#4563)
    + Get allAddonExample to build in Travis CI (#4162)

CORE
----

### app
    / fix 90 degree orientation in fullscreen (#4697)
    / save window position on OS X before going fullscreen (#4655)
    / fix retina window size (#4746)

### 3d
    / fix getMeshForIndices() (#4083)
    / better default lookup for ofNode (#4594, #4625)
    / Correct strides for vertices and indices in the 3d primitives since the updated resolution changes (#4218)
    + add ofNode example (#4667)

### communication

### events

### gl
    / fix copying and moving ofFbo, ofShader, ofTexture, ofVboMesh (#4555)
    / fix fbo error messages on allocate (#2834)
    / fix state leak when recursively drawing MSAA fbos (#4102)
    / fix AVF video player on draw, by checking that textures are allocated before binding (#4743)

### graphics
    / fix copying and moving ofImage, ofTrueTypeFont (#4555)
    / make width and height protected, access using getWidth() and getHeight() (#3831, #3973)
    / fix swapRG for float and short images (#4609)

### math

### projectGenerator

### sound

### types
    + Implement draw(x, y) with default width and height (#772)

### utils
    / ofUtil: fix ofGetElapsedTimef being called every frame, now just called when needed (#4595)
    / ofFileUtils: fix relative path calculation in ofToDataPath (#4564, #4563, #4598)
    / ofThread: fix thread join (#4637)

### video
    / fix crash when closing unused ofVideoGrabber (#4554)
    / now correctly shows when a video is loaded for AVF based players (#4737)
    / gst grabber: workaround for bad functioning drivers returning wrong fps (#4647)
    / ofAVFoundationVideoPlayer: simplified setVolume (#4669)
    / ofDirectShowPlayer: add support for all different RGB formats (#4679, #4679, #4707)

PLATFORM SPECIFIC
-----------------

### Emscripten

### ofxiOS

### Android
    / fix android swipe example (#4539)

### Linux (incl. ARM)
    / fix compiling on Ubuntu, Linux Mint (#4550, #4546, #4543, #4342, #3687, #1902, #4550)
    / fix compiling on arm7 raspberry pi 2 (#4409, #4704, #4710, #4711)
    / fix GLES compilation (#4634)

### Microsoft Windows
    / fix getPixels() for ofDirectShowPlayer (#4716)
    / fix setVolume for ofDirectShowPlayer (#3818)

### OSX and iOS
    / fix for retina fullscreen pixel density calculations (#4699, #3733)
    / fix ofAVFoundationPlayer close-open issue (#4547)
    / update osx opencv (#4406, #4587, #4671, #4672)
    / apothecary: audit build scripts for osx/iOS (#4451)
    / fix install_name_tool and apple store compatability (#4479, #4615, #4622)
    / fix backgrounded app not being able to execute commands on the graphics hardware (#4544)
    / fix iOSNativeARCExample (#4606)
    / fix for Cannot find protocol declaration iOS UIViewControllerTransitionCoordinator (#4670)

CORE ADDONS
-----------
### ofx3DModelLoader

### ofxAssimpModelLoader
    / fix drawing wireframe and verts on assimp model (#4553)

### ofxGui
    / ofxGui crashes ofTessellator::performTessellation with ofParameter<float> (#4717)

### ofxiOSVideoPlayer

### ofxKinect

### ofxNetwork
    / fix TcpClient error handling (#3478, #4339, #4556)
    / implement tests for ofxUdpManager (#3656)

### ofxOpenCV
    / fix apothecary build (#4576)

### ofxSynth

### ofxTCPManager

### ofxUDPManager
    / fix timeouts in blocking mode (#4597)

### ofxMultiTouch


PROJECT GENERATOR
-----------------
  / fix overwriting iOS icons/images on project update (#4102)
  / apothecary: update formulas (#4588, #4620)

------------------------------------------------------------------------------

                                 .----.
                               .   _   \
                              /  .' )   |
                             |   (_.    /
        .-''` ''-.            \     ,  /      .-''` ''-.
      .'          '.           `'-'/  /     .'          '.
     /              `      .-.    /  /     /              `
    '                '     \  '--'  /     '                '
    |         .-.    | ,.--.'-....-',.--. |         .-.    |
    .        |   |   .//    \      //    \.        |   |   .
     .       '._.'  / \\    /      \\    / .       '._.'  /
      '._         .'   `'--'        `'--'   '._         .'
         '-....-'`                             '-....-'`

OF 0.9.0
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------
GENERAL
-------

+ added a new Project Generator
+ added CODE_OF_CONDUCT.md based on the Contributor Covenant and Berlin Code of Conduct, iteratively refined through group discussion
/ code cleanup (fixed many typos, warnings, memleaks, implicit conversions, types, const-correctness, duplicated and missing definitions/functions, removed internal use of deprecated methods and unnecessary code)
+ move towards C++11
+ added automated (unit) testing on several OSes via travis-ci.org
/ updated third party libraries (Poco 1.6, Freetype 2.5.5, FreeImage 3.17, OpenSSL 1.01j)
+ higher precision and monotonic timers
/ documentation and many examples fixed/adjusted to new code base
+ QtCreator project files and wizard added
+ Apothecary received a lot of fixes and updates to adjust to the new codebase
+ Makefile enforces recompilation when flags change


CORE
----

### app
    / setupOpenGL and ofApp___Window use ofWindowMode instead of int
    / fix exit callbacks to allow for calling of the destructors, and better signal handling
    / changed windowEntry event to mouseEntered/mouseExted, added these callbacks when registering for mouse events
    + clipboard supports UTF8
    + added scrollwheel support for GLFW
    / ofAppNoWindow refactored to remove unused methods
    + main loop can be set via ofSetMainLoop()
    + support for semantic versioning

### 3d
    / ofEasyCam: removes roll rotation when rotating inside the arcball
    / ofEasyCam: disables mouse events when moving ofxGui or other higher priority events
    / ofDrawGrid: arguments changed to ```float stepSize, size_t numberOfSteps``` instead of ```float scale, float ticks```
    / ofDrawGridPlane: arguments changed to ```float stepSize, size_t numberOfSteps``` instead of ```float scale, float ticks```
    / ofCamera: fix calculations on first frame or before first call to begin
    + ofCamera: orthograpic projection with coordinates relative to viewport
    / ofNode orbit now uses quaternions to avoid Gimball Lock
    / ofGraphics immediate mode style primitives renamed to ofDraw*

### communication
    / ofArduino: added missing initializations
    / easyCam: zRot flipped correctly

### events
    + added isEnabled() and size() to ofBaseEvent

### gl
    + programmable lights and materials
    + new area light type on programmable renderer
    + separate model matrix
    + ofBufferObject wraps GL buffers for easy use, see compute shader
      and pixel buffer examples for usage
    + ofGetCurrentNormalMatrix
    + ofSetOpenGL(ES)Version, allows to set any specific GL version
    + ofTexture::enableMipmap() auto-generate mipmaps on load - for textures which support mipmaps (that's TEXTURE_2D textures loaded after ofDisableArbTex() )
    + ofTexture::generateMipmap() generate mipmap after a texture was loaded, if the texture target supports mipmaps (see above).
    + ofTexture::disableMipmap() disables auto-generation of mipmap for texture
    - removed compressed mipmaps based on deprecated glu methods
    - ofSetTextureWrap() : deprecated (use corresponding ofTexture member method)
    - ofGetUsingCustomTextureWrap() : deprecated
    - ofRestoreTextureWrap() : deprecated
    - ofSetMinMagFilters() : deprecated (use corresponding ofTexture member method)
    - ofGetUsingCustomMinMagFilters() : deprecated
    - ofRestoreMinMagFilters() : deprecated
    + ofTexture::allocateAsBufferTexture(), a dedicated way to allocate ofTextures as Buffer Texture. Useful to pass large amounts of uniform data to shaders. See TextureBufferInstanceExample
    + ofShader: report offending lines on compiling errors
    + ofShader: setUniformMatrix now accepts an (optional) count parameter, so you can pass an array of matrices
    + ofGLUtils: better support for half float types
    / syntax change: getTextureReference() -> getTexture()
    / GPU optimisations giving the GPU driver more leverage to optimise drawing speed
    / ofVbo refector: ofVbo will, in programmable renderer, automatically recognise attribute locations 0, 1, 2, 3 as position, color, texture, normal attributes can be set using either using convenience methods e.g. `ofVbo::setTexCoordData(...)` or the more flexible `ofVbo::setAttributeData(ofShader::TEXCOORD_ATTRIBUTE, ...)` which allows for 3D texture coordinates.
    / Fix ofTexture::readToPixels for non RGBA or 4 aligned formats
    / Rename ofTextureData.glTypeInternal -> ofTextureDataData.glInternalFormat (this brings the parameter's name into sync with the OpenGL enum it represents)
    / Rename ofFbo::destroy -> clear to be consistent with other classes
    + added Data struct to ofMaterial
    / drawElements() now has an offset into the index buffer


### graphics
    + ofTruetypeFont: kerning and better hinting and spacing
    + ofDrawBitmapString: can draw any type not only strings
    / ofImage: can load image via https
    / getPixelsRef() deprecated and getPixels() now returns ofPixels temporarily ofPixels auto casts to unsigned char* so old code is still compatible
    / ofPixels::getPixels() -> getData()
    + ofPixels support for YUV formats, the prefered allocation method changes from channels to ofPixelFormat like: pix.allocate(w,h,OF_PIXELS_RGB)
    / fixed ofSetBackgroundColor
    / added ofGetBackgroundColor and ofGetBackgroundAuto
    - removed ofbBGColor and ofbBGClear
    / ofImage::loadImage() -> load()
    / ofImage::saveImage() -> save()
    + ofBeginSaveScreenAsSVG
    / ofPolyline::addVertexes changed to ofPolyline::addVertices
    / ofImage allows pixel color access via index
    / ellipse supported correctly in ofCairoRenderer

### math
    / updated ofMath.h
    + ofVecXf based ofNoise functions

### projectGenerator
    / moved apps & common code for PG to a separate repo
    + added a new app to replace the simple and legacy projectGenerators, using an HTML-based frontend built with electron
    / updated commandline PG with more features & better output, both for standalone use and to support the electron-based PG
    + added support for local addons outside the ../../../addons folder via the commandLine PG and the addons.make file
    + added templates for platform-specific configs, different OpenGL versions, etc
    + added scrolling feature for addons panel in legacy PG

### sound
    / ofFmodSoundPlayer: fix for file handles not being closed
    + ofSoundBuffer class for encapsulating a buffer of raw samples (see new soundBufferExample)
    + ofSoundDevice class for representing sound i/o devices
    + ofSoundStream::getMatchingDevices() for finding sound devices by name and / or channel count
    / deprecated all ofBaseSoundInput/Output methods that do not take an ofSoundBuffer
    / ofSoundPlayer::getIsPlaying() changed to ::isPlaying()
    - removed ofSoundStream::listDevices(), see ::getDeviceList() or ::printDeviceList()

### types
    + ofRectangle: added `setSize` method.
    + ofRectangle: added `operator -` method.

### utils
    / better timing in fps calculation
    + ofFpsCounter allows to easily meassure the fps of any element
    + ofTimer allows to create a precise periodic waitable timer
    / ofBuffer::getBinaryBuffer() -> getData()
    + ofThreadChannel introduced for easier multithreading
    / ofSerial::enumerateDevices() -> listDevices()
    / "ms" is now always 3 digits long when generating timestamps
    - removed ofThreadLogger
    + more and better logging

### video
    / gstreamer: fix memory leaks when closing a video element
    + gstreamer: support for YUV formats when using the programmable renderer using OF_PIXELS_NATIVE as pixel format will choose the fastest format
    / gstreamer: faster reload by default and optional asynchronous load
    / ofVideoPlayer::loadMovie() -> load()
    / ofVideoGrabber::initGrabber() -> setup()
    / AVFoundation player is now the default for OS X 10.8+
    + AVFoundation player now has a loadAsync for asynchronous video loading
    / DirectShow video player is now default for Microsoft Windows
    / Replaced deprecated methods in ofAVFoundationPlayer
    + test for player during initialization


PLATFORM SPECIFIC
-----------------

### Emscripten
    + new platform!
    / Raspberry Pi 2 is now default ARMv7 variant

### ofxiOS
    + Xcode 6.0 updates
    + deployment target moved up to iOS 5.1.1
    + iOS ARM64 C++11 support
    / tab-indentation specified in the openFrameworks Xcode project (ofApp is still user-specified)
    / AVSoundPlayer now uses shared audio session to better cooperate with AVFoundationVideoPlayer
    / no longer calling ofReloadGLResources in ofxiOS
    + ofxiOSKeybaord exposes UITextField
    + added ofAppiOSWindow::Settings for controlling the app settings
    / replaced ofPtr with shared_ptr in ofxiOSVideoGrabber
    - removed old target checks
    + adjusted scale factor for iPhone6 and iPhone6+
    + added simulator support iPhones
    + hint at using SDK 5.0 or later

### Android
    + fix camera stretching artifacts experienced on some devices when using 16:9 aspect ratios
    + support for Android Studio
    + remove support for ARM5 since no devices seem to have that cpu anymore and it was problematic with certain libraries
    - removed statistics and globals
    + added multiwindow and touch support
    + added helper method ofCallStaticVoidJavaMethod()
    / better handling of video when app is paused
    / getTextureReference() refactored to getTexture()
    + added gradle.properties file

### Linux (incl. ARM)
    + allow passing -j<num_jobs> to compileOF.sh for parallel compilation
    / native package managers handle OpenCV libraries
    / adjusted Makefile for Raspberry Pi 2

### Microsoft Windows
    + activated high precision timing to fix inaccurate fps calculations
    / multiprocessor compilation enabled by default
    / added support for Visual Studio 2015 and Windows 10

### OSX and iOS
    / moved ofApp system framework dependencies from Xcode project to CoreOF.xcconfig
    / removed GLUT from project template, see libs/glut/lib/osx if you need GLUT
    / tab-indentation specified in the openFrameworks Xcode project (ofApp is still user-specified)
    / use robocopy instead of xcopy in the post-build step to minimize file transfers
    + 10.8+ support
    / empty clipboard doesn’t crash anymore
    / compilation via Makefile also works without Xcode
    / unified AVFoundation video player

CORE ADDONS
-----------

### ofx3DModelLoader
    - removed as it was using some really old gl calls and ofxAssimpModelLoader already loads 3D studio models

### ofxAssimpModelLoader
    / Updated to assimp 3

### ofxGui
    + extendent ofxBaseGui

### ofxiOSVideoPlayer
    / API update
    / ofxButton::valueChanged() includes sender

### ofxKinect
    / Bugfix for kinects not keeping their IDs when using multiple kinects
    + 64bit updates for Microsoft Windows

### ofxNetwork
    + peek() to look into sockets

### ofxOpenCV
    / updated ofxCvHaarFinder to not use textures on internal image objects, allowing for usage within threads.
    / removed OpenCV binary libraries, installation moved to Linux package managers
    / OpenCV updated to 2.4.9

### ofxSynth
    - Removed, see ofxMaxim for a replacement (https://github.com/micknoise/Maximilian)

### ofxTCPManager
    + timeout after 5 seconds by default
    / using std mutexes to lock

### ofxUDPManager
    / updated error logging

### ofxMultiTouch
    - Removed - functionality provided by core events

PROJECT GENERATOR
-----------------

+ Added scroll feature in the Addons part of the (full) Project Generator

------------------------------------------------------------------------------

      .oooo.        .ooooo.             .o
     d8P'`Y8b      d88'   `8.         .d88
    888    888     Y88..  .8'       .d'888
    888    888      `88888b.      .d'  888
    888    888     .8'  ``88b     88ooo888oo
    `88b  d88' .o. `8.   .88P .o.      888
     `Y8bd8P'  Y8P  `boood8'  Y8P     o888o

OF 0.8.4
========

#### change key
          + added
          - removed
          / modified

------------------------------------------------------------------------------

PLATFORM SPECIFIC
-----------------

###OSX
    / Fixes for xcode 6

###RPI
    / Fixes install_dependencies and makefiles

------------------------------------------------------------------------------

      .oooo.        .ooooo.         .oooo.
     d8P'`Y8b      d88'   `8.     .dP""Y88b
    888    888     Y88..  .8'           ]8P'
    888    888      `88888b.          <88b.
    888    888     .8'  ``88b          `88b.
    `88b  d88' .o. `8.   .88P .o. o.   .88P
     `Y8bd8P'  Y8P  `boood8'  Y8P `8bd88P'

OF 0.8.3
========

#### change key
      + added
      - removed
      / modified

------------------------------------------------------------------------------

PLATFORM SPECIFIC
-----------------
###OSX
    / ofAppGLFWWindow: Drag drop files were coming in empty
### Windows
    / ofAppGLFWWindow: Drag drop files were coming in empty


CORE ADDONS
-----------
### ofxKinect
    / small fixes for CodeBlocks compilation on Windows

------------------------------------------------------------------------------

      .oooo.        .ooooo.         .oooo.
     d8P'`Y8b      d88'   `8.     .dP""Y88b
    888    888     Y88..  .8'           ]8P'
    888    888      `88888b.          .d8P'
    888    888     .8'  ``88b       .dP'
    `88b  d88' .o. `8.   .88P .o. .oP     .o
     `Y8bd8P'  Y8P  `boood8'  Y8P 8888888888


OF 0.8.2
========

#### change key
      + added
      - removed
      / modified

------------------------------------------------------------------------------

CORE
----
### 3d
    / fix of3DPrimitive copy constructor not copying node

### app
    + ofAppEGLWindow added new methods hasMouse()/hasKeyboard() to provide info on whether mouse/keyboard were detected
    + ofAppEGLWindow has new method setThreadTimeout allowing adjustable wait time for the thread to join
    / ofAppGLFWWindow: fix key mappings on non english keyboards
    / ofAppGLFWWindow: Cleaner exit code for GLFW based apps.
    / ofAppGLFWWindow: fixes for better Retina support.
    / Updated GLFW to latest master branch
    / ofAppGLFWWindow: added patch to handle special keys and cases.
    / ofAppGLFWWindow: better support for non-english keyboards.
    / ofAppGLFWWindow: small bugfix for GLFW drag drop callback for retina screens. Closes
    / ofSystem bugfix for successive calls. Now uses pclose instead of fclose.
### events
    + fix ofSetFrameRate to be more accurate
    + add keycode, scancode and unicode codepoint to ofKeyEventArgs
### gl
    + ofFbo: added new method to attach an external texture
    + ofFbo: fix regression on min/max filter settings
### Graphics
    / ofImage: convert format when loading different image types
    / Fix ofSetupPerspective not using passed width and height
    / of3DPrimitives: fix copy constructor
### Math
    / fix ofQuaternion setOrientation and getEulerOrientation
    / ofMatrix: add case to catch invalid matrix queries
    + ofGetCurrentOrientationMatrix(): query current orientation matrix state (supported by ofGLProgrammableRenderer, ofGLRenderer)
### Utils
    / ofThread now uses Class name for logging channel (was thread name)
    / ofURLFileLoader: better shutdown, don't shutdown if it wasn't initialized + wait thread
    / ofXml: fixes crash on empty document
    / ofSystem: fix for second and later calls failing
    + ofThread argument for tryJoin time.
    / ofThread overall, documentation and example fixes
    / ofThread change to infinite wait.
    / ofParameterGroup: fix to allow nested ofParameterGroups
    / ofParameterGroup: memory leak fix
    / ofThread: detach thread on stop if it's not being waited.
    / ofParameter(Group) fix polymorphism on parameter groups

PLATFORM SPECIFIC
-----------------
### Android
    / fix package generation, was missing paths.make
    / fix 32 bits ndk path
    / update project files to latest SDK/ADT (20140321)
    / fixed shader in shader example
    / disable x86 in assimp and opencv examples
    / shader and assimp examples reload gl resources when the app resumes
### iOS
    + implement setSpeed() for video player
    + ofxiOSGetDeviceInfo for querying major / minor versions, device type and device string
    + move to cleaner NSAutoreleasePool for iOS projects.
    / iOS-VideoGrabber memory related fixes
    / ofxiOSVideoPlayer: setSpeed functionality.
    + ofxiOSSoundPlayer: added getAVSoundPlayer() method
    / bugfix for ofxiOSImagePicker crashing on exit.
    / ofxiOSImagePicker: switch to using ofPixels.
### Linux
    / video: fixes for videos which pixels have padding
    / video: close pipeline properly by sending and waiting for EOS event
    / system: fix dialog boxes
    / install & make: detect and use gtk 3 instead of 2 if it's available
###OSX
    / Reduce warnings on OSX
    / ofxOsc: Fix for issue with UDP multicast socket on OSX.
### Windows
    / ofAppGLFWWindow: Correct fullscreen multi monitor display on Windows


CORE ADDONS
-----------
### ofxAssimpModelLoader
    / fix to be able to put loaders on vectors
    / use shared_ptr for assimp scene
### ofxGui
    + ofxSlider: added get/setMin/Max
    / ofxGui: fixed for mouse registration issues with ofxGui. ofxPanel was registering for mouse events 3 times on setup! - ofxButton was registering for events in setup and registering again in the destructor. - ofxSlider was never unregistering mouse events.
### ofxKinect
    / bugfix to fix Kinect not being able to re-open if device was not correctly shutdown.
    / bugfix for Kinect IR image crashing. Now sets the correct image sizes.
### ofxNetwork
    / fix port reuse on osx
### ofxOsc
    + added blob support. ability to send ofBuffers as blobs. Note: max data size limited by network buffer sizes.



OF 0.8.1
========

#### change key
      + added
      - removed
      / modified

------------------------------------------------------------------------------

```
_____/\\\\\\\______________/\\\\\\\\\_______________/\\\_
 ___/\\\/////\\\__________/\\\///////\\\_________/\\\\\\\_
  __/\\\____\//\\\________\/\\\_____\/\\\________\/////\\\_
   _\/\\\_____\/\\\________\///\\\\\\\\\/_____________\/\\\_
    _\/\\\_____\/\\\_________/\\\///////\\\____________\/\\\_
     _\/\\\_____\/\\\________/\\\______\//\\\___________\/\\\_
      _\//\\\____/\\\________\//\\\______/\\\____________\/\\\_
       __\///\\\\\\\/____/\\\__\///\\\\\\\\\/____/\\\_____\/\\\_
        ____\///////_____\///_____\/////////_____\///______\///_

```
MAJOR CHANGES
-------------
    / "testApp" changed to "ofApp" everywhere.

DEPRECATIONS & REMOVALS
-----------------------
    - ofxOpenCv: removed deprecated ofxCvMain.h. Use ofxOpenCv.h instead.

CORE
----
### 3d
    + ofGetCurrentMatrix() - returns current OpenGL matrix state (modelView-, projection- or textureMatrix) for ofGLProgrammableRenderer, ofGLRenderer.
### App
    / updated GLFW to latest master (20131204)
    - delete temporary buffer in ofAppGlutWindow
    + GLFW now detects when an OF app has been launched as Retina
    / Fixed unset variables in ofMouseEventArgs
    - remove early, unnecessary ESC check
    / ofxAppEGLWindow fix
### Communication
    / use binary mode (don't convert breaklines)
### Events
### Graphics
    / ofCairoRenderer: fix for moveTo when drawing ofPaths
    / ofDrawBox: on wireframe mode was recreating vertices on every call
    / fix ofImageType retrieval issue in ofTexture.readToPixels()
    + ofColor: added setHueAngle and getHueAngle
    / Fix for (ofColor == and != operators ignoring alpha)
### Math
        / ofMatrix3x3: fix incorrect multiplication
    + Added read-only access to current Matrix Stack
    / fix critical ModelViewMatrix issue when ofFbo::begin(setupScreen==false)
### GL
    / ofMaterial: fixed back emmisive material
    / Fix point sprites in openGL 3+
        / ofShader: #pragma include directive can now deal with relative paths for include files
    / Fix for mesh resolution parameters
    / Fix for the drawing of ofDrawArrow
    / fix critical issue w/ ofVbo shallow copies (custom attributes)
        / ofVbo custom attribute stride
        / fix ofShader include path behaviour
    / Modify ofVbo bind method to allow 2d vertices
    / Enabling GL_PROGRAM_POINT_SIZE in ofGLProgrammableRenderer
    - remove setName from ofMesh
    / Assign allocated flag to true after allocating offscreenGWorldPixels
    / Fix gl programmable renderer of draw axis
    / fixed ofMaterial for opengl ES1
    / check if currentShader exists - prevents segfault
    / add more sophisticated openGL capability checks
    + displacement map example
### Sound
### Types
### Utils
    + ofXml(const& string) constructor added
    / Fixed loadDialogBrowseCallback to return UNICODE string
    / ofXml addressing seg errors when clearing
    / tiny log fix in ofSerial
    + ofXml get attribute to work with paths && non-paths
### Video

PLATFORM SPECIFIC
-----------------
### Android
    / fix for texture reload
    / change to work with NDK r9b by default
    / zip uncompress moved to java
    / several fixes in pause / resume workflow
    / fix camera not working on some devices + use external texture, should be faster
    / camera: add recording hint for fastest fps on newest devices
    + support for x86
    / Android building script error when building from Windows
    / Bug fixed, Compile error in ofxAndroidSoundPlayer
### Mac OS X
    / tr1/memory compile issue on OS X 10.9
    / use cc.openframeworks.ofapp as identifier on osx
    / ES3 + xcode5 fix for missing GL_STENCIL_INDEX
### iOS
    / iOSVideoGrabber memory leak fixes
    / iOS Example Fixes
    + added iosStoryboardExample
    / iOS-Storyboard Example Fix for <iOS7
    / Fix iOS SoundEngine memory leak
    / Use correct audio category for routing audio to speaker
    + Enabled pointsprites for OpenGLES
    / ofxiOSUIImageToOFTexture fix inside ofxiOSExtras
    / fixed depth issue with ios assimp example
    / texture cache fix for ofxiOSVideoPlayer
    / ios movie player example fix
    / ios feature for getting / setting clipboard string
### Linux
    / alsa as default instead of pulseaudio lower latency
    / makefile fixes for better eclipse parsing
    / videoplayer: fix alpha formats for gstreamer 0.10
    + install_dependencies installs libusb so ofxKinect compiles
    - removed portaudio soundstream and all dependencies
    / Fix install_dependencies.sh for Fedora
    / project generator simple/linux: compile in release mode instead of debug
### Windows
    / fixed disable vertical sync through update in glfw
    / fix for ofGetFrameRate() going to infinity at high fps on windows
    / replaced poco 1.4.6 libs with 1.4.3p libs for vs2012
    / Changes to ofGstVideo files to compile with 0.10 gstreamer and fixup file names in Win32
    / Improve Dialog Win32
### Linux Arm
    / Fbo with ofClear now works properly
    / Fbo/Shader behavior now consistent with other platforms (can be used in update and draw)
    / Shaders used in draw() no longer cause GL Errors and corrupt display


CORE ADDONS
-----------
### ofxAssimpModelLoader
    / Remove ofMesh setName() in ofxAssimpModelLoader
### ofxKinect
    + Support for k4w and 1473 kinect with motor / accel / led control
    / Bugfix for disconnecting 1473 and K4W devices on OS X
    / Fixes typo in ofxCvImage that broke getRoiPixelsRef
    + added Windows driver install info to kinectExample header
### ofxGui
    + added ofParameter in the setup routine of ofxGuiGroup

PROJECT GENERATOR
-----------------

    / PG recompiled against 0.8.1
    + retina enabled for OSX
    + added support for adding frameworks on OSX


EXAMPLES
--------

------------------------------------------------------------------------------
                       ,---.-,
                      '   ,'  '.
        ,----..      /   /      \       ,----..
       /   /   \    .   ;  ,/.  :      /   /   \
      /   .     :   '   |  | :  ;     /   .     :
     .   /   ;.  \  '   |  ./   :    .   /   ;.  \
    .   ;   /  ` ;  |   :       ,   .   ;   /  ` ;
    ;   |  ; \ ; |   \   \     /    ;   |  ; \ ; |
    |   :  | ; | '    ;   ,   '\    |   :  | ; | '
    .   |  ' ' ' :   /   /      \   .   |  ' ' ' :
    '   ;  \; /  |  .   ;  ,/.  :   '   ;  \; /  |
     \   \  ',  /__ '   |  | :  ; ___\   \  ',  /
      ;   :    /  .\'   |  ./   :/  .\;   :    /
       \   \ .'\  ; |   :      / \  ; |\   \ .'
        `---`   `--" \   \   .'   `--"  `---`

OF 0.8.0
========

#### change key
      + added
      - removed
      / modified

------------------------------------------------------------------------------


MAJOR CHANGES
-------------
    + programmable GL renderer (ability to use OpenGL 3+ / GLES 2.0)
     linux support for ARM devices (e.g. Raspberry Pi)
    + ofXml, a full xml parser. NOTE: ofXML is still in development, more convenience functions will be added in future releases.
    + ofParameter, allows for datatypes to have names, default values and ranges, check out the examples!
    + 4x anti-aliasing is now on by default (osx/linux/win)
    + ofxKinect and ofxGui as core addons
    + new module-based logging system
    + apps now get special keys, shift/super/alt/ctrl etc
    + debug and release icons for all apps
    + textures and text are now correctly flipped when in ofCamera view. NOTE: this might mean that some projects which are manually flipping textures might see those textures inverted
    + orientation affects ofCamera. NOTE: old projects correcting this manually will break.
    + OpenGL ES 2.0 support on ofxiOS
    + support for Visual Studio 2012 (drop support for earlier VS versions)
    / changed default window manager from GLUT to GLFW
    / vsync and alpha blending now on by default
    / new documentation structure and now written in Markdown
    / ofxiPhone completely renamed to ofxiOS

DEPRECATIONS & REMOVALS
-----------------------
removed functions which were deprecated in 0072:

    - ofVec*f::squareLength()
    - ofVideoGrabber::grabFrame()
    - ofVideoPlayer::idleMovie()
    - ofPolyline::addVertexes
    - ofGraphics: ofVertexes and ofCurveVertexes
    - ofQTKitPlayer::bind() and ofQTKitPlayer::unbind()
    - ofAppiPhoneWindow::enableRetinaSupport(), isRetinaSupported(), isDepthEnabled()

deprecated in this release:

    / ofBox, ofCone, ofSphere deprecated in favour of ofDrawBox and ofDrawSphere
    / ofxiPhoneSetOrientation and ofxiPhoneGetOrientation -> ofSet/GetOrientation
    / ofxOpenALSoundPlayer
    / ofSetupScreenPerspective(), ofSetupScreenOrtho() don't accept orientation and vflip parameters anymore, use ofSetOrientation() to specify them
    / ofPath::set/getArcResolution -> set/getCircleResolution


CORE
----
### 3d

    of3dPrimitive
                + ofPlanePrimitive
                + ofSpherePrimitive
                + ofIcoSpherePrimitive
                + ofCylinderPrimitive
                + ofConePrimitive
                + ofBoxPrimitive

    ofMesh
                + ofMesh::setColorForIndices( int startIndex, int endIndex, ofColor color )
                + ofMesh::getMeshForIndices( int startIndex, int endIndex, int startVertIndex, int endVertIndex );
                + ofMesh::mergeDuplicateVertices()
                + vector<ofMeshFace> & ofMesh::getUniqueFaces()
                + vector<ofVec3f> ofMesh::getFaceNormals( bool perVertex )
                + ofMesh::setFromTriangles( const vector<ofMeshFace>& tris, bool bUseFaceNormal )
                + smoothNormals( float angle )
                + get meshes for primitives: plane, sphere, icosphere, cylinder, cone, box
                + ofMeshFace class with additional functions like getFaceNormal()
                / fixed bug where it was assumed the count of vertices / normals / indices / colours / texCoords was the same


    ofNode
                / ofNode::setParent() and clearParent() now accept a flag which allow nodes to maintain their global position

        ofCamera
            + ofCamera::setVFlipped/ofCamera::isVFlipped

### App
        + ofAppGLFWWindow::setMultiDisplayFullscreen(bool)
        + ofEnable/DisableAntiAliasing that works at runtime
        + ofGetOrientation() and ofSetOrientation()
        + OF_KEY_TAB
        / fixed bug where ofAppGlutWindow in fullscreen would report incorrect window size on the first frame

### Communication
        + HTTPS support

### Events
        + ofAddListener now has an optional param for listener priority
        + event listeners can now return a bool to indicate whether an event should be marked as attended
        + support for modificator keys: control, alt and shift (only glfw)

### Graphics

    of3DGraphics
                + normalized texture coords by default
                + added get / set functions for setting resolutions for new primitives
                + ofDrawPlane
                + ofDrawIcoSphere
                + ofDrawCylinder
                + ofDrawCone
                / functions like ofBox and ofSphere merged into of3dGraphics

    ofPolyline
                + resample a single point anywhere along an ofPolyline
                + cache and get normals, lengths, curvature, and rotation vector at any point
                + find the nearest index at a given length along the path
                + insert a vertex at any given point
                + required utils and mods to make the above possible

    ofTrueTypeFont
                + support for iso8859-1 on utf-8 encoded files
                + loading of system fonts by name
                + defaults for system fonts (OF_TTF_SANS, OF_TTF_SERIF and OF_TTF_MONO)

        + ofGetBackground()
        + ofColor::blueSteel
        + ofShortPixelsRef and ofFloatPixelsRef
        + ofPixels::setColor(ofColor) and ofImage::setColor(ofColor)
        / ofColor fix for colors wrapping.
        / ofPath refactored to be easier to integrate into other graphics libraries
        / ofDrawBitmapStringHighlight now modifies less GL state
        / ofDrawBitmapString now properly interprets tabs
        / fixed bug where ofTrueTypeFont::drawStringAsShapes didn't respect spaceSize and letterSpacing
        / fixed getHex and setHex for ofFloatColor and ofShortColor
        / fixed infinite recursion bug in ofCairoRenderer::background(float)
        / ofBox, ofCone, ofSphere deprecated in favour of ofDrawBox and ofDrawSphere
### Math
        + 1D interpolation functions: ofCosine|Cubic|Catmull|HermiteInterpolate
        + generic wrapping function float ofWrap(float value, float from, float to)
        / ofWrapDegrees, ofWrapRadians only wrap the new ofWrap, don't enforce a 360Â°/2Pi cycle anymore.
        / ofxVectorMath now included in core (as ofVectorMath)

### GL
        + ability to draw instanced geometry using ofVbo::drawInstanced()
        + ofSetDepthTest(), ofEnableDepthTest(), ofDisableDepthTest()
        + ofEnableAntiAliasing() / ofDisableAntiAliasing(), to toggle antialiasing on runtime. NOTE: needs to be enabled in the window.
        + ofShader support for #pragma includes
        + ofVbo support for VAOs
        / ofVbo and ofVboMesh accessors marked const
        / ofTexture::loadData will now reallocate if necessary
        / fixed ofLight global position and orientation issues when parented
        / fixed some private members of the gl renderer not being initialized
        / fixed the logic of vflip, handedness and orientation

### Sound
        + ofSoundStream: getters for sampleRate, bufferSize and numChannels

### Types
        + ofScopedLock (typedef for Poco::FastMutex::ScopedLock)
        + ofBaseHasShortPixels and ofBaseShortImage
        / typedefs that create compilation errors in C++11 now avoided

### Utils
        + ofGetVersionMajor(), ofGetVersionMinor(), ofGetVersionPatch()
        + ofSerial::isInitialized()
        / ofLog level & module header now more compact & readable, fixed bug where the default "OF" module was being printed to log files
        / ofFile now creates missing directories when saving
        / ofDirectory listings now properly interpret numerical file names (so "2.jpg" will show up before "10.jpg")
        / fixed bug where ofURLFileLoader wouldn't unlock mutex when request pool is empty
        / ofThread now waits during destructor, with a timeout of 10 seconds

### Video
        / ofVideoGrabber::listDevices() now returns a vector<ofVideoDevice>

PLATFORM SPECIFIC
-----------------
### Android
        + support for choosing GLES renderer at runtime
        + ofxAndroidVibrator
        + ofxAndroidVideoPlayer
        / fixed bug where app width and height weren't set before calling setup()
        / fixed bug in onTouchMoved where historical event calls were being given incorrect arguments

### Mac OS X
        + default app icons
        + makefile support
        / Xcode now shows more compilation warnings
        / template project file has been updated
        / file dialogs now resolve alias files

### iOS
        + launch images and icons for iPhone5 and retina iPad
        + ofxiOSSoundStream::setMixWithOtherApps() allows other apps' sound to play in the background
        + ofiPhoneVideoPlayer::setFrame, getCurrentFrame, getTotalNumFrames, etc
        + option to enable hardware orientation in ofAppiOSWindow
        + ofxiOSSoundPlayer as default sound player (based on AVAudioPlayer)
        / ofxiPhone renamed to ofxiOS
        / ofxiPhoneSoundStream refactored and renamed to ofxiOSSoundStream
        / ofCamera now behaves correctly according to device orientation
        / app now unregisters for ofEvents on exit
        / external display mirror mode is now default
        / fixed some functions which were not properly returning BOOL
        / fixed bug where sound stream would not resume after interruptions
        / fixed bug where converting a UIImage to an ofImage would pull in a bit of garbage memory
        / ofxiOS-Info.plist no longer being copied into project bundle
        / deprecated ofxiPhoneSetOrientation and ofxiPhoneGetOrientation
        / deprecated ofxOpenALSoundPlayer

### Linux
        / Add glfw3.0 for Linux 32bit
        + core support for armv6 and armv7
        + drag & drop is now supported on linux through glfw

### Windows
        + Visual Studio 2012 support (drop support for earlier VS versions)
        + added ability to get current .exe path
        / ofSerial can now connect to COM ports higher than COM9
        / capitalized target names ("Debug" and "Release")
        / added "compile C as C++" flag for VS project template

CORE ADDONS
-----------
### All addons
        - install.xml removed in favour of addons_config.mk

### ofxKinect
        + isFrameNewVideo(), isFrameNewDepth()

### ofxOpenCV

### ofxAssimpModelLoader
        / loader now caches repeated textures
        / fixed crash where a scene is released while the application is exiting

### ofxTCP
        / ofxTCPServer now thread-safe

### ofxSvg
        / fixed bug where transform attribute was ignored
        / fixed bug where non-allocated memory was being free()d

### ofxOsc
        / ofxOscMessage accessors marked const

### ofxGui
        + added a new GUI addon using ofXml

PROJECT GENERATOR
-----------------
### project generator (simple)
        / fixed issue where xibs weren't being properly handled in iOS projects

EXAMPLES
--------

### All Platforms
        + gui examples, demonstrating ofParameter and ofxGui
        + vboMeshDrawInstanced demonstrates instanced drawing
        + 3DPrimitives example
        / many general bugfixes
        / many examples reworked to demonstrate better threading practices

### Android
        + ofxGui example
        / all examples now have WRITE_EXTERNAL_STORAGE permission

### iOS
        + ofxGui example
        - OpenAlExample

---------------------------------------------------------------

      .oooo.        ooooooooo           .o
     d8P'`Y8b      d"""""""8'         .d88
    888    888           .8'        .d'888
    888    888          .8'       .d'  888
    888    888         .8'        88ooo888oo
    `88b  d88' .o.    .8'     .o.      888
     `Y8bd8P'  Y8P   .8'      Y8P     o888o

OF 0.7.4
========

#### change key
      + added
      - removed
      / modified

------------------------------------------------------------------------------


MAJOR CHANGES
-------------

New Codeblocks (windows):
For 0.7.4 we updated all core libraries to work against the new version of codeblocks, which itself uses a new version of GCC.  Anyone using codeblocks on windows will need to update to 12.11, because these libraries will not work with older versions of codeblocks.

System Libraries (linux):
Linux now uses some system libraries instead of including them in the core.  Please ensure you have installed the system dependencies for your OS.  You can install the system dependencies by executing the install_dependencies.sh script in  $OF_ROOT/scripts/linux/DISTRO/install_dependencies.sh, where DISTRO is your running linux distribution.  Currently OF has install scripts for Debian, Ubuntu, ArchLinux, and Fedora.

New versioning system:
From now on, OF version are numbered Major.Minor.Patch (e.g. 0.7.4). See Utils section and readme.md for details.

DEPRECATIONS & REMOVALS
-----------------------
- removed OF_USING_POCO conditionals
- support for Codeblocks version 10.04 on windows
- removed portaudio and freetype libraries.  Linux now uses some system libraries.  Please ensure you have installed the system dependencies.

CORE
----
### 3d

    ofCamera
        + added getForceAspectRatio() and getAspectRatio()
        - dependencies with glu, openGL
        / fix const correctness
        / fixed worldToScreen() bug
        / lens offset

    ofEasyCam
        / setPosition fix

    ofMesh
        / fixed load and save type mismatch
        / fixed ofMesh.load() to load Normals

    3DModelBuilder
        / to work with OpenGL ES

### App
    No Updates
### Communication
    No Updates
### Events
    + added void events (events that can be listened to from a method without arguments)
    - removed OF_USING_POCO conditionals

### Graphics

    ofTrueTypeFont
       / fixed out-of-bounds access in string display functions
       / fixed getStringAsPoints() when using full character set

    ofColor
       + css3-style colors (ofColor::mediumAquaMarine, ofColor::lightSkyBlue, etc.)

    ofPath
        + added ofCurveVertex(x, y, z) overload

### Math
    No Updates
### GL
    No Updates
### Sound
    No Updates
### Types
    No Updates
### Utils
    + added ofToDouble()
    + added Android and iOS functionality to ofLaunchBrowser
    / ofGetVersionInfo() now returns only the version string.

    ofBuffer
        + added append()

    ofConstants
        + added #include <cfloat>
        / The OF_VERSION* defines have been changed for the new version system.

### Video

    ofGstVideoGrabber
        + included support for PREFER_RGB_OVER_YUV and PREFER_NON_COMPRESSED
        / updated gstreamer player/grabber to be compatible with gstreamer 1.0

PLATFORM SPECIFIC
-----------------

### Android
    + added ofxAndroidLaunchBrowser(String url)to ofxAndroidUtils
    + added parent OFActivity class
    + added wifi multicast lock
    + added unloadTextures() to ofxAndroidApp

    / fixed getPan() and multiplay resume in ofxAndroidSoundPlayer
    / exposed getRealExternalStorageDirectory() as a public method
    / changed dataPath location to use Android standards
    / updated to Android SDK 21 and NDK r8d
    / updated eclipse project to use default run method instead of AndroidInstall target
    / fixed windows platform detection
    / added ability to choose arm targets through config.make
    / updates to the accelerometer units
    / updated readme file to reflect latest changes in Eclipse setup

    ofImage
        / register images on load

### Mac OS X
    / ofQTKitVideoGrabber now uses BGRA with BGRA to RGB blitting for fast capture.

### iOS
    ofImage
        / register images on load

### Linux
    - removed portaudio and freetype libraries

### Windows
    + codeblocks 12.11 support (drop support for earlier CB versions)
    / fixed grayscale image on windows
    / fixed microsecond timer overflow

CORE ADDONS
-----------

### ofxOpenCV
    / fixed broken hole detection in CV contour finder
    / applied patch for opencv bug (http://code.opencv.org/issues/1643)

### ofxOpenALSoundPlayer
    / fixed static ofMutex issue in ofxOpenALSoundPlayer

### ofxAssimpModelLoader
    / Assimp makeover
    / aiNode bugfix

### ofxTCP
    + adds rawMsg send and receive
    / fixes tcp close

### ofxSvg
    / several bugfixes for ofxSvg

PROJECT GENERATOR
-----------------

### project generator (simple)
    / updated the git submodule and gitignore for develop branch
    / fix project generator on Linux

EXAMPLES
--------

### All Platforms
    + added simpleEventsExample
    + added graphics/colorExtended example
    / fixed GPUparticleSystemExample unexpected behavior
    / added missing baud variable to communication/serialExample
    / updated firmataExample and fontShapesExample to use ofBaseApp instead of ofSimpleApp

### Android
    + androidAccelerometerExample
    + android3DModelLoaderExample
    / updated android examples to use ofxAndroidApp instead of ofBaseApp


---------------------------------------------------------------
          .oooo.     .oooo.    ooooooooo   .oooo.
         d8P'`Y8b   d8P'`Y8b  d"""""""8' .dP""Y88b
        888    888 888    888       .8'        ]8P'
        888    888 888    888      .8'       <88b.
        888    888 888    888     .8'         `88b.
        `88b  d88' `88b  d88'    .8'     o.   .88P
         `Y8bd8P'   `Y8bd8P'    .8'      `8bd88P'

OF 0073
=======

#### change key
    + added
    - removed
    / modified

---------------------------------------------------------------

/ ofGetElapsedTimef() no longer wraps after ~71 minutes, which was a bug introduced with 0072. ofGetElapsedTimeMicros() previously also wrapped after ~71 minutes, and this has been fixed as well.
/ fixed broken pause and other fixes in ofVideoPlayer on OSX
/ fixes to AndroidVideoGrabber
/ Android empty example inherits from ofxAndroidApp + adds android methods
+ auto register android touch events
+ ofMultMatrix(), ofLoadMatrix(), ofLoadIdentityMatrix()

---------------------------------------------------------------

          .oooo.     .oooo.    ooooooooo   .oooo.
         d8P'`Y8b   d8P'`Y8b  d"""""""8' .dP""Y88b
        888    888 888    888       .8'        ]8P'
        888    888 888    888      .8'       .d8P'
        888    888 888    888     .8'      .dP'
        `88b  d88' `88b  d88'    .8'     .oP     .o
         `Y8bd8P'   `Y8bd8P'    .8'      8888888888

OF 0072
=======

#### change key
    + added
    - removed
    / modified

-------------------------------------------------------------

DEPRECATIONS & REMOVALS
-----------------------

### Deprecations (will be removed in a future release, replacements noted after --->)

    / ofVec*f: squareLength() ---> lengthSquared()
    / ofVideoGrabber: grabFrame() ---> update()
    / ofVideoPlayer: idleMovie() ---> update()
    / ofPolyline: addVertexes() ---> addVertices()

    ofGraphics
        / ofVertexes() ---> ofVertices()
        / ofCurveVertexes() ---> ofCurveVertices()

    ofQTKitPlayer
        / bind() ---> getTexture()->bind()
        / unbind() ---> getTexture()->unbind()

    ofAppiPhoneWindow
        / enableRetinaSupport() ---> enableRetina()
        / isRetinaSupported() ---> isRetinaSupportedOnDevice()
        / isDepthEnabled() ---> isDepthBufferEnabled()

### Removals (have been deprecated previously, their functionality has been migrated to the OF core)

    - ofxDirList ---> ofFileUtils: ofFile, ofFilePath, ofDirectory, etc
    - ofxThread ---> ofThread
    - ofxVectorMath ---> ofVectorMath: ofVec2f, ofVec3f, etc

CORE
----

    / fixed lots of compilation warnings in the core library

### 3d
    / ofMaterial fixed incorrect storing / restoring of shininess

    ofEasyCam
        + enable/disable/get now uses middle mouse button for translation, activated by default
        + added set/get translation key, pressing this key and dragging with the left mouse button
           will translate the camera instead of rotating
        - removed mouseEvent listener methods
        / underlying math rewritten for readability, efficiency, & reliability
        / fixed strange or erratic behaviour and blocked rotation
        / setTarget now behaves as expected
        / fixed camera movement before dragging
        / updated rotation interactions to rotate over all three axes, rotation over the X or Y axes is
          achieved by starting to drag inside a circle with it's diameter set to the minimum between
          the viewport width and height, when the drag starts outside this circle Z axis rotation is achieved

    ofMesh
        / convert all index types from int to ofIndexType
        / fixes to multiLightExample

    ofNode:
        + missing implementation for rotateAround()
        + added getGlobalScale()

### App
    / ofGetElapsedTimeMillis() now returns an unsigned long

### Communication
    + added ofSerial speed: 230400

### Events
    / ofNotifyMousePressed() wasn't updating currentMouseX and currentMouseY
    / ofEvents: const parameters in events can be notifed by passing a value directly

### Graphics
    + added ofClear(ofColor c)
    + added ofRectRounded functions, bring on the lawsuit APPL ...
    / fixed anti-aliasing issue when auto background set to false
    / ofFloatImage and ofShortImage display correctly again
    / ofDrawBitmapString(): default style changed to be backwards compatible with pre-007
    / ofDrawBitmapString(): fixed drawing in simple mode, and drawing with alpha enabled
    / ofDrawBitmapString(): fixed rendering to FBOs (i.e. upside-down inside FBO)
    / ofGstUtils: several fixes for network streams and performance

    ofCairoRenderer
        + added support for rendering to a png
        / now sets current style every frame since Cairo wasn't preserving the style
        / now gueses type from file extension and renders to memory
        / fixed broken Z-rotation when called with ofRotate(X,0,0,1)
        / use ofSaveImage() to save image buffers instead of Cairo png

    ofImage
        / saveImage() now flattens color channels (4 to 3) when saving a jpeg
        / fixed memcpy bug in getBmpFromPixels()
        / update() rewritten to use ofGetGlInternalFormat so ofImage can handle short and float images

    ofPolyline
        + added fromRectangle()
        / getBoundingBox() is now const

    ofTrueTypeFont
        / make font rendering whitespace and printable character width the same

### Math
    / ofPoint & ofVec*f: fixed myVec = float only assigning to the x value,
      now prints an error message to help track down bugs, you should now use myVec.set(float)
      instead to set all three values to the same float value
    / all instances of functions named *Vertexes have been changed to *Vertices
    / ofVec3f: removed default argument for z component in 3 value constructor

### OpenGL
    - removed unused ofCheckGLTypesEqual(int, int)
    / ofLight::customDraw(): fixed bug

    ofFbo:
        / fixed FBO error when trying to retain depth texture
        / cleanup and bugfixes for both iOS and desktop
        / fixed bug where ofFbo was broken on systems that did not support MSAA

    ofTexture
        + added setUseExternalTextureID for setting external Texture ID which will not be released by ofTexture,
           was causing crashes for people trying to use external texture ids
        / can now read to pixels from depth buffers
        / added depth texture to image type support for OpenGL ES

    ofVboMesh
        + added enabled/disable of flags for colors, texCoords, indices, & normals
        / fixed problem when changing size to 0
        / fixed clearing of vbo buffers when mesh is reallocated to 0

### Sound
    / setPan() now works consistently (range is -1..1) over all objects that use sound
    / ofOpenALSoundPlayer: fix for stopThread with no arguments
    / ofSoundPlayer::loadSound() now properly returns a bool when it succeeds or fails

### Types
     / ofPixels: fixes to channel count in OF_PIXELS_RGBA

    ofRectangle
        + added getters and setters for x , y, width, & height
        + added methods for working with lines, translation, & scaling
        + edge x/y locations can now be called by their "anchor" name (ofAlignHorz and ofAlignVert)
           using getHorzAnchor() and getVertAnchor()
        + calling standardize() on a given ofRectangle will adjust the x / y position to make both
           the width / height to greater than zero
        + added a + operator, which is equivalent to calling translate(ofPoint ...)
        - removed getAsPolyline() and growToInclude()
        / x & y vars are now references to an underlying ofPoint, you can get a reference to
          the underlying ofPoint via getPositionRef()

    ofColor
        / fixes to reading from byte streams
        / fixes to setHsb for ofFloatColor and ofShortColor

### Utils
    + added deprecation mechanism: compile-time warnings tell you if you are using legacy/old functions,
       alternatives are mentioned where appropriate
    + ofSystemTextDialog
    + ofToString: added float formatting args
    + ofThread: added getter for poco thread
    / ofLog messages are significantly shorter
    / ofToDataPath() bugfixes
    / ofToString(value,int,int,char) now unambiguous
    / ofToHex(): fixed incorrect return value for non-ascii input
    / ofGetModifierPressed(): fixes to getting whether modifier keys are pressed
    / ofGetElapsedTimeMillis(): fixes to time counter functions, and increased accuracy
    / ofFileUtils: fixed comparison operators
    / ofThread: stopThread() fix
    / ofBuffer: set and operator= from string + append method
    / ofLaunchBrowser: now accepts https URLs
    / ofPtr: extended facade to allow for dynamic_pointer_cast

    ofConstants
        + added ofAlignVert & ofAlignHorz enums based on Qt,
           could be combined into a single bit-wise combined alignment flag
        + added ofAspectRatioMode enum, describes how an aspect ratio is preserved (or not) during scaling operations,
           can be combined with ofAlignVert and ofAlignHorz to describe a rectangular scaling operation
        + added ofScaleMode enum, represents a set of common scaling modes where each scaling mode can also be
           described by a specific combination of ofAspectRatioMode, an ofAlignVert, and an ofAlignHorz
           making ofScaleMode makes common scaling operations less verbose

    ofURLFileLoader
        / now catches all exceptions
        / fixed issue with ofRemoveRequest if ofURLFileLoader currently loading the request

### Video
    / solved dropped frame bug on setFrame
    / fixed createMovieFromURL
    / fixed problem when reloading movies
    / fixed setTexture(true) (allocates texture if it wasn't already allocated)
    / fixes to GStreamer streaming and initialization, squash unnecessary re-init on stop

    ofVideoPlayer
        + added functionality to load video from a URL
        + added movie path variable and getter
        / setVolume changed to take a float 0..1 with a warning message on out-of-range
        / updated to handle pixel formats more optimally

    ofVideoGrabber
        / switched to 24RGB pixel format to avoid manual conversion
        / now allows up to 24 cameras on Windows

PLATFORM SPECIFIC
-----------------

### Android
    + ofSystemTextAlert: added android version
    + ofSoundPlayer: implemented multiplay and streaming
    / Makefile now includes ofVideoPlayer
    / Eclipse project: update android target to use Android toolchain
    / updated setPixelFormat and getPixelFormat
    / updated OpenCV library
    / no more modal dialogs, were causing problems with gui thread
    / ofSoundPlayer: resource usage and bug fixes
    / updates Hardware Key handling to use characters instead of keycodes

    ofAndroidLib
        + added support for touchCancelled and swipe
        / updated to the latest Android SDK R20 and Android-NDK-R8B
        / updated minimum SDK level to 8 or Android 2.2
        / multitouch now supports 10+ pointers

    ofxAndroid
        + added listAlert
        + added network alerts
        + added ofxAndroidEvents
        / no longer crashes on 4.0.4 and 4.1.1 devices
        / updates to fix paths to external storage
        / ofThread now registers pthreads through JNI

    ofAppAndroidWindow
        / no longer crashes when returning from onPause()
        / fix for non-threaded mouse events
        / java side is already checking resume is not called on start

### Mac OS X
    / OpenFrameworks now works "out of the box" on 10.7+ by using QTKit for video grabbing & playback,
    / If compiling with the 10.6 SDK, the old Quicktime framework based players and grabbers are still included
    ofVideoPlayer
        / uses Core Video and QTKit internally
        + added support for getCurrentFrame() and setFrame() on videos with non-static framerates
        + added support for more codecs through QTKit framework
        + added support for direct-to-texture video rendering
        + added option to enabled asynchronous seeking for non-blocking calls to setFrame()
    ofVideoGrabber
        / uses Core Video and QTKit internally
        + added ability to record videos to disk
        + added support supports a wider range of video inputs, for example HDV over FireWire
        - removed video setting dialog for 10.7+

### iOS
    + fixed/added antialiasing for iOS
    + added ofxiPhoneGetGLParentView() for accessing the GLView parent UIView,
       this is now where other UIViews should be added to (was ofxiPhoneGetUIWindow()),
       also fixes a bug where the native video player was not displaying correctly on first load
    + added EAGLViewDelegate for getting state updates from EAGLView, also added a destroy method
    + added ofxiOSExtensions giving access to functions for reloading OpenGL textures,
       called when a ofxiOSEAGLView is destroyed and another fresh ofxiOSEAGLView is created
    + added ofxiPhoneLaunchBrowser(string url) to ofxiPhoneExtras
    - removed ofFmodSoundPlayer files from iOS+OF project which were causing compile issues with nightly builds
    - removed ofxiPhoneSoundPlayer.mm, was never used
    / iOS Xcode project valid architecture is now set to armv6 armv7 by default
    / iOS+OFLib deployment target has been set to 4.0, the base supported version of iOS by OF
    / fixed buggy compiler error with iOS events
    / fixed create_package to not delete iOS libraries
    / ofiPhoneVideoPlayer and AVFoundationVideoPlayer rewritten / cleanedup
    / fixed AVPlayerItem crash on unloading video
    / glViewController is now set as the window's rootViewController,
      this is a much nicer implementation than adding the glViewController's view to the window
    / fixed ofxiPhoneSendGLViewToFront() and ofxiPhoneSendGLViewToBack() to always work with the GLView superview
    / GLView is now brought to front using ofxiPhoneSendGLViewToFront()
    / ofImage::grabScreen fixed for iOS
    / ofxiPhoneScreenGrab: updated for retina display
    / isRetinaSupported() made more reliable
    / fixed accelerometer readings for devices with default landscape orientation (i.e. tablets)
    / EAGLView, ES1Renderer, and ES2Renderer now do not have any OF dependencies,
      can now be used to setup openGL ES without OF (if not required)
    / ofOpenALSoundPlayer: safety fixes for iOS

    ofAppiPhoneWindow
        + enabling retina was ambiguous, method names are now more concise
        + added methods for disabling retina, depth, & anti-aliasing
        / isRetinaSupported() made more reliable
        / now accesses window position, size, and screen size from the ofxiOSEAGLView

    ofxiOSEAGLView
        + ofxiOSEAGLView is the OF specific extension of EAGLView,
           most of the logic from the ofxiPhoneViewController has been move into here,
           it is now possible to create a ofxIOSEAGLView without the need for a ofxiPhoneViewController container
        + only accepts ofxiPhoneApp instead of ofBaseApp, this is more strict and safe
        + moved window size and position code from ofAppiPhoneWindow into ofxiOSEAGLView,
           makes more sense for ofxiOSEAGLView to return these values and only recalculate them when
           layoutSubviews() is called on a ofxiOSEAGLView resize

     ofxiPhoneViewController
        + now adds itself as a delegate when creating a glView,
           this is handy to get glView callbacks when extending in another class
        / completely refactored to only create and set up the glView, now rotates the glView to the device orientation

### Linux
    + added new method for setting vsync
    - removed unused FMOD scripts
    / ofOpenALSoundPlayer: bugfixes for Linux

### Windows
    + added readme for Visual Studio
    / added missing elif statement for MiniGW, changed the compiler from 4.4.3 to 4.6
    / ofSystemAlertBoxDialog: fixed for VS2010 and CB Windows
    / updated to Fmod 4.4
    / ofSystemUtils: fix windows dialog
    / ofUtils: fixed VS2010 compilation and dynamic shared pointer casting

CORE ADDONS
------------

    - removed ofAddons.h
    / ofxOsc: install.xml brought up to date
    / ofxOpenCV: partially fixed OpenCV runtime errors with MiniGW -  needs newer GCC for proper fix
    / ofxXmlSettings::setValue() fixed truncated floating point arguments bug introduced in 007
    / ofxXmlSettings: increased precision when saving double values

PROJECT GENERATOR
------------------

    / updated project generator template to include QTKit and CoreVideo frameworks on Mac OS X

EXAMPLES
---------

    + added rectangleAlignmentAndScaling example: demonstrates all of the new ofRectangle alignment and scaling features
    / videoGrabber example: updated with comments about legacy video settings and info on how to enable alpha movies
    / asciiVideo example: cleaned-up and clarified, now uses OF convenience functions for lightness and charToString conversion
    / shader example: fixed for ATI cards
    / fixed billboard and pointsAsTextures examples
    / updated easyCam example, now explains the reimplementation of ofEasyCam in greater detail

### Mac
    + added osxVideoRecorder example demonstrating recording videos to disk on Mac OS X using the ofQTKitGrabber
    + added osxHighPerformanceVideoPlayer demonstrating accelerated playback of HD videos on OS X using the ofQTKitPlayer

### iOS
    + moviePlayer example: added video player volume
    / Mapkit example fixed

### Android
    + added OpenCVFace face tracking example
    / soundPlayer example: updated to show multiplay on Android
    / touch example: bug fixes
    / image example: updated to fix path to image in loadImage(FILE)


---------------------------------------------------------------
      .oooo.     .oooo.    ooooooooo   .o
     d8P'`Y8b   d8P'`Y8b  d"""""""8' o888
    888    888 888    888       .8'   888
    888    888 888    888      .8'    888
    888    888 888    888     .8'     888
    `88b  d88' `88b  d88'    .8'      888
     `Y8bd8P'   `Y8bd8P'    .8'      o888o

OF 0071
=======

#### key
    + = added
    - = subtracted
    / = modified

---------------------------------------------------------------

The main focus for 0071 was to add new examples to better show the features added in 007.
0071 is mainly an 'examples + bug fixes' release, with some additional, much needed, updating to the mobile releases ( iOS and Android ).

Critical changes ( might need to update your projects to fix )
* ofEvents -> ofEvents() singleton - this means that projects which register calls using "ofEvents." will need to replace these with "ofEvents()." (note the parentheses).
* OF Examples now are found in the "examples/" folder in the root of the OF folder. The "apps/" is still included but is kept empty as a place to put your OF apps and experiments.
* Because of the project generator (see below) some addons have been standardized, and we've added "addons.make" to each project file that uses addons.  Prior to this release, addons.make was only used on linux / android platforms.  As we develop the project generator, we'll work to document these changes.
* audioRequested and audioReceived on the ofBaseApp/testApp are now called audioOut and audioIn


Project generator
A significant amount of the work which went into 0071 was in the development of a Project Generator. The Project Generator can make project files for all the OF platforms and it was critical to have this in order to increase the number of examples we could include with the release. The Project Generator is also able to add addons to existing projects, making the task of adding addons much faster than the current manual approach. Going forward, the Project Generator will be included with the new releases and be a key tool for generating, modifying and updating your OF projects. Currently, the Project Generator is in our github repository (https://github.com/openframeworks/openFrameworks) in apps/devApps/.

### core
    + loads of new examples! 96 examples now come with the desktop releases of OF. For a full list see: https://gist.github.com/2836913
    platform specific:
    + android support for front camera
    + android support for ofSoundPlayer
    + new iOS examples showing how to mix OF with Native Apps
    / android compatibility  with ndk r7+ and lots of bugfixes
    / loads of iOS bugfixes - including retina issue, FBO issues, native GUI fixes, video fixes.
    / XCode fix to avoid constant rebuild of projects
    / Some MacOS 10.7 fixes ( ofShowCursor / HideCursor )
    / Better compatiblity with XCode 4
    / linux: createProjects.py script dropped in favor of command line version of the projectGenerator

### utils
    + implemented getUserHomeDir() for Windows
    / ofSystemSaveFileDialog fix for linux
    / ofFile and ofDirectory fixes for overwrite failing.
    / ofThread now uses Poco::Thread
    / bugfix for ofFile::copyFromTo
    / makefiles now are modular + ensure compatibility with future versions

### addons
    + OSC allows multiple binds to the same port and support for multicast / unicast
    / Loads of ofxOpenCV fixes to prevent crashing when images are not allocated or the wrong size
    library changes:
    / OpenCv library in the ofxOpenCv addon updated to version 2.3.1
    / Poco library updated to version 1.4.3

### 3d
    + ofDrawGrid() for showing an multi-axis xyz grid, ofDrawGridPlane() for a single plane
    + ofDrawArrow() for lines with arrow heads (using ofCone())
    + ofCone() for draw cone primitives
    + 3D versions of ofCurve(), ofBezier(), ofVertex() and ofBezierVertex().
    + ofMesh now allows you to explicitly enable/disable colors/textures/normals before drawing using ofMesh::[enable/disable][Colors/Textures/Normals]()
    + ofxAssimpModelLoader utility methods
    / faster ofSphere() without GLUT dependancies.

### math
    + ofQuaternion::normalize()
    + ofTexture/ofImage::drawSubsection() for drawing portions of images
    + ofBackgroundGradient() for gradient app backgrounds, with OF_GRADIENT_CIRCULAR, OF_GRADIENT_LINEAR, OF_GRADIENT_BAR
    + ofPolyline::getCentroid2D() and ofPolyline::getArea()
    /  fix mismatch between * and *= operators for ofMatrix3x3. Implemented separate hadamard/schur product ofMatrix3x3::entrywiseTimes()
    / modify ofMatrix4x4::getRotate() method to operate on a scale-invariant version of the matrix
    Graphics:
    / ofColor fixes for HSB color wrapping
    / ofPixels optimizations for swapRGB() and mirror() and rotate()
    / ofPolyline::getSmoothed() works with open and closed lines

### app
    + ofAppGlutWindow::setDoubleBuffering() gets rid of flickering when drawing without clearing. use this in your main.cpp instead of just saying ofSetBackgroundAuto(false)

### openGL
    + ofFbo added option to have the depth/stencil buffer as a texture
    / ofFbo lots of fixes for iOS and POT textures

### communication
    / ofSerial bugfixes

### sound
    + ofSoundStream allow for different input and output devices.

For a full list of commits for 0071 see: https://gist.github.com/2836647

---------------------------------------------------------------

          0000             0000        7777777777777777/========___________
       00000000         00000000      7777^^^^^^^7777/ || ||   ___________
      000    000       000    000     777       7777/=========//
     000      000     000      000             7777// ((     //
    0000      0000   0000      0000           7777//   \\   //
    0000      0000   0000      0000          7777//========//
    0000      0000   0000      0000         7777
    0000      0000   0000      0000        7777
     000      000     000      000        7777
      000    000       000    000       77777
       00000000         00000000       7777777
         0000             0000        777777777

OF 007
======

#### key
    + = added
    - = subtracted
    / = modified

---------------------------------------------------------------

This is the biggest release yet. There are a ton of changes. The 0062 core was 624KB over 49 files. The 007 core is 1.12MB, 148 files. In other words, the core of OF has basically doubled.

Some of the favorite changes to OF include:
* PDF and SVG export with cairo
* ofMesh, ofPolyline utils and ofPath tessellation
* advanced ofColor support, like fromHex() and fromHsb()
* web-based resource and image loading
* advanced ofPixels power like getColor(x, y)
* the new Assimp addon
* advanced logging capabilities
* the ofPtr shared pointer, and shallow copy semantics: it's now safe to copy ofTexture
* moving ofxVectorMath, ofxDirList, ofxThread, and the previously unofficial ofxShader, ofxFbo, ofxVbo all into the core

Here's a fairly complete list of the changes.

### core changes
    + drag and drop support in applications (except in linux)
    + ofPtr for shared pointers, is actually std::tr1::shared_ptr and will be std::shared_ptr when c++11 is supported
    / ofVideoGrabber, ofVideoPlayer, ofSoundPlayer and ofSoundStream use "swappable apis". so does rendering, which means you can use cairo and output to PDFs or SVG for 2d and 3d.
    / new fps counting technique that works a little better across platforms
    / ofApp*Window have been refactored so now all the common functionality is in ofEvents which makes easier to create new windows and maintain the current ones
    / easier registering of core events with the new ofRegister*Events functions
    / all the classes with opengl resources have shallow copy + shared pointer semantics so textureA = textureB makes textureB point to the same texture in the graphics card as textureA, and the texture is released when the last copy is destroyed
    / all swappable api objects (ofVideoGrabber, ofVideoPlayer, ofSoundPlayer and ofSoundStream) have shallow copy + shared pointer semantics too
    / all memory structures (ofPixels, ofImage, ofBuffer, ofMesh...) have deep copy semantics, so pixelsA = pixelsB makes b a full new copy of pixelsA
    / Poco library is changed from 1.3.3 to 1.4

### video and cv
    + ofxCvHaarFinder is brought into the core ofxOpenCv addon
    / openCV is updated to 2.2
    / better gstreamer support on linux

### 2d
    + ofImage uses new web-based loading
    + ofPath and ofPolyline wraps all the 2d drawing functionality previously supported by OF, and adds more. ofPolyline has some new advanced methods like getResampledBySpacing(), getClosestPoint(), simplify()
    + ofSetOrientation() allows you to rotate the entire OF drawing space
    + ofPixels and ofImage have getColor() which returns an ofColor, and setColor(x,y,ofColor)
    + ofEnableBlendMode() supports additive, subtractive, alpha, etc. blending
    + ofColor has constants for 8 basic colors (cmy, rgb, bw), for example ofColor::magenta
    + ofColor finally adds HSB support. ofColor is always an RGB color, but you can get and set its hue, saturation and lightness via getHue()/setHue() etc. it also distinguishes lightness (value) from brightness. to create a color using HSB values, use ofColor::fromHsb(h,s,b,a). to set from hex, use ofColor::fromHex(hex)
    / ofColor, ofPixels and ofImage are now templated classes with support for 8bit, 16bit and 32bit and easy conversion between them using the = operator
    / ofDrawBitmapString() has been massively refactored to support all the different 3d drawing cases you can imagine via ofSetDrawBitmapMode()
    / ofPixels is now a fully-formed class that is used by a variety of classes
    / ofSetColor(int) is now interpreted as a grayscale value, ofSetHexColor(int) if you want to use hex values
    / ofTrueTypeFont, now generates a texture atlas when loading a font instead of a texture per char. It also has bind()/unbind() functions that allow to bind the texture only once to render several texts, making font rendering much faster.
    / ofTrueTypeFont getShapes uses ofPath
    / new ofTessellator class used internally in OF by ofPath and ofBegin/EndShape bassed on the tess2 library (with some modifications so it also works on openGL ES) which is faster and has a cleaner api than the previous one from GLU

### 3d
    + ofMesh, ofVbo and ofVboMesh provide a clean interface for drawing, and for gpu-accelerated drawing
    + ofNode is a new 3d node class with position and orientation that can be manipulated with methods like truck() and tilt()
    + ofCamera extends ofNode, is a new camera class for representing a camera position, orientation, and other parameters
    + ofEasyCam wraps ofCamera with automatic mouse binding that resembles processing's PeasyCam
    + ofLight also extends ofNode, provides a simple interface for placing and describing opengl lighting
    + ofBox and ofSphere draw the respective 3d shapes
    + multiple versions of ofxShader, ofxVbo and ofxFbo have been brought into the core as ofShader, ofVbo, and ofFbo
    + ofxAssimpModelLoader for loading a huge variety of 3d formats

### utils
    + functions and classes to load resources from the web to memory, or save to disk. using HTTP, either synchronously and asynchronously: ofSaveURLTo(), ofSaveURLAsync()
    + added the ofxThreadedImageLoader addon
    + cross-platform file dialog and alert dialog, ofSystemAlertDialog(), ofSystemLoadDialog(), ofSystemSaveDialog()
    + a ton of conversion methods have been added: ofToHex, ofToBinary, ofToString, ofHexToInt, ofHexToFloat, etc. they're all inside ofUtils.h
    + ofFile and ofBuffer have also been added for working with the filesystem
    / the core addons ofxVectorMath, ofxThread, and ofxDirList have been turned into ofVectorMath, ofThread and ofDirectory
    / new logging system with support for << operator, for example: ofLogVerbose() << "some message", logging to file and logging channels which makes it easier to redirect the logging output

### sound
    + working towards a sound synthesis addon, ofxSynth
    / slowly transitioning away from fmod toward openal for sound support
    / ofSoundStream has been converted to an object oriented api, and now is possible to open several devices and choose which device you want to use.
    - FMOD is removed on linux, so there are no dependencies with any non-free library

### mobile
    + android support
    / better iPhone video and camera support

---------------------------------------------------------------
      .oooo.     .oooo.       .ooo     .oooo.
     d8P'`Y8b   d8P'`Y8b    .88'     .dP""Y88b
    888    888 888    888  d88'            ]8P'
    888    888 888    888 d888P"Ybo.     .d8P'
    888    888 888    888 Y88[   ]88   .dP'
    `88b  d88' `88b  d88' `Y88   88P .oP     .o
     `Y8bd8P'   `Y8bd8P'   `88bod8'  8888888888

OF 0062 Pre-release
===================

#### key
    + = added
    - = subtracted
    / = modified

---------------------------------------------------------------

This is mostly minor changes, as it's a maintenance release with small bug fixes a few extra features and critical updates for Visual Studio 2010, Linux and iPhone developers.

    / altered framerate calculation
    / altered data path, adding ofSetDataPathRoot for OSX
    / ofSerial - bug fixes, including EAGAIN
    + added touchEvents
    / ofDrawBitMapString - added openglES compatible mode
    + ofGraphics - added universal functions to help control textures, such as "ofEnableNormalizedTexCoords()"
    + ofImage - setCompression code via SoSoLimited
    + ofTexture - added compression options, via SoSoLimited
    / ofSoundStream - bug fixes for audio clearning
    / ofConstants - switch some includes, for example, from <stdio.h> to <cstdio>
    + ofMath - added some functions to help with angle math, such as ofLerpDegrees, and ofAngleDifferenceDegrees
    + ofMath - added some polygon / polyline / line methods, such as ofInsidePoly and ofLineSegmentIntersection
    + ofMath - added curve functions, ofBezierPoint, ofBezierTangent, ofCurvePoint, ofCurveTangent
    + ofTypes - added functionality to ofRectangle, added [] options for ofColor
    - ofTypes - removed getPixels() from ofBaseVideo, as is was unnecessary
    + ofUtils - new ofSetDataPathRoot code for OSX, changes to ofToDataPath


---------------------------------------------------------------
          :::::::   :::::::   ::::::::    :::
        :+:   :+: :+:   :+: :+:    :+: :+:+:
       +:+   +:+ +:+   +:+ +:+          +:+
      +#+   +:+ +#+   +:+ +#++:++#+    +#+
     +#+   +#+ +#+   +#+ +#+    +#+   +#+
    #+#   #+# #+#   #+# #+#    #+#   #+#
    #######   #######   ########  #######

OF 0061 Pre-release
===================

#### key
    + = added
    - = subtracted
    / = modified

---------------------------------------------------------------
### major changes

- In this release we've moved to a *compiled* openFrameworks library as opposed to un-compiled.  a few motivations:  cutting down on directory size of OF, reducing redundant compiles, making compile times faster and making development of OF easier.   This changes quite a bit the organization of projects but it should lead to a much easier to use and maintain OF.

- we've moved from svn to git / github, and now have an automated system of building the OF package.  This should also greatly ease our development of openFrameworks.  Before we had separate SVNs for the OF lib, the OF examples, and the core addons.  Now, we've got one major place where changes and development of OF can be tracked.   see http://github.com/openFrameworks/openFrameworks.

- To encourage faster releases, we've moved to a major / minor release schedule, with minor releases (0061, 0062) relating to bug fixes and major releases (007, 008) relating to major features.

### core

    / folder structure is now changed so that all ide/packages can share one directory structure. Download the 0061 all release to check it out.
    - core addons now don't have libs from other platforms
    - core addons now don't have example project in the addons folder
    / core FreeImage.h -> move BOOL typedefs etc to #defines and do #undef at the end
    + Put MIT license in OF headers

### OSX

    + xcode has snow leopard fixes, and an SL release
    / xcode projects are now renamed to have the folder name (ie, every project has a unique target name)
    / xcode apps now have max optimization settings for release.
    + xcode now has 3 build modes, Debug, Release, Release Universal.  Debug and Release are for your current platform (either intel or PPC), Release Universal is for both.
    / renamed libs - change all libSomething.a to be something.a - this prevents xcode linking to root level dynamic libraries.

### Win32 CodeBlocks

    + added Poco linker flag for CB - needed for some network calls
    / fixed Poco linking order on win32.

### linux

    + changed the linux default videograbber to gstreamer
    - remove ofV4lUtils
    / smarter makefiles
    / modified install scripts to fix karmic problems &amp; build OF compile on install

### examples

    / fixed missing files, missing calls to windowResized, removed unnecessary calls to ofSetupSCreen() and include stdio.

### app

    / Fullscreen on OS X is not top level anymore (no more crazy fullscreen app crashes and can't escape).
    / OSX now using custom GLUT framework
    / ofSetBackgroundAuto(false) on windows OF now works for accumulation in both windowed and fullscreen mode.
    / Windows FPS issues have been fixed.
    / closing the window on PC now exits the application properly.
    / ofAppGlutWindow - frameRate is now set to 60.0 at the start - before it was un-initialized.
    + ofGetLastFrameTime - to get the elapsed time of the last frame.
    / fixes for ofGetWidth() &amp; ofGetHeight() being super slow, we now store them in vars and change on windowResize
    + ofGetAppPtr(), returns a pointer to the baseApp.
    + glutInitDisplayString option for initializing glut with a unique string, useful for FSAA or altering the setup.
    / fixed vertical sync for linux: http://www.openframeworks/forum/viewtopic.php?f=7&amp;t=561&amp;p=10683#p10683
    + ofBaseVideo added isFrameNew()

### utils

    + ofxNoise / ofxSignedNoise - Added simplex ( similar to perlin ) noise to core
    + ofBaseHasTexture now has ofSetUseTexture ( to enable / disable use of the texture )
    + ofBaseImage and ofBaseVideo have the same inheritance base
    / ofMap - now has a clamp argument, defaults to false
    / ofMap - added kyle's solution for division by 0 in ofMap: http://www.openframeworks/forum/viewtopic.php?f=6&amp;t=1413&amp;view=unread#unread
    / detect C:/ as a root path in ofToDataPath
    / fixed ofPoint warnings

### events

    / fixed vs2008 intellisense doesn't like using Poco::delegate; in ofEventUtils: http://www.openframeworks/forum/viewtopic.php?f=9&amp;t=1877

### graphics

    / ofImage - Loading a corrupted image in linux made the app crash, fixed now.
    / ofImage setImageType now should properly alter the texture if we need to.
    + ofTexture - get texPoint / texPercent, should help for doing texCoordinate stuff without knowing the target of the texture (ie, RECTANGLE_2D or ARB)
    + ofTexture - setTextureWrap &amp; setMinMagFilters
    + ofTexture - ofTexture float *, char *, short * -- added ability to make floating point textures, or upload float data.
    / ofTrueTypeFont - fixed TTF mem leak - http://www.openframeworks/forum/viewtopic.php?p=10178&#p10178
    / ofImage fixed resize color bug
    / ofImage inline functions cause problems with inhereited classes that want to use them in CB. Switch inline -> static fixes this issue.
    / ofGraphics - numCirclePts was being stored on ofPushStyle but not used, fixed now.
    / ofTrueTypeFont - Xcode projects now use the same version of the freetype as win/linux
    + ofSetCurveResolution - makes curved shapes a lot faster!
    / setAnchorPoint - move arguments from int to float

### communication

    / merged ofStandardFirmata into ofArduino, and removed ofStandardFirmata files from all projects.
    / fixed Linux serial issues with device number.
    / fixed firmata connect inconsistency: http://www.openframeworks/forum/viewtopic.php?t=1765
    / serial enumerateDevices is doing ofLog so not always printing out: http://www.openframeworks/forum/posting.php?mode=reply&amp;f=6&amp;t=1860
    / fix for ofArduino: http://www.openframeworks/forum/viewtopic.php?f=8&amp;t=3042&amp;p=16425#p16425

### video

    + add setFrameRate for ofVideoGrabber
    / update the quicktime settings dialog so that PS3 eye panel shows up.
    / ofVideoPlayer and ofVideoGrabber on OS X Intel now using k24RGBPixelFormat - this has a huge speed improvement.
    / ofVideoGrabber on mac seems to not list devices unless log level is verbose http://www.openframeworks/forum/viewtopic.php?f=9&amp;t=1799&amp;view=unread#unread
    / ofVideoPlayer uninitialized bool
    / ofVideoPlayer - pause delay
    / ofUCUtils - PIX_FMT_YUV422 won't exist anymore on future ffmpeg versions
    / ofUCUtils - sws_getContext needs PixFmt as parameter instead of int since karmic

### sound

    / updated ofSoundStream to use rtAudio 4
    + Add a ofSoundPlayerUpdate call - for keeping fmod on track - should be called once per frame to make sure sounds don't drop out.

### addons

    / ofxXmlSettings fixed a printf(tag.c_str()) that xcode complained about
    / ofxXmlSettings memory leak
    / ofxXmlSettings read attributes
    / ofxXmlSettings read from string / save to string

    ofxVectorGraphics

        + Update to CreEPS 2.0 - has MIT license and new features.

    ofxOpenCv

        + ofxCvColorImage - convertToPlanarImage(...) for one channel (ie, grabbing just sautration)
        / ofxCvShortImage - fixed assignment operators
        + ofxCvGrayscaleImage - add brightness / contrast to grayscale image
        / ofxCvContourFinder converts centroid to ints
        / ofxOpenCv - fixed grayscale to planar / planar to grayscale conversions
        - ofxOpenCv - remove implicit ROI intersection
        + ofxCvImage - re-added drawBlobIntoMe() for painting a blob into an ofxCvImage
        + ofxOpenCv - added appropriate iphone #ifdefs
        / ofxOpenCv - do quicker copy for getPixels and setPixels when image is memory alligned already.
        / ofxOpenCv scale conversions for floatImage
        / ofxOpenCv swapTemp bug

    ofxOsc

        / ofxOsc - shutdown leak
        / ofxOsc - memory leak
        / ofxOsc - Shutdown issue - fix here: http://damian.dreamhosters.com/svn/public/ofxOsc/latest/

    ofxThread

        / stopThread now called when threaded function returns

    ofxVectorMath

        / went back to inlining, for performance
        + added ofxMatrx4x4 and ofxQuaternion
        / ofxPoin2/3/4f deprecated merged with ofxVec2/3/4f

    ofxNetwork

        / fixed for xcode needs tcp.h included in ofxUdpManager

---------------------------------------------------------------

      .oooo.     .oooo.       .ooo
     d8P'`Y8b   d8P'`Y8b    .88'
    888    888 888    888  d88'
    888    888 888    888 d888P"Ybo.
    888    888 888    888 Y88[   ]88
    `88b  d88' `88b  d88' `Y88   88P
     `Y8bd8P'   `Y8bd8P'   `88bod8'

OF 006 Pre-release
==================

#### key
    + = added
    - = subtracted
    / = modified

---------------------------------------------------------------
### major changes

- in this release we've added another library Poco, v 1.3.3 ( http://pocoproject.org ),
just now only used for events but is included in the core available for addons like ofxHttpUtils.
For java folks, Poco is a bit like the java sdk (java.io, java.util, java.net, etc)
- added the event system for base events (setup, update, draw, mouse, keyboard) and constructing new events.
- dropped the addons.h system in place of direct inclusion of addons .h files.  Old code will get warnings but not break for this release
- abstracted the window toolkit, so that other windowing toolkits besides glut can be used with OF, such as iphone, glfw, non-windowed (command line).
- all graphics have been opengl-es-ified, allowing for iphone support
- all objects have virtual destructors (thanks memo for the tip)
- all objects that draw can return their internal texture
- adding base types for OF object (ofBaseDraws, ofBaseHasPixels) which should allow better OOP programming with OF
- added advanced user functionality, like choosing the ARB extension usage, disabling setupScreen, binding of textures, etc.
- tons and tons of bug fixes and small functions (ofClamp, push and pop styles) that should make programming in OF easier

---------------------------------------------------------------

### app

    / ofAppRunner - setup update draw should be called in the right order (prior was setup, draw, update)
    + ofAppBaseWindow / ofAppGlutWindow - glut abstraction and base window toolkit, tested with glfw, iphone and no window
    + disable / enable setup screen
    / ofSimpleApp - is now ofBaseApp
    / ofSimpleApp - mouseRelease receives now x, y and button
    + ofSimpleApp - added a windowResized function called when the window size changes

### utils

    + ofTypes - added an inheritance hierarchy, ofBaseDraws, ofBaseUpdates, ofBaseHasTexture, ofBaseHasPixels and ofBaseVideo
    / ofTypes - added operator overloading to ofPoint
    / ofUtils - ofToDataPath can now be deactivated or queried for absolute path
    + ofUtils - ofSplitString for tokenization
    + ofUtils - added ofLog and logging system with warning levels
    / ofUtils - ofLaunchBrowser now works on linux
    + ofMath - added ofNormalize, ofMap, ofClamp, ofLerp, ofDist, ofDistSquared, ofSign, ofInRange, ofRadToDeg, ofDegToRad, ofRandomWidth, ofRandomHeight
    ( thanks todd, memo. kyle )
    / ofMath - better seeding for ofRandom

### graphics

    + ofTrueType - added that ability to get fonts as polygons, bezier interpolation, with polygon simplification for performance
    + ofGraphics - added an ofColor type, for push and pop style
    + ofGraphics - ofRestoreGraphicsDefaults, for reseting all graphical changes
    / ofGraphics - ofEllipse fixes
    / ofGraphics - changed ofCircle from display list to a pre cache vertex
    list - faster!
    + ofGraphics - basic tranformations and opengl wrapping, ie, ofRotate(), ofRotateX(), ofRotateY(), ofRotateZ(),
    + ofGraphics - added an ofStyle struct - it holds color, line width, circle resolution, blend mode, smoothing, fill mode
    + ofGraphics - push style / pop style
     getStyle and setStyle to get and set the style
    + ofGraphics - ofSetLineWidth
    / ofGraphics - fixed a bug with projection vs modelview matrix order -

http://www.openframeworks/forum/viewtopic.php?p=5213#5213

    + ofGraphics - changed immediate mode to vertex arrays for compatibility with opengl es
    + ofGraphics / ofBitmapFont - wrapped the glut bitmapped font as a separate file, so that ofDrawBitmapString can be used in non glut windowing kits.
    + ofTexture - ofTextureData contains info about the texId, etc so that advanced folks can bind
    + ofTexture - calls to bind / unbind
    + ofTexture - setAnchorPercent / setAnchorPoint
    + ofTexture - enable / disable our texture "hack", a small padding that helps ofTextues look good in
    + ofTexture - bUseARBExtention is controllable on allocate (and also, controllable globally), which can help make shaders work better
    / ofImage - restructured for cleaner code -- freeImage moved to only a few places
    / ofImage - grayscale saveImage bug fixed
    / ofImage - grayscale setFromPixels fix -- http://www.openframeworks/forum/viewtopic.php?t=929
    / ofImage - allocate now calls update -- http://www.openframeworks/forum/viewtopic.php?t=835
    / ofImage - 8bpp fix - http://www.openframeworks/forum/viewtopic.php?t=712
    / ofImage - grayscale save fix - http://www.openframeworks/forum/viewtopic.php?t=943
    / ofImage - fixed loadImage() not returning true if successful
    / ofImage - added setAnchorPercent / setAnchorPoint
    / ofImage - fixed resize color swap problem - http://www.openframeworks/forum/viewtopic.php?t=1242

### events

    + added an event system that allows objects receive core events ( draw, mouse, key...), and to create other events.

### video

    / ofVideoGrabber - (quicktime) much better device listing / selection.
    / ofVideoGrabber - (quicktime / mac) fixed isFrameNew always returning true
    / ofVideoGrabber - (linux) some changes to size and format detection
    / ofVideoGrabber - (linux) better support for yuv and other colorspaces (won't work on ubuntu hardy because of a problem with the ffmpeg version)
    / ofVideoGrabber - (linux) resize and colorspace conversion through ffmpeg
    / ofVideoGrabber - (linux) corrected errors on close
    / ofVideoGrabber - (windows) using new video input library (0.1995)
    + ofVideoPlayer - (quicktime) added rtsp for quicktime -

http://www.openframeworks/forum/viewtopic.php?p=7665#7665

    + ofVideoPlayer - added a frame selection system, ie, goToFrame(x), getNumFrames(), goToFirstFrame(), getCurrentFrame(), goToNextFrame(), goToPreviousFrame()
    + ofVideoPlayer - added getIsMovieDone() - let's you know if the movie hit the end
    / ofVideoPlayer - (linux) all functionality working now
    / ofVideoPlayer - (linux) changed fobs out for gstreamer
    / ofVideoPlayer - (linux) sound in videos

### communication

    / ofSerial - fixed read byte errors
    / ofSerial - verbose, with non reads
    / ofSerial - 0 / -1 error reporting better
    + ofSerial - flush, allows you to clear the serial buffer
    + ofSerial - available() lets you query how many bytes are available.
    + ofStandardFirmata - added this class (thanks erik!) for easier serial support
    + ofArduino - added this class which extends ofStandardFirmata with specific arduino functions

### sound

    / ofSoundPlayer - (linux) alsa as default backend

addons changes that are part of FAT package

in this release, we've moved the use of ofAddons.h and the define system.
now, we just include the main .h file per addon.

    / ofxNetwork - disconnection detection
    / ofxNetwork - no data loss on tcp
    + ofxNetwork - udp examples

    / ofxThread - Applied the fix for startThread checking it is already
    running and moving mutex creation -

http://www.openframeworks/forum/viewtopic.php?p=7221#7221

    + ofxVectorMath - add copy constructor - http://www.openframeworks/forum/viewtopic.php?t=617
    / ofxVectorMath - fixed ofxVec2f::perpendicular bug, http://www.openframeworks/forum/viewtopic.php?t=4835
    / ofxVectorMath - fixed /= operator
    + ofxVectorMath - added some better names for functions (for example, getRotated() instead of rotated()).  All "d" names, ie rotated, normalized are deprecated for the next release)

    + ofxOpenCv - added ROI functionality for all ofxCvImage
    / ofxOpenCv - ofxContourFinder fixed getBlob(), which existed only in .h before
    / ofxOpenCv - consolidated ofxCvImage - most functionality now across all image types
    / ofxOpenCv - operator overloading changed to const
    / ofxOpenCv - scaleIntoMe bug is fixed, http://www.openframeworks/forum/viewtopic.php?t=828
    / ofxOpenCv - reallocation bug fixed, http://www.openframeworks/forum/viewtopic.php?t=935
    / ofxOpenCv - warns on non odd valued blur param

    + ofxOsc - now supports bundles
    + ofxOsc - better error handling

---------------------------------------------------------------

      .oooo.     .oooo.     oooooooo
     d8P'`Y8b   d8P'`Y8b   dP"""""""
    888    888 888    888 d88888b.
    888    888 888    888     `Y88b
    888    888 888    888       ]88
    `88b  d88' `88b  d88' o.   .88P
     `Y8bd8P'   `Y8bd8P'  `8bd88P'

OF 005 Pre-release
==================

#### key
    + = added
    - = subtracted
    / = modified

---------------------------------------------------------------
### major changes

in this release we introduced a "FAT" package as well as an addons.h system that uses #defines.
removed the ofCore to fix some include issues.

    / better ofMain / addons system
    - no ofCore

---------------------------------------------------------------
### other changes

### app

    + ofAppRunner - recording of the screens original position
    / ofAppRunner - jorge's fix for idle / setFrameRate()

http://www.openframeworks/forum/viewtopic.php?t=515&amp;highlight=frame+rate

    / ofAppRunner - mac hide cursor

### communication

    / ofSerial - joerg's fixes for pc serial
    / ofSerial - better support for non "COM" ports
    / ofSerial - improved write bytes
    / ofSerial - no longer using "string" for serial, which was failing for binary data. that was a terrible idea.
    / ofSerial - returning -1 for errors on read byte;

### graphics

    / ofGraphics - fixed the tesselator to work on different versions of xcode
    / ofImage - moved constants to ofConstants
    / ofImage - made freeImage stuff more integrated
    + ofImage - added copy and equals overloaders, so that imageA = imageB will work as intended
    + ofImage - added a clear() function
    / ofImage - better cloning
    + ofTexture - added copy and equals overloaders to prevent pass by copy
    + ofTrueTypeFont - getStringBoundingBox() to get the bounding box

### utils

    + ofTypes - added ofTypes, for base types like ofPoint, ofRectangle
    + ofConstants - added a #define WIN32_LEAN_AND_MEAN for window
    + ofConstants - some GLUT defines that help for windows
    / ofConstants - disabled another VS warning
    + ofConstants - added some std stuff, like iostream and vector
    / ofConstants - backspace vs del fixes for OSX

http://www.openframeworks/forum/viewtopic.php?t=494

    + ofUtils - added ofGetYear, ofGetMonth,ofGetDay(),ofGetWeekDay()

### sound

    + ofSoundPlayer - added isStreaming field
    / ofSoundStream  - fixed ofSoundStreamEnd
    / ofSoundStream  - renamed ofSoundStreamEnd ofSoundStreamClose

### video

    + ofVideoGrabber - added linux support for unicap as well as V4L, settable in ofContstants
    + ofVideoGrabber - added ofUCUtils
    + ofVideoGrabber - tons of V4L fixes
    + ofVideoPlayer - createMovieFromURL for quicktime
    / ofVideoPlayer - fix for bHavePixelsChanged on QT (isFrameNew returning true)

---------------------------------------------------------------
      .oooo.     .oooo.         .o
     d8P'`Y8b   d8P'`Y8b      .d88
    888    888 888    888   .d'888
    888    888 888    888 .d'  888
    888    888 888    888 88ooo888oo
    `88b  d88' `88b  d88'      888
     `Y8bd8P'   `Y8bd8P'      o888o

OF 004 Pre-release
==================

#### key
    + = added
    - = subtracted
    / = modified

---------------------------------------------------------------
### major changes

- fixed an issue with addons include, now its a system with ofCore.h / ofMain.h / ofAddons.h
- addons folder added to the setup

---------------------------------------------------------------
a brief explanation about addons:

now the structure look like:

- apps
- libs
- addons

where libs are the core libraries for OF (which shouldn't change much), and addons is everything else.

Adding a library will work in the following way:

a) add to the addons folder everything downloaded
b) add to addons.h the "addon.h" lines (found in addon instructions), such as:

    #ifdef OFADDON_USING_OF_CV
        #include "ofCvMain.h"
    #endif

c) follow any steps about adding includes, sources, and libs to the project (or makefile)
d) when you want to use the addon, use the #define before including ofMain in testApp.h, such as:

    #define     OFADDON_USING_OF_CV
    #define     OFADDON_USING_OF_VECTOR_MATH
    #include    "ofMain.h"
e) place any dll / .so files in the right place (after compiling)

---------------------------------------------------------------
### other changes

    /   ofGraphics - fixed a bug with OS X, 10.5 compiling
    /   ofSerial - fixed a bug with win32 destructor
    /   ofVideoGrabber - mac grabber fixed bug with settings loading
    /   ofMain - is now just ofCore and ofAddons
    +   ofAddons - is the place where addons will go
    +   ofCore - is the what ofMain was, all the "core" OF code  (internal and addons should include "core" not main to avoid recursive linking)
    /   movie grabber example - win32 / fixed an issue where some of the code was commented out.

---------------------------------------------------------------

      .oooo.     .oooo.     .oooo.
     d8P'`Y8b   d8P'`Y8b  .dP""Y88b
    888    888 888    888       ]8P'
    888    888 888    888     <88b.
    888    888 888    888      `88b.
    `88b  d88' `88b  d88' o.   .88P
     `Y8bd8P'   `Y8bd8P'  `8bd88P'

OF 003 Pre-release
==================

#### key
    + = added
    - = subtracted
    / = modified

---------------------------------------------------------------
### major changes

- start of a structure for of addons
- all classes feature protected variables instead of priavete for easier extending
- linux is *really* integrated into the codebase, this is BIG!
- fmod is updated to fmodex
- glu is included for tesselation routines
- ofSerial is a class now
- much more careful classes in terms of memory usage
- much better closing routine
- string is used everywhere we can instead of char * or char arrays (much usage stays the same though)
- ofVertex for curves, more graphics options added
- every app now uses a "data" folder for any OF loading and saving media

---------------------------------------------------------------
### other changes

(there are likely more, this is what we can remember but we will update as we update the API)

    +   ofTexture - non power of 2 when possible using GLEE / ARB extensions
    +   ofGraphics - curves: beziers and curve vertex, etc.
    +   ofGraphics - polygons (poly shapes) using ofBeginShape() and tesselation
    +   ofGraphics - polygon w/ multiple contours for holes (using ofNextContour())
    +   ofImage -  copy image operator (clone)
    +   ofImage - upload data (fix rgb/bgr issues)
    /   whole code - remove all the top left bools, top left as 0,0 is fixed in the code now.
    +   ofVideoGrabber - (OSX) Save user preferences with qtVideoCapture dialog
    +   ofVideoGrabber - new VI input lib in for video grabber
    /   ofVideoGrabber / ofVideoPlayer - isFrameNew() behaves better, newness is per idle call
    +   ofSimpleApp - key release in addition to key press
    /   ofSimpleApp - special keys coming through uniquely (see constants.h)
    +   ofAppRunner - ofFullScreen opition to alter the screen
    +   ofAppRunner - get monitor info (w/h) and position window options
    +   ofAppRunner - glut game mode
    /   whole code - no "../../../" BS, we now have data folders!
    +   ofAppRunner - set window title
    +   ofUtils - open a URL in default browser
    +   ofConstants - clamp in ofConstants.h
    +   ofUtils - simple version printout
    /   whole code - destructors everywhere

---------------------------------------------------------------

      .oooo.     .oooo.     .oooo.
     d8P'`Y8b   d8P'`Y8b  .dP""Y88b
    888    888 888    888       ]8P'
    888    888 888    888     .d8P'
    888    888 888    888   .dP'
    `88b  d88' `88b  d88' .oP     .o
     `Y8bd8P'   `Y8bd8P'  8888888888

OF 002 Pre-release
==================

#### key
    + = added
    - = subtracted
    / = modified

---------------------------------------------------------------
### major changes
- added ofSerial class for serial communication
- added ofSoundPlayer class for fmod / sound sample playing
- ofAudio now named ofSoundStream to make more sense
- added glee and fmod libraries to the path
- added videoInput library on windows for grabbing video (via direct show)
- renamed ofQtVideoGrabber to ofVideoGrabber (since it doesn't nec.
use quicktime)
- renamed ofQtVideoPlayer to ofVideoPlayer
- lots of fixes to get rid of warnings
- xcode libs now all universal.
- xcode can now build universal apps when in 'Release' mode.

---------------------------------------------------------------

### others
    + ofSimpleApp - mouseX, mouseY now added for p5 compatability
    / ofSimpleApp - fixed some bugs, like mouseMoved before the windows is open
    + ofUtils - ofSetFrameRate() to set a target framerate
    + ofUtils - ofSetVertical sync to set vertical sync on or off
    / ofGraphics - 0,0 is now top left by default
    + ofGraphics - ofEnableSmoothing(), works just for lines for now.
    + ofGraphics - ofSetCornerMode for drawing rects on center or corner
    + ofGraphics - ofSetBackgroundAuto() to enable manual background
    clearing (works for fullscreen)
    + ofTexture - loadScreenData(), upload screen data to texture
    / ofTrueTypeFont - many bug fixes for the black edges and cut off curves
    / ofVideoPlayer - setPaused() bug fix
    + ofVideoGrabber - device selection options
    + ofVideoGrabber - multiple devices works both qt and dshow
    + ofConstants - added some high res timer code in win32, accessible
    through commenting in a #define and recompiling
    + ofConstants - ability for win32 to choose quicktime or directshow (videoInput)
    through #define

---------------------------------------------------------------

most ascii art generated with http://patorjk.com/software/taag/ using the font 'roman'

---------------------------------------------------------------
