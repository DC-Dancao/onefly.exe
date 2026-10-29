 
 

#ifndef BASE_THREAD_H
#define BASE_THREAD_H

 \
\
\
\
\
\
\


 \
\
\
\
\
\
\
\
\
\

void *thread_init(void (*threadfunc)(void *), void *user, const char *name);

 \
\
\
\
\
\

void thread_wait(void *thread);

 \
\
\
\

void thread_yield();

 \
\
\
\
\
\
\
\

void thread_request_precise_wakeups();

 \
\
\
\
\
\
\
\

void thread_detach(void *thread);

 \
\
\
\
\
\
\
\

void thread_init_and_detach(void (*threadfunc)(void *), void *user, const char *name);

#endif
