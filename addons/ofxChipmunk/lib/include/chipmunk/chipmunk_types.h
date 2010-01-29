#ifdef __APPLE__
   #import "TargetConditionals.h"
#endif

// Use single precision floats on the iPhone.
#ifdef TARGET_OS_IPHONE
	#define CP_USE_DOUBLES 0
#else
	// use doubles by default for higher precision
	#define CP_USE_DOUBLES 1
#endif

#if CP_USE_DOUBLES
	typedef double cpFloat;
	#define cpfsqrt sqrt
	#define cpfsin sin
	#define cpfcos cos
	#define cpfacos acos
	#define cpfatan2 atan2
	#define cpfmod fmod
	#define cpfexp exp
	#define cpfpow pow
	#define cpffloor floor
	#define cpfceil ceil
#else
	typedef float cpFloat;
	#define cpfsqrt sqrtf
	#define cpfsin sinf
	#define cpfcos cosf
	#define cpfacos acosf
	#define cpfatan2 atan2f
	#define cpfmod fmodf
	#define cpfexp expf
	#define cpfpow powf
	#define cpffloor floorf
	#define cpfceil ceilf
#endif

#if TARGET_OS_IPHONE
	// CGPoints are structurally the same, and allow
	// easy interoperability with other iPhone libraries
	#import <CoreGraphics/CGGeometry.h>
	typedef CGPoint cpVect;
#else
	typedef struct cpVect{cpFloat x,y;} cpVect;
#endif

typedef unsigned int cpHashValue;
typedef void * cpDataPointer;
typedef unsigned int cpCollisionType;
typedef unsigned int cpLayers;
typedef unsigned int cpGroup;

#define CP_NO_GROUP ((cpGroup)0)
#define CP_ALL_LAYERS (~(cpLayers)0)
