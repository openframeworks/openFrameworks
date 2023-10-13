#ifndef OF_RANDOM_DISTRIBUTIONS_H_
#define OF_RANDOM_DISTRIBUTIONS_H_

#include "ofRandomEngine.h"

// https://gist.github.com/imneme/540829265469e673d045
// https://github.com/effolkronium/random/tree/master
// https://www.theanalysisfactor.com/differences-between-normal-and-poisson-distributions
// https://blogs.sas.com/content/iml/2019/07/22/extreme-value-normal-data.html
// https://rovdownloads.com/blog/quick-overview-of-probability-distributions/

namespace of::random {

// MARK: - TEMPLATES OF TEMPLATES

template <typename T, typename Distribution>
T make_vector(Distribution dist) {
	if constexpr (std::is_same_v<T, glm::vec2>) {
		return {
			dist(of::random::gen()),
			dist(of::random::gen())
		};
	} else if constexpr (std::is_same_v<T, glm::vec3>) {
		return {
			dist(of::random::gen()),
			dist(of::random::gen()),
			dist(of::random::gen())
		};
	} else if constexpr (std::is_same_v<T, glm::vec4>) {
		return {
			dist(of::random::gen()),
			dist(of::random::gen()),
			dist(of::random::gen()),
			dist(of::random::gen())
		};
	}
}

template <typename T, typename Distribution, class Q>
T make_vector(Q a) {
	if constexpr (std::is_arithmetic_v<Q>) {
		return make_vector<T>(Distribution { a });
	} else {
		if constexpr (std::is_same_v<T, glm::vec2>) {
			return {
				Distribution { a.x }(of::random::gen()),
				Distribution { a.y }(of::random::gen())
			};
		} else if constexpr (std::is_same_v<T, glm::vec3>) {
			return {
				Distribution {
					a.x,
				}(of::random::gen()),
				Distribution { a.y }(of::random::gen()),
				Distribution { a.z }(of::random::gen())
			};
		} else if constexpr (std::is_same_v<T, glm::vec4>) {
			return {
				Distribution { a.x }(of::random::gen()),
				Distribution { a.y }(of::random::gen()),
				Distribution { a.z }(of::random::gen()),
				Distribution { a.w }(of::random::gen())
			};
		}
	}
}

template <typename T, typename Distribution, class Q, class R>
T make_vector(Q a, R b) {
	if constexpr (std::is_arithmetic_v<Q>) {
		return make_vector<T>(Distribution { a, b });

	} else {
		if constexpr (std::is_same_v<T, glm::vec2>) {
			return {
				Distribution { a.x, b.x }(of::random::gen()),
				Distribution { a.y, b.y }(of::random::gen())
			};
		} else if constexpr (std::is_same_v<T, glm::vec3>) {
			return {
				Distribution { a.x, b.x }(of::random::gen()),
				Distribution { a.y, b.y }(of::random::gen()),
				Distribution { a.z, b.z }(of::random::gen())
			};
		} else if constexpr (std::is_same_v<T, glm::vec4>) {
			return {
				Distribution { a.x, b.x }(of::random::gen()),
				Distribution { a.y, b.y }(of::random::gen()),
				Distribution { a.z, b.z }(of::random::gen()),
				Distribution { a.w, b.w }(of::random::gen())
			};
		}
	}
}

// MARK: - UNIFORM

// special case of distinguishing real and integer for uniform distribution

template <typename T = float>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(T max = 1.0) {
	std::uniform_real_distribution<T> distr(0, max);
	return distr(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(T max) {
	std::uniform_int_distribution<T> distr(0, max);
	return distr(of::random::gen());
}

template <typename T = float>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(T min, T max) {
	std::uniform_real_distribution<T> distr(min, max);
	return distr(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(T min, T max) {
	std::uniform_int_distribution<T> distr(min, max);
	return distr(of::random::gen());
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
uniform(float min, float max) {
	return make_vector<T, std::uniform_real_distribution<float>>(min, max);
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
uniform(float max = 1.0) {
	return make_vector<T, std::uniform_real_distribution<float>>(0.0f, max);
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
uniform(T min, T max) {
	return make_vector<T, std::uniform_real_distribution<float>>(min, max);
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

template <typename T, typename... Args>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
normal(float mean, float stddev) {
	return make_vector<T, std::normal_distribution<float>>(mean, stddev);
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
normal(T mean, T stddev) {
	return make_vector<T, std::normal_distribution<float>>(mean, stddev);
}

// MARK: LOGNORMAL

template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
lognormal(double mean, double stddev) {
	return std::lognormal_distribution { mean, stddev }(of::random::gen());
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
lognormal(float mean, float stddev) {
	return make_vector<T, std::lognormal_distribution<float>>(mean, stddev);
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
lognormal(T mean, T stddev) {
	return make_vector<T, std::lognormal_distribution<float>>(mean, stddev);
}

// MARK: GAMMA

template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
gamma(double alpha, double beta) {
	return std::gamma_distribution { alpha, beta }(of::random::gen());
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
gamma(float alpha, float beta) {
	return make_vector<T, std::gamma_distribution<float>>(alpha, beta);
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
gamma(T alpha, T beta) {
	return make_vector<T, std::gamma_distribution<float>>(alpha, beta);
}

// MARK: POISSON

template <typename T = int>
std::enable_if_t<std::is_arithmetic_v<T>, T>
poisson(double mean) {
	return std::poisson_distribution<T> { mean }(of::random::gen());
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
poisson(double mean) {
	return make_vector<T, std::poisson_distribution<int>>(mean);
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
poisson(T mean) {
	return make_vector<T, std::poisson_distribution<int>>(mean);
}

// MARK: EXPONENTIAL

template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
exponential(T lambda) {
	return std::exponential_distribution<T> { lambda }(of::random::gen());
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
exponential(float lambda) {
	return make_vector<T, std::exponential_distribution<float>>(lambda);
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
exponential(T lambda) {
	return make_vector<T, std::exponential_distribution<float>>(lambda);
}

// MARK: CHI_SQUARED

template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
chi_squared(double freedom) {
	std::chi_squared_distribution dist(freedom);
	return dist(of::random::gen());
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
chi_squared(float freedom) {
	return make_vector<T, std::chi_squared_distribution<float>>(freedom);
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
chi_squared(T freedom) {
	return make_vector<T, std::chi_squared_distribution<float>>(freedom);
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
std::enable_if_t<!std::is_arithmetic_v<T>, T>
binomial(int p, double t) {
	return make_vector<T, std::binomial_distribution<int>>(p, t);
}

// binomial is restrictive on param p and needs a bit of coercing vs glm::vec

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
binomial(T p, T t) {
	return {
		std::binomial_distribution { int(p.x), t.x }(of::random::gen()),
		std::binomial_distribution { int(p.y), t.y }(of::random::gen())
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
std::enable_if_t<!std::is_arithmetic_v<T>, T>
geometric() {
	return make_vector<T>(std::geometric_distribution<int> {});
}

// MARK: BERNOULLI

template <typename T = bool>
std::enable_if_t<std::is_arithmetic_v<T>, bool>
bernoulli(double p) {
	return std::bernoulli_distribution(p)(of::random::gen());
}

template <typename T, typename Q>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
bernoulli(Q p) {
	return make_vector<T, std::bernoulli_distribution>(p);
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
