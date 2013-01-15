/*
 * The Distributed Lock Manager project
 *
 * Client library functions
 *
 * Authors: Piotr Bieńkowski, Tomasz Krawczyk, Mikołaj Piechocki
 */

#include "dlm.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

/* A special lock type for freeing the resource */
#define FREERESOURCE -1

int ask_question(DLM_question* question)
{
    char path[64];
    int server_fifo;
    int client_fifo;
    DLM_answer answer;

    sprintf(path, "%s%d", DLM_PATH, question->PID);
    if ((server_fifo = open(DLM_FIFO_PATH, O_WRONLY)) < 0)
    {
        return EOPENserver_fifo;
    }
    if (mkfifo(path, 00666) < 0)
    {
        close(server_fifo);
        return ECREATEFIFO;
    }
    if ((client_fifo = open(path, O_RDWR)) < 0)
    {
        close(server_fifo);
        unlink(path);
        return EOPENclient_fifo;
    }
    if (write(server_fifo, question, sizeof(*question)) != sizeof(*question))
    {
        close(server_fifo);
        close(client_fifo);
        unlink(path);
        return EWRITE;
    }
    if (read(client_fifo, &answer, sizeof(answer)) != sizeof(answer))
    {
        close(server_fifo);
        close(client_fifo);
        unlink(path);
        return EREAD;
    }
    close(server_fifo);
    close(client_fifo);
    unlink(path);
    return answer.answer;
}

int DLM_lock(int resource_id, int lock_type, long timeout)
{
    if (lock_type < 0 || lock_type > 4)
    {
        return EBADLOCKTYPE;
    }
    if (timeout < -1)
    {
        return EBADTIMEOUT;
    }
    DLM_question question =
    {
        LOCK,
        getpid(),
        resource_id,
        lock_type,
        timeout
    };
    return send_question(&question);
}

int DLM_unlock(int resource_id)
{
    DLM_question question =
    {
        UNLOCK,
        getpid(),
        resource_id,
        FREERESOURCE,
        NOLIMIT
    };
    return send_question(&question);
}

int DLM_trylock(int resource_id, int lock_type)
{
    if(lock_type < 0 || lock_type > 4)
    {
        return EBADLOCKTYPE;
    }
    DLM_question question =
    {
        TRYLOCK,
        getpid(),
        resource_id,
        lock_type,
        NONBLOCK
    };
    return send_question(&question);
}
