#ifndef OF_RANDOM_DISTRIBUTIONS_H_
#define OF_RANDOM_DISTRIBUTIONS_H_

#include "ofRandomEngine.h"

// https://gist.github.com/imneme/540829265469e673d045
// https://github.com/effolkronium/random/tree/master
// https://www.theanalysisfactor.com/differences-between-normal-and-poisson-distributions
// https://blogs.sas.com/content/iml/2019/07/22/extreme-value-normal-data.html
// https://rovdownloads.com/blog/quick-overview-of-probability-distributions/

namespace of::random {

// MARK: - UNIFORM

// special case of distinguishing real and integer for uniform distribution

template <typename T = float, typename... Args>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(T max = 1.0) {
	std::uniform_real_distribution<T> distr(0, max);
	return distr(of::random::gen());
}

template <typename T, typename... Args>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(T max) {
	std::uniform_int_distribution<T> distr(0, max);
	return distr(of::random::gen());
}

template <typename T = float, typename... Args>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(T min, T max) {
	std::uniform_real_distribution<T> distr(min, max);
	return distr(of::random::gen());
}

template <typename T, typename... Args>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(T min, T max) {
	std::uniform_int_distribution<T> distr(min, max);
	return distr(of::random::gen());
}

template <typename T, typename... Args>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
uniform(Args &&... args) {
	std::uniform_real_distribution<float> dist(std::forward<Args>(args)...);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
uniform(T min, T max) {
	return {
		std::uniform_real_distribution<float> { min.x, max.x }(of::random::gen()),
		std::uniform_real_distribution<float> { min.y, max.y }(of::random::gen())
	};
}

template <typename T, typename... Args>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
uniform(Args &&... args) {
	std::uniform_real_distribution<float> dist(std::forward<Args>(args)...);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
uniform(T min, T max) {
	return {
		std::uniform_real_distribution<float> { min.x, max.x }(of::random::gen()),
		std::uniform_real_distribution<float> { min.y, max.y }(of::random::gen()),
		std::uniform_real_distribution<float> { min.z, max.z }(of::random::gen())
	};
}

template <typename T, typename... Args>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
uniform(Args &&... args) {
	std::uniform_real_distribution<float> dist(std::forward<Args>(args)...);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
uniform(T min, T max) {
	return {
		std::uniform_real_distribution<float> { min.x, max.x }(of::random::gen()),
		std::uniform_real_distribution<float> { min.y, max.y }(of::random::gen()),
		std::uniform_real_distribution<float> { min.z, max.z }(of::random::gen()),
		std::uniform_real_distribution<float> { min.w, max.w }(of::random::gen())
	};
}

// MARK: NORMAL (gaussian)

// better to cast params as double (otherwise casual ints produce unexpected results)

template <typename T = float>
std::enable_if_t<std::is_floating_point_v<T>, T>
normal(double mean, double stddev) {
	return std::normal_distribution<T>(mean, stddev)(of::random::gen());
}

// use double for integer types
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T>
normal(double mean, double stddev) {
	return std::normal_distribution<double>(mean, stddev)(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
normal(double mean, double stddev) {
	std::normal_distribution<float> dist(mean, stddev);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
normal(T mean, T stddev) {
	return {
		std::normal_distribution<float> { mean.x, stddev.x }(of::random::gen()),
		std::normal_distribution<float> { mean.y, stddev.y }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
normal(double mean, double stddev) {
	std::normal_distribution<float> dist(mean, stddev);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
normal(T mean, T stddev) {
	return {
		std::normal_distribution<float> { mean.x, stddev.x }(of::random::gen()),
		std::normal_distribution<float> { mean.y, stddev.y }(of::random::gen()),
		std::normal_distribution<float> { mean.z, stddev.z }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
normal(double mean, double stddev) {
	std::normal_distribution<float> dist(mean, stddev);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
normal(T mean, T stddev) {
	return {
		std::normal_distribution<float> { mean.x, stddev.x }(of::random::gen()),
		std::normal_distribution<float> { mean.y, stddev.y }(of::random::gen()),
		std::normal_distribution<float> { mean.z, stddev.z }(of::random::gen()),
		std::normal_distribution<float> { mean.w, stddev.w }(of::random::gen())
	};
}

// MARK: LOGNORMAL

template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
lognormal(double mean, double stddev) {
	return std::lognormal_distribution { mean, stddev }(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
lognormal(double mean, double stddev) {
	std::lognormal_distribution dist(mean, stddev);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
lognormal(T mean, T stddev) {
	return {
		std::lognormal_distribution { mean.x, stddev.x }(of::random::gen()),
		std::lognormal_distribution { mean.y, stddev.y }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
lognormal(double mean, double stddev) {
	std::lognormal_distribution dist(mean, stddev);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
lognormal(T mean, T stddev) {
	return {
		std::lognormal_distribution { mean.x, stddev.x }(of::random::gen()),
		std::lognormal_distribution { mean.y, stddev.y }(of::random::gen()),
		std::lognormal_distribution { mean.z, stddev.z }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
lognormal(double mean, double stddev) {
	std::lognormal_distribution dist(mean, stddev);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
lognormal(T mean, T stddev) {
	return {
		std::lognormal_distribution { mean.x, stddev.x }(of::random::gen()),
		std::lognormal_distribution { mean.y, stddev.y }(of::random::gen()),
		std::lognormal_distribution { mean.z, stddev.z }(of::random::gen()),
		std::lognormal_distribution { mean.w, stddev.w }(of::random::gen())
	};
}

// MARK: GAMMA

template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
gamma(double alpha, double beta) {
	return std::gamma_distribution { alpha, beta }(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
gamma(double alpha, double beta) {
	std::gamma_distribution dist(alpha, beta);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
gamma(T alpha, T beta) {
	return {
		std::gamma_distribution { alpha.x, beta.x }(of::random::gen()),
		std::gamma_distribution { alpha.y, beta.y }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
gamma(double alpha, double beta) {
	std::gamma_distribution dist(alpha, beta);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
gamma(T alpha, T beta) {
	return {
		std::gamma_distribution { alpha.x, beta.x }(of::random::gen()),
		std::gamma_distribution { alpha.y, beta.y }(of::random::gen()),
		std::gamma_distribution { alpha.z, beta.z }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
gamma(double alpha, double beta) {
	std::gamma_distribution dist(alpha, beta);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
gamma(T alpha, T beta) {
	return {
		std::gamma_distribution { alpha.x, beta.x }(of::random::gen()),
		std::gamma_distribution { alpha.y, beta.y }(of::random::gen()),
		std::gamma_distribution { alpha.z, beta.z }(of::random::gen()),
		std::gamma_distribution { alpha.w, beta.w }(of::random::gen())
	};
}

// MARK: POISSON

template <typename T = int>
std::enable_if_t<std::is_arithmetic_v<T>, T>
poisson(double mean) {
	return std::poisson_distribution<T> { mean }(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
poisson(double mean) {
	std::poisson_distribution dist(mean);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
poisson(T mean) {
	return {
		std::poisson_distribution { mean.x }(of::random::gen()),
		std::poisson_distribution { mean.y }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
poisson(double mean) {
	std::poisson_distribution dist(mean);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
poisson(T mean) {
	return {
		std::poisson_distribution { mean.x }(of::random::gen()),
		std::poisson_distribution { mean.y }(of::random::gen()),
		std::poisson_distribution { mean.z }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
poisson(double mean) {
	std::poisson_distribution dist(mean);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
poisson(T mean) {
	return {
		std::poisson_distribution { mean.x }(of::random::gen()),
		std::poisson_distribution { mean.y }(of::random::gen()),
		std::poisson_distribution { mean.z }(of::random::gen()),
		std::poisson_distribution { mean.w }(of::random::gen())
	};
}

// MARK: EXPONENTIAL

template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
exponential(T lambda) {
	return std::exponential_distribution<T> { lambda }(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
exponential(double lambda) {
	std::exponential_distribution dist(lambda);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
exponential(T lambda) {
	return {
		std::exponential_distribution { lambda.x }(of::random::gen()),
		std::exponential_distribution { lambda.y }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
exponential(double lambda) {
	std::exponential_distribution dist(lambda);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
exponential(T lambda) {
	return {
		std::exponential_distribution { lambda.x }(of::random::gen()),
		std::exponential_distribution { lambda.y }(of::random::gen()),
		std::exponential_distribution { lambda.z }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
exponential(double lambda) {
	std::exponential_distribution dist(lambda);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
exponential(T lambda) {
	return {
		std::exponential_distribution { lambda.x }(of::random::gen()),
		std::exponential_distribution { lambda.y }(of::random::gen()),
		std::exponential_distribution { lambda.z }(of::random::gen()),
		std::exponential_distribution { lambda.w }(of::random::gen())
	};
}

// MARK: CHI_SQUARED

template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
chi_squared(double freedom) {
	std::chi_squared_distribution dist(freedom);
	return dist(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
chi_squared(double freedom) {
	std::chi_squared_distribution dist(freedom);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
chi_squared(T freedom) {
	return {
		std::chi_squared_distribution { freedom.x }(of::random::gen()),
		std::chi_squared_distribution { freedom.y }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
chi_squared(double freedom) {
	std::chi_squared_distribution dist(freedom);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
chi_squared(T freedom) {
	return {
		std::chi_squared_distribution { freedom.x }(of::random::gen()),
		std::chi_squared_distribution { freedom.y }(of::random::gen()),
		std::chi_squared_distribution { freedom.z }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
chi_squared(double freedom) {
	std::chi_squared_distribution dist(freedom);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
chi_squared(T freedom) {
	return {
		std::chi_squared_distribution { freedom.x }(of::random::gen()),
		std::chi_squared_distribution { freedom.y }(of::random::gen()),
		std::chi_squared_distribution { freedom.z }(of::random::gen()),
		std::chi_squared_distribution { freedom.w }(of::random::gen())
	};
}

// MARK: BINOMIAL

template <typename T = int>
std::enable_if_t<std::is_arithmetic_v<T>, T>
binomial(int p, double t) {
	if (t >= 1) {
		std::cout << "of::random::binomial(): t must be < 1.0\n";
		return 0;
	} else {
		return std::binomial_distribution { p, t }(of::random::gen());
	}
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
binomial(int p, double t) {
	std::binomial_distribution<int> dist(p, t);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
binomial(T p, T t) {
	return {
		std::binomial_distribution { int(p.x), t.x }(of::random::gen()),
		std::binomial_distribution { int(p.y), t.y }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
binomial(int p, double t) {
	std::binomial_distribution<int> dist(p, t);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T = int>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
binomial(T p, T t) {
	return {
		std::binomial_distribution { int(p.x), t.x }(of::random::gen()),
		std::binomial_distribution { int(p.y), t.y }(of::random::gen()),
		std::binomial_distribution { int(p.z), t.z }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
binomial(int p, double t) {
	std::binomial_distribution<int> dist(p, t);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T = int>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
binomial(T p, T t) {
	return {
		std::binomial_distribution { int(p.x), t.x }(of::random::gen()),
		std::binomial_distribution { int(p.y), t.y }(of::random::gen()),
		std::binomial_distribution { int(p.z), t.z }(of::random::gen()),
		std::binomial_distribution { int(p.w), t.w }(of::random::gen())
	};
}

// MARK: GEOMETRIC

template <typename T = int>
std::enable_if_t<std::is_arithmetic_v<T>, T>
geometric() {
	return std::geometric_distribution {}(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
geometric() {
	std::geometric_distribution<int> dist;
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
geometric() {
	std::geometric_distribution<int> dist;
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
geometric() {
	std::geometric_distribution<int> dist;
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

// MARK: BERNOUILLI

template <typename T = bool>
std::enable_if_t<std::is_arithmetic_v<T>, bool>
bernoulli(double p) {
	return std::bernoulli_distribution(p)(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
bernoulli(double p) {
	std::bernoulli_distribution dist(p);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
bernoulli(T p) {
	return {
		std::bernoulli_distribution { p.x }(of::random::gen()),
		std::bernoulli_distribution { p.y }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
bernoulli(double p) {
	std::bernoulli_distribution dist(p);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
bernoulli(T p) {
	return {
		std::bernoulli_distribution { p.x }(of::random::gen()),
		std::bernoulli_distribution { p.y }(of::random::gen()),
		std::bernoulli_distribution { p.z }(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
bernoulli(double p) {
	std::bernoulli_distribution dist(p);
	return {
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
bernoulli(T p) {
	return {
		std::bernoulli_distribution { p.x }(of::random::gen()),
		std::bernoulli_distribution { p.y }(of::random::gen()),
		std::bernoulli_distribution { p.z }(of::random::gen()),
		std::bernoulli_distribution { p.w }(of::random::gen())
	};
}

// MARK: - CONVENIENT FORWARDING ALIASES

template <class T = float, typename... Args>
constexpr auto gaussian(Args &&... args) -> decltype(normal<T>(std::forward<Args>(args)...)) {
	return normal<T>(std::forward<Args>(args)...);
}

template <class T = bool, typename... Args>
constexpr auto yes(Args &&... args) -> decltype(bernoulli<T>(std::forward<Args>(args)...)) {
	return bernoulli<T>(std::forward<Args>(args)...);
}

// whilst nerdy enough, the above do not work with {initializer list} for glm::vec parameters such as:
// of::random::gaussian<glm::vec2>({0,10},{1,100});
// so this additional explicit nerdiness is required to wrap them:

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
gaussian(T min, T max) {
	return normal<T>(min, max);
}

// MARK: - OTHER FUNCTIONS

template <class T = float> // works for all non-refined
T bound_normal(float min, float max, float focus = 4.0f) {
	if (min >= max) {
		std::cout << "ofRandomNormalLimits()"
				  << "max must be > than min\n";
		return {};
	} else {
		if (focus <= .0099) {
			std::cout << "ofRandomNormalLimits()"
					  << "focus must be at least .01\n";
			return {};
		} else {
			T v;
			do {
				v = of::random::normal<T>((max + min) / 2.0f, (max - min) / (2 * focus));
			} while (v < min || v > max);
			return v;
		}
	}
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
bound_normal(T min, T max, T focus = { 4.0f, 4.0f }) {
	if (min.x >= max.x || min.y >= max.y) {
		std::cout << "ofRandomNormalLimits()"
				  << "max must be > than min\n";
		return {};
	} else {
		if (focus.x < 1 || focus.y < 1) {
			std::cout << "ofRandomNormalLimits()"
					  << "focus must be at least 1\n";
			return {};
		} else {
			T v;
			do {
				v.x = of::random::normal<float>((max.x + min.x) / 2.0f, (max.x - min.x) / (2 * focus.x));
			} while (v.x < min.x || v.x > max.x);
			do {
				v.y = of::random::normal<float>((max.y + min.y) / 2.0f, (max.y - min.y) / (2 * focus.y));
			} while (v.y < min.y || v.y > max.y);
			return v;
		}
	}
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
bound_normal(T min, T max, T focus = { 4.0f, 4.0f, 4.0f }) {
	if (min.x >= max.x || min.y >= max.y || min.z >= max.z) {
		std::cout << "ofRandomNormalLimits()"
				  << "max must be > than min\n";
		return {};
	} else {
		if (focus.x < 1 || focus.y < 1 || focus.z < 1) {
			std::cout << "ofRandomNormalLimits()"
					  << "focus must be at least 1\n";
			return {};
		} else {
			T v;
			do {
				v.x = of::random::normal<float>((max.x + min.x) / 2.0f, (max.x - min.x) / (2 * focus.x));
			} while (v.x < min.x || v.x > max.x);
			do {
				v.y = of::random::normal<float>((max.y + min.y) / 2.0f, (max.y - min.y) / (2 * focus.y));
			} while (v.y < min.y || v.y > max.y);
			do {
				v.z = of::random::normal<float>((max.z + min.z) / 2.0f, (max.z - min.z) / (2 * focus.z));
			} while (v.z < min.z || v.z > max.z);
			return v;
		}
	}
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
bound_normal(T min, T max, T focus = { 4.0f, 4.0f, 4.0f, 4.0f }) {
	if (min.x >= max.x || min.y >= max.y || min.z >= max.z || min.w >= max.w) {
		std::cout << "ofRandomNormalLimits()"
				  << "max must be > than min\n";
		return {};
	} else {
		if (focus.x < 1 || focus.y < 1 || focus.z < 1 || focus.w < 1) {
			std::cout << "ofRandomNormalLimits()"
					  << "focus must be at least 1\n";
			return {};
		} else {
			T v;
			do {
				v.x = of::random::normal<float>((max.x + min.x) / 2.0f, (max.x - min.x) / (2 * focus.x));
			} while (v.x < min.x || v.x > max.x);
			do {
				v.y = of::random::normal<float>((max.y + min.y) / 2.0f, (max.y - min.y) / (2 * focus.y));
			} while (v.y < min.y || v.y > max.y);
			do {
				v.z = of::random::normal<float>((max.z + min.z) / 2.0f, (max.z - min.z) / (2 * focus.z));
			} while (v.z < min.z || v.z > max.z);
			return v;
		}
	}
}

} // end namespace of::random

// MARK: - ANONYMOUS NAMESPACE FUNCTIONS

namespace {

// again, a few templates per function, for the convenience of implicit brace init of glm::vec parameters

float ofRandomUniform() { return of::random::uniform<float>(0, 1); }

template <class T, typename... Args>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
ofRandomUniform(Args &&... args) { return of::random::uniform<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomUniform(T max) { return of::random::uniform<T>(0, max); }

template <class T>
T ofRandomUniform(T min, T max) { return of::random::uniform<T>(min, max); }

template <class T, typename... Args>
T ofRandomNormal(Args &&... args) { return of::random::normal<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomNormal(T min, T max) { return of::random::normal<T>(min, max); }

template <class T, typename... Args>
T ofRandomGaussian(Args &&... args) { return of::random::gaussian<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomGaussian(T mean, T stddev) { return of::random::gaussian<T>(mean, stddev); }

template <class T, typename... Args>
T ofRandomBernoulli(Args &&... args) { return of::random::bernoulli<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomBernoulli(T prob) { return of::random::bernoulli<T>(prob); }

template <class T, typename... Args>
T ofRandomYes(Args &&... args) { return of::random::yes<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomYes(T prob) { return of::random::yes<T>(prob); }

template <class T, typename... Args>
T ofRandomPoisson(Args &&... args) { return of::random::poisson<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomPoisson(T mean) { return of::random::poisson<T>(mean); }

template <class T, typename... Args>
T ofRandomExponential(Args &&... args) { return of::random::exponential<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomExponential(T lambda) { return of::random::exponential<T>(lambda); }

template <class T, typename... Args>
T ofRandomChiSquared(Args &&... args) { return of::random::chi_squared<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomChiSquared(T freedom) { return of::random::chi_squared<T>(freedom); }

template <class T, typename... Args>
T ofRandomGamma(Args &&... args) { return of::random::gamma<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomGamma(T a, T b) { return of::random::gamma<T>(a, b); }

template <class T, typename... Args>
T ofRandomLognormal(Args &&... args) { return of::random::lognormal<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomLognormal(T mean, T stddev) { return of::random::lognormal<T>(mean, stddev); }

template <class T = int, typename... Args>
T ofRandomBinomial(Args &&... args) { return of::random::binomial<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomBinomial(T mean, T stddev) { return of::random::binomial<T>(mean, stddev); }

template <class T = int, typename... Args>
T ofRandomGeometric(Args &&... args) { return of::random::geometric<T>(std::forward<Args>(args)...); }

template <class T>
T ofRandomGeometric() { return of::random::geometric<T>(); }

template <class T = float>
T ofRandomBoundNormal(float min, float max, float focus = 4.0f) {
	return of::random::bound_normal(min, max, focus);
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
ofRandomBoundNormal(T min, T max, T focus = { 4.0f, 4.0f }) {
	return of::random::bound_normal(min, max, focus);
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
ofRandomBoundNormal(T min, T max, T focus = { 4.0f, 4.0f, 4.0f }) {
	return of::random::bound_normal(min, max, focus);
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
ofRandomBoundNormal(T min, T max, T focus = { 4.0f, 4.0f, 4.0f, 4.0f }) {
	return of::random::bound_normal(min, max, focus);
}

} // end anonymous namespace
#endif // OF_RANDOM_DISTRIBUTIONS_H_
