// TODO: add error handling

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <string.h>

#define TOKENPASTE(x, y) x##y
#define QUEUE_INIT(T) TOKENPASTE(T, _queue_init)
#define QUEUE_PUSH(T) TOKENPASTE(T, _queue_push)
#define QUEUE_POP(T) TOKENPASTE(T, _queue_pop)
#define QUEUE_FREE(T) TOKENPASTE(T, _queue_free)

#define QUEUE(T) TOKENPASTE(T, _queue)

#endif

typedef struct QUEUE(TYPE) {
  TYPE** arr;
  size_t len;
  size_t back;
  size_t front;
  size_t size;
} QUEUE(TYPE);


QUEUE(TYPE)* QUEUE_INIT(TYPE) (void) {
  TYPE** arr = (TYPE**) calloc(sizeof(TYPE*), 100);
  QUEUE(TYPE)* q = malloc(sizeof(QUEUE(TYPE)));

  q->arr = arr;
  q->len = 10;
  q->front = 0;
  q->back = 0;
  q->size = 0;

  return q;
}

void QUEUE_PUSH(TYPE) (QUEUE(TYPE)* q, TYPE* t) {
  if (q->size == q->len-1) {
    q->arr = realloc(q->arr, sizeof(TYPE*) * 2 * q->len);
    memmove(&q->arr[q->len], q->arr, q->back);
    q->back += (q->len - 1);
    q->len *= 2;
  }
  q->arr[q->back] = t;
  q->back = (q->back + 1) % q->len;
  q->size++;
}


TYPE* QUEUE_POP(TYPE) (QUEUE(TYPE)* q) {
  if (q->size == 0) return NULL;
  
  TYPE* ret_val = q->arr[q->front];
  q->arr[q->front] = NULL;
  q->front = (q->front + 1) % q->len;
  q->size--;
  return ret_val;
}


void QUEUE_FREE(TYPE) (QUEUE(TYPE)* q) {
  free(q);
}
