#ifndef OF_SINGLETON_H_
#define OF_SINGLETON_H_

// atomic C++17 DCLP CRTP singleton adapted by burton@artificiel.org from
// https://github.com/jimmy-park/singleton/blob/main/include/singleton_dclp.hpp (as of df5e4a2)

// the atomic CRTP Singleton is explicitely constructed (not simply as a side effect
// of being the first call to the instance) which makes it easier to audit and
// configure (arguments are explictely passed as required))
//
// using the Singleton make the code thread-safe and better contained than
// arbitrarily allocated pointers in the global namespace
//
// this implements abstract class injection to prevent construction at compile time

#include <atomic>
#include <cassert>
#include <shared_mutex>
#include <utility>
#include <mutex>

namespace of::utils {

template <typename Derived>
class Singleton {
public:
	/// \brief constructs the instance
	template <typename... Args>
	static void construct(Args &&... args) {
		struct Dummy final : Derived {
			using Derived::Derived;
			void prohibit_construct_from_derived() const noexcept override { }
		};

		using Instance = Dummy;
		if (!instance_.load(std::memory_order_acquire)) {
			std::lock_guard lock { mutex_ };
			if (!instance_.load(std::memory_order_relaxed)) {
				instance_.store(new Instance { std::forward<Args>(args)... }, std::memory_order_release);
			}
		}
	}

	/// \brief destroys the instance
	static void destruct() {
		if (instance_.load(std::memory_order_acquire)) {
			std::lock_guard lock { mutex_ };
			if (auto * the_instance = instance_.load(std::memory_order_relaxed); the_instance) {
				delete the_instance;
				instance_.store(nullptr, std::memory_order_release);
			}
		}
	}

	/// \brief returns the instance
	static Derived * instance() {
		auto * the_instance = instance_.load(std::memory_order_acquire);
		if (!the_instance) {
			std::shared_lock lock { mutex_ };
			the_instance = instance_.load(std::memory_order_relaxed);
		}
		return the_instance;
	}

protected:
	Singleton() = default;
	Singleton(const Singleton &) = delete;
	Singleton(Singleton &&) = delete;
	Singleton & operator=(const Singleton &) = delete;
	Singleton & operator=(Singleton &&) = delete;
	virtual ~Singleton() = default;

private:
	virtual void prohibit_construct_from_derived() const noexcept = 0;
	inline static std::atomic<Derived *> instance_ { nullptr };
	inline static std::shared_mutex mutex_;
};

} // end namespace of::utils
#endif // OF_SINGLETON_H_
