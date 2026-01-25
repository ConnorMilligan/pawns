#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

// Unsigned int types.
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Regular int types.
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Floating point types
typedef float f32;
typedef double f64;

// Size
typedef size_t usize;

// Vector
#define VECTOR_MIN_CAPACITY 4

enum ErrorVector {
    VECTOR_SUCCESS,
    VECTOR_NULL_POINTER,
    VECTOR_MEMORY_ALLOCATION,
    VECTOR_INDEX_OUT_OF_BOUNDS,
    VECTOR_UNINITIALIZED,
    VECTOR_DESTROY_FAILED,
};

typedef struct {
    void **items;
    usize size;
    usize capacity;
    u8 initialized;
} Vector;

u8 vectorInit(Vector *vec);
u8 vectorPush(Vector *vec, void *item);
u8 vectorSet(Vector *vec, void *item, usize index);
void *vectorPop(Vector *vec);
void *vectorGet(Vector *vec, usize index);
void *vectorDelete(Vector *vec, usize index);
u8 vectorDestroy(Vector *vec);

#endif // UTIL_H
