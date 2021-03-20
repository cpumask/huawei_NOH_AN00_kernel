/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: LZ4K compression algorithm
 * Author: Aleksei Romanovskii aleksei.romanovskii@huawei.com
 * Created: 2020-03-25
 */

#ifndef _LZ4K_H
#define _LZ4K_H

/* file lz4k.h
  This file contains the platform-independent API of LZ-class
  lossless codecs (compressors/decompressors) with complete
  in-place documentation.  The documentation is formatted
  in accordance with DOXYGEN mark-up format.  So, one can
  generate proper documentation, e.g. in HTML format, using DOXYGEN.

  Currently, LZ-class codecs, documented here, implement following
  algorithms for lossless data compression/decompression:
  \li "LZ HUAWEI" proprietary codec competing with LZ4 - LZ4K_Encode(),
  LZ4K_EncodeWithHistory(), LZ4K_Decode(), LZ4K_DecodeWithHistory()

  The LZ HUAWEI compressors accept any data as input and compress it
  without loss to a smaller size if possible.
  Compressed data produced by LZ HUAWEI compressor API LZ4K_Encode*(),
  can be decompressed only by LZ4K_Decode() API documented below.\n
  */
#ifdef __cplusplus
extern "C" {
#endif

/*
  lz4k_status defines simple set of status values returned by Huawei APIs
 */
typedef enum {
	LZ4K_STATUS_INCOMPRESSIBLE =  0, /* !< Returned when data is incompressible */
	LZ4K_STATUS_FAILED         = -1, /* !< Returned on general failure */
	LZ4K_STATUS_READ_ERROR =     -2, /* !< Returned when data reading failed */
	LZ4K_STATUS_WRITE_ERROR =    -3  /* !< Returned when data writing failed */
} lz4k_status;

/*
  LZ4K_Version() returns static unmutable string with algorithm version
 */
const char *lz4k_version(void);

/*
  LZ4K_EncodeStateBytesMin() returns number of bytes for state parameter,
  supplied to LZ4K_Encode(), LZ4K_EncodeWithHistory(), LZ4K_UpdateHashTable().
  So, state should occupy at least LZ4K_EncodeStateBytesMin() for mentioned
  functions to work correctly.
 */
unsigned lz4k_encode_state_bytes_min(void);

/*
  LZ4K_Encode() encodes/compresses one input buffer at *in, places
  result of encoding into one output buffer at *out if encoded data
  size fits specified values of out_max and outLimit.
  It returs size of encoded data in case of success or value<=0 otherwise.
  The result of successful encoding is in HUAWEI proprietary format, that
  is the encoded data can be decoded only by LZ4K_Decode().

  \return
    \li positive value\n
      if encoding was successful. The value returned is the size of encoded
      (compressed) data always <=out_max.
    \li non-positive value\n
      if in==0||in_max==0||out==0||out_max==0 or
      if out_max is less than needed for encoded (compressed) data.
    \li 0 value\n
      if encoded data size >= outLimit

  \param[in] state
    !=0, pointer to state buffer used internally by the function.  Size of
    state in bytes should be at least LZ4K_EncodeStateBytesMin().  The content
    of state buffer will be changed during encoding.

  \param[in] in
    !=0, pointer to the input buffer to encode (compress).  The content of
    the input buffer does not change during encoding.

  \param[in] out
    !=0, pointer to the output buffer where to place result of encoding
    (compression).
    If encoding is unsuccessful, e.g. out_max or outLimit are less than
    needed for encoded data then content of out buffer may be arbitrary.

  \param[in] in_max
    !=0, size in bytes of the input buffer at *in

  \param[in] out_max
    !=0, size in bytes of the output buffer at *out

  \param[in] outLimit
    encoded data size soft limit in bytes. Due to performance reasons it is
    not guaranteed that
    LZ4K_Encode will always detect that resulting encoded data size is
    bigger than outLimit.
    Hovewer, when reaching outLimit is detected, LZ4K_Encode() returns
    earlier and spares CPU cycles.  Caller code should recheck result
    returned by LZ4K_Encode() (value greater than 0) if it iss really
    less or equal than outLimit.
    outLimit is ignored if it is equal to 0.
 */
int lz4k_encode(
	void *const state,
	const char *const in,
	char *out,
	unsigned in_max,
	unsigned out_max,
	unsigned outLimit);

/*
  LZ4K_UpdateHashTable() fills/updates state (hash table) in the same way as
  LZ4K_Encode does while encoding (compressing).
  The state and its content can then be used by LZ4K_EncodeWithHistory()
  to encode (compress) data more efficiently.
  By other words, effect of LZ4K_UpdateHashTable() is the same as
  LZ4K_Encode() with all encoded output discarded.

  Example sequence of calls for LZ4K_UpdateHashTable and
  LZ4K_EncodeWithHistory:
    //dictionary (1st) block
    int result0=LZ4K_UpdateHashTable(state, in0, in0, inMax0);
//delta (2nd) block
    int result1=LZ4K_EncodeWithHistory(state, in0, in, out, in_max,
                                       out_max);

  \param[in] state
    !=0, pointer to state buffer used internally by LZ4K_Encode*.
    Size of state in bytes should be at least LZ4K_EncodeStateBytesMin().
    The content of state buffer is zeroed at the beginning of
    LZ4K_UpdateHashTable ONLY when in0==in.
    The content of state buffer will be changed inside
    LZ4K_UpdateHashTable.

  \param[in] in0
    !=0, pointer to the reference/dictionary input buffer that was used
    as input to preceding call of LZ4K_Encode() or LZ4K_UpdateHashTable()
    to fill/update the state buffer.
    The content of the reference/dictionary input buffer does not change
    during encoding.
    The in0 is needed for use-cases when there are several dictionary and
    input blocks interleaved, e.g.
    <dictionaryA><inputA><dictionaryB><inputB>..., or
    <dictionaryA><dictionaryB><inputAB>..., etc.

  \param[in] in
    !=0, pointer to the input buffer to fill/update state as if encoding
    (compressing) this input.  This input buffer is also called dictionary
    input buffer.
    The content of the input buffer does not change during encoding.
    The two buffers - at in0 and at in - should be contiguous in memory.
    That is, the last byte of buffer at in0 is located exactly before byte
    at in.

  \param[in] in_max
    !=0, size in bytes of the input buffer at in.
 */
int lz4k_update_hash_table(
	void *const state,
	const char *const in0,
	const char *const in,
	unsigned in_max);

/*
  LZ4K_EncodeWithHistory() encodes (compresses) data from one input buffer
  using one reference buffer as dictionary and places the result of
  compression into one output buffer.
  The result of successful compression is in HUAWEI proprietary format, so
  that compressed data can be decompressed only by LZ4K_DecodeWithHistory().
  Reference/dictionary buffer and input buffer should be contiguous in
  memory.

  Example sequence of calls for LZ4K_UpdateHashTable and
  LZ4K_EncodeWithHistory:
//dictionary (1st) block
    int result0=LZ4K_UpdateHashTable(state, in0, in0, inMax0);
//delta (2nd) block
    int result1=LZ4K_EncodeWithHistory(state, in0, in, out, in_max,
                                       out_max);

  Example sequence of calls for LZ4K_Encode and LZ4K_EncodeWithHistory:
//dictionary (1st) block
    int result0=LZ4K_Encode(state, in0, out0, inMax0, outMax0);
//delta (2nd) block
    int result1=LZ4K_EncodeWithHistory(state, in0, in, out, in_max,
                                       out_max);

  \return
    \li positive value\n
      if encoding was successful. The value returned is the size of encoded
      (compressed) data.
    \li non-positive value\n
      if state==0||in0==0||in==0||in_max==0||out==0||out_max==0 or
      if out_max is less than needed for encoded (compressed) data.

  \param[in] state
    !=0, pointer to state buffer used internally by the function.  Size of
    state in bytes should be at least LZ4K_EncodeStateBytesMin().  For more
    efficient encoding the state buffer may be filled/updated by calling
    LZ4K_UpdateHashTable() or LZ4K_Encode() before LZ4K_EncodeWithHistory().
    The content of state buffer is zeroed at the beginning of
    LZ4K_EncodeWithHistory ONLY when in0==in.
    The content of state will be changed during encoding.

  \param[in] in0
    !=0, pointer to the reference/dictionary input buffer that was used as
    input to preceding call of LZ4K_Encode() or LZ4K_UpdateHashTable() to
    fill/update the state buffer.
    The content of the reference/dictionary input buffer does not change
    during encoding.

  \param[in] in
    !=0, pointer to the input buffer to encode (compress).  The input buffer
    is compressed using content of the reference/dictionary input buffer at
    in0. The content of the input buffer does not change during encoding.
    The two buffers - at *in0 and at *in - should be contiguous in memory.
    That is, the last byte of buffer at *in0 is located exactly before byte
    at *in.

  \param[in] out
    !=0, pointer to the output buffer where to place result of encoding
    (compression). If compression is unsuccessful then content of out
    buffer may be arbitrary.

  \param[in] in_max
    !=0, size in bytes of the input buffer at *in

  \param[in] out_max
    !=0, size in bytes of the output buffer at *out.
 */
int lz4k_encode_with_history(
	void *const state,
	const char *const in0,
	const char *const in,
	char *out,
	unsigned in_max,
	unsigned out_max);

/*
  LZ4K_Decode() decodes (decompresses) data from one input buffer and places
  the result of decompression into one output buffer.  The encoded data in input
  buffer should be in HUAWEI proprietary format, produced by LZ4K_Encode()
  or by LZ4K_EncodeWithHistory().

  \return
    \li positive value\n
      if decoding was successful. The value returned is the size of decoded
      (decompressed) data.
    \li non-positive value\n
      if in==0||in_max==0||out==0||out_max==0 or
      if out_max is less than needed for decoded (decompressed) data or
      if input encoded data format is corrupted.

  \param[in] in
    !=0, pointer to the input buffer to decode (decompress).  The content of
    the input buffer does not change during decoding.

  \param[in] out
    !=0, pointer to the output buffer where to place result of decoding
    (decompression). If decompression is unsuccessful then content of out
    buffer may be arbitrary.

  \param[in] in_max
    !=0, size in bytes of the input buffer at in

  \param[in] out_max
    !=0, size in bytes of the output buffer at out
 */
int lz4k_decode(
	const char *const in,
	char *const out,
	unsigned in_max,
	unsigned out_max);

/*
  LZ4K_DecodeWithHistory() decodes (decompresses) data from one input buffer
  and places the result of decompression into one output buffer.  The
  compressed data in input buffer should be in format, produced by
  LZ4K_EncodeWithHistory().

  Example sequence of calls for LZ4K_Decode and LZ4K_DecodeWithHistory:
//dictionary (1st) block
    int result0=LZ4K_Decode(in0, out0, inMax0, outMax0);
//delta (2nd) block
    int result1=LZ4K_DecodeWithHistory(in, out0, out, in_max, out_max);

  \return
    \li positive value\n
      if decoding was successful. The value returned is the size of decoded
      (decompressed) data.
    \li non-positive value\n
      if in==0||in_max==0||out==0||out_max==0 or
      if out_max is less than needed for decoded (decompressed) data or
      if input data format is corrupted.

  \param[in] in
    !=0, pointer to the input buffer to decode (decompress).  The content of
    the input buffer does not change during decoding.

  \param[in] out0
    !=0, pointer to the dictionary input buffer that was used as input to
    LZ4K_UpdateHashTable() to fill/update the state buffer.  The content
    of the dictionary input buffer does not change during decoding.

  \param[in] out
    !=0, pointer to the output buffer where to place result of decoding
    (decompression). If decompression is unsuccessful then content of out
    buffer may be arbitrary.
    The two buffers - at *out0 and at *out - should be contiguous in memory.
    That is, the last byte of buffer at *out0 is located exactly before byte
    at *out.

  \param[in] in_max
    !=0, size in bytes of the input buffer at *in

  \param[in] out_max
    !=0, size in bytes of the output buffer at *out
 */
int lz4k_decode_with_history(
	const char *in,
	char *const out0,
	char *const out,
	unsigned in_max,
	unsigned out_max);

#ifdef __cplusplus
}
#endif

#endif /* _LZ4K_H */
