#include <windows.h>
#include <stdio.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

HANDLE mutex;
HANDLE S[N];

void test(int phnum)
{
    if (state[phnum] == HUNGRY
        && state[LEFT] != EATING
        && state[RIGHT] != EATING) {
        
        state[phnum] = EATING;

        Sleep(2000);

        printf("Philosopher %d takes fork %d and %d\n",
            phnum + 1, LEFT + 1, phnum + 1);

        printf("Philosopher %d is Eating\n", phnum + 1);

        ReleaseSemaphore(S[phnum], 1, NULL);
    }
}

void take_fork(int phnum)
{
    WaitForSingleObject(mutex, INFINITE);

    state[phnum] = HUNGRY;

    printf("Philosopher %d is Hungry\n", phnum + 1);

    test(phnum);

    ReleaseMutex(mutex);

    WaitForSingleObject(S[phnum], INFINITE);

    Sleep(1000);
}

void put_fork(int phnum)
{
    WaitForSingleObject(mutex, INFINITE);

    state[phnum] = THINKING;

    printf("Philosopher %d putting fork %d and %d down\n",
        phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);

    test(LEFT);
    test(RIGHT);

    ReleaseMutex(mutex);
}

DWORD WINAPI philosopher(LPVOID num)
{
    int* i = (int*)num;

    while (1) {
        Sleep(1000);
        take_fork(*i);
        Sleep(0);
        put_fork(*i);
    }

    return 0;
}

int main()
{
    HANDLE thread_id[N];

    mutex = CreateMutex(NULL, FALSE, NULL);

    for (int i = 0; i < N; i++)
        S[i] = CreateSemaphore(NULL, 0, 1, NULL);

    for (int i = 0; i < N; i++) {
        thread_id[i] = CreateThread(NULL, 0, philosopher, &phil[i], 0, NULL);
        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (int i = 0; i < N; i++)
        WaitForSingleObject(thread_id[i], INFINITE);

    return 0;
}
