#include <Logger.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "List.h"
#include "Thread.h"
#include "thread_lock.h"

// Metadata of sleeping thread
typedef struct SleepingThread {
    Thread *thread;
    int wakeupTick;
} SleepingThread;

const char *readyList = NULL;

// Global sleep list of threads
static const char *sleepList = NULL;

bool compareThreadsPri(void *item1, void *item2) {
    Thread *thread1 = (Thread *) item1;
    Thread *thread2 = (Thread *) item2;

    // Higher priority at front
    return thread1->priority >= thread2->priority;
}

static bool compareWakeup(void *item1, void *item2) {
    SleepingThread *thread1 = (SleepingThread *) item1;
    SleepingThread *thread2 = (SleepingThread *) item2;

    // Earlier wakeup time at front
    return thread1->wakeupTick <= thread2->wakeupTick;
}

Thread *createAndSetThreadToRun(const char *name,
                                void *(*func)(void *),
                                void *arg,
                                int pri) {
    Thread *ret = (Thread *) malloc(sizeof(Thread));
    ret->name = (char *) malloc(strlen(name) + 1);
    strcpy(ret->name, name);
    ret->func = func;
    ret->arg = arg;
    ret->priority = pri;
    ret->originalPriority = pri;

    createThread(ret);
    addToList(readyList, (void *) ret);
    return ret;
}

void destroyThread(Thread *thread) {
    free(thread->name);
    free(thread);
}

// Try to wake up thread at current tick. This may or may not wake up thread.
static void tryWakeupThread(int currentTick) {
    // If no threads are sleeping, return
    if (listSize(sleepList) == 0)
        return;

    // Check the first thread in list, which has the earliest wakeup time
    SleepingThread *firstSleepingThread = ((SleepingThread *) listGet(sleepList, 0));

    // Still not the time...
    if (firstSleepingThread->wakeupTick > currentTick) {
        return;
    } else {
        // Move from sleep list to ready list
        removeFromList(sleepList, (void *) firstSleepingThread);
        addToList(readyList, (void *) firstSleepingThread->thread);
        free(firstSleepingThread);
    }
}

Thread *nextThreadToRun(int currentTick) {

    // First, try to wake up sleeping threads before determining which thread to schedule
    tryWakeupThread(currentTick);

    char line[1024];
    if (listSize(readyList) == 0)
        return NULL;
    Thread *ret = NULL;
    do {
        int threadIndex = 0;
        sprintf(line, "[nextThreadToRun] trying thread index %d\n",
                threadIndex);
        verboseLog(line);

        // Sort the ready list
        sortList(readyList, compareThreadsPri);

        // Return the thread with highest priority
        ret = ((Thread *) listGet(readyList, threadIndex));
        if (ret->state == TERMINATED) {
            sprintf(line, "[nextThreadToRun] thread %d was terminated\n",
                    threadIndex);
            verboseLog(line);
            removeFromList(readyList, ret);
            ret = NULL;
        }

    } while (listSize(readyList) > 0 && ret == NULL);
    return ret;
}

void initializeCallback() {
    readyList = createNewList();
    sleepList = createNewList();
}

void shutdownCallback() {
    destroyList(readyList);
    destroyList(sleepList);
}

int tickSleep(int numTicks) {
    Thread *curThread = getCurrentThread();
    int curTick = getCurrentTick();

    // Move from ready list to sleep list
    // Sort the sleep list
    removeFromList(readyList, (void *) curThread);

    SleepingThread *sleepingThread = (SleepingThread *) malloc(sizeof(SleepingThread));
    sleepingThread->thread = curThread;
    sleepingThread->wakeupTick = curTick + numTicks;

    addToList(sleepList, (void *) sleepingThread);
    sortList(sleepList, compareWakeup);

    // Stop what the thread is doing currently
    stopExecutingThreadForCycle();

    return curTick;
}

void setMyPriority(int priority) {
    getCurrentThread()->priority = priority;
}
