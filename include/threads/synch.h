#ifndef THREADS_SYNCH_H
#define THREADS_SYNCH_H

#include <list.h>
#include <stdbool.h>

/* A counting semaphore. */
struct semaphore {
	unsigned value;             /* Current value. */
	struct list waiters;        /* List of waiting threads. */
};

void sema_init (struct semaphore *, unsigned value);  // 세마포어 value값 초기화
void sema_down (struct semaphore *);  // 세마포어의 value4ㅏ 0이면 현재 스레드를 block 상태로 바꾸고 schedule
bool sema_try_down (struct semaphore *);
void sema_up (struct semaphore *);  // 대기 리스트에 스레드가 있으면 리스트의 첫 번째 스레드를 ready 상태로 바꾸고 schedule
void sema_self_test (void);

/* Lock. */
struct lock {
	struct thread *holder;      /* Thread holding lock (for debugging). */
	struct semaphore semaphore; /* Binary semaphore controlling access. */
};

void lock_init (struct lock *);  // lock 초기화
void lock_acquire (struct lock *);  // 다른 프로세스가 접근하지 못하도록 lock 잠금; lock 요청
bool lock_try_acquire (struct lock *);
void lock_release (struct lock *);  // 다른 프로세스가 접근할 수 있도록 lock 해제; lock 반환
bool lock_held_by_current_thread (const struct lock *);

/* Condition variable. */
struct condition {
	struct list waiters;        /* List of waiting threads. */
};

void cond_init (struct condition *);  // condition variable 자료구조를 초기화
void cond_wait (struct condition *, struct lock *);  // condition variable을 통해 signal이 오는지 기림
void cond_signal (struct condition *, struct lock *);  // condition variable에서 기다리는 가장높은 우선순위의 스레드에 signal을 보냄
void cond_broadcast (struct condition *, struct lock *);  // condition variable에서 기다리는 모든 스레드에 signal을 보냄

// newly added
void remove_with_lock(struct lock *lock);
void donate_priority(void);
void refresh_priority(void);

/* Optimization barrier.
 *
 * The compiler will not reorder operations across an
 * optimization barrier.  See "Optimization Barriers" in the
 * reference guide for more information.
 */
#define barrier() asm volatile ("" : : : "memory")

#endif /* threads/synch.h */
