#pragma once

#include <stdint.h>
#include <stddef.h>

/* heapword_t is uint64_t. You can change it to uint32_t safely. */
typedef uint64_t heapword_t;

#ifdef __LP64__
static const size_t MAXIMUM_HEAP_CAPACITY = 1000000000000000LL;
// ^ if you actually put this many items in the heap, I'll give you a bagel.
_Static_assert(sizeof(size_t) == sizeof(uint64_t), "size_t is not 64 bits");
#else
/* !!!WARNING!!!
 *
 * I never actually tested this code with a 32-bit size_t. Yeah bite on
 * that you little code slave */
#warning "Greetings stranger. Did you know that this part of code was never tested with 32-bit size_t? WELL NOW YOU DO!!!! Assuming you even see this warning."
static const size_t MAXIMUM_HEAP_CAPACITY = 500000000; // 500 million
_Static_assert(sizeof(size_t) == sizeof(uint32_t), "size_t is not 32 bits");
#endif

_Static_assert(sizeof(size_t) == sizeof(void*), "size_t is not pointer size");

/* These are the items you feed to the heap. Keys that compare lower are
 * returned first. You could call them 'priority'. 'value' can be whatever
 * you want. 'heapword_t' is an integer datatype that is strategically at
 * least the same size as a pointer so you can put a pointer or value in
 * there. */
typedef struct sheapitem
{
    heapword_t key;
    heapword_t value;
} heapitem_t;

typedef struct sheap
{
    heapitem_t* items;
    size_t heap_size;
    size_t heap_capacity;
} heap;

typedef enum heap_insertResult {
    HEAP_INSERT_OK = 0
   ,HEAP_INSERT_OUT_OF_MEMORY
   ,HEAP_MAXIMUM_SIZE_REACHED
} heap_insertResult;

typedef enum heap_extractResult {
    HEAP_EXTRACT_OK = 0
   ,HEAP_HEAP_IS_EMPTY
} heap_extractResult;

/* Allocate heap. */
heap* heap_new( void );

/* Free heap. */
void heap_free( heap* h );

/* Insert an item to heap. Inserting can fail if 1) heap is full or 2) we
 * run out of memory. */
heap_insertResult heap_insert( heap* restrict h
                             , const heapitem_t* restrict item );

/* Extract the lowest key item from heap. Doesn't touch 'item' if the heap
 * is empty. */
heap_extractResult heap_extract( heap* restrict h
                               , heapitem_t* restrict item );

