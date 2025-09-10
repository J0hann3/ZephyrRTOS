/*
 * NEXELEC
 * Alban MEFFRE (alban.meffre@nexelec.fr)
 * 2025
 */

/* INCLUDES */

#include "work_queue.h"
#include "SEGGER_SYSVIEW.h"

/* DEFINES */

#define __WQ_CRITICAL_IN() \
    {                      \
        __disable_irq();   \
    }
#define __WQ_CRITICAL_OUT() \
    {                       \
        __enable_irq();     \
    }

typedef enum
{
    WQ_NOERROR = 0,
    WQ_ERROR_QUEUE_OVERFLOW,
    WQ_ERROR_NULL_WORKER_PTR,
} wq_errno_t;

/* PROTOTYPES */

void wq_error_handler(wq_errno_t errno);

/* GLOBALS */

/* Arrays for function and p_args pointers storage */
/* Reset at system startup */
worker_t a_wq_fptr[WQ_MAX_WORKER_NUM]  = {0};
void    *a_wq_pargs[WQ_MAX_WORKER_NUM] = {0};

static volatile uint8_t wq_count       = 0;
static volatile uint8_t wq_write_index = 0;
static volatile uint8_t wq_read_index  = 0;

/* IMPLEMENTATION */

void wq_init()
{
    DEBUG_SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_WORK_QUEUE_INIT);
    for (int i = 0; i < WQ_MAX_WORKER_NUM; i++)
    {
        a_wq_fptr[i]  = NULL;
        a_wq_pargs[i] = NULL;
    }
    wq_count       = 0;
    wq_write_index = 0;
    wq_read_index  = 0;
}

void wq_enqueue(worker_t fptr, void *const p_args)
{
    DEBUG_SEGGER_SYSVIEW_RecordU32x2(ID_SYSVIEW_WORK_QUEUE_ENQUEUE, wq_count + 1, WQ_MAX_WORKER_NUM);
    DEBUG_SEGGER_SYSVIEW_OnTaskStartReady((U32)fptr);
    if (fptr == NULL)
    {
        /* catch error: NULL fptr */
        wq_error_handler(WQ_ERROR_NULL_WORKER_PTR);
    }
    /* critical section:
     * fetch-modify-store operations must be ATOMIC.
     * any nested call to any enqueue function from an IRQ handler
     * must be prevented in this section, so we disable IRQ globally.
     */
    __WQ_CRITICAL_IN();
    if (wq_count == WQ_MAX_WORKER_NUM)
    { /* ATOMIC */
        __WQ_CRITICAL_OUT();
        /* catch error: overflow */
        wq_error_handler(WQ_ERROR_QUEUE_OVERFLOW);
    }
    a_wq_pargs[wq_write_index]  = p_args;
    a_wq_fptr[wq_write_index++] = fptr;      /* ATOMIC */
    wq_write_index &= WQ_MAX_WORKER_NUM - 1; /* ATOMIC */
    wq_count++;                              /* ATOMIC */
    __WQ_CRITICAL_OUT();
}

void wq_process()
{
    DEBUG_SEGGER_SYSVIEW_RecordVoid(ID_SYSVIEW_WORK_QUEUE_PROCESS);
    worker_t fptr   = NULL;
    void    *p_args = NULL;
    if (wq_count != 0)
    {
        p_args = a_wq_pargs[wq_read_index];
        fptr   = a_wq_fptr[wq_read_index++];
        wq_read_index &= WQ_MAX_WORKER_NUM - 1;
        __WQ_CRITICAL_IN();
        wq_count--; /* ATOMIC */
        __WQ_CRITICAL_OUT();
        DEBUG_SEGGER_SYSVIEW_OnTaskStartExec((U32)fptr);
        (*fptr)(p_args);
        DEBUG_SEGGER_SYSVIEW_OnTaskStopExec();
    }
}

bool wq_not_empty()
{
    DEBUG_SEGGER_SYSVIEW_RecordU32(ID_SYSVIEW_WORK_QUEUE_IS_EMPTY, wq_count != 0);
    return wq_count != 0;
}

void wq_error_handler(wq_errno_t errno)
{
    DEBUG_SEGGER_SYSVIEW_RecordU32(ID_SYSVIEW_WORK_QUEUE_ERROR, errno);
    /* trace and catch */
    while (1)
    {
        (void) (errno); /* UNUSED PARAMETER */
    }
}
