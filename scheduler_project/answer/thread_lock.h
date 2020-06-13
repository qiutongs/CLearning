#ifndef _THREAD_LOCK_H
#define _THREAD_LOCK_H

// You can use this header to share data and/or definitions between your
// lock.cpp and thread.cpp.

// For example, if you wanted to share a map between the two...

// extern const char *sharedMap;

// Then in ONE OF lock.cpp or thread.cpp you would write

// const char *sharedMap = CREATE_MAP(const char*);

// Now both lock.cpp and thread.cpp may refer to sharedMap.

#include "Map.h"
#include "List.h"

// Metadata of lock with thread
typedef struct LockThreadsInfo {
    Thread *lockOwner;     // the owner which holds the lock currently (might be NULL)
    const char *waitList;  // a list of threads which are waiting for this lock
} LockThreadsInfo;

// Global map of storing lock information
// This map is lockid -> list of LockThreadsInfo
extern const char *lockThreadsMap;

// Global ready list of threads which is to determine the next thread to run
extern const char *readyList;

// Compare threads by their priority
bool compareThreadsPri(void *item1, void *item2);

#endif