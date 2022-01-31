// TODO: add error handling, condition variables

// impl. of a very basic threadpool, not recommended for heavy usage - yet

#ifndef THREADING_H
#define THREADING_H
#include <pthread.h>
#include "utils.h"

#define TYPE pthread_t
#include "queue.h"
#undef TYPE

typedef void (*func_ptr)(void* args);
typedef struct threadwork {
  func_ptr func;
  void* args;
} threadwork;

#define TYPE threadwork
#include "queue.h"
#undef TYPE

typedef struct threadpool {
  pthread_t_queue* thread_q;
  threadwork_queue* work_q;
  pthread_mutex_t work_lock;
} threadpool;


void* worker_task(void* arg) {
  threadpool* pool = arg;
  for (;;) {
    pthread_mutex_lock(&pool->work_lock);
    threadwork* work = threadwork_queue_pop(pool->work_q);
    pthread_mutex_unlock(&pool->work_lock);
    if (work == NULL) continue;
    work->func(work->args);
    free(work);
  }
  return NULL;
}

threadpool* threadpool_init(size_t size) {
  threadpool* pool = (threadpool*) malloc(sizeof(threadpool));
  pool->thread_q = pthread_t_queue_init();
  pool->work_q = threadwork_queue_init();
  if (pthread_mutex_init(&pool->work_lock, NULL) != 0) {
    threadwork_queue_free(pool->work_q);
    pthread_t_queue_free(pool->thread_q);
    return NULL;
  }
  
  // init threads
  pthread_t* thread;
  for (u32 i = 0; i < size; i++) {
    thread = (pthread_t*) malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, worker_task, pool);
    pthread_t_queue_push(pool->thread_q, thread);
    pthread_detach(*thread);
  }
  
  return pool;
}

void init_and_add_work(threadpool* pool, void* func, void* args) {
  threadwork* work = (threadwork*) malloc(sizeof(threadwork));
  work->func = func;
  work->args = args;
  
  pthread_mutex_lock(&pool->work_lock);
  threadwork_queue_push(pool->work_q, work);
  pthread_mutex_unlock(&pool->work_lock);
}


#endif
