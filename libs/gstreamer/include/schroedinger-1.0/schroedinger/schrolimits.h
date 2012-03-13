
#ifndef _SCHRO_LIMIT_H_
#define _SCHRO_LIMIT_H_

#include <schroedinger/schroutils.h>

SCHRO_BEGIN_DECLS

/**
 * Section:schrolimit:
 *
 * This section contains a number of arbitrary limits that are
 * compiled into Schroedinger.  These can be changed as necessary
 * if they are unsuitable.
 */
/**
 * SCHRO_LIMIT_TRANSFORM_DEPTH:
 *
 * The maximum transform depth that the decoder can handle.
 */
#define SCHRO_LIMIT_TRANSFORM_DEPTH 6

/**
 * SCHRO_LIMIT_ENCODER_TRANSFORM_DEPTH:
 *
 * The maximum transform depth that the encoder can handle.  Must be
 * less than SCHRO_LIMIT_TRANSFORM_DEPTH.
 */
#define SCHRO_LIMIT_ENCODER_TRANSFORM_DEPTH 6

/**
 * SCHRO_LIMIT_SUBBANDS:
 *
 * The maximum number of subbands.
 */
#define SCHRO_LIMIT_SUBBANDS (1+3*SCHRO_LIMIT_TRANSFORM_DEPTH)

/**
 * SCHRO_LIMIT_REFERENCE_FRAMES:
 *
 * The maximum number of active reference frames.  In the encoder,
 * the number of active reference frames may be much larger than in
 * the resulting stream.
 */
#define SCHRO_LIMIT_REFERENCE_FRAMES 8

/**
 * SCHRO_LIMIT_FRAME_QUEUE_LENGTH:
 *
 * The maximum number of pictures being processed by the encoder.
 */
#define SCHRO_LIMIT_FRAME_QUEUE_LENGTH 40

/**
 * SCHRO_LIMIT_WIDTH:
 *
 * Maximum picture width.
 */
#define SCHRO_LIMIT_WIDTH 2048

/**
 * SCHRO_LIMIT_BLOCK_SIZE
 *
 * Maximum block size.  Both length and separation must be less than
 * or equal to this limit.
 */
#define SCHRO_LIMIT_BLOCK_SIZE 64

SCHRO_END_DECLS

#endif

