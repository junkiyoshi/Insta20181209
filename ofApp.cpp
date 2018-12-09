#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	this->cap.open("C:\\Develop\\video\\image3.mp4");
	this->cap_size = cv::Size(1280, 720);

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->rect_size = 80;
	for (int x = 0; x < this->cap_size.width; x += this->rect_size) {

		for (int y = 0; y < this->cap_size.height; y += this->rect_size) {

			auto image = make_unique<ofImage>();
			image->allocate(this->rect_size, this->rect_size, OF_IMAGE_COLOR);
			cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
			cv::Rect rect = cv::Rect(x, y, this->rect_size, this->rect_size);

			this->rect_images.push_back(move(image));
			this->rect_frames.push_back(frame);
			this->cv_rects.push_back(rect);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	cv::Mat src;
	this->cap >> src;
	if (src.empty()) {

		this->cap.set(cv::CAP_PROP_POS_FRAMES, 1);
		return;
	}

	cv::resize(src, this->frame, this->cap_size);
	cv::cvtColor(this->frame, this->frame, cv::COLOR_BGR2RGB);
	for (int i = 0; i < this->rect_images.size(); i++) {

		cv::Mat tmp_box_image(this->frame, this->cv_rects[i]);
		tmp_box_image.copyTo(this->rect_frames[i]);

		this->rect_images[i]->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->rect_images.size(); i++) {

		this->rect_images[i]->draw(this->cv_rects[i].x + ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.1), 0, 1, -15, 15), this->cv_rects[i].y + ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.1), 0, 1, -15, 15));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}