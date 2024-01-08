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

/// \section flexible glm::vec2, vec3 and vec4 makers
///
/// \brief produces a T filled with distribution D
/// \tparam T the type of glm::vec
/// \tparam Distribution the type of std::distribution
/// \param dist the distribution itself
/// \returns a new vector T
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

/// \brief produces a vector T filled with distribution D requiring 1 parameter
/// either scalar (if A is arithmetic) or specialized (if A is a vector and of the same type as T)
/// \tparam T the type of glm::vec
/// \tparam Distribution the type of std::distribution
/// \tparam A the type of the configuration
/// \param a the configuration (scalar or vector)
/// \returns a new vector T
template <typename T, typename Distribution, class A>
T make_vector(A a) {
	if constexpr (std::is_arithmetic_v<A>) {
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

/// \brief produces a vector T filled with distribution D requiring 2 parameters
/// either scalar (if A is arithmetic) or specialized (if A is a vector and of the same type as T)
/// \tparam T the type of glm::vec
/// \tparam Distribution the type of std::distribution
/// \tparam A the type of the first configuration
/// \tparam B the type of the second configuration
/// \param a the first parameter configuration (scalar or vector)
/// \param b the second parameter configuration (scalar or vector)
/// \returns a new vector T
template <typename T, typename Distribution, class A, class B>
T make_vector(A a, B b) {
	if constexpr (std::is_arithmetic_v<A>) {
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

/// \section distribution wrappers

// special case of distinguishing real and integer for uniform distribution

/// \brief Generates a real uniform random number between 0 and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// /// \tparam T the desired real type
/// \param max the maximum value, defaults to 1
/// \return a T real value in the [0, max) range
template <typename T = float>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(T max = 1.0) {
	std::uniform_real_distribution<T> distr(0, max);
	return distr(of::random::gen());
}

/// \brief Generates an integral uniform random number between 0 and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
/// \tparam T the desired integer type
/// \param max the maximum value (no default for integral types)
/// \return a T integer value in the [0, max] range
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(T max) {
	std::uniform_int_distribution<T> distr(0, max);
	return distr(of::random::gen());
}

/// \brief Generates an real uniform random number between min and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// /// \tparam T the desired real type
/// \param min the minimum value (no default)
/// \param max the maximum value (no default)
/// \return a T real value in the [min, max) range
template <typename T = float>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(T min, T max) {
	std::uniform_real_distribution<T> distr(min, max);
	return distr(of::random::gen());
}

/// \brief Generates an integral uniform random number between min and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
/// \tparam T the desired integer type
/// \param min the minimum value (no default)
/// \param max the maximum value (no default)
/// \return a T integer value in the [min, max] range
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(T min, T max) {
	std::uniform_int_distribution<T> distr(min, max);
	return distr(of::random::gen());
}

/// \brief Generates a glm::vec of uniform random value between 0 and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// \tparam T the desired vector type
/// \param max the maximum value (default to 1.0)
/// \return a T vec of floats in the [0, max) range
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
uniform(float max = 1.0) {
	return make_vector<T, std::uniform_real_distribution<float>>(0.0f, max);
}

/// \brief Generates a glm::vec of uniform random value between min and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// \tparam T the desired vector type
/// \param min the minimum value (no default)
/// \param max the maximum value (no default)
/// \return a T vec of floats in the [min, max) range
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
uniform(float min, float max) {
	return make_vector<T, std::uniform_real_distribution<float>>(min, max);
}

/// \brief Generates a glm::vec of uniform random number
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// with specialized parameters (different params for members of the vector)
/// T, min and max must be of the same type
/// \tparam T the desired vector type
/// \param min the minimum values (no default)
/// \param max the maximum values (no default)
/// \return a T vec of uniform random floats in the [min, max) range
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
uniform(T min, T max) {
	return make_vector<T, std::uniform_real_distribution<float>>(min, max);
}

// MARK: NORMAL (gaussian)

// better to cast params as double (otherwise casual ints produce unexpected results)

/// \brief Generates a real normal random number 
/// /// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// (note that this is aliased to 'gaussian' too)
/// \tparam T the desired output type; defaults to float
/// \param mean the mean
/// \param stddev the standard deviation
/// \return a real T of normal random
template <typename T = float>
std::enable_if_t<std::is_floating_point_v<T>, T>
normal(double mean, double stddev) {
	return std::normal_distribution<T>(mean, stddev)(of::random::gen());
}

/// \brief Generates a integer random number
/// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// Template specialization is required to force the type of distribution (will not take an int type)
/// (note that this is aliased to 'gaussian' too)
/// \tparam T the desired output type
/// \param mean the mean
/// \param stddev the standard deviation
/// \return an integer T of normal random
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T>
normal(double mean, double stddev) {
	return std::normal_distribution<double>(mean, stddev)(of::random::gen());
}

/// \brief Generate a glm::vec of random numbers
/// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// (note that this is aliased to 'gaussian' too)
/// \tparam T the desired vector type
/// \param min the mean values
/// \param max the stddev values
/// \return a T vec of normally-distributed random floats with homogenous parameters
template <typename T, typename... Args>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
normal(float mean, float stddev) {
	return make_vector<T, std::normal_distribution<float>>(mean, stddev);
}

/// \brief Generates a glm::vec of random numbers 
/// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// with specialized parameters (different params for members of the vector)
/// where T, mean and stdev must be of the same type
/// \tparam T the desired vector type
/// \param min the mean values
/// \param max the stddev values
/// \return a T vec of normally-distributed random floats with distinct parameters
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
normal(T mean, T stddev) {
	return make_vector<T, std::normal_distribution<float>>(mean, stddev);
}

// MARK: LOGNORMAL

/// \brief Generates a  lognormal random number
/// https://en.cppreference.com/w/cpp/numeric/random/lognormal_distribution
/// \tparam T the desired output type; defaults to float
/// \param mean the mean
/// \param stddev the standard deviation
/// \return a  T of lognormal random
template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
lognormal(double mean, double stddev) {
	return std::lognormal_distribution { mean, stddev }(of::random::gen());
}

/// \brief Generates a glm::vec of lognormal random float values
/// https://en.cppreference.com/w/cpp/numeric/random/lognormal_distribution
/// \tparam T the desired vector type
/// \param mean the mean
/// \param stddev the standard deviation
/// \return a  T of lognormal homogeneous random float values
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
lognormal(float mean, float stddev) {
	return make_vector<T, std::lognormal_distribution<float>>(mean, stddev);
}

/// \brief Generates a glm::vec of lognormal random float values
/// https://en.cppreference.com/w/cpp/numeric/random/lognormal_distribution
/// with specialized parameters (different params for members of the vector)
/// \tparam T the desired vector type
/// \param mean the vector of mean parameters
/// \param stddev the vector standard deviation parameters
/// \return a vector T of lognormal specialized random float values
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
lognormal(T mean, T stddev) {
	return make_vector<T, std::lognormal_distribution<float>>(mean, stddev);
}

// MARK: GAMMA

/// \brief Generates random positive floating-point values x, distributed according to gamma distribution
/// https://en.cppreference.com/w/cpp/numeric/random/gamma_distribution
/// \tparam T the desired output type; defaults to float
/// \param alpha the alpha
/// \param beta the beta
/// \return a  T of gamma random value
template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
gamma(double alpha, double beta) {
	return std::gamma_distribution { alpha, beta }(of::random::gen());
}

/// \brief Generates a glm::vec distributed according to gamma distribution
/// https://en.cppreference.com/w/cpp/numeric/random/gamma_distribution
/// \tparam T the desired vector type
/// \param alpha the alpha
/// \param beta the beta
/// \return a T vector of gamma random values
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
gamma(float alpha, float beta) {
	return make_vector<T, std::gamma_distribution<float>>(alpha, beta);
}

/// \brief Generates a glm::vec distributed according to gamma distribution
/// https://en.cppreference.com/w/cpp/numeric/random/gamma_distribution
/// with specialized parameters (different params for members of the vector)
/// \tparam T the desired vector type
/// \param alpha the vector of alphas
/// \param beta the vector of betas
/// \return a T vector of gamma random values
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
gamma(T alpha, T beta) {
	return make_vector<T, std::gamma_distribution<float>>(alpha, beta);
}

// MARK: POISSON

/// \brief Generates random non-negative integer values according to poisson distribution
/// https://en.cppreference.com/w/cpp/numeric/random/poisson_distribution
/// \tparam T the desired output type; defaults to int
/// \param mean the mean
/// \return a gamma random value ot type T
template <typename T = int>
std::enable_if_t<std::is_arithmetic_v<T>, T>
poisson(double mean) {
	return std::poisson_distribution<T> { mean }(of::random::gen());
}

/// \brief Generates a vector of random non-negative integer values according to poisson distribution
/// https://en.cppreference.com/w/cpp/numeric/random/poisson_distribution
/// note that this distribution produces int, which are cast into floats for glm
/// \tparam T the desired output vector type
/// \param mean the mean
/// \return a vector T of poisson random values
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
poisson(double mean) {
	return make_vector<T, std::poisson_distribution<int>>(mean);
}

/// \brief Generates a vector of random non-negative integer values according to poisson distribution
/// https://en.cppreference.com/w/cpp/numeric/random/poisson_distribution
/// (note that this distribution produces int, which are cast into floats for glm)
/// with specialized parameters (different params for members of the vector)
/// \tparam T the desired output vector type
/// \param mean the vector of means (double)
/// \return a vector T of poisson random values 
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
poisson(T mean) {
	return make_vector<T, std::poisson_distribution<int>>(mean);
}

// MARK: EXPONENTIAL

/// \brief Generates random non-negative values according to exponential distribution
/// https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
/// \tparam T the desired output type; defaults to float
/// \param lambda the lambda values
/// \return an exponential random value ot type T
template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
exponential(T lambda) {
	return std::exponential_distribution<T> { lambda }(of::random::gen());
}

/// \brief Generates a vector of random non-negative values according to exponential distribution
/// https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
/// \param mean the lambda
/// \tparam T the desired vector output type
/// \return a vector T of exponential random values
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
exponential(float lambda) {
	return make_vector<T, std::exponential_distribution<float>>(lambda);
}

/// \brief Generates a vector of random non-negative values according to exponential distribution
/// https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
/// with specialized parameters (different params for members of the vector)
/// \param mean the vector of lambdas (double)
/// \tparam T the desired vector output type
/// \return a vector T of exponential random values
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
exponential(T lambda) {
	return make_vector<T, std::exponential_distribution<float>>(lambda);
}

// MARK: CHI_SQUARED

/// \brief Generates a random non-negative value according to the chi squared distribution
/// https://en.cppreference.com/w/cpp/numeric/random/chi_squared_distribution
/// \tparam T the desired output type; defaults to float
/// \param freedom the degrees of freedom
/// \return an chi-squared random value ot type T
template <typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
chi_squared(double freedom) {
	std::chi_squared_distribution dist(freedom);
	return dist(of::random::gen());
}

/// \brief Generates a vector of random non-negative values according to chi squared distribution
/// https://en.cppreference.com/w/cpp/numeric/random/chi_squared_distribution
/// \tparam T the desired output type
/// \param freedom the degrees of freedom
/// \return a vector T of chi-squared random values
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
chi_squared(float freedom) {
	return make_vector<T, std::chi_squared_distribution<float>>(freedom);
}

/// \brief Generates a vector of random non-negative values according to chi squared distribution
/// https://en.cppreference.com/w/cpp/numeric/random/chi_squared_distribution
/// with specialized parameters (different params for members of the vector)
/// \tparam T the desired output type
/// \param freedom the vector of degrees of freedom
/// \return a vector T of chi-squared random values
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
chi_squared(T freedom) {
	return make_vector<T, std::chi_squared_distribution<float>>(freedom);
}

// MARK: BINOMIAL

/// \brief Generates a random non-negative integer value according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// \tparam T the desired output type; defaults to int
/// \param p the first parameter
/// \param t the second parameter
/// \return a binomial random value ot type T
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

/// \brief Generates a rvector of andom non-negative integer value according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// (note that this distribution produces int, which are cast into floats for glm)
/// \tparam T the desired vector type
/// \param p the first parameter
/// \param t the second parameter
/// \return a vector T of binomial random values (int cast to float)
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
binomial(int p, double t) {
	return make_vector<T, std::binomial_distribution<int>>(p, t);
}

/// \brief Generates a glm::vec2 of andom non-negative integer value according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// with specialized parameters (different params for members of the vector)
/// (note that this distribution produces int, which are cast into floats for glm)
/// also the template is a bit different as the distribution is restrictive on the type of param p
/// \tparam T the desired vector type
/// \param p the glm::vec2 of first parameters
/// \param t the glm::vec2 of second parameters
/// \return a glm::vec2 of binomial random values (int cast to float)
template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
binomial(T p, T t) {
	return {
		std::binomial_distribution { int(p.x), t.x }(of::random::gen()),
		std::binomial_distribution { int(p.y), t.y }(of::random::gen())
	};
}

/// \brief Generates a glm::vec3 of random non-negative integer values according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// with specialized parameters (different params for members of the vector)
/// (note that this distribution produces int, which are cast into floats for glm)
/// also the template is a bit different as the distribution is restrictive on the type of param p
/// \tparam T the desired vector type
/// \param p the  glm::vec3  of first parameters
/// \param t the  glm::vec3  of second parameters
/// \return a glm::vec3 of binomial random values (int cast to float)
template <typename T = int>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
binomial(T p, T t) {
	return {
		std::binomial_distribution { int(p.x), t.x }(of::random::gen()),
		std::binomial_distribution { int(p.y), t.y }(of::random::gen()),
		std::binomial_distribution { int(p.z), t.z }(of::random::gen())
	};
}

/// \brief Generates a glm::vec4 of random non-negative integer values according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// with specialized parameters (different params for members of the vector)
/// (note that this distribution produces int, which are cast into floats for glm)
/// also the template is a bit different as the distribution is restrictive on the type of param p
/// \tparam T the desired vector type
/// \param p the  glm::vec4  of first parameters
/// \param t the  glm::vec4  of second parameters
/// \return a glm::vec4 of binomial random values (int cast to float)template <typename T = int>
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

/// \brief Generates a random non-negative integer value according to the geometric distribution
/// https://en.cppreference.com/w/cpp/numeric/random/geometric_distribution
/// \tparam T the desired output type; defaults to int
/// \return a geometric random value ot type T
template <typename T = int>
std::enable_if_t<std::is_arithmetic_v<T>, T>
geometric() {
	return std::geometric_distribution {}(of::random::gen());
}

/// \brief Generates a vector of random non-negative integer value according to the geometric distribution
/// https://en.cppreference.com/w/cpp/numeric/random/geometric_distribution
/// \tparam T the desired vetor type
/// \return a vector T of geometric random valus
template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
geometric() {
	return make_vector<T>(std::geometric_distribution<int> {});
}

// MARK: BERNOULLI

/// \brief Generates a random boolean value according to the bernoulli distribution
/// https://en.cppreference.com/w/cpp/numeric/random/bernoulli_distribution
/// \tparam T the desired output type; defaults to int
/// \param p the probability of generating true
/// \return a boolean value cast to T; probably 0 or 1
template <typename T = bool>
std::enable_if_t<std::is_arithmetic_v<T>, bool>
bernoulli(double p) {
	return std::bernoulli_distribution(p)(of::random::gen());
}

/// \brief Generates a vector of random boolean values according to the bernoulli distribution
/// https://en.cppreference.com/w/cpp/numeric/random/bernoulli_distribution
/// \tparam T the desired output type; defaults to int
/// \param p the probability of generating true
/// \return a boolean value cast to T; probably 0 or 1
template <typename T, typename Q>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
bernoulli(Q p) {
	return make_vector<T, std::bernoulli_distribution>(p);
}

// MARK: - CONVENIENT FORWARDING ALIASES

/// \brief alias for of::random::normal
template <class T = float, typename... Args>
constexpr auto gaussian(Args &&... args) -> decltype(normal<T>(std::forward<Args>(args)...)) {
	return normal<T>(std::forward<Args>(args)...);
}

/// \brief alias for of::random::bernoulli
template <class T = bool, typename... Args>
constexpr auto yes(Args &&... args) -> decltype(bernoulli<T>(std::forward<Args>(args)...)) {
	return bernoulli<T>(std::forward<Args>(args)...);
}

// whilst nerdy enough, the above do not work with {initializer list} for glm::vec parameters such as:
// of::random::gaussian<glm::vec2>({0,10},{1,100});
// so this additional explicit nerdiness is required to wrap them:

/// \brief alias for of::random::bernoulli with glm initializer lists
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

template <class T, typename... Args>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
ofRandomUniform(Args &&... args) { return of::random::uniform<T>(std::forward<Args>(args)...); }

template <class T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
ofRandomUniform(T max = 1.0f) { return of::random::uniform<T>(0, max); }

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
