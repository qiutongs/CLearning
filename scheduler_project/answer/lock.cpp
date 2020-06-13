#include "Lock.h"
#include <Logger.h>
#include <stddef.h>
#include "thread_lock.h"

// See os_simulator/includes/Lock.student.h for explanations of these functions
// Not all of these functions must be implemented
// Remember to delete these three comments before grading for full marks

#pragma region Lock Callbacks

// The type of key is lockid: const char *
const char *lockThreadsMap = CREATE_MAP(const char *);

void lockCreated(const char *lockId) {
    // Initialize the list for the lock and put to map
    LockThreadsInfo *lockThreadInfo = (LockThreadsInfo *) malloc(sizeof(LockThreadsInfo));
    lockThreadInfo->lockOwner = NULL;
    lockThreadInfo->waitList = createNewList();
    PUT_IN_MAP(const char *, lockThreadsMap, lockId, lockThreadInfo);
}

void lockAttempted(const char *lockId, Thread *thread) {
    LockThreadsInfo *lockThreadInfo = (LockThreadsInfo *) GET_FROM_MAP(const char *, lockThreadsMap, lockId);

    // If the attempt is doomed failure...
    if (lockThreadInfo->lockOwner != NULL) {

        // Move from ready list to lock wait list
        removeFromList(readyList, (void *) thread);
        addToList(lockThreadInfo->waitList, (void *) thread);
        sortList(lockThreadInfo->waitList, compareThreadsPri);

        // Boost the priority of the current owner of lock: Priority Donation
        if (lockThreadInfo->lockOwner->priority < thread->priority) {
            lockThreadInfo->lockOwner->priority = thread->priority;
            sortList(readyList, compareThreadsPri);
        }
    }
}

void lockAcquired(const char *lockId, Thread *thread) {
    // Simply set the owner of the lock
    LockThreadsInfo *lockThreadInfo = (LockThreadsInfo *) GET_FROM_MAP(const char *, lockThreadsMap, lockId);
    lockThreadInfo->lockOwner = thread;
}

void lockFailed(const char *lockId, Thread *thread) {}

void lockReleased(const char *lockId, Thread *thread) {
    LockThreadsInfo *lockThreadInfo = (LockThreadsInfo *) GET_FROM_MAP(const char *, lockThreadsMap, lockId);
    lockThreadInfo->lockOwner = NULL;

    // Move a thread from lock wait list to ready list
    if (listSize(lockThreadInfo->waitList) > 0) {
        Thread *highestPriThread = (Thread *) listGet(lockThreadInfo->waitList, 0);
        removeFromList(lockThreadInfo->waitList, highestPriThread);
        addToList(readyList, (void *) highestPriThread);
        sortList(readyList, compareThreadsPri);
    }

    // Reset the priority and adjust the readylist
    if (thread->priority > thread->originalPriority) {
        thread->priority = thread->originalPriority;
        sortList(readyList, compareThreadsPri);
    }
}

#pragma endregion

#pragma region Lock Functions

Thread *getThreadHoldingLock(const char *lockId) {
    LockThreadsInfo *lockThreadInfo = (LockThreadsInfo *) GET_FROM_MAP(const char *, lockThreadsMap, lockId);
    return lockThreadInfo->lockOwner;
}

#pragma endregion