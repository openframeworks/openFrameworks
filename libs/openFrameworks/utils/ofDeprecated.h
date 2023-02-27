// Cross-platform deprecation warning
#ifdef __GNUC__
	// clang also has this defined. deprecated(message) is only for gcc>=4.5
	#if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || __GNUC__ > 4
        #define OF_DEPRECATED_MSG(message, ...) __VA_ARGS__ __attribute__ ((deprecated(message)))
    #else
        #define OF_DEPRECATED_MSG(message, ...) __VA_ARGS__ __attribute__ ((deprecated))
    #endif
	#define OF_DEPRECATED(...) __VA_ARGS__ __attribute__ ((deprecated))
	#define OF_INTERNAL_DEPRECATED(...) __VA_ARGS__ __attribute__ ((deprecated("OF core deprecated")))
#elif defined(_MSC_VER)
	#define OF_DEPRECATED_MSG(message, ...) __declspec(deprecated(message)) __VA_ARGS__
	#define OF_DEPRECATED(...) __declspec(deprecated) __VA_ARGS__
	#define OF_INTERNAL_DEPRECATED(...) __declspec(deprecated("OF core deprecated")) __VA_ARGS__
#else
	#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
	#define OF_DEPRECATED_MSG(message, ...) __VA_ARGS__
	#define OF_DEPRECATED(...) __VA_ARGS__
#endif