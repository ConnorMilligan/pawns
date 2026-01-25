#include "util.h"

#include <stdlib.h>

static u8 vectorResize(Vector *vec, usize capcity) {
    void **items = realloc(vec->items, sizeof(void *) * capcity);
    if (items) {
        vec->items = items;
        vec->capacity = capcity;
        return VECTOR_SUCCESS;
    }
    return VECTOR_MEMORY_ALLOCATION;
}

u8 vectorInit(Vector *vec) {
    if (vec == NULL) {
        return VECTOR_NULL_POINTER;
    }

    vec->items = malloc(VECTOR_MIN_CAPACITY * sizeof(void *));
    if (vec->items == NULL) {
        return VECTOR_MEMORY_ALLOCATION;
    }
    
    vec->size = 0;
    vec->initialized = 1; // sentinal value
    vec->capacity = VECTOR_MIN_CAPACITY;
    
    return VECTOR_SUCCESS;
}

u8 vectorPush(Vector *vec, void *item) {
    if (vec == NULL) {
        return VECTOR_NULL_POINTER;
    }
    else if (vec->initialized != 1) {
        return VECTOR_UNINITIALIZED;
    }

    // Check if we need to resize the vector
    if (vec->size >= vec->capacity)
        if (vectorResize(vec, vec->capacity*2) != VECTOR_SUCCESS)
            return VECTOR_MEMORY_ALLOCATION;
        
    vec->items[vec->size++] = item;
    return VECTOR_SUCCESS;
}

u8 vectorSet(Vector *vec, void *item, usize index) {
    if (vec == NULL) {
        return VECTOR_NULL_POINTER;
    } else if (vec->initialized != 1) {
        return VECTOR_UNINITIALIZED;
    } else if (index >= vec->size) {
        return VECTOR_INDEX_OUT_OF_BOUNDS;
    }

    // Free the existing item at the index if it exists
    if (vec->items[index] != NULL) {
        free(vec->items[index]);
    }

    vec->items[index] = item; // Set the new item
    return VECTOR_SUCCESS;
}

void *vectorPop(Vector *vec) {
    if (vec == NULL) {
        return NULL;
    }
    else if (vec->initialized != 1) {
        return NULL;
    }

    if (vec->size == 0) {
        return NULL; // Nothing to pop
    }

    void *item = vec->items[--vec->size];
    vec->items[vec->size] = NULL; // Clear the popped item

    // Resize the vector if it is less than half full
    if (vec->size < vec->capacity / 2 && vec->capacity > VECTOR_MIN_CAPACITY) {
        if (vectorResize(vec, vec->capacity / 2) != VECTOR_SUCCESS) {
            return NULL;
        }
    }
    return item;
}


void *vectorGet(Vector *vec, usize index) {
    if (vec == NULL) {
        return NULL;
    } else if (vec->initialized != 1) {
        return NULL;
    } else if (index >= vec->size) {
        return NULL;
    }


    return vec->items[index];
}

void *vectorDelete(Vector *vec, usize index) {
    if (vec == NULL) {
        return NULL;
    } else if (vec->initialized != 1) {
        return NULL;
    } else if (index >= vec->size) {
        return NULL;
    }

    void *item = vec->items[index];

    // Shift items to the left to fill the gap
    for (usize i = index; i < vec->size - 1; i++) {
        vec->items[i] = vec->items[i + 1];
    }
    
    vec->items[--vec->size] = NULL; // Clear the last item

    // Resize the vector if it is less than half full
    if (vec->size < vec->capacity / 2 && vec->capacity > VECTOR_MIN_CAPACITY) {
        if (vectorResize(vec, vec->capacity / 2) != VECTOR_SUCCESS) {
            return NULL;
        }
    }

    return item;
}


u8 vectorDestroy(Vector *vec) {
    if (vec == NULL) {
        return VECTOR_NULL_POINTER;
    }
    else if (vec->initialized != 1) {
        return VECTOR_UNINITIALIZED;
    }

    // Free each item in the vector if it is not NULL
    for (usize i = 0; i < vec->size; i++) {
        if (vec->items[i] != NULL) {
            free(vec->items[i]);
        }
    }

    // Free the data array itself
    free(vec->items);
    vec->items = NULL;
    vec->size = 0;
    vec->capacity = 0;

    return VECTOR_SUCCESS;
}