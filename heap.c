/* Binary heap implementation, specifically min-heap */

#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>

#define NOT_OK(a) ((a) != 0)

static bool is_at_capacity( const heap* h )
{
    return h->heap_size >= h->heap_capacity;
}

static heap_insertResult grow_if_at_capacity( heap* h )
{
    if ( !is_at_capacity( h ) ) {
        return HEAP_INSERT_OK;
    }

    size_t new_capacity = h->heap_capacity * 2;
    if ( new_capacity == 0 ) {
        new_capacity = 1;
    }

    /* Overflow? */
    if ( new_capacity > MAXIMUM_HEAP_CAPACITY ) {
        new_capacity = MAXIMUM_HEAP_CAPACITY;
    }

    if ( new_capacity == h->heap_capacity ) {
        return HEAP_MAXIMUM_SIZE_REACHED;
    }

    heapitem_t* new_items = realloc( h->items, new_capacity*sizeof(heapitem_t) );
    if ( !new_items ) {
        return HEAP_INSERT_OUT_OF_MEMORY;
    }

    h->items = new_items;
    h->heap_capacity = new_capacity;

    return HEAP_INSERT_OK;
}

static void shrink_if_small_enough( heap* h )
{
    if ( h->heap_capacity < 1 || h->heap_size < 1) {
        return;
    }

    if ( h->heap_capacity >= h->heap_size * 4 ) {
        size_t new_capacity = h->heap_capacity / 2;
        heapitem_t* new_items = realloc( h->items, new_capacity*sizeof(heapitem_t) );
        if ( !new_items ) {
            return;
        }
        h->items = new_items;
        h->heap_capacity = new_capacity;
    }
}

heap* heap_new( void )
{
    return calloc( sizeof(heap), 1 );
}

void heap_free( heap* h )
{
    if ( h ) {
        free( h->items );
        free( h );
    }
}

static void reheap( heap* h, size_t index )
{
again:
    if ( index == 0 ) {
        return;
    }

    size_t parent_index = index/2;
    if ( h->items[parent_index].key > h->items[index].key ) {
        heapitem_t tmp = h->items[parent_index];
        h->items[parent_index] = h->items[index];
        h->items[index] = tmp;

        index = parent_index;
        goto again;
    }
}

static void reheap_down( heap* h, size_t index )
{
    heapword_t key = h->items[index].key;
again:
    if ( index*2 >= h->heap_size ) {
        return;
    }

    heapword_t left_key = h->items[index*2].key;
    if ( index*2+1 >= h->heap_size ) { /* no right child? */
        if ( left_key < key ) {
            heapitem_t tmp = h->items[index*2];
            h->items[index*2] = h->items[index];
            h->items[index] = tmp;
            return;
        }
        return;
    }

    heapword_t right_key = h->items[index*2+1].key;
    if ( left_key < key || right_key < key ) {
        if ( left_key < right_key ) {
            heapitem_t tmp = h->items[index*2];
            h->items[index*2] = h->items[index];
            h->items[index] = tmp;
            index = index*2;
            key = h->items[index].key;
            goto again;
        } else {
            heapitem_t tmp = h->items[index*2+1];
            h->items[index*2+1] = h->items[index];
            h->items[index] = tmp;
            index = index*2+1;
            key = h->items[index].key;
            goto again;
        }
    }
}

heap_extractResult heap_extract( heap* restrict h, heapitem_t* restrict item )
{
    if ( h->heap_size == 0 ) {
        return HEAP_HEAP_IS_EMPTY;
    }

    (*item) = h->items[0];
    if ( h->heap_size == 1 ) {
        h->heap_size = 0;
        h->heap_capacity = 0;
        free( h->items );
        h->items = NULL;
        return HEAP_EXTRACT_OK;
    }

    h->items[0] = h->items[h->heap_size-1];
    h->heap_size--;

    reheap_down( h, 0 );
    shrink_if_small_enough( h );
    return HEAP_EXTRACT_OK;
}

heap_insertResult heap_insert( heap* restrict h, const heapitem_t* restrict item )
{
    heap_insertResult result = grow_if_at_capacity( h );
    if ( NOT_OK(result) ) {
        return result;
    }

    h->heap_size++;

    h->items[h->heap_size-1] = (*item);
    if ( h->heap_size == 1 ) {
        return HEAP_INSERT_OK;
    }

    reheap( h, h->heap_size-1);
    return HEAP_INSERT_OK;
}



