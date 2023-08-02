#ifndef OF_RANDOM_HPP_
#define OF_RANDOM_HPP_

#include <random>

#include "ofSingleton.hpp"
#include <glm/glm.hpp>

// https://stackoverflow.com/questions/34934480/how-efficient-is-it-to-make-a-temporary-uniform-random-distribution-each-time-ro
// https://simplecxx.github.io/2018/11/03/seed-mt19937.html

namespace of::random
{

class Engine: public of::utils::Singleton<Engine> {
	std::random_device rd_{ };
	std::seed_seq seq_ { rd_(), rd_(), rd_(), rd_() };
	std::mt19937 g_{ seq_ };
	bool deterministic_ { false };
public:
	auto & generator() { return g_; }
	auto seed(unsigned long new_seed) {
		g_.seed(new_seed);
		deterministic_ = true;
	}
};

auto seed(unsigned long seed) {
	of::random::Engine::instance()->seed(seed);
}

// MARK: - DISTRIBUTIONS -

// special case of distinguishing real and integer for uniform distribution

template<typename T = float, typename ... Args>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(Args&&... args) {
	std::uniform_real_distribution<T> distr(std::forward<Args>(args)...);
	return distr(of::random::Engine::instance()->generator());
}

template<typename T, typename ... Args>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(Args&&... args) {
	std::uniform_int_distribution<T> distr(std::forward<Args>(args)...);
	return distr(of::random::Engine::instance()->generator());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
uniform(double min, double max) {
	std::uniform_real_distribution<float> dist(min,max);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
uniform(T min, T max) {
	return {
		std::uniform_real_distribution<float>{min.x, max.x}(of::random::Engine::instance()->generator()),
		std::uniform_real_distribution<float>{min.y, max.y}(of::random::Engine::instance()->generator())
	};
}

template <typename T, typename ... Args>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
uniform(Args&&... args) {
	std::uniform_real_distribution<float> dist(std::forward<Args>(args)...);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
uniform(T min, T max) {
	return {
		std::uniform_real_distribution<float>{min.x, max.x}(of::random::Engine::instance()->generator()),
		std::uniform_real_distribution<float>{min.y, max.y}(of::random::Engine::instance()->generator()),
		std::uniform_real_distribution<float>{min.z, max.z}(of::random::Engine::instance()->generator())
	};
}

// MARK: -

// better to cast params as double (otherwise ints produce unexpected results)

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
normal(double mean, double stddev) {
	return std::normal_distribution{mean, stddev}(of::random::Engine::instance()->generator());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
normal(double mean, double stddev) {
	std::normal_distribution dist(mean, stddev);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
normal(T mean, T stddev) {
	return {
		std::normal_distribution{mean.x, stddev.x}(of::random::Engine::instance()->generator()),
		std::normal_distribution{mean.y, stddev.y}(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
normal(double mean, double stddev) {
	std::normal_distribution dist(mean, stddev);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
normal(T mean, T stddev) {
	return {
		std::normal_distribution{mean.x, stddev.x}(of::random::Engine::instance()->generator()),
		std::normal_distribution{mean.y, stddev.y}(of::random::Engine::instance()->generator()),
		std::normal_distribution{mean.z, stddev.z}(of::random::Engine::instance()->generator())
	};
}

// MARK: -

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
gamma(double alpha, double beta) {
	return std::gamma_distribution{alpha, beta}(of::random::Engine::instance()->generator());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
gamma(double alpha, double beta) {
	std::gamma_distribution dist(alpha, beta);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
gamma(T alpha, T beta) {
	return {
		std::gamma_distribution{alpha.x, beta.x}(of::random::Engine::instance()->generator()),
		std::gamma_distribution{alpha.y, beta.y}(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
gamma(double alpha, double beta) {
	std::gamma_distribution dist(alpha, beta);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
gamma(T alpha, T beta) {
	return {
		std::gamma_distribution{alpha.x, beta.x}(of::random::Engine::instance()->generator()),
		std::gamma_distribution{alpha.y, beta.y}(of::random::Engine::instance()->generator()),
		std::gamma_distribution{alpha.z, beta.z}(of::random::Engine::instance()->generator())
	};
}
// MARK: -

// https://www.theanalysisfactor.com/differences-between-normal-and-poisson-distributions/#:~:text=Both%20are%20discrete%20and%20bounded,tail%20extending%20to%20the%20right.

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
poisson(double mean) {
	return std::poisson_distribution{mean}(of::random::Engine::instance()->generator());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
poisson(double mean) {
	std::poisson_distribution dist(mean);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
poisson(T mean) {
	return {
		std::poisson_distribution{mean.x}(of::random::Engine::instance()->generator()),
		std::poisson_distribution{mean.y}(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
poisson(double mean) {
	std::poisson_distribution dist(mean);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
poisson(T mean) {
	return {
		std::poisson_distribution{mean.x}(of::random::Engine::instance()->generator()),
		std::poisson_distribution{mean.y}(of::random::Engine::instance()->generator()),
		std::poisson_distribution{mean.z}(of::random::Engine::instance()->generator())
	};
}

// MARK: -

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
exponential(double lambda) {
	return std::exponential_distribution{lambda}(of::random::Engine::instance()->generator());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
exponential(double lambda) {
	std::exponential_distribution dist(lambda);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
exponential(T lambda) {
	return {
		std::exponential_distribution{lambda.x}(of::random::Engine::instance()->generator()),
		std::exponential_distribution{lambda.y}(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
exponential(double lambda) {
	std::exponential_distribution dist(lambda);
	return {
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator()),
		dist(of::random::Engine::instance()->generator())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
exponential(T lambda) {
	return {
		std::exponential_distribution{lambda.x}(of::random::Engine::instance()->generator()),
		std::exponential_distribution{lambda.y}(of::random::Engine::instance()->generator()),
		std::exponential_distribution{lambda.z}(of::random::Engine::instance()->generator())
	};
}

} // end namespace of::random

// MARK: - ANONYMOUS NAMESPACE FUNCTIONS

namespace {
auto ofSetRandomSeed(unsigned long new_seed) {
	of::random::seed(new_seed);
}

template<class T, typename ... Args>
T ofRandomUniform(Args&&... args) { return of::random::uniform<T>(std::forward<Args>(args)...); }

// for the convenience of implicit brace init of glm::vec parameters (not deduced by the above forward)
template<class T>
T ofRandomUniform(T min, T max) { return of::random::uniform<T>(min, max); }

// MARK: - PRACTICAL WRAPPERS

// https://blogs.sas.com/content/iml/2019/07/22/extreme-value-normal-data.html

template<class T = float> // works for all non-refined
T ofRandomNormalLimits(float min, float max, float focus = 4.0f) {
	if (min >= max) {
		std::cout << "ofRandomNormalLimits()" << "max must be > than min\n";
		return {};
	} else {
		if (focus <= .0099) {
			std::cout << "ofRandomNormalLimits()" << "focus must be at least .01\n";
			return {};
		} else {
			T v;
			do { v = of::random::normal<T>((max+min)/2.0f, (max-min)/(2*focus)); } while (v < min || v > max);
			return v;
		}
	}
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
ofRandomNormalLimits(T min, T max, T focus = {4.0f, 4.0f}) {
	if (!min.x < max.x || !min.y < max.y) {
		std::cout << "ofRandomNormalLimits()" << "max must be > than min\n";
		return {};
	} else {
		if (focus.x < 1 || focus.y < 1 ) {
			std::cout << "ofRandomNormalLimits()" << "focus must be at least 1\n";
			return {};
		} else {
			T v;
			do { v.x = of::random::normal<float>((max.x+min.x)/2.0f, (max.x-min.x)/(2*focus.x)); } while (v.x < min || v.x > max);
			do { v.y = of::random::normal<float>((max.y+min.y)/2.0f, (max.y-min.y)/(2*focus.y)); } while (v.y < min || v.y > max);
			return v;
		}
	}
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
ofRandomNormalLimits(T min, T max, T focus = {4.0f, 4.0f, 4.0f}) {
	if (!min.x < max.x || !min.y < max.y || !min.z < max.z) {
		std::cout << "ofRandomNormalLimits()" << "max must be > than min\n";
		return {};
	} else {
		if (focus.x < 1 || focus.y < 1 || focus.z < 1) {
			std::cout << "ofRandomNormalLimits()" << "focus must be at least 1\n";
			return {};
		} else {
			T v;
			do { v.x = of::random::normal<float>((max.x+min.x)/2.0f, (max.x-min.x)/(2*focus.x)); } while (v.x < min || v.x > max);
			do { v.y = of::random::normal<float>((max.y+min.y)/2.0f, (max.y-min.y)/(2*focus.y)); } while (v.y < min || v.y > max);
			do { v.z = of::random::normal<float>((max.z+min.z)/2.0f, (max.z-min.z)/(2*focus.z)); } while (v.z < min || v.z > max);
			return v;
		}
	}
}

} // end anonymous namespace
#endif // OF_RANDOM_HPP_
