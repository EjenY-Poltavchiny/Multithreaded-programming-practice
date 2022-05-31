#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

sem_t sem;

typedef struct for_threads{
    pthread_t thread_id;
    int N;
    double x1, x2; 
    double y1, y2; 
    double* I;
    int queue; 
} Thr, *Thr_pntr; 

int Is_normal(double x, double y)
{
    return (x >= 0) && (x <= M_PI) && (y >= 0) && (y <= sin(x));
}

void *start_func(void *param)
{
    Thr_pntr thread = (Thr_pntr) param;
    unsigned int rseed = thread->queue;
    int N = thread->N;
    double x1 = thread->x1;
    double x2 = thread->x2;
    double y1 = thread->y1;
    double y2 = thread->y2;
    double x, y;  
    double points = 0.0;
   
    for (int i = 0; i < N; i++)
    {   
        x = (double) rand_r(&rseed)/RAND_MAX*(x2 - x1) + x1; 
        y = (double) rand_r(&rseed)/RAND_MAX*(y2 - y1) + y1; 
        if (Is_normal(x, y)) 
           points += x*y;
    }   
    // crit memory
    sem_wait(&sem);
    *(thread->I) += points;
    sem_post(&sem);
    // using join/exit
    double * answer = (double *)malloc(sizeof(double));
    *(answer) = points;
    pthread_exit(answer);
}

int main(int argc, char *argv[])
{
    struct timespec start, end;
    double T_cons;
    double T_par;
    int N = 1e8;
    double x1 = 0;
    double x2 = M_PI;
    double y1 = 0;
    double y2  = 1;
    int p = atoi(argv[1]);
    // one process
    /*clock_gettime(CLOCK_REALTIME, &start); 
    double x, y;
    double sum = 0.0;
    double answ;
    unsigned int xrand, yrand;
    for (int i = 0; i < N; i++)
    {
        x = (double) rand_r(&xrand)/RAND_MAX*(x2 - x1) + x1;
        y = (double) rand_r(&yrand)/RAND_MAX*(y2 - y1) + y1;
        if (Is_normal(x, y) == 1)
            sum += x*y;
    }
    answ = sum*(x2-x1)*(y2-y1)/N;
    clock_gettime(CLOCK_REALTIME, &end);
    T_cons = end.tv_sec - start.tv_sec;
    T_cons += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("One process: I = %f; time = %f \n", answ, T_cons);*/
    // par process
    clock_gettime(CLOCK_REALTIME, &start);
    sem_init(&sem, 0, 1);
    Thr_pntr *New_thr = (Thr_pntr *) malloc(sizeof(Thr_pntr)*p);
    double I = 0.0;
    for (int i = 0; i < p; i++)
    {
        Thr_pntr thread = (Thr_pntr) malloc(sizeof(Thr));
        New_thr[i] = thread;
    }
    for (int i = 0; i < p; i++)
    {
        Thr_pntr thread = New_thr[i];
        thread->x1 = x1;
        thread->x2 = x2;
        thread->y1 = y1;
        thread->y2 = y2;
        thread->N = N / p;
        thread->I = &I;
        thread->queue = i;
        int e = pthread_create(&(New_thr[i]->thread_id), NULL, start_func, New_thr[i]);
    }
    double new_ans = 0.0;
    for (int i = 0; i < p; i++)
    {
        void* rt_value;
        pthread_join(New_thr[i]->thread_id, &rt_value);
        double thr_ret_val = *(double *)rt_value;
        new_ans += thr_ret_val;
    }
    sem_destroy(&sem);
    clock_gettime(CLOCK_REALTIME, &end);
    T_par = end.tv_sec - start.tv_sec;
    T_par += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("=================================\n");
    printf("Par_proc with crit section: Integral = %f; time = %f \n", I*(x2-x1)*(y2-y1)/N, T_par);
    printf("=================================\n");
    printf("Par_proc with ptr_join/ptr_exit: %f\n", new_ans*(x2-x1)*(y2-y1)/N);
    printf("=================================\n");
    printf("exact: %f\n", M_PI*M_PI/8);
    for (int i = 0; i < p; i++)
        free(New_thr[i]);
    return 0;
}
