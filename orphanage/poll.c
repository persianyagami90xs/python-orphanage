#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

// Copy this to the Python modules since any modification
const int ORPHANAGE_POLL_OK = 0x00000000;
const int ORPHANAGE_POLL_PT_CREATE_ERROR = 0x00000001;
const int ORPHANAGE_POLL_PT_DETACH_ERROR = 0x00000002;
const int ORPHANAGE_POLL_PT_CANCEL_ERROR = 0x00000003;

// It seems that the CFFI disallows to include local header files (for now).
typedef struct orphanage_poll_t {
    pthread_t pt;
    pid_t pid, ppid;
    unsigned started;
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
        pthread_testcancel();
        sleep(1);
    }
    return NULL;
}

orphanage_poll_t *orphanage_poll_create() {
    orphanage_poll_t *t = malloc(sizeof(orphanage_poll_t));
    if (t != NULL) {
        t->pt = 0;
        t->pid = getpid();
        t->ppid = getppid();
        t->started = 0;
    }
    return t;
}

void orphanage_poll_close(orphanage_poll_t *t) {
    free(t);
}

int orphanage_poll_start(orphanage_poll_t *t) {
    int retval;

    retval = pthread_create(&t->pt, NULL, &orphanage_poll_routine, (void *) t);
    if (retval) {
        return ORPHANAGE_POLL_PT_CREATE_ERROR;
    }

    retval = pthread_detach(t->pt);
    if (retval) {
        return ORPHANAGE_POLL_PT_DETACH_ERROR;
    }

    t->started = 1;

    return ORPHANAGE_POLL_OK;
}

int orphanage_poll_stop(orphanage_poll_t *t) {
    int retval;

    if (!t->started) {
        return ORPHANAGE_POLL_OK;
    }

    retval = pthread_cancel(t->pt);
    if (retval && errno != ESRCH) {
        return ORPHANAGE_POLL_PT_CANCEL_ERROR;
    }

    return ORPHANAGE_POLL_OK;
}
