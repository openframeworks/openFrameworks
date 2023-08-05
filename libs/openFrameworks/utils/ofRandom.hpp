#ifndef OF_RANDOM_HPP_
#define OF_RANDOM_HPP_

#include <random>
#include <glm/glm.hpp>

#include "ofSingleton.hpp"

// https://stackoverflow.com/questions/34934480/how-efficient-is-it-to-make-a-temporary-uniform-random-distribution-each-time-ro
// https://simplecxx.github.io/2018/11/03/seed-mt19937.html
// https://gist.github.com/imneme/540829265469e673d045
// https://github.com/effolkronium/random/tree/master
// https://www.theanalysisfactor.com/differences-between-normal-and-poisson-distributions/#:~:text=Both%20are%20discrete%20and%20bounded,tail%20extending%20to%20the%20right.
// https://blogs.sas.com/content/iml/2019/07/22/extreme-value-normal-data.html
// https://rovdownloads.com/blog/quick-overview-of-probability-distributions/

namespace of::random
{

class Engine: public of::utils::Singleton<Engine> {
	std::random_device rd_{ };
	std::seed_seq seq_{ rd_(), rd_(), rd_(), rd_() };
	std::mt19937 g_{ seq_ };
	bool deterministic_{ false };
public:
    auto & gen() { return g_; }
	auto seed(unsigned long new_seed) {
		deterministic_ = true;
        return g_.seed(new_seed);
	}
    auto is_deterministic() const { return deterministic_; }
};

auto engine() {
    return of::random::Engine::instance();
}

auto & gen() {
    return of::random::engine()->gen();
}

auto seed(unsigned long seed) {
	return of::random::engine()->seed(seed);
}

// MARK: - UNIFORM

// special case of distinguishing real and integer for uniform distribution

template<typename T = float, typename ... Args>
std::enable_if_t<std::is_floating_point_v<T>, T>
uniform(Args&&... args) {
	std::uniform_real_distribution<T> distr(std::forward<Args>(args)...);
	return distr(of::random::gen());
}

template<typename T, typename ... Args>
std::enable_if_t<std::is_integral_v<T>, T>
uniform(Args&&... args) {
	std::uniform_int_distribution<T> distr(std::forward<Args>(args)...);
	return distr(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
uniform(double min, double max) {
	std::uniform_real_distribution<float> dist(min,max);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
uniform(T min, T max) {
	return {
		std::uniform_real_distribution<float>{min.x, max.x}(of::random::gen()),
		std::uniform_real_distribution<float>{min.y, max.y}(of::random::gen())
	};
}

template <typename T, typename ... Args>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
uniform(Args&&... args) {
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
		std::uniform_real_distribution<float>{min.x, max.x}(of::random::gen()),
		std::uniform_real_distribution<float>{min.y, max.y}(of::random::gen()),
		std::uniform_real_distribution<float>{min.z, max.z}(of::random::gen())
	};
}

// MARK: NORMAL (gaussian)

// better to cast params as double (otherwise casual ints produce unexpected results)

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
normal(double mean, double stddev) {
	return std::normal_distribution{mean, stddev}(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
normal(double mean, double stddev) {
	std::normal_distribution dist(mean, stddev);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
normal(T mean, T stddev) {
	return {
		std::normal_distribution{mean.x, stddev.x}(of::random::gen()),
		std::normal_distribution{mean.y, stddev.y}(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
normal(double mean, double stddev) {
	std::normal_distribution dist(mean, stddev);
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
		std::normal_distribution{mean.x, stddev.x}(of::random::gen()),
		std::normal_distribution{mean.y, stddev.y}(of::random::gen()),
		std::normal_distribution{mean.z, stddev.z}(of::random::gen())
	};
}

// MARK: LOGNORMAL

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
lognormal(double mean, double stddev) {
    return std::lognormal_distribution{mean, stddev}(of::random::gen());
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
        std::lognormal_distribution{mean.x, stddev.x}(of::random::gen()),
        std::lognormal_distribution{mean.y, stddev.y}(of::random::gen())
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
        std::lognormal_distribution{mean.x, stddev.x}(of::random::gen()),
        std::lognormal_distribution{mean.y, stddev.y}(of::random::gen()),
        std::lognormal_distribution{mean.z, stddev.z}(of::random::gen())
    };
}

// MARK: GAMMA

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
gamma(double alpha, double beta) {
	return std::gamma_distribution{alpha, beta}(of::random::gen());
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
		std::gamma_distribution{alpha.x, beta.x}(of::random::gen()),
		std::gamma_distribution{alpha.y, beta.y}(of::random::gen())
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
		std::gamma_distribution{alpha.x, beta.x}(of::random::gen()),
		std::gamma_distribution{alpha.y, beta.y}(of::random::gen()),
		std::gamma_distribution{alpha.z, beta.z}(of::random::gen())
	};
}
// MARK: POISSON

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
poisson(double mean) {
	return std::poisson_distribution<T>{mean}(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
poisson(double mean) {
	std::poisson_distribution<T> dist(mean);
	return {
		dist(of::random::gen()),
		dist(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
poisson(T mean) {
	return {
		std::poisson_distribution<T>{mean.x}(of::random::gen()),
		std::poisson_distribution<T>{mean.y}(of::random::gen())
	};
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
poisson(double mean) {
	std::poisson_distribution<T> dist(mean);
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
		std::poisson_distribution<T>{mean.x}(of::random::gen()),
		std::poisson_distribution<T>{mean.y}(of::random::gen()),
		std::poisson_distribution<T>{mean.z}(of::random::gen())
	};
}

// MARK: EXPONENTIAL

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
exponential(T lambda) {
    return std::exponential_distribution<T>{lambda}(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
exponential(double lambda) {
    std::exponential_distribution<T> dist(lambda);
    return {
        dist(of::random::gen()),
        dist(of::random::gen())
    };
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
exponential(T lambda) {
    return {
        std::exponential_distribution<T>{lambda.x}(of::random::gen()),
        std::exponential_distribution<T>{lambda.y}(of::random::gen())
    };
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
exponential(double lambda) {
    std::exponential_distribution<T> dist(lambda);
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
        std::exponential_distribution<T>{lambda.x}(of::random::gen()),
        std::exponential_distribution<T>{lambda.y}(of::random::gen()),
        std::exponential_distribution<T>{lambda.z}(of::random::gen())
    };
}

// MARK: CHI_SQUARED

template<typename T = float>
std::enable_if_t<std::is_arithmetic_v<T>, T>
chi_squared(T n) {
    return std::chi_squared_distribution<T>{n}(of::random::gen());
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
chi_squared(double n) {
    std::chi_squared_distribution<T> dist(n);
    return {
        dist(of::random::gen()),
        dist(of::random::gen())
    };
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
chi_squared(T n) {
    return {
        std::chi_squared_distribution<T>{n.x}(of::random::gen()),
        std::chi_squared_distribution<T>{n.y}(of::random::gen())
    };
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
chi_squared(double n) {
    std::chi_squared_distribution<T> dist(n);
    return {
        dist(of::random::gen()),
        dist(of::random::gen()),
        dist(of::random::gen())
    };
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
chi_squared(T n) {
    return {
        std::chi_squared_distribution<T>{n.x}(of::random::gen()),
        std::chi_squared_distribution<T>{n.y}(of::random::gen()),
        std::chi_squared_distribution<T>{n.z}(of::random::gen())
    };
}

// MARK: BINOMIAL

template<typename T = int>
std::enable_if_t<std::is_arithmetic_v<T>, T>
binomial(int p, double t) {
    if (t>=1) {
        std::cout << "of::random::binomial(): t must be < 1.0\n";
        return 0;
    } else {
        std::binomial_distribution<T> dist(p,t);
        return dist(of::random::gen());
    }
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
binomial(int p, double t) {
    std::binomial_distribution<int> dist(p,t);
    return {
        dist(of::random::gen()),
        dist(of::random::gen())
    };
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
binomial(T p,T t) {
    return {
        std::binomial_distribution<int>{p.x,t.x}(of::random::gen()),
        std::binomial_distribution<int>{p.y,t.y}(of::random::gen())
    };
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
binomial(int p, double t) {
    std::binomial_distribution<int> dist(p,t);
    return {
        dist(of::random::gen()),
        dist(of::random::gen()),
        dist(of::random::gen())
    };
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
binomial(T p,T t) {
    return {
        std::binomial_distribution{p.x,t.x}(of::random::gen()),
        std::binomial_distribution{p.y,t.y}(of::random::gen()),
        std::binomial_distribution{p.z,t.z}(of::random::gen())
    };
}

// MARK: GEOMETRIC

template<typename T = int>
std::enable_if_t<std::is_arithmetic_v<T>, T>
geometric() {
    return std::geometric_distribution{}(of::random::gen());
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

// MARK: BERNOUILLI

template <typename T = bool>
auto bernoulli(double p) {
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
        std::bernoulli_distribution{p.x}(of::random::gen()),
        std::bernoulli_distribution{p.y}(of::random::gen())
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
        std::bernoulli_distribution{p.x}(of::random::gen()),
        std::bernoulli_distribution{p.y}(of::random::gen()),
        std::bernoulli_distribution{p.z}(of::random::gen())
    };
}
// MARK: - CONVENIENT FORWARDING ALIASES

template <typename... Args>
auto gaussian(Args&&... args) -> decltype(normal(std::forward<Args>(args)...)) {
  return normal(std::forward<Args>(args)...);
}

template <typename... Args>
auto yes(Args&&... args) -> decltype(bernoulli(std::forward<Args>(args)...)) {
  return bernoulli(std::forward<Args>(args)...);
}

// whilst nerdy enough, the above does not work with {initializer list} for glm::vec parameters such as:
// of::random::gaussian<glm::vec2>({0,10},{1,100});
// so this additional explicit nerdiness is required to wrap them:

template<class T>
auto gaussian(T min,  T max) -> decltype(normal<T>(min, max)) {
  return normal(min, max);
}
template<class T>
auto yes(T p) -> decltype(bernoulli(p)) {
  return bernoulli(p);
}

// MARK: - OTHER FUNCTIONS

template<class T = float> // works for all non-refined
T bound_normal(float min, float max, float focus = 4.0f) {
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
bound_normal(T min, T max, T focus = {4.0f, 4.0f}) {
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
bound_normal(T min, T max, T focus = {4.0f, 4.0f, 4.0f}) {
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

template<class T>
void shuffle(std::vector<T>& values) {
    std::shuffle(values.begin(), values.end(), of::random::Engine::instance()->gen());
}

} // end namespace of::random

// MARK: - ANONYMOUS NAMESPACE FUNCTIONS

namespace {

auto ofSetRandomSeed(unsigned long new_seed) {
	of::random::seed(new_seed);
}

// again, 2 templates per function, for the convenience of implicit brace init of glm::vec parameters

template<class T, typename ... Args>
T ofRandomUniform(Args&&... args) { return of::random::uniform<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomUniform(T min, T max) { return of::random::uniform<T>(min, max); }

template<class T, typename ... Args>
T ofRandomNormal(Args&&... args) { return of::random::normal<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomNormal(T min, T max) { return of::random::normal<T>(min, max); }

template<class T, typename ... Args>
T ofRandomGaussian(Args&&... args) { return of::random::gaussian<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomGaussian(T mean, T stddev) { return of::random::gaussian<T>(mean, stddev); }

template<class T, typename ... Args>
T ofRandomBernoulli(Args&&... args) { return of::random::bernoulli<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomBernoulli(T prob) { return of::random::bernoulli<T>(prob); }

template<class T, typename ... Args>
T ofRandomYes(Args&&... args) { return of::random::yes<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomYes(T prob) { return of::random::yes<T>(prob); }

template<class T, typename ... Args>
T ofRandomPoisson(Args&&... args) { return of::random::poisson<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomPoisson(T mean) { return of::random::poisson<T>(mean); }

template<class T, typename ... Args>
T ofRandomExponential(Args&&... args) { return of::random::exponential<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomExponential(T lambda) { return of::random::exponential<T>(lambda); }

template<class T, typename ... Args>
T ofRandomChiSquared(Args&&... args) { return of::random::chi_squared<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomChiSquared(T freedom) { return of::random::chi_squared<T>(freedom); }

template<class T, typename ... Args>
T ofRandomGamma(Args&&... args) { return of::random::gamma<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomGamma(T a, T b) { return of::random::gamma<T>(a, b); }

template<class T, typename ... Args>
T ofRandomLogNormal(Args&&... args) { return of::random::lognormal<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomLogNormal(T mean, T stddev) { return of::random::lognormal<T>(mean, stddev); }

template<class T, typename ... Args>
T ofRandomGeometric(Args&&... args) { return of::random::geometric<T>(std::forward<Args>(args)...); }

template<class T>
T ofRandomGeometric() { return of::random::geometric<T>(); }

template<class T = float> 
T ofBoundNormal(float min, float max, float focus = 4.0f) {
    return of::random::bound_normal(min, max, focus);
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec2>, T>
ofRandomBoundNormal(T min, T max, T focus = {4.0f, 4.0f}) {
    return of::random::bound_normal(min, max, focus);
}

template <typename T>
std::enable_if_t<std::is_same_v<T, glm::vec3>, T>
ofRandomBoundNormal(T min, T max, T focus = {4.0f, 4.0f, 4.0f}) {
    return of::random::bound_normal(min, max, focus);
}

// MARK: - PRACTICAL WRAPPERS

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
