#ifndef DIST_HPP
#define DIST_HPP

#include "ofMain.h"
#include "ofxGui.h"

struct Dist {

	inline static std::string base_url_ { "https://en.cppreference.com/w/cpp/numeric/random/" };
	std::optional<std::string> url_;
	ofParameterGroup parameters_;
	ofColor color_ { 128, 128, 128 };
	std::string info_;
	glm::vec2 range_ {};
	bool discrete_ { false };
	size_t underflow_;
	size_t overflow_;
	std::size_t max_ { 0 };
	float cost_ { 0.0f };

	virtual auto gen() -> void = 0;
	virtual auto clear() -> void = 0;
	virtual auto compile() -> void = 0;
	virtual auto draw(float x, float y, float w, float h) -> void = 0;
	virtual ~Dist() = default;

	Dist() {};
};

template <typename T>
struct ConcreteDist : public Dist {

	std::vector<T> data_;
	std::function<T()> gen_;
	std::vector<T> bins_;
	int autorot_ { 0 };

	ofParameter<void> url_button_;

	ConcreteDist(std::string label, std::string info, std::optional<std::string> url,
		std::vector<ofAbstractParameter *> params, std::function<T()> gen,
		std::size_t num_bins = 101, glm::vec2 range = { 0, 100 }, bool discrete = false)
		: gen_(gen) {
		info_ = info;
		range_ = range;
		discrete_ = discrete;
		bins_.resize(num_bins);
		parameters_.setName(label);
		for (auto & p : params)
			parameters_.add(*p);
		if (url) {
			url_ = base_url_ + url.value();
			url_button_.set("click for reference");
			url_button_.addListener(this, &ConcreteDist::open_url);
			parameters_.add(url_button_);
		}
	}
	auto open_url() {
		if (url_) ofLaunchBrowser(url_.value());
	}

	auto gen() -> void override {
		data_.push_back(gen_());
	}

	auto clear() -> void override {
		overflow_ = 0;
		underflow_ = 0;
		data_.clear();
		std::fill(bins_.begin(), bins_.end(), T { 0 });
	}

	auto compile() -> void override {
		// histograms non-vecs only
		if constexpr (std::is_arithmetic_v<T>) {
			float divisor = (range_.y - range_.x) / float(bins_.size() - 1);
			for (auto & v : data_) {
				v /= divisor;
				if (v < range_.x) {
					underflow_++;
				} else if (v >= bins_.size()) {
					overflow_++;
				} else {
					bins_.at(v) = bins_.at(v) + 1;
				}
			}

			max_ = 0.0;
			for (size_t i = 0; i < bins_.size(); i++) {
				if (bins_.at(i) > max_) max_ = bins_.at(i);
			}
		} else {
			// no histograms for vecN
		}
	}

	auto draw(float x, float y, float w, float h) -> void override {
		ofPushStyle();
		ofPushMatrix();
		{
			ofTranslate(x, y);
			ofSetColor(color_);
			ofDrawRectangle(0, 0, w, h);

			ofSetColor(ofColor::darkRed);
			if (underflow_) ofDrawBitmapString("undershoot: " + ofToString(underflow_), w + 5, 58);
			if (overflow_) ofDrawBitmapString("overshoot: " + ofToString(overflow_), w + 5, 74);

			ofSetColor(192, 192, 192, 255);
			ofDrawBitmapString(info_, w + 5, 35);
			ofSetColor(255, 255, 255, 255);
			ofDrawBitmapStringHighlight(parameters_.getName() + " " + ofToString(cost_ * 1000, 2, 5) + "ms", w + 5, 12);

			if constexpr (std::is_arithmetic_v<T>) {

				auto p = 0.0f;
				double incr = w / bins_.size();
				auto fact = h / max_;
				if (discrete_) {

					// line bars for discrete
					ofTranslate(incr / 2, 0);
					for (auto y : bins_) {
						ofDrawLine(0, h, 0, h - float(y) * fact);
						if (y == 0) {
							ofNoFill();
							ofDrawCircle(0, h - float(y) * fact, 2.5);
							ofFill();
						} else {
							ofDrawCircle(0, h - float(y) * fact, 3);
						}
						ofTranslate(int(incr), 0);
					}
				} else {

					// integral for reals
					ofPolyline line;
					line.addVertex(0, h - bins_[0] * fact);
					for (auto y : bins_)
						line.lineTo(p += incr, h - float(y) * fact);
					line.draw();
				}

			} else if constexpr (std::is_same_v<T, glm::vec2>) {

				ofSetColor(255, 255, 255, 96);
				for (const auto & d : data_)
					ofDrawCircle(d, .5);

			} else if constexpr (std::is_same_v<T, glm::vec3>) {

				ofSetColor(255, 255, 255, 32);
				of3dPrimitive prim;
				prim.getMesh().getVertices() = data_;
				prim.getMesh().setMode(OF_PRIMITIVE_POINTS);
				prim.rotateDeg(70, { 0.2, 0.3, 0.5 }); // just some perspective

				ofPushMatrix();
				{
					ofTranslate(w * 0.2, h * 0.2);
					prim.drawWireframe();
					prim.drawAxes(w * 0.5);
				}
				ofPopMatrix();
			} else {
				ofDrawBitmapString("unsupported visualisation", 10, 10);
			}
		}
		ofPopMatrix();
		ofPopStyle();
	}
};

struct DistGroup {
	std::vector<std::shared_ptr<Dist>> dists_;
	ofxPanel panel_;
	DistGroup(std::vector<std::shared_ptr<Dist>> dists)
		: dists_(dists) { }

	auto draw(std::string label, int square, int gap) {
		panel_.draw();
		ofPushMatrix();
		{
			ofTranslate(panel_.getPosition());
			ofDrawBitmapString(label, 0, -10);
			ofTranslate(panel_.getWidth() + 20, 0);
			for (const auto & dist : dists_) {
				dist->draw(0, 0, square, square);
				ofTranslate(0, square + gap);
			}
		}
		ofPopMatrix();
	}
};

#endif /* DIST_HPP */
