#ifndef OF_RANDOM_HPP_
#define OF_RANDOM_HPP_

#include <random>
#include <glm/glm.hpp>

#include "ofSingleton.hpp"

// https://stackoverflow.com/questions/25360241/using-random-number-generator-multiple-instances-or-singleton-approach
// https://simplecxx.github.io/2018/11/03/seed-mt19937.html

namespace of::random
{

class Engine: public of::utils::Singleton<Engine> {
    
	std::random_device rd_{ };
	std::seed_seq seq_{ rd_(), rd_(), rd_(), rd_() };
	std::mt19937 gen_{ seq_ };
	bool deterministic_{ false };
    
public:
    auto & gen() { return gen_; }
	auto seed(unsigned long new_seed) {
		deterministic_ = true;
        return gen_.seed(new_seed);
	}
    auto is_deterministic() const { return deterministic_; }
};

inline auto engine() {
    return of::random::Engine::instance();
}

inline auto & gen() {
    return of::random::engine()->gen();
}

inline auto seed(unsigned long seed) {
	return of::random::engine()->seed(seed);
}

template<class T>
void shuffle(std::vector<T>& values) {
    std::shuffle(values.begin(), values.end(), of::random::gen());
}

} // end namespace of::random

#endif // OF_RANDOM_HPP_
