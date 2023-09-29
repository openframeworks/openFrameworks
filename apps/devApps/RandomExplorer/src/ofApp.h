#pragma once

#include "ofMain.h"
#include "Dist.hpp"
#include "OKColor.h"
#include "ofxGui.h"

using namespace std::string_literals;

class ofApp : public ofBaseApp {

	float ofOldRandom(float x, float y) {
		float high = std::max(x, y);
		float low = std::min(x, y);
		return std::max(low, (low + ((high - low) * rand() / float(RAND_MAX))) * (1.0f - std::numeric_limits<float>::epsilon()));
	}

	ofxPanel panel_;
	std::vector<std::shared_ptr<ofxPanel>> group_panels_;
	ofParameter<size_t> size_ { "size (cube root)", 25, 1, 50 };
	ofParameter<unsigned long> seed_ { "seed", 0, 0, 1000 };
	ofParameter<void> reinit_ { "re-init engine" };
	ofParameter<bool> ok_color_ { "ok_color" };
	ofParameter<float> saturation_ { "saturation", 0.9 };
	ofParameter<float> value_ { "value", .25 };
	ofParameter<float> offset_ { "offset", 0 };

	size_t col_w_ = 640;
	size_t square_ = 110;
	size_t gap_ = 20;

	std::array<uint8_t, 8> dna_;
	std::vector<uint8_t> shuffle_ { 1, 2, 3, 4, 5, 6, 7, 8 };
	std::string dna_string_;
	std::string shuffle_string_;
	std::string size_string_;

	ofParameter<float> rand_min_ { "min", 0, 0, 100 };
	ofParameter<float> rand_max_ { "max", 100, 0, 100 };
	ofParameter<float> uni_min_ { "min", 0, 0, 100 };
	ofParameter<float> uni_max_ { "max", 100, 0, 100 };
	ofParameter<int> uni_int_min_ { "min", 1, 0, 9 };
	ofParameter<int> uni_int_max_ { "max", 6, 1, 10 };
	ofParameter<float> norm_mean_ { "mean", 100, 0, 200 };
	ofParameter<float> norm_dev_ { "stddev", 20, 0, 50 };
	ofParameter<float> log_mean_ { "mean", 1.6, 0, 5 };
	ofParameter<float> log_dev_ { "stddev", 0.25, 0, 10 };
	ofParameter<float> bn_min_ { "min", 35, 0, 200 };
	ofParameter<float> bn_max_ { "max", 165, 0, 200 };
	ofParameter<float> bn_focus_ { "focus", 1.75, 0.01, 20 };
	ofParameter<float> gamma_alpha_ { "alpha", 3, 1, 20 };
	ofParameter<float> gamma_beta_ { "beta", 5, 0, 50 };
	ofParameter<float> poiss_mean_ { "mean", 4, 0, 20 };
	ofParameter<float> exp_lambda_ { "lambda", .1, 0.01, 0.5 };
	ofParameter<glm::vec2> vec_min_ { "min", { 40, 10 }, { 0, 0 }, { 200, 200 } };
	ofParameter<glm::vec2> vec_max_ { "max", { 60, 90 }, { 0, 0 }, { 200, 200 } };
	ofParameter<glm::vec2> vec_mean_ { "mean", { 50, 50 }, { 0, 0 }, { 100, 100 } };
	ofParameter<glm::vec2> vec_dev_ { "stddev", { 50, 15 }, { 0, 0 }, { 50, 50 } };
	ofParameter<float> vec_gamma_a_ { "alpha", 1.5, 1, 20 };
	ofParameter<float> vec_gamma_b_ { "beta", 10, 0, 50 };
	ofParameter<float> yes_ { "yes", .33, 0, 1 };
	ofParameter<int> bin_p_ { "p", 5, 0, 40 };
	ofParameter<float> bin_t_ { "t", .5, 0, .99 };
	ofParameter<float> chi_n_ { "freedom", 4, 0, 10 };

	std::map<std::string, std::shared_ptr<DistGroup>> dists_ {
		{ "old",
			std::make_shared<DistGroup>(std::vector<std::shared_ptr<Dist>> {

				std::make_shared<ConcreteDist<float>>("rand()", "old-skool (thread risky)\nbased on srand()", std::nullopt, std::vector<ofAbstractParameter *> { &rand_min_, &rand_max_ },
					[&] { return ofOldRandom(rand_min_, rand_max_); }) }) },

		{ "core",
			std::make_shared<DistGroup>(std::vector<std::shared_ptr<Dist>> {

				std::make_shared<ConcreteDist<float>>("uniform <real>", "modern, thread safe\nright bound improbable", "uniform_real_distribution", std::vector<ofAbstractParameter *> { &uni_min_, &uni_max_ },
					[&] { return of::random::uniform<float>(uni_min_, uni_max_); }),

				std::make_shared<ConcreteDist<int>>(
					"uniform <int>", "unambiguously\nincludes bounds", "uniform_int_distribution", std::vector<ofAbstractParameter *> { &uni_int_min_, &uni_int_max_ },
					[&] { return of::random::uniform<int>(uni_int_min_, uni_int_max_); }, 11, glm::vec2 { 0, 10 }, true),

				std::make_shared<ConcreteDist<double>>(
					"normal", "also aliased to random::gaussian\for stochastic familiarity", "normal_distribution", std::vector<ofAbstractParameter *> { &norm_mean_, &norm_dev_ },
					[&] { return of::random::normal<double>(norm_mean_, norm_dev_); }, 101, glm::vec2 { 0, 200 }),

				std::make_shared<ConcreteDist<float>>("exponential", "all in the title", "exponential_distribution", std::vector<ofAbstractParameter *> { &exp_lambda_ },
					[&] { return of::random::exponential<float>(exp_lambda_); }),

				std::make_shared<ConcreteDist<int>>(
					"poisson", "for sparse, \npositive discrete\n(if mean > 12: ~= normal)", "poisson_distribution", std::vector<ofAbstractParameter *> { &poiss_mean_ },
					[&] { return of::random::poisson<int>(poiss_mean_); }, 21, glm::vec2 { 0, 20 }, true),

				// note int is used here instead of bool because of compiler complaints about "bit values" whatever
				std::make_shared<ConcreteDist<int>>(
					"bernoulli", "binary prob\ne.g. toss of a coin\naliased to of::random::yes", "bernoulli_distribution", std::vector<ofAbstractParameter *> { &yes_ },
					[&] { return of::random::bernoulli(yes_); }, 2, glm::vec2 { 0, 1 }, true) }) },

		{ "special",
			std::make_shared<DistGroup>(std::vector<std::shared_ptr<Dist>> {

				std::make_shared<ConcreteDist<int>>(
					"lognormal", "like normal but log", "lognormal_distribution", std::vector<ofAbstractParameter *> { &log_mean_, &log_dev_ },
					[&] { return of::random::lognormal<int>(log_mean_, log_dev_); }, 21, glm::vec2 { 0, 20 }, true),

				std::make_shared<ConcreteDist<float>>("gamma", "for special purposes", "gamma_distribution", std::vector<ofAbstractParameter *> { &gamma_alpha_, &gamma_beta_ },
					[&] { return of::random::gamma<int>(gamma_alpha_, gamma_beta_); }),

				std::make_shared<ConcreteDist<float>>(
					"chi-squared", "cool energy bulge", "chi_squared_distribution", std::vector<ofAbstractParameter *> { &chi_n_ },
					[&] { return of::random::chi_squared<float>(chi_n_); }, 101, glm::vec2 { 0, 20 }),

				std::make_shared<ConcreteDist<int>>(
					"binomial", "~normal for ints\nsquashes nicely on zero", "binomial_distribution", std::vector<ofAbstractParameter *> { &bin_p_, &bin_t_ },
					[&] { return of::random::binomial(bin_p_, bin_t_); }, 21, glm::vec2 { 0, 20 }, true),

				std::make_shared<ConcreteDist<int>>(
					"geometric", "~expon for sparse ints\nwith no parameter", "geometric_distribution", std::vector<ofAbstractParameter *> {},
					[&] { return of::random::geometric<int>(); }, 21, glm::vec2 { 0, 20 }, true) }) },
		
		{ "of",
			std::make_shared<DistGroup>(std::vector<std::shared_ptr<Dist>> {

				std::make_shared<ConcreteDist<float>>(
					"bound normal", "practical min/max\nwith enforced limits\n(allows to \"focus\")\nintuitiver", "normal_distribution", std::vector<ofAbstractParameter *> { &bn_min_, &bn_max_, &bn_focus_ }, [&] { return ofRandomBoundNormal<int>(bn_min_, bn_max_, bn_focus_); }, 101, glm::vec2 { 0, 200 }),

				std::make_shared<ConcreteDist<glm::vec2>>(
					"vec2 uniform/axis", "all generators can\nbe typed vec2, 3 or 4", "uniform_int_distribution", std::vector<ofAbstractParameter *> { &vec_min_, &vec_max_ }, [&] { return of::random::uniform<glm::vec2>(vec_min_, vec_max_); }, 101, glm::vec2 { 0, 200 }),

				std::make_shared<ConcreteDist<glm::vec2>>(
					"vec2 normal/axis", "vecN version can be\nrefined to vecN axis", "normal_distribution", std::vector<ofAbstractParameter *> { &vec_mean_, &vec_dev_ }, [&] { return of::random::gaussian<glm::vec2>(vec_mean_, vec_dev_); }, 101, glm::vec2 { 0, 200 }),

				std::make_shared<ConcreteDist<glm::vec3>>(
					"vec3 gamma", "", "gamma_distribution", std::vector<ofAbstractParameter *> { &vec_gamma_a_, &vec_gamma_b_ }, [&] { return of::random::gamma<glm::vec3>(vec_gamma_a_, vec_gamma_b_); }, 101, glm::vec2 { 0, 200 }) }) }
	};

public:
	auto rebuild_signatures() {
		dna_string_ = "1. first 8 numbers: ";
		for (size_t i = 0; i < dna_.size(); i++) {
			dna_[i] = of::random::uniform<uint8_t>(0, 255);
			dna_string_ += ofToUpper(ofToHex(dna_[i])) + " ";
		}

		shuffle_string_ = "2. shuffled vector: ";
		shuffle_ = { 1, 2, 3, 4, 5, 6, 7, 8 }; // initial order
		of::shuffle(shuffle_);
		for (size_t i = 0; i < shuffle_.size(); i++) {
			shuffle_string_ += ofToUpper(ofToHex(shuffle_[i])) + " ";
		}
	}

	auto seed(unsigned long & seed) {
		of::random::engine()->seed(seed);
		rebuild_signatures();
		panel_.getControl("seed")->setTextColor(ofColor::green);
	}

	auto reinit() {
		seed_ = 0;
		of::random::Engine::destruct();
		of::random::Engine::construct(); // from scratch
		rebuild_signatures();
		panel_.getControl("seed")->setTextColor(ofColor::gray);
	}

	auto colorize() {

		size_t sum = 0;
		for (auto & group : { dists_["core"], dists_["special"], dists_["of"] }) {
			sum += group->dists_.size();
		}
		auto chunk = 255.0 / sum;

		size_t i = 0; // to spread the hue; order below matters
		for (auto & group : { dists_["core"], dists_["special"], dists_["of"], dists_["old"] }) {
			for (auto & dist : group->dists_) {
				if (i >= sum) {
					dist->color_ = { 48, 48, 48, 255 }; // sad
				} else {
					if (ok_color_) {
						auto color = OKColor::okhsv_to_srgb({ float(fmod((((chunk * i++) / 256.0f) + offset_), 1)), saturation_.get(), value_.get() });
						dist->color_ = ofColor(color.r * 255.0, color.g * 255.0, color.b * 255.0);
					} else {
						dist->color_.setHsb(int((chunk * i++) + (offset_ * 255)) % 255, saturation_ * 255.0, value_ * 255.0);
					}
				}
				group->panel_.getGroup(dist->parameters_.getName()).setHeaderBackgroundColor(dist->color_);
			}
		}
	}

	auto colorize_float(float & v) { colorize(); } // for non-bool ofParameters
	auto colorize_bool(bool & v) { colorize(); } // for non-bool for ofParameters

	void setup() override;
	void update() override;
	void draw() override;
};
