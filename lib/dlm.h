/*
 * The Distributed Lock Manager project
 *
 * Client library headers
 *
 * Authors: Piotr Bieńkowski, Tomasz Krawczyk, Mikołaj Piechocki
 */

#ifndef DLMLIB_H
#define DLMLIB_H

#include <sys/types.h>
#include <unistd.h>

/* Lock type symbol definitons */
#define CR 1
#define CW 2
#define PR 3
#define PW 3
#define EX 4

/* Server response type symbol definitions */
#define GRANTED 0
#define TIMEDOUT 1
#define LOCKED 2
#define UNLOCKED 3
#define FREE 4
#define EOPENDLMFIFO -1
#define ECREATEFIFO -2
#define EOPENCLIENTFIFO -3
#define EWRITE -4
#define EREAD -5
#define EBADLOCKTYPE -6
#define EBADTIMEOUT -7
#define ENOTLOCKED -8
#define EAGAIN -9

/* Question header types */
#define LOCK 1
#define UNLOCK 2
#define TRYLOCK 3

/* Timeout symbol definitions */
#define NOLIMIT 0
#define NONBLOCK -1

/* Queue paths */
#define DLM_PATH "/tmp/DLM/"
#define DLM_FIFO_PATH "/tmp/DLM/server_fifo"

typedef struct DLM_question
{
    int HEADER;
    pid_t PID;
    int resource_id;
    int lock_type;
    long timeout;
}
DLM_request;

typedef struct DLM_answer
{
    int answer;
}
DLM_response;

int DLM_lock(int resource_id, int lock_type, long timeout);
int DLM_unlock(int resource_id);
int DLM_trylock(int resource_id, int lock_type);

#endif
