#ifndef OF_RANDOM_DISTRIBUTIONS_H_
#define OF_RANDOM_DISTRIBUTIONS_H_

#include "ofRandomEngine.h"
#include "ofColor.h"
#include <iostream>
#include <optional>

#ifdef TARGET_WIN32
#include <iso646.h>
#endif


// https://gist.github.com/imneme/540829265469e673d045
// https://github.com/effolkronium/random/tree/master
// https://www.theanalysisfactor.com/differences-between-normal-and-poisson-distributions
// https://blogs.sas.com/content/iml/2019/07/22/extreme-value-normal-data.html
// https://rovdownloads.com/blog/quick-overview-of-probability-distributions/

namespace of::random {

// Trait to check if T is a random engine --
// poor man's concept; simply based on the
// fact that it's callable(), with min() & max()
//
template <typename T, typename = void>
struct is_random_engine : std::false_type {};

template <typename T>
struct is_random_engine<T, std::void_t<
	decltype(std::declval<T>()()),
	decltype(std::declval<T>().min()),
	decltype(std::declval<T>().max())
>> : std::true_type {};

template <typename T>
inline constexpr bool is_random_engine_v = is_random_engine<T>::value;

/// \section flexible glm::vec2, vec3 and vec4 and ofColor makers
/// used internally; not meant for end-user
///
/// \brief produces a T filled with distribution D
/// \tparam T the type of glm::vec  or ofFloatColor
/// \tparam D the type of std::distribution
/// \tparam G the type of random engine
/// \param d the distribution instance
/// \param g the random engine
/// \returns a new vector or color T
template <typename T, typename D, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
T make_vector(D d, G & g = of::random::gen()) {
	if constexpr (std::is_same_v<T, glm::vec2>) {
		return T {
			d(g), d(g)
		};
	} else if constexpr (std::is_same_v<T, glm::vec3>) {
		return T {
			d(g), d(g), d(g)
		};
	} else if constexpr (std::is_same_v<T, glm::vec4>) {
		return T {
			d(g), d(g), d(g), d(g)
		};
	} else if constexpr (std::is_same_v<T, ofFloatColor>) {
		return T {
			d(g), d(g), d(g), 1.0f
		};
	} else {
		static_assert(sizeof(T) == -1, "Unsupported composite type passed to of::random. vec2,3,4 and FloatColor are supported.");
	}
}

/// \brief produces a vector T filled with distribution D requiring no parameter
/// either scalar (if A is arithmetic) or specialized (if A is a vector and of the same type as T)
/// \tparam T the type of glm::vec
/// \tparam D the type of std::distribution
/// \tparam G the type of random engine
/// \param g the random engine (default: OF internal of::random::gen())
/// \returns a new vector or color T
template <typename T, typename D, typename G, typename = std::enable_if_t<is_random_engine_v<G>>>
T make_vector(G & g = of::random::gen()) {

	if constexpr (std::is_same_v<T, glm::vec2>) {
		return T {
			D{}(g), D{}(g)
		};
	} else if constexpr (std::is_same_v<T, glm::vec3>) {
		return T {
			D{}(g), D{}(g), D{}(g)
		};
	} else if constexpr (std::is_same_v<T, glm::vec4>) {
		return T {
			D{}(g), D{}(g), D{}(g), D {}(g)
		};
	} else if constexpr (std::is_same_v<T, ofFloatColor>) {
		return T {
			D{}(g), D{}(g), D{}(g), 1.0
		};
	} else {
		static_assert(sizeof(T) == -1, "Unsupported composite type passed to of::random. vec2,3,4 and FloatColor are supported.");
	}
}

/// \brief produces a vector T filled with distribution D requiring 1 parameter
/// either scalar (if A is arithmetic) or specialized (if A is a vector and of the same type as T)
/// \tparam T the type of glm::vec
/// \tparam D the type of std::distribution
/// \tparam A the type of the configuration
/// \tparam G the type of random engine
/// \param a the configuration (scalar or vector)
/// \param g the random engine (default: OF internal of::random::gen())
/// \returns a new vector or color T
template <typename T, typename D, class A, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!is_random_engine_v<A>, T>
make_vector(A a, G & g = of::random::gen()) {
	if constexpr (std::is_arithmetic_v<A>) {
		return make_vector<T>(D{a}, g);
	} else {
		if constexpr (std::is_same_v<T, glm::vec2>) {
			return T {
				D{a.x}(g), D{a.y}(g)
			};
		} else if constexpr (std::is_same_v<T, glm::vec3>) {
			return T {
				D{a.x}(g), D{a.y}(g), D{a.z}(g)
			};
		} else if constexpr (std::is_same_v<T, glm::vec4>) {
			return T {
				D{a.x}(g), D{a.y}(g), D{a.z}(g), D {a.w}(g)
			};
		} else if constexpr (std::is_same_v<T, ofFloatColor>) {
			return T {
				D{a.x}(g), D{a.y}(g), D{a.z}(g), 1.0
			};
		} else {
			static_assert(sizeof(T) == -1, "Unsupported composite type passed to of::random. vec2,3,4 and FloatColor are supported.");
		}
	}
}

/// \brief produces a vector T filled with distribution D requiring 2 parameters
/// either scalar (if A is arithmetic) or specialized (if A is a vector and of the same type as T)
/// \tparam T the type of glm::vec or ofFloatColor
/// \tparam D the type of std::distribution
/// \tparam A the type of the first configuration
/// \tparam B the type of the second configuration
/// \tparam G the type of random engine
/// \param a the first parameter configuration (scalar or vector )
/// \param b the second parameter configuration (scalar or vector)
/// \param g the random engine
/// \returns a new vector or color T
template <typename T, typename D, class A, class B, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!is_random_engine_v<A> and !is_random_engine_v<B>, T>
make_vector(A a, B b, G & g = of::random::gen()) {
	if constexpr (std::is_arithmetic_v<A>) {
		return make_vector<T>(D{a, b}, g);
	} else {
		if constexpr (std::is_same_v<T, glm::vec2>) {
			return T {
				D{a.x, b.x}(g), D{a.y, b.y}(g)
			};
		} else if constexpr (std::is_same_v<T, glm::vec3>) {
			return T {
				D{a.x, b.x}(g), D{a.y, b.y}(g), D{a.z, b.z}(g)
			};
		} else if constexpr (std::is_same_v<T, glm::vec4>) {
			return T {
				D{a.x, b.x}(g), D{a.y, b.y}(g), D{a.z, b.z}(g), D{a.w, b.w}(g)
			};
		} else if constexpr (std::is_same_v<T, ofFloatColor>) {
			return T {
				D{a.x}(g), D{a.y}(g), D{a.z}(g), 1.0
			};
		} else {
			static_assert(sizeof(T) == -1, "Unsupported composite type passed to of::random. vec2,3,4 and FloatColor are supported.");
		}
	}
}

// MARK: - UNIFORM

/// \section distribution wrappers

// special case of distinguishing real and integer for uniform distribution

/// \brief Generates a real uniform random number between 0 and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// \tparam T the desired real type
/// \tparam G the type of random engine
/// \param max the maximum value, defaults to 1
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T real value in the [0, max) range
template <typename T = float, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(T max = 1.0, G & g = of::random::gen()) {
	return std::uniform_real_distribution<T>{0, max}(g);
}

/// \brief Generates an integral uniform random number between 0 and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
/// \tparam T the desired integer type
/// \tparam G the type of random engine
/// \param max the maximum value (no default for integral types)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T integer value in the [0, max] range
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(T max, G & g = of::random::gen()) {
	return std::uniform_int_distribution<T>{0, max}(g);
}

/// \brief Generates an real uniform random number between min and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// \tparam T the desired real type
/// \tparam G the type of random engine
/// \param min the minimum value (no default)
/// \param max the maximum value (no default)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T real value in the [min, max) range
template <typename T = float, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(T min, T max, G & g = of::random::gen()) {
	return std::uniform_real_distribution<T>{min, max}(g);
}

/// \brief Generates an integral uniform random number between min and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
/// \tparam T the desired integer type
/// \tparam G the type of random engine
/// \param min the minimum value (no default)
/// \param max the maximum value (no default)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T integer value in the [min, max] range
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(T min, T max, G & g = of::random::gen()) {
	return std::uniform_int_distribution<T>{min, max}(g);
}

/// \brief Generates a glm::vec of uniform random value between 0 and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param max the maximum value (default to 1.0)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T vec of floats in the [0, max) range
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
uniform(float max = 1.0, G & g = of::random::gen()) {
	return make_vector<T, std::uniform_real_distribution<float>>(0.0f, max, g);
}

/// \brief Generates a glm::vec of uniform random value between min and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param min the minimum value (no default)
/// \param max the maximum value (no default)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T vec of floats in the [min, max) range
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
uniform(float min, float max, G & g = of::random::gen()) {
	return make_vector<T, std::uniform_real_distribution<float>>(min, max, g);
}

/// \brief Generates a glm::vec of uniform random number
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// with specialized parameters (different params for members of the vector)
/// NB: T, min and max must be of the same type
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param min the minimum values (no default)
/// \param max the maximum values (no default)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T vec of uniform random floats in the [min, max) range
///  typename EngineFunc>
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
uniform(T min, T max, G & g = of::random::gen()) {
	return make_vector<T, std::uniform_real_distribution<float>>(min, max, g);
}

// MARK: NORMAL (gaussian)

// better to cast params as double (otherwise casual ints produce unexpected results)

/// \brief Generates a real normal random number 
/// /// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// (note that this is aliased to 'gaussian' too)
/// \tparam T the desired output type; defaults to float
/// \tparam G the type of random engine
/// \param mean the mean
/// \param stddev the standard deviation
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a real T of normal random
template <typename T = float, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_floating_point_v<T>, T>
normal(double mean, double stddev, G & g = of::random::gen()) {
	return std::normal_distribution<T>(mean, stddev)(g); // parentheses allow narrowing (¿ good or bad ?)
}

/// \brief Generates an integer random number
/// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// Template specialization is required to force the type of distribution (will not take an int type)
/// (note that this is aliased to 'gaussian' too)
/// \tparam T the desired output type
/// \tparam G the type of random engine
/// \param mean the mean
/// \param stddev the standard deviation
/// \param g the random engine (default: OF internal of::random::gen())
/// \return an integer T of normal random
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_integral_v<T>, T>
normal(double mean, double stddev, G & g = of::random::gen()) {
	return std::normal_distribution<double>{mean, stddev}(g);
}

/// \brief Generate a glm::vec of random numbers
/// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// (note that this is aliased to 'gaussian' too)
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param min the mean values
/// \param max the stddev values
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T vec of normally-distributed random floats with homogenous parameters
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
normal(float mean, float stddev, G & g = of::random::gen()) {
	return make_vector<T, std::normal_distribution<float>>(mean, stddev, g);
}

/// \brief Generates a glm::vec of random numbers 
/// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// with specialized parameters (different params for members of the vector)
/// where T, mean and stdev must be of the same type
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param min the mean values
/// \param max the stddev values
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T vec of normally-distributed random floats with distinct parameters
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
normal(T mean, T stddev, G & g = of::random::gen()) {
	return make_vector<T, std::normal_distribution<float>>(mean, stddev, g);
}

// MARK: LOGNORMAL

/// \brief Generates a  lognormal random number
/// https://en.cppreference.com/w/cpp/numeric/random/lognormal_distribution
/// \tparam T the desired output type; defaults to float
/// \tparam G the type of random engine
/// \param mean the mean
/// \param stddev the standard deviation
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a  T of lognormal random
template <typename T = float, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_arithmetic_v<T>, T>
lognormal(double mean, double stddev, G & g = of::random::gen()) {
	return std::lognormal_distribution{mean, stddev}(g);
}

/// \brief Generates a glm::vec of lognormal random float values
/// https://en.cppreference.com/w/cpp/numeric/random/lognormal_distribution
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param mean the mean
/// \param stddev the standard deviation
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a  T of lognormal homogeneous random float values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
lognormal(float mean, float stddev, G & g = of::random::gen()) {
	return make_vector<T, std::lognormal_distribution<float>>(mean, stddev, g);
}

/// \brief Generates a glm::vec of lognormal random float values
/// https://en.cppreference.com/w/cpp/numeric/random/lognormal_distribution
/// with specialized parameters (different params for members of the vector)
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param mean the vector of mean parameters
/// \param stddev the vector standard deviation parameters
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a vector T of lognormal specialized random float values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
lognormal(T mean, T stddev, G & g = of::random::gen()) {
	return make_vector<T, std::lognormal_distribution<float>>(mean, stddev, g);
	
}

// MARK: GAMMA

/// \brief Generates random positive floating-point values x, distributed according to gamma distribution
/// https://en.cppreference.com/w/cpp/numeric/random/gamma_distribution
/// \tparam T the desired output type; defaults to float
/// \tparam G the type of random engine
/// \param alpha the alpha
/// \param beta the beta
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a  T of gamma random value
template <typename T = float, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_arithmetic_v<T>, T>
gamma(double alpha, double beta, G & g = of::random::gen()) {
	return std::gamma_distribution{alpha, beta}(g);
}

/// \brief Generates a glm::vec distributed according to gamma distribution
/// https://en.cppreference.com/w/cpp/numeric/random/gamma_distribution
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param alpha the alpha
/// \param beta the beta
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T vector of gamma random values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
gamma(float alpha, float beta, G & g = of::random::gen()) {
	return make_vector<T, std::gamma_distribution<float>>(alpha, beta, g);
}

/// \brief Generates a glm::vec distributed according to gamma distribution
/// https://en.cppreference.com/w/cpp/numeric/random/gamma_distribution
/// with specialized parameters (different params for members of the vector)
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param alpha the vector of alphas
/// \param beta the vector of betas
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a T vector of gamma random values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
gamma(T alpha, T beta, G & g = of::random::gen()) {
	return make_vector<T, std::gamma_distribution<float>>(alpha, beta, g);
}

// MARK: POISSON

/// \brief Generates random non-negative integer values according to poisson distribution
/// https://en.cppreference.com/w/cpp/numeric/random/poisson_distribution
/// \tparam T the desired output type; defaults to int
/// \tparam G the type of random engine
/// \param mean the mean
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a gamma random value ot type T
template <typename T = int, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_arithmetic_v<T>, T>
poisson(double mean, G & gen = of::random::gen()) {
	return std::poisson_distribution<T>{mean}(gen);
}

/// \brief Generates a vector of random non-negative integer values according to poisson distribution
/// https://en.cppreference.com/w/cpp/numeric/random/poisson_distribution
/// note that this distribution produces int, which are cast into floats for glm
/// \tparam T the desired output vector type
/// \tparam G the type of random engine
/// \param mean the mean
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a vector T of poisson random values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
poisson(double mean, G & g = of::random::gen()) {
	return make_vector<T, std::poisson_distribution<int>>(mean, g);
}

/// \brief Generates a vector of random non-negative integer values according to poisson distribution
/// https://en.cppreference.com/w/cpp/numeric/random/poisson_distribution
/// (note that this distribution produces int, which are cast into floats for glm)
/// with specialized parameters (different params for members of the vector)
/// \tparam T the desired output vector type
/// \tparam G the type of random engine
/// \param mean the vector of means (double)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a vector T of poisson random values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
poisson(T mean, G & g = of::random::gen()) {
	return make_vector<T, std::poisson_distribution<int>>(mean, g);
}

// MARK: EXPONENTIAL

/// \brief Generates random non-negative values according to exponential distribution
/// https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
/// \tparam T the desired output type; defaults to float
/// \tparam G the type of random engine
/// \param lambda the lambda values
/// \param g the random engine (default: OF internal of::random::gen())
/// \return an exponential random value ot type T
template <typename T = float, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_arithmetic_v<T>, T>
exponential(T lambda, G & g = of::random::gen()) {
	return std::exponential_distribution<T>{lambda}(g);
}

/// \brief Generates a vector of random non-negative values according to exponential distribution
/// https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
/// \param mean the lambda
/// \tparam T the desired vector output type
/// \tparam G the type of random engine
/// \param lambda the lambda values
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a vector T of exponential random values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
exponential(float lambda, G & g = of::random::gen()) {
	return make_vector<T, std::exponential_distribution<float>>(lambda, g);
}

/// \brief Generates a vector of random non-negative values according to exponential distribution
/// https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
/// with specialized parameters (different params for members of the vector)
/// \param mean the vector of lambdas (double)
/// \tparam T the desired vector output type
/// \tparam G the type of random engine
/// \param lambda the lambda values
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a vector T of exponential random values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
exponential(T lambda, G & g = of::random::gen()) {
	return make_vector<T, std::exponential_distribution<float>>(lambda, g);
}

// MARK: CHI_SQUARED

/// \brief Generates a random non-negative value according to the chi squared distribution
/// https://en.cppreference.com/w/cpp/numeric/random/chi_squared_distribution
/// \tparam T the desired output type; defaults to float
/// \tparam G the type of random engine
/// \param freedom the degrees of freedom
/// \param g the random engine (default: OF internal of::random::gen())
/// \return an chi-squared random value ot type T
template <typename T = float, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_arithmetic_v<T>, T>
chi_squared(double freedom, G & g = of::random::gen()) {
	return std::chi_squared_distribution{freedom}(g);
}

/// \brief Generates a vector of random non-negative values according to chi squared distribution
/// https://en.cppreference.com/w/cpp/numeric/random/chi_squared_distribution
/// \tparam T the desired output type
/// \tparam G the type of random engine
/// \param freedom the degrees of freedom
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a vector T of chi-squared random values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
chi_squared(float freedom, G & g = of::random::gen()) {
	return make_vector<T, std::chi_squared_distribution<float>>(freedom, g);
}

/// \brief Generates a vector of random non-negative values according to chi squared distribution
/// https://en.cppreference.com/w/cpp/numeric/random/chi_squared_distribution
/// with specialized parameters (different params for members of the vector)
/// \tparam T the desired output type
/// \tparam G the type of random engine
/// \param freedom the vector of degrees of freedom
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a vector T of chi-squared random values
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
chi_squared(T freedom, G & g = of::random::gen()) {
	return make_vector<T, std::chi_squared_distribution<float>>(freedom, g);
}

// MARK: BINOMIAL

/// \brief Generates a random non-negative integer value according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// \tparam T the desired output type; defaults to int
/// \tparam G the type of random engine
/// \param p the first parameter
/// \param t the second parameter
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a binomial random value ot type T
template <typename T = int, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_arithmetic_v<T>, T>
binomial(int p, double t, G & gen = of::random::gen()) {
	if (t >= 1) {
		std::cout << "of::random::binomial(): t must be < 1.0\n";
		return 0;
	} else {
		return std::binomial_distribution{p, t}(gen);
	}
}

/// \brief Generates a vector of random non-negative integer values according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// (note that this distribution produces int, which are cast into floats for glm)
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param p the first parameter
/// \param t the second parameter
/// \return a vector T of binomial random values (int cast to float)
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
binomial(int p, double t, G & g = of::random::gen()) {
	return make_vector<T, std::binomial_distribution<int>>(p, t, g);
}

/// \brief Generates a glm::vec2 of random non-negative integer values according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// with specialized parameters (different params for members of the vector)
/// (note that this distribution produces int, which are cast into floats for glm)
/// also the template is a bit different as the distribution is restrictive on the type of param p
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param p the glm::vec2 of first parameters
/// \param t the glm::vec2 of second parameters
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a glm::vec2 of binomial random values (int cast to float)
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
binomial(T p, T t, G & gen = of::random::gen()) {
	return {
		std::binomial_distribution { int(p.x), t.x }(gen),
		std::binomial_distribution { int(p.y), t.y }(gen)
	};
}

/// \brief Generates a glm::vec3 of random non-negative integer values according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// with specialized parameters (different params for members of the vector)
/// (note that this distribution produces int, which are cast into floats for glm)
/// also the template is a bit different as the distribution is restrictive on the type of param p
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param p the  glm::vec3  of first parameters
/// \param t the  glm::vec3  of second parameters
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a glm::vec3 of binomial random values (int cast to float)
template <typename T = int, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
binomial(T p, T t, G & gen = of::random::gen()) {
	return {
		std::binomial_distribution { int(p.x), t.x }(gen),
		std::binomial_distribution { int(p.y), t.y }(gen),
		std::binomial_distribution { int(p.z), t.z }(gen)
	};
}

/// \brief Generates a glm::vec4 of random non-negative integer values according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// with specialized parameters (different params for members of the vector)
/// (note that this distribution produces int, which are cast into floats for glm)
/// also the template is a bit different as the distribution is restrictive on the type of param p
/// \tparam T the desired vector type
/// \tparam G the type of random engine
/// \param p the  glm::vec4  of first parameters
/// \param t the  glm::vec4  of second parameters
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a glm::vec4 of binomial random values (int cast to float)template <typename T = int>
template <typename T = int, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_same_v<T, glm::vec4>, T>
binomial(T p, T t, G & gen = of::random::gen()) {
	return {
		std::binomial_distribution { int(p.x), t.x }(gen),
		std::binomial_distribution { int(p.y), t.y }(gen),
		std::binomial_distribution { int(p.z), t.z }(gen),
		std::binomial_distribution { int(p.w), t.w }(gen)
	};
}

// MARK: GEOMETRIC

/// \brief Generates a random non-negative integer value according to the geometric distribution
/// https://en.cppreference.com/w/cpp/numeric/random/geometric_distribution
/// \tparam T the desired output type; defaults to int
/// \tparam G the type of random engine
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a geometric random value ot type T
template <typename T = int, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_arithmetic_v<T>, T>
geometric(G & g = of::random::gen()) {
	return std::geometric_distribution{}(g);
}

/// \brief Generates a vector of random non-negative integer value according to the geometric distribution
/// https://en.cppreference.com/w/cpp/numeric/random/geometric_distribution
/// \tparam T the desired vetor type
/// \tparam G the type of random engine
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a vector T of geometric random valus
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
geometric(G & g = of::random::gen()) {
	return make_vector<T, std::geometric_distribution<int>>(g);
}

// MARK: BERNOULLI

/// \brief Generates a random boolean value according to the bernoulli distribution
/// https://en.cppreference.com/w/cpp/numeric/random/bernoulli_distribution
/// \tparam T the desired output type; defaults to int
/// \tparam G the type of random engine
/// \param p the probability of generating true
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a boolean value cast to T; probably 0 or 1
template <typename T = bool, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_arithmetic_v<T>, bool>
bernoulli(double p, G & g = of::random::gen()) {
	return std::bernoulli_distribution{p}(g);
}

/// \brief Generates a vector of random boolean values according to the bernoulli distribution
/// https://en.cppreference.com/w/cpp/numeric/random/bernoulli_distribution
/// \tparam T the desired output type; defaults to int
/// \tparam G the type of random engine
/// \param p the probability of generating true
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a boolean value cast to T; probably 0 or 1
template <typename T, typename Q, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
bernoulli(Q p, G & g = of::random::gen()) {
	return make_vector<T, std::bernoulli_distribution>(p, g);
}

// MARK: - CONVENIENT FORWARDING ALIASES

/// \brief alias for of::random::normal
template <class T = float, typename... Args>
constexpr auto gaussian(Args &&... args) -> decltype(normal<T>(std::forward<Args>(args)...)) {
	return normal<T>(std::forward<Args>(args)...);
}

/// \brief alias for of::random::bernoulli
template <class T = bool, typename... Args>
constexpr auto sometimes(Args &&... args) -> decltype(bernoulli<T>(std::forward<Args>(args)...)) {
	return bernoulli<T>(std::forward<Args>(args)...);
}

// whilst nerdy enough, the above do not work with {initializer list} for glm::vec parameters such as:
// of::random::gaussian<glm::vec2>({0,10},{1,100});
// so this additional explicit nerdiness is required to wrap them:

/// \brief alias for of::random::gaussian with glm initializer lists
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
gaussian(T min, T max, G & g = of::random::gen()) {
	return normal<T>(min, max, g);
}

// MARK: - OTHER FUNCTIONS

/// \brief Generates a normal distribution framed by min, max and focus
/// in order to facilite the expression of a given result, boundnormal inverses
/// the usual normal/gaussian mean/stddv (which can be difficult to previsualize)
/// and offer a fixed range.
/// \tparam T the desired output type; defaults to int
/// \tparam G the type of random engine
/// \param min the minimum value
/// \param max the maximum value
/// \param focus the tightness of stddev (defaults 4)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return a normal value bound within [min,max] according to the focus
template <class T = float, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_arithmetic_v<T>, T>
bound_normal(float min, float max, float focus = 4.0f, G & g = of::random::gen()) {
	if (min >= max) {
		std::cout << "ofRandomNormalLimits():" << "max must be > than min\n";
		return {};
	} else {
		if (focus <= .0099) {
			std::cout << "ofRandomNormalLimits():" << "focus must be at least .01\n";
			return {};
		} else {
			T v;
			do {
				v = of::random::normal<T>((max + min) / 2.0f, (max - min) / (2 * focus), g); 
			} while (v < min || v > max);
			return v;
		}
	}
}

/// \brief Generates a normal distribution framed by min, max and focus
/// in order to facilite the expression of a given result, boundnormal inverses
/// the usual normal/gaussian mean/stddv (which can be difficult to previsualize)
/// and offer a fixed range.
/// \tparam T glm::vec2
/// \tparam G the type of random engine
/// \param min the minimum value
/// \param max the maximum value
/// \param focus the tightness of stddev (defaults 4)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return vector of normal within [min,max] according to the focus
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_same_v<T, glm::vec2> or std::is_same_v<T, glm::vec3> or std::is_same_v<T, glm::vec4>, T>
bound_normal(T min, T max, std::optional<T> focus = std::nullopt, G & g = of::random::gen()) {
	
	auto focus_value = focus.value_or(T{4.0});
	
	auto generate_bound_coordinates = [&](float& c, float min, float max, float focus) {
		if (min >= max || min >= max || focus < 1) {
			std::cout << "ofRandomNormalLimits():" << "max must be > than min and focus >= 1.0\n";
			return false;
		}
		do { c = of::random::normal<float>((max + min) / 2.0f, (max - min) / (2 * focus), g); }
		while (c < min || c > max);
		return true;
	};
	
	T v;
	if (!generate_bound_coordinates(v.x, min.x, max.x, focus_value.x)) return {};
	if (!generate_bound_coordinates(v.y, min.y, max.y, focus_value.y)) return {};
	if constexpr (std::is_same_v<T, glm::vec3>) {
		if (!generate_bound_coordinates(v.z, min.z, max.z, focus_value.z)) return {};
	}
	if constexpr (std::is_same_v<T, glm::vec4>) {
		if (!generate_bound_coordinates(v.z, min.z, max.z, focus_value.z)) return {};
		if (!generate_bound_coordinates(v.w, min.w, max.w, focus_value.w)) return {};
	}
	return v;
}

/// \brief Generates a normal distribution framed by min, max and focus
/// in order to facilite the expression of a given result, boundnormal inverses
/// the usual normal/gaussian mean/stddv (which can be difficult to previsualize)
/// and offer a fixed range.
/// \tparam T glm::vec2
/// \tparam G the type of random engine
/// \param min the minimum value
/// \param max the maximum value
/// \param focus the tightness of stddev (defaults 4)
/// \param g the random engine (default: OF internal of::random::gen())
/// \return vector of normal within [min,max] according to the focus
template <typename T, typename G = decltype(of::random::gen()), typename = std::enable_if_t<is_random_engine_v<G>>>
std::enable_if_t<std::is_same_v<T, glm::vec2> or std::is_same_v<T, glm::vec3> or std::is_same_v<T, glm::vec4>, T>
bound_normal(float min, float max, float focus = 4.0f, G & g = of::random::gen()) {
	return bound_normal<T>(T{min}, T{max}, T{focus});
}

} // end namespace of::random

// MARK: - ANONYMOUS NAMESPACE FUNCTIONS

namespace {

// again, a few templates per function, for the convenience of implicit brace init of glm::vec parameters

template <class T, typename... Args>
std::enable_if_t<!std::is_arithmetic_v<T>, T>
ofRandomUniform(Args &&... args) { return of::random::uniform<T>(std::forward<Args>(args)...); }

/// \brief Generates a real uniform random number between 0 and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// \tparam T the desired output type; defaults to float
/// \param max the maximum value, defaults to 1
/// \return a T real value in the [0, max) range
template <class T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
ofRandomUniform(T max = 1.0f) { return of::random::uniform<T>(0, max); }

/// \brief Generates a real uniform random number between 0 and max
/// https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
/// \tparam T the desired output type; defaults to int
/// \param min the minimum value,
/// \param max the maximum value,
/// \return a T real value in the [min, max) range
template <class T>
T ofRandomUniform(T min, T max) { return of::random::uniform<T>(min, max); }

template <class T, typename... Args>
T ofRandomNormal(Args &&... args) { return of::random::normal<T>(std::forward<Args>(args)...); }

/// \brief Generates a real normal random number
/// /// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// (note that this is aliased to 'ofRandomGaussian' too)
/// \tparam T the desired output type;
/// \param mean the mean
/// \param stddev the standard deviation
/// \return a real T of normal random
template <class T>
T ofRandomNormal(T mean, T stddev) { return of::random::normal<T>(mean, stddev); }

template <class T, typename... Args>
T ofRandomGaussian(Args &&... args) { return of::random::gaussian<T>(std::forward<Args>(args)...); }

/// \brief Generates a real normal random number
/// /// according to the Normal (or Gaussian) random number distribution
/// https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
/// \tparam T the desired output type
/// \param mean the mean
/// \param stddev the standard deviation
/// \return a real T of normal random
template <class T>
T ofRandomGaussian(T mean, T stddev) { return of::random::gaussian<T>(mean, stddev); }

template <class T, typename... Args>
T ofRandomBernoulli(Args &&... args) { return of::random::bernoulli<T>(std::forward<Args>(args)...); }

/// \brief Generates a random boolean value according to the bernoulli distribution
/// https://en.cppreference.com/w/cpp/numeric/random/bernoulli_distribution
/// aliased to ofRandomSometimes
/// \tparam T the desired output type; defaults to int
/// \param prob the probability of generating true
/// \return a boolean value cast to T; probably 0 or 1
template <class T>
T ofRandomBernoulli(T prob) { return of::random::bernoulli<T>(prob); }

template <class T, typename... Args>
T ofRandomSometimes(Args &&... args) { return of::random::sometimes<T>(std::forward<Args>(args)...); }

/// \brief Generates a random boolean value according to the bernoulli distribution
/// https://en.cppreference.com/w/cpp/numeric/random/bernoulli_distribution
/// \tparam T the desired output type; defaults to int
/// \param prob the probability of generating true
/// \return a boolean value cast to T; probably 0 or 1
template <class T>
T ofRandomSometimes(T prob) { return of::random::sometimes<T>(prob); }

template <class T, typename... Args>
T ofRandomPoisson(Args &&... args) { return of::random::poisson<T>(std::forward<Args>(args)...); }

/// \brief Generates random non-negative integer values according to poisson distribution
/// https://en.cppreference.com/w/cpp/numeric/random/poisson_distribution
/// \tparam T the desired output type; defaults to int
/// \param mean the mean
/// \return a gamma random value ot type T
template <class T>
T ofRandomPoisson(T mean) { return of::random::poisson<T>(mean); }

template <class T, typename... Args>
T ofRandomExponential(Args &&... args) { return of::random::exponential<T>(std::forward<Args>(args)...); }

/// \brief Generates random non-negative values according to exponential distribution
/// https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
/// \tparam T the desired output type; defaults to float
/// \param lambda the lambda values
/// \return an exponential random value ot type T
template <class T>
T ofRandomExponential(T lambda) { return of::random::exponential<T>(lambda); }

template <class T, typename... Args>
T ofRandomChiSquared(Args &&... args) { return of::random::chi_squared<T>(std::forward<Args>(args)...); }

/// \brief Generates a random non-negative value according to the chi squared distribution
/// https://en.cppreference.com/w/cpp/numeric/random/chi_squared_distribution
/// \tparam T the desired output type; defaults to float
/// \param freedom the degrees of freedom
/// \return an chi-squared random value ot type T
template <class T>
T ofRandomChiSquared(T freedom) { return of::random::chi_squared<T>(freedom); }

template <class T, typename... Args>
T ofRandomGamma(Args &&... args) { return of::random::gamma<T>(std::forward<Args>(args)...); }

/// \brief Generates random positive floating-point values x, distributed according to gamma distribution
/// https://en.cppreference.com/w/cpp/numeric/random/gamma_distribution
/// \tparam T the desired output type; defaults to float
/// \param alpha the alpha
/// \param beta the beta
/// \return a  T of gamma random value
template <class T>
T ofRandomGamma(T a, T b) { return of::random::gamma<T>(a, b); }

template <class T, typename... Args>
T ofRandomLognormal(Args &&... args) { return of::random::lognormal<T>(std::forward<Args>(args)...); }

/// \brief Generates a  lognormal random number
/// https://en.cppreference.com/w/cpp/numeric/random/lognormal_distribution
/// \tparam T the desired output type; defaults to float
/// \param mean the mean
/// \param stddev the standard deviation
/// \return a  T of lognormal random
template <class T>
T ofRandomLognormal(T mean, T stddev) { return of::random::lognormal<T>(mean, stddev); }

template <class T = int, typename... Args>
T ofRandomBinomial(Args &&... args) { return of::random::binomial<T>(std::forward<Args>(args)...); }

/// \brief Generates a random non-negative integer value according to the binomial distribution
/// https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution
/// \tparam T the desired output type; defaults to int
/// \param p the first parameter
/// \param t the second parameter
/// \return a binomial random value ot type T
template <class T>
T ofRandomBinomial(T mean, T stddev) { return of::random::binomial<T>(mean, stddev); }

template <class T = int, typename... Args>
T ofRandomGeometric(Args &&... args) { return of::random::geometric<T>(std::forward<Args>(args)...); }

/// \brief Generates a random non-negative integer value according to the geometric distribution
/// https://en.cppreference.com/w/cpp/numeric/random/geometric_distribution
/// \tparam T the desired output type; defaults to int
/// \return a geometric random value ot type T
template <class T>
T ofRandomGeometric() { return of::random::geometric<T>(); }

template <class T = float>
T ofRandomBoundNormal(float min, float max, float focus = 4.0f) {
	return of::random::bound_normal<T>(min, max, focus);
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2> or std::is_same_v<T, glm::vec3> or std::is_same_v<T, glm::vec4>, T>
ofRandomBoundNormal(T min, T max, std::optional<T> focus = std::nullopt) {
	return of::random::bound_normal<T>(min, max, focus.value_or(T{4.0}));
}

} // end anonymous namespace
#endif // OF_RANDOM_DISTRIBUTIONS_H_
