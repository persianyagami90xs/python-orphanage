#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

// It seems that the CFFI disallows to include local header files (for now).
typedef struct orphanage_poll_t {
    pthread_t pt;
    pid_t pid, ppid;
} orphanage_poll_t;
int orphanage_poll_routine_callback(orphanage_poll_t *t);

static void *orphanage_poll_routine(void *userdata) {
    pid_t ppid;
    orphanage_poll_t *t = userdata;
    while (1) {
        ppid = getppid();
        if (ppid != t->ppid) {
            orphanage_poll_routine_callback(t);
            break;
        }
        sleep(1);
    }
    return NULL;
}

orphanage_poll_t *orphanage_poll_create() {
    return (orphanage_poll_t *) malloc(sizeof(orphanage_poll_t));
}

void orphanage_poll_close(orphanage_poll_t *t) {
    free(t);
}

int orphanage_poll_start(orphanage_poll_t *t) {
    int retval;

    t->pid = getpid();
    t->ppid = getppid();

    retval = pthread_create(&t->pt, NULL, &orphanage_poll_routine, (void *) t);
    if (retval) {
        perror("orphanage_poll_start/pthread_create");
        return retval;
    }

    return 0;
}

int orphanage_poll_stop(orphanage_poll_t *t) {
    int retval, last_errno;

    last_errno = errno;
    retval = pthread_cancel(t->pt);
    if (retval && errno == ESRCH) {
        retval = pthread_detach(t->pt);
        if (retval && errno != ESRCH) {
            perror("orphanage_poll_stop/pthread_detach");
            errno = last_errno;
            return retval;
        }
    }
    errno = last_errno;

    return 0;
}
