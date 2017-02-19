#if !defined(COMMON_H)
#include <inttypes.h>
#include <assert.h>
typedef enum {false, true} bool;

#define ARRAY_SIZE(arr) sizeof(arr)/sizeof((arr)[0])

#define kilobyte(val) ((val)*1024LL)
#define megabyte(val) (kilobyte(val)*1024LL)
#define gigabyte(val) (megabyte(val)*1024LL)
#define terabyte(val) (gigabyte(val)*1024LL)

#define invalid_code_path assert(0)

typedef struct {
    uint8_t *data;
    uint32_t size;
    uint32_t used;
} memory_stack_t;

void memory_stack_init (memory_stack_t *stack, uint32_t size, uint8_t* data)
{
    stack->data = data;
    stack->size = size;
    stack->used = 0;
}

#define push_struct(memory_stack, type) push_size(memory_stack, sizeof(type))
#define push_array(memory_stack, n, type) push_size(memory_stack, n*sizeof(type))
void *push_size (memory_stack_t *stack, uint32_t size)
{
    assert (stack->used+size <= stack->size);
    void *res = &stack->data[stack->used];
    stack->used += size;
    return res;
}

typedef struct {
    memory_stack_t *stack;
    uint32_t used;
} temporary_marker_t;

temporary_marker_t begin_temporary_memory (memory_stack_t *stack)
{
    temporary_marker_t res;
    res.used = stack->used;
    res.stack = stack;
    return res;
}

void end_temporary_memory (temporary_marker_t mrkr)
{
    assert (mrkr.stack->used >= mrkr.used);
    mrkr.stack->used = mrkr.used;
}

bool in_array (int i, int* arr, int size)
{
    while (size) {
        size--;
        if (arr[size] == i) {
            return true;
        }
    }
    return false;
}

void array_print (int *arr, int n)
{
    int i;
    for (i=0; i<n-1; i++) {
        printf ("%d ", arr[i]);
    }
    printf ("%d\n", arr[i]);
}

void swap (int*a, int*b)
{
    *a = *a^*b;
    *b = *a^*b;
    *a = *a^*b;
}

// Merge sort implementation
void sort (int *arr, int n)
{
    if (n==1) {
        return;
    } else if (n == 2) {
        if (arr[1] < arr[0]) {
            swap (&arr[0], &arr[1]);
        }
    } else if (n==3) {
        if (arr[0] > arr[1]) swap(&arr[0],&arr[1]);
        if (arr[1] > arr[2]) swap(&arr[1],&arr[2]);
        if (arr[0] > arr[1]) swap(&arr[0],&arr[1]);
    } else {
        int res[n];
        sort (arr, n/2);
        sort (&arr[n/2], n-n/2);

        int i;
        int a=0;
        int b=n/2;
        for (i=0; i<n; i++) {
            if ((a<n/2 && arr[a] < arr[b]) || b==n) {
                res[i] = arr[a];
                a++;
            } else {
                res[i] = arr[b];
                b++;
            }
        }
        for (i=0; i<n; i++) {
            arr[i] = res[i];
        }
    }
}

#define COMMON_H
#endif
