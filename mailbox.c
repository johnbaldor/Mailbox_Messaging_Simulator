#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define MAXTHREAD 10

struct msg {
    int iFrom;
    int value;
    int cnt;
    int tot;
};

struct msg mailboxes[MAXTHREAD + 1];
sem_t semaphores[MAXTHREAD + 1];
pthread_mutex_t print_mutex;

void InitMailBox(int numThreads) {
    for (int i = 0; i <= numThreads; i++) {
        sem_init(&semaphores[i], 0, 0);
    }
}

void SendMsg(int iTo, struct msg *pMsg) {
    mailboxes[iTo] = *pMsg;
    sem_post(&semaphores[iTo]);
}

void RecvMsg(int iRecv, struct msg *pMsg) {
    sem_wait(&semaphores[iRecv]);
    *pMsg = mailboxes[iRecv];
}

void *adder(void *arg) {
    int index = *(int *)arg;
    struct msg message;
    time_t startTime, endTime;

    startTime = time(NULL);
    int sum = 0, count = 0;

    while (1) {
        RecvMsg(index, &message);
        if (message.value < 0) {
            break;
        }
        sum += message.value;
        count++;
        sleep(1);
    }

    endTime = time(NULL);

    struct msg result;
    result.iFrom = index;
    result.value = sum;
    result.cnt = count;
    result.tot = (int)(endTime - startTime);
    SendMsg(0, &result);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        exit(1);
    }

    int numThreads = atoi(argv[1]);
    if (numThreads < 1 || numThreads > MAXTHREAD) {
        fprintf(stderr, "Number of threads must be between 1 and %d\n", MAXTHREAD);
        exit(1);
    }

    pthread_mutex_init(&print_mutex, NULL);
    InitMailBox(numThreads);

    pthread_t threads[MAXTHREAD];
    int threadIndices[MAXTHREAD];

    for (int i = 1; i <= numThreads; i++) {
        threadIndices[i] = i;
        pthread_create(&threads[i], NULL, adder, &threadIndices[i]);
    }

    int value, index;
    struct msg message;
    char input[100];

    printf("Enter value and thread index pairs (e.g., '3 1'), and press Enter to finish:\n");
    while (fgets(input, sizeof(input), stdin) != NULL && input[0] != '\n') {
        if (sscanf(input, "%d %d", &value, &index) != 2) {
            fprintf(stderr, "Invalid input format.\n");
            continue;
        }
        if (index < 1 || index > numThreads) {
            fprintf(stderr, "Invalid thread index: %d\n", index);
            continue;
        }
        message.iFrom = 0;
        message.value = value;
        SendMsg(index, &message);
    }

    for (int i = 1; i <= numThreads; i++) {
        message.iFrom = 0;
        message.value = -1;
        SendMsg(i, &message);
    }

    for (int i = 1; i <= numThreads; i++) {
        struct msg result;
        RecvMsg(0, &result);
        pthread_mutex_lock(&print_mutex);
        printf("The result from thread %d is %d from %d operations during %d secs.\n", result.iFrom, result.value, result.cnt, result.tot);
        pthread_mutex_unlock(&print_mutex);
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i <= numThreads; i++) {
        sem_destroy(&semaphores[i]);
    }
    pthread_mutex_destroy(&print_mutex);

    return 0;
}
