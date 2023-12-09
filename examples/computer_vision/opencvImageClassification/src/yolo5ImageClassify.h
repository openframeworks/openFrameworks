//
//  imageClassify.h
//  opencvExample
//
//  Created by Theo on 9/22/22.
//

#pragma once

#include "ofxOpenCv.h"

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//based on code from https://github.com/doleron/yolov5-opencv-cpp-python/

//License is listed as:

//MIT License
//
//Copyright (c) 2022 doleron
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

class yolo5ImageClassify{

	public:
		
		struct Result{
			ofRectangle rect;
			float confidence;
			string label;
		};
		
		//leave these unless your model is expecting different dimmensions
		//(some are 224 x 224) - more info here: https://github.com/ultralytics/yolov5/releases
		const float INPUT_WIDTH = 640.0;
		const float INPUT_HEIGHT = 640.0;
		
		//higer means less results but more accurate
		const float SCORE_THRESHOLD = 0.2;
		const float NMS_THRESHOLD = 0.4;
		const float CONFIDENCE_THRESHOLD = 0.3;
		
		//enabling CUDA might need a different OpenCV lib built with CUDA support
		//------------------------------------------------------------------------------------------------------------------------------------
		void setup(string modelFile, string classesFile, bool useCuda){
										
			//needs OpenCV 4.6 as earier has errors
			net = cv::dnn::readNetFromONNX(ofToDataPath(modelFile));
			
			if(useCuda){
				std::cout << "Attempt Yolo with CUDA\n";
				net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
				net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
			}else{
				std::cout << "Running Yolo on CPU with OpenCL if available\n";
				net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
				net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);
			}

			//load the classes from text file
			classes.clear();
			auto buffer = ofBufferFromFile(classesFile);
			for( auto line : buffer.getLines() ){
				classes.push_back( line );
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------------
		vector <Result> classifyFrame(cv::Mat frame){
							
			vector <yolo5ImageClassify::Detection> output;
			detect(frame, net, output, classes);
			
			results.clear();
			for (int i = 0; i < output.size(); ++i){
				
				auto detection = output[i];
				auto box = detection.box;
				auto classId = detection.class_id;
				
				Result res;
				res.rect = ofRectangle(box.x, box.y, box.width, box.height);
				res.label = classes[classId];
						
				results.push_back(res);
			}
				
			return results;
		}
		
	protected:
	
		struct Detection{
			int class_id;
			float confidence;
			cv::Rect box;
		};

		//------------------------------------------------------------------------------------------------------------------------------------
		void detect(cv::Mat &image, cv::dnn::Net &net, std::vector<Detection> &output, const std::vector<std::string> &className) {
			cv::Mat blob;

			auto input_image = format_yolov5(image);
			
			cv::dnn::blobFromImage(input_image, blob, 1./255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), false, false);
			net.setInput(blob);
			std::vector<cv::Mat> outputs;
			net.forward(outputs, net.getUnconnectedOutLayersNames());

			float x_factor = input_image.cols / INPUT_WIDTH;
			float y_factor = input_image.rows / INPUT_HEIGHT;
			
			float *data = (float *)outputs[0].data;

			const int dimensions = 85;
			const int rows = 25200;
			
			std::vector<int> class_ids;
			std::vector<float> confidences;
			std::vector<cv::Rect> boxes;

			for (int i = 0; i < rows; ++i) {

				float confidence = data[4];
				if (confidence >= CONFIDENCE_THRESHOLD) {

					float * classes_scores = data + 5;
					cv::Mat scores(1, className.size(), CV_32FC1, classes_scores);
					cv::Point class_id;
					double max_class_score;
					minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
					if (max_class_score > SCORE_THRESHOLD) {

						confidences.push_back(confidence);

						class_ids.push_back(class_id.x);

						float x = data[0];
						float y = data[1];
						float w = data[2];
						float h = data[3];
						int left = int((x - 0.5 * w) * x_factor);
						int top = int((y - 0.5 * h) * y_factor);
						int width = int(w * x_factor);
						int height = int(h * y_factor);
						boxes.push_back(cv::Rect(left, top, width, height));
					}

				}

				data += 85;

			}

			std::vector<int> nms_result;
			cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, nms_result);
			for (int i = 0; i < nms_result.size(); i++) {
				int idx = nms_result[i];
				Detection result;
				result.class_id = class_ids[idx];
				result.confidence = confidences[idx];
				result.box = boxes[idx];
				output.push_back(result);
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------------
		cv::Mat format_yolov5(const cv::Mat &source) {
			int col = source.cols;
			int row = source.rows;
			int _max = MAX(col, row);
			cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);
			source.copyTo(result(cv::Rect(0, 0, col, row)));
			return result;
		}
	
		
		cv::dnn::Net net;
		vector <string> classes;
		vector <Result> results;
		
};

