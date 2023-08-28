#ifndef OF_SINGLETON_HPP_
#define OF_SINGLETON_HPP_

// atomic C++17 DCLP CRTP singleton adapted by burton@artificiel.org from
// https://github.com/jimmy-park/singleton/blob/main/include/singleton_dclp.hpp (1d26f91)

#include <cassert>
#include <mutex>
#include <atomic>

namespace of::utils
{

template <typename Derived>
class Singleton {
public:
	template <typename... Args>
	static void construct(Args&&... args) {
		struct Dummy : public Derived {
			using Derived::Derived;
			void prohibit_construct_from_derived() const override { }
		};

		if (!instance_.load(std::memory_order_acquire)) {
			if (std::lock_guard lock { mutex_ }; !instance_.load(std::memory_order_relaxed)) {
				instance_.store(new Dummy { std::forward<Args>(args)... }, std::memory_order_release);
			}
		}
	}

	static Derived * instance() {
		auto * the_instance = instance_.load(std::memory_order_acquire);
		assert(the_instance);
		return the_instance;
	}

	static void destruct() {
		if (auto * the_instance = instance_.exchange(nullptr, std::memory_order_acq_rel)) {
			delete the_instance;
		}
	}

protected:
	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) noexcept = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) noexcept = delete;
	virtual ~Singleton() = default;

private:
	virtual void prohibit_construct_from_derived() const = 0;
	inline static std::atomic<Derived*> instance_ { nullptr };
	inline static std::mutex mutex_;
};

} // end namespace of::utils
#endif // OF_SINGLETON_HPP_
