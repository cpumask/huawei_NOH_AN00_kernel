/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: LZ4K compression algorithm
 * Author: Aleksei Romanovskii aleksei.romanovskii@huawei.com
 * Created: 2020-03-25
 */

#ifndef _LZ4K_PRIVATE_H
#define _LZ4K_PRIVATE_H

#if !defined(__KERNEL__)

#include "lz4k.h"
#include <stdint.h> /* uint*_t */
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h> /* memcpy() */

#define likely(e) __builtin_expect(e, true)
#define unlikely(e) __builtin_expect(e, false)

#else

#include <linux/lz4k.h>
#define __STDC_WANT_LIB_EXT1__ 1
#include <linux/string.h> /* memcpy() */
#include <linux/types.h> /* uint8_t, int8_t, uint16_t, int16_t,
uint32_t, int32_t, uint64_t, int64_t */
#include <stddef.h>

typedef uint64_t uint_fast32_t;
typedef int64_t int_fast32_t;

#endif /* __KERNEL */

#if defined(__x86_64__)||defined(__x86_64_)||defined(__x86_64)
#define LZ4K_FOR_X86_64
#elif defined(i386) || defined(__i386__) || defined(__i386) ||\
	defined(__i486__) || defined(__i586__) || defined(__i686__) ||\
	defined(_M_IX86) || defined(__X86__) || defined(_X86_) ||\
	defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__)
#define LZ4K_FOR_X86
#elif defined(__aarch64__)
#define LZ4K_FOR_AARCH64
#endif

#if defined(__GNUC__) && (__GNUC__>=4)
#define LZ4K_WITH_GCC_INTRINSICS
#endif

#if !defined(__GNUC__)
#define __builtin_expect(e, v) (e)
#endif /* defined(__GNUC__) */

#if defined(__cplusplus)
/* bool, false, true are keywords */
#elif !defined(__STDC__) || (defined(__STDC_VERSION__) && \
	   __STDC_VERSION__ < 199901L)
typedef enum { false = 0, true = 1 } bool;
#elif !defined(__KERNEL__)
#include <stdbool.h>
#else
#include <linux/types.h>
#endif /* defined(__plus) */



#define STATUS_INCOMPRESSIBLE LZ4K_STATUS_INCOMPRESSIBLE
#define STATUS_FAILED		LZ4K_STATUS_FAILED
#define STATUS_READ_ERROR	LZ4K_STATUS_READ_ERROR
#define STATUS_WRITE_ERROR   LZ4K_STATUS_WRITE_ERROR


#ifdef max
#undef max
#endif
#define max(a, b) ((a) > (b) ? (a) : (b))

#ifdef min
#undef min
#endif
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifdef bit
#undef bit
#endif
#define bit(n) (1U << (n))

#ifdef bit64
#undef bit64
#endif
#define bit64(n) (1ULL << (n))

#ifdef mask
#undef mask
#endif
#define mask(n) (bit(n) - 1U)

#ifdef mask64
#undef mask64
#endif
#define mask64(n) (bit64(n) - 1ULL)

enum {
	BYTE_BITS = 8,
	BYTE_BITS_LOG2 = 3,
	BYTE_MAX = 255U,
	REPEAT_MIN = 4,
	TAG_BYTES_MAX = 3,
	TAG_BITS_MAX  = TAG_BYTES_MAX * 8,
	TAG_MASK = mask(TAG_BITS_MAX),
	BLOCK_4KB_LOG2  = 12,
	BLOCK_8KB_LOG2  = 13,
	BLOCK_16KB_LOG2 = 14,
	BLOCK_32KB_LOG2 = 15,
	BLOCK_64KB_LOG2 = 16,
	ENCODE_STATE_BYTES_LOG2 = 14 /* 14 for 16KB */
};

#if defined LZ4K_WITH_GCC_INTRINSICS
#define most_significant_bit_of(u) (int)(__builtin_expect((u) == 0, false) ? \
					-1 : 31 ^ __builtin_clz((unsigned)(u)))
#else /* #!defined LZ4K_WITH_GCC_INTRINSICS */
#error undefined most_significant_bit_of(unsigned u)
#endif /* #if defined LZ4K_WITH_GCC_INTRINSICS */

inline static uint64_t round_up_to_log2(uint64_t u, uint8_t log2)
{
	return (uint64_t)((u + mask64(log2)) & ~mask64(log2));
} /* round_up_to_log2 */

inline static uint64_t round_up_to_power_of2(uint64_t u)
{
	const int_fast32_t msb = most_significant_bit_of(u);
	return round_up_to_log2(u, (uint8_t)msb);
} /* round_up_to_power_of2 */

inline static void m_copy(void *dst, const void *src, uint_fast32_t total)
{
#if defined(__STDC_LIB_EXT1__)
	(void)memcpy_s(dst, total, src, (total * 2) >> 1); /* *2 >> 1 to avoid bot errors */
#else
	(void)__builtin_memcpy(dst, src, total);
#endif
}

inline static void m_set(void *dst, uint8_t value, uint_fast32_t total)
{
#if defined(__STDC_LIB_EXT1__)
	(void)memset_s(dst, total, value, (total * 2) >> 1); /* *2 >> 1 to avoid bot errors */
#else
	(void)__builtin_memset(dst, value, total);
#endif
}

inline static uint32_t read4_at(const void *p)
{
	uint32_t result;
	m_copy(&result, p, sizeof(result));
	return result;
}

inline static uint64_t read8_at(const void *p)
{
	uint64_t result;
	m_copy(&result, p, sizeof(result));
	return result;
}

inline static bool equal4(const uint8_t *const q, const uint8_t *const r)
{
	return read4_at(q) == read4_at(r);
}

inline static uint32_t hash32v_2(const uint64_t r, uint32_t shift)
{
	const uint32_t m = 3266489917U;
	return ((uint32_t)r * m) >> (32 - shift);
}

inline static uint32_t hash32_2(const uint8_t *r, uint32_t shift)
{
	return hash32v_2(read4_at(r), shift);
}


inline static uint32_t hash32v(const uint64_t r, uint32_t shift)
{
	const uint32_t m = 2654435761U;
	return ((uint32_t)r * m) >> (32 - shift);
}

inline static uint_fast32_t hash32(const uint8_t *r, uint32_t shift)
{
	return hash32v(read4_at(r), shift);
}

inline static uint32_t hash64v_5b(const uint64_t r, uint32_t shift)
{
	const uint64_t m = 889523592379ULL;
	return (uint32_t)(((r << 24) * m) >> (64 - shift));
}

inline static uint_fast32_t hash64_5b(const uint8_t *r, uint32_t shift)
{
	return hash64v_5b(read8_at(r), shift);
}

inline static uint32_t hash64v_6b(const uint64_t r, uint32_t shift)
{
	const uint64_t m = 227718039650203ULL;
	return (uint32_t)(((r << 16) * m) >> (64 - shift));
}

inline static uint_fast32_t hash64_6b(const uint8_t *r, uint32_t shift)
{
	return hash64v_6b(read8_at(r), shift);
}

inline static uint32_t hash64v_7b(const uint64_t r, uint32_t shift)
{
	const uint64_t m = 58295818150454627ULL;
	return (uint32_t)(((r << 8) * m) >> (64 - shift));
}

inline static uint_fast32_t hash64_7b(const uint8_t *r, uint32_t shift)
{
	return hash64v_7b(read8_at(r), shift);
}

inline static uint32_t hash64v_8b(const uint64_t r, uint32_t shift)
{
	const uint64_t m = 2870177450012600261ULL;
	return (uint32_t)((r * m) >> (64 - shift));
}

inline static uint_fast32_t hash64_8b(const uint8_t *r, uint32_t shift)
{
	return hash64v_8b(read8_at(r), shift);
}

static void copy_x_while_total(
	uint8_t *dst,
	const uint8_t *src,
	const size_t total,
	const size_t copy_min)
{
	m_copy(dst, src, copy_min);
	if (total > copy_min) {
		uint8_t * const dst_end_minus_x = dst + total - copy_min;
		do {
			dst += copy_min, src += copy_min;
			m_copy(dst, src, copy_min);
		} while (dst < dst_end_minus_x);
	}
} /* copy_x_while_total */

static void while_lt_copy_2x_as_x2(
	uint8_t *dst,
	const uint8_t *src,
	const uint8_t *dst_end,
	size_t copy_min)
{
	const size_t copy_max = copy_min << 1;
	while (dst < dst_end) {
		m_copy(dst, src, copy_min);
		m_copy(dst + copy_min, src + copy_min, copy_min);
		dst += copy_max;
		src += copy_max;
	}
}

static void while_lt_copy_x(
	uint8_t *dst,
	const uint8_t *src,
	const uint8_t *dst_end,
	size_t copy_min)
{
	while (dst < dst_end) {
		m_copy(dst, src, copy_min);
		dst += copy_min;
		src += copy_min;
	}
}

static void copy_x_while_lt(
	uint8_t *dst,
	const uint8_t *src,
	const uint8_t *dst_end,
	size_t copy_min)
{
	m_copy(dst, src, copy_min);
	while (dst + copy_min < dst_end) {
		dst += copy_min;
		src += copy_min;
		m_copy(dst, src, copy_min);
	}
}

static void copy_2x_as_x2_while_lt(
	uint8_t *dst,
	const uint8_t *src,
	const uint8_t *dst_end,
	size_t copy_min)
{
	const size_t copy_max = copy_min << 1;
	m_copy(dst, src, copy_min);
	m_copy(dst + copy_min, src + copy_min, copy_min);
	while (dst + copy_max < dst_end) {
		dst += copy_max;
		src += copy_max;
		m_copy(dst, src, copy_min);
		m_copy(dst + copy_min, src + copy_min, copy_min);
	}
}

#endif /* _LZ4K_PRIVATE_H */
