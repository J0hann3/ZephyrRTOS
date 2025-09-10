/*
 * NEXELEC
 * Alban MEFFRE (alban.meffre@nexelec.fr)
 * 2025
 */

#ifndef __WORK_QUEUE_H
#define __WORK_QUEUE_H

#include <stdbool.h> /* bool */
#include <stddef.h>  /* NULL */
#include <stdint.h>  /* uints */

#include "cmsis_gcc.h" /* __disable_irq() and __enable_irq() */

typedef void (*worker_t)(void *const p_args);

#ifndef WQ_MAX_WORKER_NUM
    #define WQ_MAX_WORKER_NUM 16 /* power of 2 */
#endif                           /* WQ_MAX_WORKER_NUM */
#define IS_POWER_OF_TWO(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))
#if !IS_POWER_OF_TWO(WQ_MAX_WORKER_NUM)
    #error "work_queue.h: WQ_MAX_WORKER_NUM must be a power of 2 !"
#endif

void wq_init(); /* private variables are reset at startup. call this to force reset the workqueue*/
void wq_enqueue(worker_t, void *const);
void wq_process();
bool wq_not_empty();

#define WQ_PROCESS   wq_process()
#define WQ_NOT_EMPTY wq_not_empty()

/* helper macro to prevent wrong pointer type warnings
 * if no p_args are needed just use WQ_ENQUEUE(pfun, NULL);
 * */
#define WQ_ENQUEUE(fptr, p_args) wq_enqueue((worker_t) (fptr), (void *const) (p_args))

#endif /* __WORK_QUEUE_H */
