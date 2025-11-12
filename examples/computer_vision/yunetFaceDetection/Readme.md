# Face detection with yunet

### Face detection in OpenFrameworks using dnn

It turns out that doing face detection in 2025 is much more efficient with neural networks than cascades.
So here is an OF example that uses opencv's facedetectoryn (for yunet).

You will need the file with the pre-trained model. The model version must be compatible with your opencv version.

How to find out your opencv version?
 * It will be printed out during the ofApp::setup() function.
 * If you have a version greater than 4.9, you can download the current [face_detection_yunet_2023mar.onnx](https://github.com/opencv/opencv_zoo/tree/main/models/face_detection_yunet) model
 * If your version is 4.6 or earlier, download the [2022](https://github.com/opencv/opencv_zoo/blob/088c3571ec70df15100a5e4c26894d95951e92e9/models/face_detection_yunet/face_detection_yunet_2022mar.onnx) version of the model.

The versions are coded in the ofApp::setup() function. Different versions might require editing the code.

### Expected behavior

The camera starts, and a red square will be drawn around the faces found.
