/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Basic constants and macros */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated field from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* Global variables */
static void *heap_listp;


/* Function headers*/
int mm_int(void);
void *mm_malloc(size_t);
void mm_free(void *);
void *mm_realloc(void *, size_t);
int mm_check(void);
static void *extend_heap(size_t);
static void *find_fit(size_t);
static void place(void *, size_t);
static void *coalesce(void *);

/*
 * mm_check - debugger
 */
int mm_check(void)
{
    return 0;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
        return -1;
    /* Initialized the alignment padding, prologue header, 
     * prologue footer, and epilogue header */
    PUT(heap_listp, 0);
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));
    heap_listp += (2 * WSIZE);

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return -1;

    return 0;
}

/*
 * extend_heap - extend the size of heap of words size 
 * when initialize and free block is not enough
 */
static void *extend_heap(size_t words)
{
    char *ptr;
    size_t size;

    /* Allocate an the number of size that align the requirement */
    size = ALIGN(words);
    if ((long)(ptr = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(ptr), PACK(size, 0));           /* Free block header */
    PUT(FTRP(ptr), PACK(size, 0));           /* Free block footer */
    PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 1));   /* New epilogue header */
    
    /* Coalesce if the previous block was free */

    return coalesce(ptr);
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 *     Need to add the size of header and footer and align.
 */
void *mm_malloc(size_t size)
{
    size_t asize; /* Added and adjusted block size*/
    size_t extendsize; /* Amount of extend heap if no fit*/
    char *ptr;

    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include header and footer and alignment reqs*/
    asize = ALIGN(size + 2 * SIZE_T_SIZE);

    /* Search the free list for a fit*/
    if ((ptr = find_fit(asize)) != NULL) {
        place(ptr, asize);
        return ptr;
    }

    /* No fit is found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((ptr = extend_heap(extendsize / WSIZE)) == NULL)
        return NULL;
    place(ptr, asize);
    return ptr;
    /*
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
    */
}

/*
 * find_fit - use first fit to search for the block to allocate
 */
static void *find_fit(size_t asize)
{
    /* Start from the prologue until the epilogue, 
     * if there is a block size >= asize and not allocated, find the fit */
    char *ptr = heap_listp;
    size_t block_size = GET_SIZE(HDRP(ptr));
    int alloc = GET_ALLOC(HDRP(ptr));
    while (!(!block_size && alloc)) {
        if ((block_size >= asize) && !alloc) {
            return ptr;              /* Find the first suitable free block then return the block location*/
        }
        ptr = NEXT_BLKP(ptr);
        block_size = GET_SIZE(HDRP(ptr));
        alloc = GET_ALLOC(HDRP(ptr));
    }
    return NULL;                    /* Return NULL if no suitable free block is found*/
}

/*
 * place - place the size and allocate bit in to the block,
 * turn the remain space into a new free block.
 */
static void place(void *ptr, size_t asize)
{
    size_t remained_size = GET_SIZE(HDRP(ptr)) - asize;
    char *free_ptr;
    /* Put the size and allocated bit into the header 
     * and footer of the block that placed into the new data */
    PUT(HDRP(ptr), PACK(asize, 1));
    PUT(FTRP(ptr), PACK(asize, 1));

    /* If there is a remaining space, mark as a new free block */
    if (remained_size) {
        free_ptr = NEXT_BLKP(ptr);
        PUT(HDRP(free_ptr), PACK(remained_size, 1));
        PUT(FTRP(free_ptr), PACK(remained_size, 1));
    }
    return;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));

    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
}

/* coalesce - coalesce adjacent free blocks when freeing the blocks */
static void *coalesce(void *ptr)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(ptr)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t size = GET_SIZE(HDRP(ptr));

    /* Case1: both prev and next block are allocated */
    if (prev_alloc && next_alloc) {
        return ptr;
    }
    else if (prev_alloc && !next_alloc) {
        /* Case2: prev block is allocated and next block is free */
        size += GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        PUT(HDRP(ptr), PACK(size, 0));
        PUT(FTRP(ptr), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc) {
        /* Case3: prev block is free and next block is allocated */
        size += GET_SIZE(HDRP(PREV_BLKP(ptr)));
        PUT(FTRP(ptr), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        ptr = PREV_BLKP(ptr);
    }
    else {
        /* Case4: both the prev and next blocks are free */
        size += GET_SIZE(HDRP(PREV_BLKP(ptr))) + GET_SIZE(FTRP(NEXT_BLKP(ptr)));
        PUT(HDRP(PREV_BLKP(ptr)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(size, 0));
        ptr = PREV_BLKP(ptr);
    }
    return ptr;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














