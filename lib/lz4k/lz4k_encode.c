/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: LZ4K compression algorithm
 * Author: Aleksei Romanovskii aleksei.romanovskii@huawei.com
 * Created: 2020-03-25
 */

#if !defined(__KERNEL__)
#include "lz4k.h"
#else
#include <linux/lz4k.h>
#include <linux/module.h>
#endif

#include "lz4k_private.h"

/* <nrSize bytes for whole block>+<1 terminating 0 byte> */
inline static uint_fast32_t size_bytes_max(uint_fast32_t u)
{
	return (u + BYTE_MAX - 1) / BYTE_MAX;
}

/* minimum encoded size for non-compressible data */
static uint_fast32_t encoded_bytes_min(
	uint_fast32_t nr_log2,
	uint_fast32_t in_max)
{
	return in_max < mask(nr_log2) ?
		TAG_BYTES_MAX + in_max :
		TAG_BYTES_MAX + size_bytes_max(in_max - mask(nr_log2)) +
		in_max;
}

enum {
	NR_COPY_LOG2 = 4,
	NR_COPY_MIN = 1 << NR_COPY_LOG2
};

/* maximum encoded size for non-comprressible data if "fast" encoder is used */
static uint_fast32_t encoded_bytes_max(
	uint_fast32_t nr_log2,
	uint_fast32_t in_max)
{
	return in_max < mask(nr_log2) ?
		TAG_BYTES_MAX + round_up_to_log2(in_max, NR_COPY_LOG2) :
		TAG_BYTES_MAX + size_bytes_max(in_max - mask(nr_log2)) +
			round_up_to_log2(in_max, NR_COPY_LOG2);
}

#if defined(LZ4K_BETTER_CR)

#define USE_HASH
#define USE_HASH2

/* max CR with 2-3 hashes */
#define HASH0_V hash64v_6b
#define HASH0 hash64_6b

#define HASH1_V hash64v_5b
#define HASH1 hash64_5b

#define HASH2_V hash32v
#define HASH2 hash32

#else

/* to compete with LZ4 */
#define HASH0_V hash64v_6b
#define HASH0 hash64_6b /* CR increase order: +STEP, have OFFSETS,
			    use HASH_64_5B */

#endif

#if !defined(USE_HASH) && !defined(USE_HASH2)
enum {
	HT_LOG2 = ENCODE_STATE_BYTES_LOG2 - 1
};
#else
enum {
	HT_LOG2 = ENCODE_STATE_BYTES_LOG2 - 2
};
#endif

unsigned lz4k_encode_state_bytes_min(void)
{
	enum {
		BYTES_LOG2 = HT_LOG2 + 1
	};
	unsigned bytes_total = (1U << BYTES_LOG2);
#ifdef USE_HASH
	bytes_total <<= 1;
#endif
#ifdef USE_HASH2
	bytes_total += (1U << BYTES_LOG2);
#endif
	return bytes_total;
} /* lz4k_encode_state_bytes_min */
#if defined(__KERNEL__)
EXPORT_SYMBOL(lz4k_encode_state_bytes_min);
#endif

inline static uint_fast32_t u_32(int64_t i)
{
	return (uint_fast32_t)i;
}

static uint8_t *out_size_bytes(uint_fast32_t u, uint8_t *out_at)
{
	for (; unlikely(u >= BYTE_MAX); u -= BYTE_MAX)
		*out_at++ = (uint8_t)BYTE_MAX;
	*out_at++ = (uint8_t)u;
	return out_at;
} /* out_size_bytes */

static int out_tail(
	const uint8_t *nr0,
	const uint8_t *const in_end,
	const uint8_t *const out,
	uint8_t *out_at,
	uint8_t *out_end,
	const uint_fast32_t nr_log2,
	const uint_fast32_t off_log2,
	bool check_out)
{
	const uint_fast32_t nr_mask = mask(nr_log2),
		r_log2 = TAG_BITS_MAX - (off_log2 + nr_log2);
	uint8_t *const tag_at = (uint8_t *)out_at;
	const uint_fast32_t nr_bytes_max = u_32(in_end - nr0);
	if (encoded_bytes_min(nr_log2, nr_bytes_max) > u_32(out_end - out_at))
		return check_out ? STATUS_WRITE_ERROR :
				  STATUS_INCOMPRESSIBLE;
	out_at += TAG_BYTES_MAX;
	if (nr_bytes_max < nr_mask) {
		/* caller guarantees at least one nr-byte */
		uint_fast32_t utag = (nr_bytes_max << (off_log2 + r_log2));
		m_copy(tag_at, &utag, TAG_BYTES_MAX);
	} else { /* nr_bytes_max>=nr_mask */
		uint_fast32_t bytes_left = nr_bytes_max - nr_mask;
		uint_fast32_t utag = (nr_mask << (off_log2 + r_log2));
		m_copy(tag_at, &utag, TAG_BYTES_MAX);
		out_at = out_size_bytes(bytes_left, out_at);
	} /* if (nr_bytes_max<nr_mask) */
	m_copy(out_at, nr0, nr_bytes_max);
	return (int)(out_at + nr_bytes_max - out);
} /* out_tail */

static uint8_t *out_non_repeat(
	uint_fast32_t *utag,
	uint8_t *out_at,
	uint8_t *out_end,
	const uint8_t *const nr0,
	uint_fast32_t nr_bytes_max,
	const uint_fast32_t nr_log2,
	const uint_fast32_t off_log2,
	bool check_out)
{
	const uint_fast32_t nr_mask = mask(nr_log2),
		r_log2 = TAG_BITS_MAX - (off_log2 + nr_log2);
	if (likely(nr_bytes_max < nr_mask)) {
		if (check_out && nr_bytes_max > u_32(out_end - out_at))
			return NULL;
		*utag |= (nr_bytes_max << (off_log2 + r_log2));
	} else { /* nr_bytes_max >= nr_mask */
		uint_fast32_t bytes_left = nr_bytes_max - nr_mask;
		*utag |= (nr_mask << (off_log2 + r_log2));
		if (check_out &&
		    size_bytes_max(bytes_left) + nr_bytes_max >
		    u_32(out_end - out_at))
			return NULL;
		out_at = out_size_bytes(bytes_left, out_at);
	} /* if (nr_bytes_max<nr_mask) */
	if (check_out)
		m_copy(out_at, nr0, nr_bytes_max);
	else
		copy_x_while_total(out_at, nr0, nr_bytes_max, NR_COPY_MIN);
	out_at += nr_bytes_max;
	return out_at;
} /* out_non_repeat */

static uint8_t *out_tag_repeat(
	uint_fast32_t r_bytes_max,
	uint_fast32_t utag,
	uint8_t *const tag_at,
	uint8_t *out_at,
	uint8_t *const out_end,
	const uint_fast32_t nr_log2,
	const uint_fast32_t off_log2,
	const bool check_out) /* =false when
out_max>=encoded_bytes_max(in_max), =true otherwise */
{
	const uint_fast32_t r_mask = mask(TAG_BITS_MAX - (off_log2 + nr_log2));
	if (likely(r_bytes_max - REPEAT_MIN < r_mask)) {
		utag |= ((r_bytes_max - REPEAT_MIN) << off_log2);
	} else {
		uint_fast32_t bytes_left = r_bytes_max - REPEAT_MIN - r_mask;
		if (check_out && size_bytes_max(bytes_left) > u_32(out_end - out_at))
			return NULL;
		utag |= (uint_fast32_t)(r_mask << off_log2);
		out_at = out_size_bytes(bytes_left, out_at);
	}
	m_copy(tag_at, &utag, TAG_BYTES_MAX);
	return out_at; /* SUCCESS: continue compression */
} /* out_tag_repeat */

static const uint8_t *repeat_start(
	const uint8_t *q,
	const uint8_t *r,
	const uint8_t *const nr0,
	const uint8_t *const in0)
{
	while (likely(q > in0) && unlikely(q[-1] == r[-1])) {
		--q, --r;
		if (r <= nr0)
			break;
	} /* while */
	return r;
} /* repeat_start */

static const uint8_t *repeat_end(
	const uint8_t *q,
	const uint8_t *r,
	const uint8_t *const in_end_minus16)
{
	q += REPEAT_MIN;
	r += REPEAT_MIN;
	/* caller guarantees r+12<=in_end */
	do {
		const uint64_t x = read8_at(q) ^ read8_at(r);
		if (x)
			return r + (__builtin_ctzl(x) >> BYTE_BITS_LOG2);
		/* some bytes differ:+ count of trailing 0-bits/bytes */
		q += sizeof(uint64_t), r += sizeof(uint64_t);
	} while (likely(r <= in_end_minus16)); /* once, at input block end */
	return read4_at(q) == read4_at(r) ? r + sizeof(uint32_t) : r;
} /* repeat_end */

static uint_fast32_t repeat_found(
	uint16_t *const ht0,
	const uint8_t *const in0,
	const uint8_t *const r)
{
	const uint64_t r_little_endian = read8_at(r);
	const uint_fast32_t h0 = HASH0_V(r_little_endian, HT_LOG2);
	const uint8_t *const q0 = in0 + ht0[h0];
	ht0[h0] = (uint16_t)(r - in0);
	if (equal4(q0, r))
		return (uint_fast32_t)(r - q0);
#ifdef USE_HASH
	uint16_t *const ht1 = ht0 + (1 << HT_LOG2);
	const uint_fast32_t h1 = HASH1_V(r_little_endian, HT_LOG2);
	const uint8_t *const q1 = in0 + ht1[h1];
	ht1[h1] = (uint16_t)(r - in0);
	if (equal4(q1, r))
		return (uint_fast32_t)(r - q1);
#endif
#ifdef USE_HASH2
	uint16_t *const ht2 = ht1 + (1 << HT_LOG2);
	const uint_fast32_t h2 = HASH2_V(r_little_endian, HT_LOG2);
	const uint8_t *const q2 = in0 + ht2[h2];
	ht2[h2] = (uint16_t)(r - in0);
	if (equal4(q2, r))
		return (uint_fast32_t)(r - q2);
#endif
	return 0;
} /* repeat_found */

static void extend_repeat(
	const uint8_t *q,
	const uint8_t *const s,
	const uint8_t *const nr0,
	const uint8_t *const in0,
	const uint8_t *const in_end_safe,
	const uint8_t **r,
	uint_fast32_t *r_bytes_max,
	uint_fast32_t *utag)
{
	const uint8_t *t = repeat_start(q, s, nr0, in0);
	const uint8_t *const t_end = repeat_end(q, s, in_end_safe);
	const uint_fast32_t t_bytes_total = u_32(t_end - t);
	if (*r_bytes_max < t_bytes_total) {
		*r_bytes_max = t_bytes_total;
		*r = t;
		*utag = u_32(s - q);
	}
} /* extend_repeat */

static uint_fast32_t get_repeat(
	uint16_t *const ht0,
	const uint8_t *const nr0,
	const uint8_t *const in0,
	const uint8_t *const in_end_safe,
	const uint8_t **r,
	uint_fast32_t *utag)
{
	const uint8_t *const s = *r + 1;
	const uint8_t *const r_end = repeat_end(*r - *utag, *r, in_end_safe);
	uint_fast32_t r_bytes_max = 0;
	*r = repeat_start(*r - *utag, *r, nr0, in0);
	r_bytes_max = (uint_fast32_t)(r_end - *r);
	if (likely(s <= in_end_safe)) {
		const uint_fast32_t h0 = HASH0(s, HT_LOG2);
		const uint8_t *q0 = in0 + ht0[h0];
		if (*utag != (uint_fast32_t)(s - q0) && equal4(q0, s))
			extend_repeat(q0, s, nr0, in0, in_end_safe, r,
				&r_bytes_max, utag);
#ifdef USE_HASH
		uint16_t *const ht1 = ht0 + (1 << HT_LOG2);
		const uint_fast32_t h1 = HASH1(s, HT_LOG2);
		const uint8_t *q1 = in0 + ht1[h1];
		if (*utag != (uint_fast32_t)(s - q1) && equal4(q1, s))
			extend_repeat(q1, s, nr0, in0, in_end_safe, r,
				&r_bytes_max, utag);
#endif
#ifdef USE_HASH2
		uint16_t *const ht2 = ht1 + (1 << HT_LOG2);
		const uint_fast32_t h2 = HASH2(s, HT_LOG2);
		const uint8_t *q2 = in0 + ht2[h2];
		if (*utag != (uint_fast32_t)(s - q2) && equal4(q2, s))
			extend_repeat(q2, s, nr0, in0, in_end_safe, r,
				&r_bytes_max, utag);
#endif
	}
	return r_bytes_max;
} /* get_repeat */

static void hash_repeat_tail(
	uint16_t *const ht0,
	const uint8_t *in0,
	const uint8_t *r)
{
	enum {
		DEC1 = 1,
		DEC2 = 2
	};
	uint64_t r_little_endian = read8_at(r - DEC2);
	ht0[HASH0_V(r_little_endian, HT_LOG2)] = (uint16_t)(r - DEC2 - in0);
#ifdef USE_HASH
	uint16_t *const ht1 = ht0 + (1 << HT_LOG2);
	ht1[HASH1_V(r_little_endian, HT_LOG2)] = (uint16_t)(r - DEC2 - in0);
#endif
#ifdef USE_HASH2
	uint16_t *const ht2 = ht1 + (1 << HT_LOG2);
	ht2[HASH2_V(r_little_endian, HT_LOG2)] = (uint16_t)(r - DEC2 - in0);
#endif
	r_little_endian = read8_at(r - DEC1);
	ht0[HASH0_V(r_little_endian, HT_LOG2)] = (uint16_t)(r - DEC1 - in0);
#ifdef USE_HASH
	ht1[HASH1_V(r_little_endian, HT_LOG2)] = (uint16_t)(r - DEC1 - in0);
#endif
#ifdef USE_HASH2
	ht2[HASH2_V(r_little_endian, HT_LOG2)] = (uint16_t)(r - DEC1 - in0);
#endif
}

/*
 * Compressed data format (where {} means 0 or more occurrences, [] means
 * optional):
 * <24bits tag: (off_log2 rOffset| r_log2 rSize|nr_log2 nrSize)>
 * {<nrSize byte>}[<nr bytes>]{<rSize byte>}
 * <rSize byte> and <nrSize byte> bytes are terminated by byte != 255
*/

/*
 *Proof that 's' increments are safe-NO pointer overflows are possible:
 *
 * While using STEP_LOG2=5, step_start=1<<STEP_LOG2 == 32 we increment s
 * 32 times by 1, 32 times by 2, 32 times by 3, and so on:
 * 32*1+32*2+32*3+...+32*31 == 32*SUM(1..31) == 32*((1+31)*15+16).
 * So, we can safely increment s by at most 31 for input block size <=
 * 1<<13 < 15872.
 *
 * More precisely, STEP_LIMIT == x for any input block  calculated as follows:
 * 1<<off_log2 >= (1<<STEP_LOG2)*((x+1)(x-1)/2+x/2) ==>
 * 1<<(off_log2-STEP_LOG2+1) >= x^2+x-1 ==>
 * x^2+x-1-1<<(off_log2-STEP_LOG2+1) == 0, which is solved by standard
 * method.
 * To avoid overhead here conservative approximate value of x is calculated
 * as average of two nearest square roots, see STEP_LIMIT above.
 */
static int encode_any(
	uint16_t *const ht0,
	const uint8_t *const in0,
	const uint8_t *const in,
	uint8_t *const out,
	const uint8_t *const in_end,
	uint8_t *const out_end, /* ==out_limit for !check_out */
	const uint_fast32_t nr_log2,
	const uint_fast32_t off_log2,
	const bool check_out)
{   /* caller guarantees off_log2 <=16 */
	enum {
		STEP_LOG2 = 4 /* increase for better CR */
	};
	const uint8_t *r = in;
	uint8_t *out_at = out;
	const uint8_t *const in_end_safe = in_end - NR_COPY_MIN;
	uint8_t *tag_at = out_at;
	for (;;) {
		uint_fast32_t r_bytes_max = 0;
		uint_fast32_t nr_bytes_max = 0;
		uint_fast32_t utag = 0;
		const uint8_t *const nr0 = r++;
		uint_fast32_t step = 0;
		for (step = 1 << STEP_LOG2; ; r += (++step >> STEP_LOG2)) {
			if (unlikely(r > in_end_safe))
				return in == nr0 ? STATUS_INCOMPRESSIBLE :
					out_tail(nr0, in_end, out, out_at, out_end,
						 nr_log2, off_log2, check_out);
			utag = repeat_found(ht0, in0, r);
			if (utag)
				break;
		} /* for */
		r_bytes_max = get_repeat(ht0, nr0, in0, in_end_safe, &r, &utag);
		nr_bytes_max = (uint_fast32_t)(r - nr0);
		out_at = out_non_repeat(&utag, out_at + TAG_BYTES_MAX, out_end,
				nr0, nr_bytes_max, nr_log2, off_log2, check_out);
		if (check_out && out_at == NULL)
			return STATUS_WRITE_ERROR;
		for (;;) {
			out_at = out_tag_repeat(r_bytes_max, utag, tag_at,
				out_at, out_end, nr_log2, off_log2, check_out);
			if (check_out && out_at == NULL)
				return STATUS_WRITE_ERROR;
			r += r_bytes_max;
			if (unlikely(r >= in_end_safe)) /* >=:r+1>in_end_safe */
				return r == in_end ? (int)(out_at - out) :
					out_tail(r, in_end, out, out_at, out_end,
						 nr_log2, off_log2, check_out);
			if (check_out && out_end - out_at < TAG_BYTES_MAX)
				return STATUS_WRITE_ERROR;
			tag_at = out_at;
			hash_repeat_tail(ht0, in0, r);
			utag = repeat_found(ht0, in0, r);
			if (!utag)
				break;
			r_bytes_max = u_32(repeat_end(r - utag, r, in_end_safe) - r);
			out_at += TAG_BYTES_MAX; /* see check_out above */
		} /* for */
	} /* for */
} /* encode_any */

static int encode_fast(
	uint16_t *const ht,
	const uint8_t *const in,
	uint8_t *const out,
	const uint8_t *const in_end,
	uint8_t *const out_end, /* ==out_limit for !check_out */
	const uint_fast32_t nr_log2,
	const uint_fast32_t off_log2)
{ /* caller guarantees off_log2 <=16 */
	return encode_any(ht, in, in, out, in_end, out_end, nr_log2, off_log2,
		false); /* !check_out */
} /* encode_fast */

static int encode_slow(
	uint16_t *const ht,
	const uint8_t *const in,
	uint8_t *const out,
	const uint8_t *const in_end,
	uint8_t *const out_end, /* ==out_limit for !check_out */
	const uint_fast32_t nr_log2,
	const uint_fast32_t off_log2)
{ /* caller guarantees off_log2 <=16 */
	return encode_any(ht, in, in, out, in_end, out_end, nr_log2, off_log2,
		true); /* check_out */
} /* encode_slow */

static int encode4kb(
	uint16_t *const state,
	const uint8_t *const in,
	uint8_t *out,
	const uint_fast32_t in_max,
	const uint_fast32_t out_max,
	const uint_fast32_t out_limit)
{
	enum {
		NR_LOG2 = 6
	};
	const int result = (encoded_bytes_max(NR_LOG2, in_max) > out_max) ?
		encode_slow(state, in, out, in + in_max, out + out_max, NR_LOG2,
			BLOCK_4KB_LOG2) :
		encode_fast(state, in, out, in + in_max, out + out_limit, NR_LOG2,
			BLOCK_4KB_LOG2);
	return result <= 0 ? result : result + 1; /* +1 for in_log2 */
}

static int encode8kb(
	uint16_t *const state,
	const uint8_t *const in,
	uint8_t *out,
	const uint_fast32_t in_max,
	const uint_fast32_t out_max,
	const uint_fast32_t out_limit)
{
	enum {
		NR_LOG2 = 5
	};
	const int result = (encoded_bytes_max(NR_LOG2, in_max) > out_max) ?
		encode_slow(state, in, out, in + in_max, out + out_max, NR_LOG2,
			BLOCK_8KB_LOG2) :
		encode_fast(state, in, out, in + in_max, out + out_limit, NR_LOG2,
			 BLOCK_8KB_LOG2);
	return result <= 0 ? result : result + 1; /* +1 for in_log2 */
}

static int encode16kb(
	uint16_t *const state,
	const uint8_t *const in,
	uint8_t *out,
	const uint_fast32_t in_max,
	const uint_fast32_t out_max,
	const uint_fast32_t out_limit)
{
	enum {
		NR_LOG2 = 5
	};
	const int result = (encoded_bytes_max(NR_LOG2, in_max) > out_max) ?
		encode_slow(state, in, out, in + in_max, out + out_max, NR_LOG2,
			BLOCK_16KB_LOG2) :
		encode_fast(state, in, out, in + in_max, out + out_limit, NR_LOG2,
			BLOCK_16KB_LOG2);
	return result <= 0 ? result : result + 1; /* +1 for in_log2 */
}

static int encode32kb(
	uint16_t *const state,
	const uint8_t *const in,
	uint8_t *out,
	const uint_fast32_t in_max,
	const uint_fast32_t out_max,
	const uint_fast32_t out_limit)
{
	enum {
		NR_LOG2 = 4
	};
	const int result = (encoded_bytes_max(NR_LOG2, in_max) > out_max) ?
		encode_slow(state, in, out, in + in_max, out + out_max, NR_LOG2,
			BLOCK_32KB_LOG2) :
		encode_fast(state, in, out, in + in_max, out + out_limit, NR_LOG2,
			BLOCK_32KB_LOG2);
	return result <= 0 ? result : result + 1; /* +1 for in_log2 */
}

static int encode64kb(
	uint16_t *const state,
	const uint8_t *const in,
	uint8_t *out,
	const uint_fast32_t in_max,
	const uint_fast32_t out_max,
	const uint_fast32_t out_limit)
{
	enum {
		NR_LOG2 = 4
	};
	const int result = (encoded_bytes_max(NR_LOG2, in_max) > out_max) ?
		encode_slow(state, in, out, in + in_max, out + out_max, NR_LOG2,
			BLOCK_64KB_LOG2) :
		encode_fast(state, in, out, in + in_max, out + out_limit, NR_LOG2,
			BLOCK_64KB_LOG2);
	return result <= 0 ? result : result + 1; /* +1 for in_log2 */
}

static int encode(
	uint16_t *const state,
	const uint8_t *const in,
	uint8_t *out,
	uint_fast32_t in_max,
	uint_fast32_t out_max,
	uint_fast32_t out_limit)
{
	m_set(state, 0, lz4k_encode_state_bytes_min());
	const uint8_t in_log2 = (uint8_t)(most_significant_bit_of(
	round_up_to_power_of2(in_max - REPEAT_MIN)));
	*out = (uint8_t)(in_log2 - BLOCK_4KB_LOG2);
	++out;
	--out_max;
	--out_limit;
	if (in_log2 < BLOCK_8KB_LOG2) {
		return encode4kb(state, in, out, in_max, out_max, out_limit);
	}
	if (in_log2 == BLOCK_8KB_LOG2) {
		return encode8kb(state, in, out, in_max, out_max, out_limit);
	}
	if (in_log2 == BLOCK_16KB_LOG2) {
		return encode16kb(state, in, out, in_max, out_max, out_limit);
	}
	if (in_log2 == BLOCK_32KB_LOG2) {
		return encode32kb(state, in, out, in_max, out_max, out_limit);
	}
	if (in_log2 == BLOCK_64KB_LOG2) {
		return encode64kb(state, in, out, in_max, out_max, out_limit);
	}
	return STATUS_FAILED;
}

int lz4k_encode(
	void *const state,
	const char *const in,
	char *out,
	unsigned in_max,
	unsigned out_max,
	unsigned out_limit)
{
	const unsigned min_gain = max(64, in_max >> 5);
	if (unlikely(state == NULL))
		return STATUS_FAILED;
	if (unlikely(in == NULL || out == NULL))
		return STATUS_FAILED;
	if (unlikely(in_max <= min_gain))
		return STATUS_INCOMPRESSIBLE;
	if (unlikely(out_max <= min_gain)) /* need 1 byte for in_log2 */
		return STATUS_FAILED;
	/* ++use volatile pointers to prevent compiler optimizations */
	const uint8_t *volatile in_end = (const uint8_t*)in + in_max;
	const uint8_t *volatile out_end = (uint8_t*)out + out_max;
	if (unlikely((const uint8_t*)in >= in_end || (uint8_t*)out >= out_end))
		return STATUS_FAILED;
	const void *volatile state_end =
		(uint8_t*)state + lz4k_encode_state_bytes_min();
	if (unlikely(state >= state_end))
		return STATUS_FAILED; /* pointer overflow */
	if (!out_limit || out_limit >= min(in_max, out_max))
		out_limit = min(in_max, out_max) - min_gain;
	return encode((uint16_t*)state, (const uint8_t*)in,
		(uint8_t*)out, in_max, out_max, out_limit);
} /* lz4k_encode */
#if defined(__KERNEL__)
EXPORT_SYMBOL(lz4k_encode);
#endif

const char *lz4k_version(void)
{
	static const char *version = "2020.05.07";
	return version;
}
#if defined(__KERNEL__)
EXPORT_SYMBOL(lz4k_version);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("LZ4K encoder");
#endif
