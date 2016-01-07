#ifndef ATOMIC_H
#define ATOMIC_H

#include <stdint.h>

#define	atomic_read_uint64(p)	atomic_add_uint64(p, 0)
#define	atomic_read_uint32(p)	atomic_add_uint32(p, 0)

#if (LG_SIZEOF_PTR == 3)
#  define atomic_read_z(p)						\
    (size_t)atomic_add_uint64((uint64_t *)p, (uint64_t)0)
#  define atomic_add_z(p, x)						\
    (size_t)atomic_add_uint64((uint64_t *)p, (uint64_t)x)
#  define atomic_sub_z(p, x)						\
    (size_t)atomic_sub_uint64((uint64_t *)p, (uint64_t)x)
#elif (LG_SIZEOF_PTR == 2)
#  define atomic_read_z(p)						\
    (size_t)atomic_add_uint32((uint32_t *)p, (uint32_t)0)
#  define atomic_add_z(p, x)						\
    (size_t)atomic_add_uint32((uint32_t *)p, (uint32_t)x)
#  define atomic_sub_z(p, x)						\
    (size_t)atomic_sub_uint32((uint32_t *)p, (uint32_t)x)
#endif

/******************************************************************************/
/* 64-bit operations. */
#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
#define HAVE_ATOMIC_UINT64
inline uint64_t
atomic_add_uint64(uint64_t *p, uint64_t x)
{

	return (__sync_add_and_fetch(p, x));
}

inline uint64_t
atomic_sub_uint64(uint64_t *p, uint64_t x)
{

	return (__sync_sub_and_fetch(p, x));
}

#elif (defined(__amd64_) || defined(__x86_64__))
#define HAVE_ATOMIC_UINT64
inline uint64_t
atomic_add_uint64(uint64_t *p, uint64_t x)
{

	asm volatile (
	    "lock; xaddq %0, %1;"
	    : "+r" (x), "=m" (*p) /* Outputs. */
	    : "m" (*p) /* Inputs. */
	    );

	return (x);
}

inline uint64_t
atomic_sub_uint64(uint64_t *p, uint64_t x)
{

	x = (uint64_t)(-(int64_t)x);
	asm volatile (
	    "lock; xaddq %0, %1;"
	    : "+r" (x), "=m" (*p) /* Outputs. */
	    : "m" (*p) /* Inputs. */
	    );

	return (x);
}
#else
#  if (LG_SIZEOF_PTR == 3)
#    error "Missing implementation for 64-bit atomic operations"
#  endif
#endif

/******************************************************************************/
/* 32-bit operations. */
#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
#define HAVE_ATOMIC_UINT32
inline uint32_t
atomic_add_uint32(uint32_t *p, uint32_t x)
{

	return (__sync_add_and_fetch(p, x));
}

inline uint32_t
atomic_sub_uint32(uint32_t *p, uint32_t x)
{

	return (__sync_sub_and_fetch(p, x));
}
#elif (defined(__i386__) || defined(__amd64_) || defined(__x86_64__))
#define HAVE_ATOMIC_UINT32
inline uint32_t
atomic_add_uint32(uint32_t *p, uint32_t x)
{

	asm volatile (
	    "lock; xaddl %0, %1;"
	    : "+r" (x), "=m" (*p) /* Outputs. */
	    : "m" (*p) /* Inputs. */
	    );

	return (x);
}

inline uint32_t
atomic_sub_uint32(uint32_t *p, uint32_t x)
{

	x = (uint32_t)(-(int32_t)x);
	asm volatile (
	    "lock; xaddl %0, %1;"
	    : "+r" (x), "=m" (*p) /* Outputs. */
	    : "m" (*p) /* Inputs. */
	    );

	return (x);
}
#else
#  error "Missing implementation for 32-bit atomic operations"
#endif

class AtomicInt {
public:
#ifdef HAVE_ATOMIC_UINT64
	typedef uint64_t Type;
#else
	typedef uint32_t Type;
#endif

	AtomicInt(): value_(0) {}

	AtomicInt(Type i): value_(i) {}

	~AtomicInt() {}

	inline Type add(Type i) {
#ifdef HAVE_ATOMIC_UINT64
		return atomic_add_uint64(&value_, i);
#else
		return atomic_add_uint32(&value_, i);
#endif
	}

	inline Type operator+=(Type i) {
		return add(i);
	}

	inline Type incr() {
		return add(1);
	}

	inline Type operator++() {
		return incr();
	}

	inline Type sub(Type i) {
#ifdef HAVE_ATOMIC_UINT64
		return atomic_sub_uint64(&value_, i);
#else
		return atomic_sub_uint32(&value_, i);
#endif
	}

	inline Type operator-=(Type i) {
		return sub(i);
	}

	inline Type decr() {
		return sub(1);
	}

	inline Type operator--() {
		return decr();
	}

	//atomic read
	inline Type get() {
#ifdef HAVE_ATOMIC_UINT64
		return atomic_add_uint64(&value_, 0);
#else
		return atomic_add_uint32(&value_, 0);
#endif
	}

	//non atomic read
	/*
	inline Type value() const {
		return value_;
	}*/

	inline void set(Type i) {
#ifdef HAVE_ATOMIC_UINT64
		atomic_add_uint64(&value_, i);
#else
		atomic_add_uint32(&value_, i);
#endif
	}

	inline void operator=(Type i) {
		set(i);
	}

private:
	Type value_;
};

#endif	//ATOMIC_H
