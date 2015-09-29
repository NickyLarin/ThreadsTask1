#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "stack.h"
#include "stack_int.h"

#define MAX_CHARS 100
#define WORKERS 4

extern char *strtok_r(char *, const char *, char **);

struct ReaderArgs
{
    Stack *string_stack;
    pthread_mutex_t *string_stack_mutex;
    pthread_cond_t *string_stack_condition;

	pthread_mutex_t *read_done_mutex;
	int *read_done;
};
typedef struct ReaderArgs ReaderArgs;

struct WorkerArgs
{
    Stack *string_stack;
    StackInt *int_stack;
    pthread_mutex_t *string_stack_mutex;
    pthread_mutex_t *string_stack_cond_mutex;
    pthread_cond_t *string_stack_condition;
    pthread_mutex_t *int_stack_mutex;
    pthread_cond_t *int_stack_condition;

	pthread_mutex_t *read_done_mutex;
	int *read_done;

	pthread_mutex_t *work_done_mutex;
	int *work_done;
};
typedef struct WorkerArgs WorkerArgs;

struct WriterArgs
{
    StackInt *int_stack;
    pthread_mutex_t *int_stack_mutex;
    pthread_mutex_t *int_stack_cond_mutex;
    pthread_cond_t *int_stack_condition;

	pthread_mutex_t *work_done_mutex;
	int *work_done;
	
};
typedef struct WriterArgs WriterArgs;

void * reader(void * arg)
{
    ReaderArgs *r_args = arg;
    FILE *fptr;
    fptr = fopen("input.txt", "r");
    char c[MAX_CHARS];
    while(!feof(fptr))
    {
        if(fgets(c, MAX_CHARS, fptr))
        {
            printf("reader have read line: %s", c);
            pthread_mutex_lock(r_args->string_stack_mutex);
            stackPush(r_args->string_stack, c);
            pthread_mutex_unlock(r_args->string_stack_mutex);
            pthread_cond_signal(r_args->string_stack_condition);
        }
    }
    fclose(fptr);
}

void * worker(void * arg)
{
    int id = (int)pthread_self();
    printf("worker started id: %d\n", id);
    WorkerArgs *w_args = arg;
	int cond = 0;
    while (!cond)
    {
        if(w_args->string_stack->size > 0)
        {
            char string[MAX_CHARS];
            pthread_mutex_lock(w_args->string_stack_mutex);
            stackPop(w_args->string_stack, string);
            pthread_mutex_unlock(w_args->string_stack_mutex);
            printf("worker %d got string: %s", id, string);
            const char separator[] = " \n";
            int sum = 0;
			int a = 0;
			char *z;
            char *res = strtok_r(string, separator, &z);
            while(res != NULL)
            {
				a = (int)strtol(res, NULL , 10);
				printf("worker %d res: %d\n", id, a);
                sum += a;
                res = strtok_r(NULL, separator, &z);
            }
            printf("worker %d sum: %d\n", id, sum);
            pthread_mutex_lock(w_args->int_stack_mutex);
            stackIntPush(w_args->int_stack, sum);
            pthread_mutex_unlock(w_args->int_stack_mutex);
            pthread_cond_signal(w_args->int_stack_condition);
        }
        else
        {
			pthread_mutex_lock(w_args->read_done_mutex);
			if(w_args->read_done)
			{
				cond = 1;
			}
        	pthread_mutex_unlock(w_args->read_done_mutex);
            printf("worker %d waiting for strings\n", id);
            pthread_mutex_lock(w_args->string_stack_cond_mutex);
            pthread_cond_wait(w_args->string_stack_condition, w_args->string_stack_cond_mutex);
            pthread_mutex_unlock(w_args->string_stack_cond_mutex);
        }
    }
	printf("worker %d stopped\n", id);
}

void * writer(void * arg)
{
    printf("writer started id: %d\n", (int)pthread_self());
    WriterArgs *wr_args = arg;
    FILE *fptr;
    fptr = fopen("output.txt", "w");
    int result = 0;
    while(1)
    {
        if(wr_args->int_stack->size > 0)
        {
            pthread_mutex_lock(wr_args->int_stack_mutex);
			int i;
			stackIntPop(wr_args->int_stack, &i);
            result += i;
            pthread_mutex_unlock(wr_args->int_stack_mutex);
            fptr = fopen("output.txt", "w");
            fprintf(fptr, "%d", result);
            fclose(fptr);
            printf("writer wrote result: %d\n", result);
        }
        else
        {
            printf("writer waiting for sums\n");
            pthread_mutex_lock(wr_args->int_stack_cond_mutex);
            pthread_cond_wait(wr_args->int_stack_condition, wr_args->int_stack_cond_mutex);
            pthread_mutex_unlock(wr_args->int_stack_cond_mutex);
        }
    }
}

int main()
{
    Stack string_stack;
    stackInitialize(&string_stack, 100);

    StackInt int_stack;
    stackIntInitialize(&int_stack, 100);

    pthread_mutex_t string_stack_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t int_stack_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_cond_t string_stack_condition = PTHREAD_COND_INITIALIZER;
    pthread_cond_t int_stack_condition = PTHREAD_COND_INITIALIZER;

    pthread_mutex_t string_stack_cond_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t int_stack_cond_mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_t read_done_mutex = PTHREAD_MUTEX_INITIALIZER;
	int read_done = 0;	

	pthread_mutex_t work_done_mutex = PTHREAD_MUTEX_INITIALIZER;
	int work_done = 0;	

    ReaderArgs r_args;
    r_args.string_stack = &string_stack;
    r_args.string_stack_mutex = &string_stack_mutex;
    r_args.string_stack_condition = &string_stack_condition;
	
	r_args.read_done_mutex = &read_done_mutex;
	r_args.read_done = &read_done;

    pthread_t reader_t;
    pthread_create(&reader_t, NULL, reader, (void *)&r_args);

    WorkerArgs w_args;
    w_args.string_stack = &string_stack;
    w_args.int_stack = &int_stack;
    w_args.string_stack_mutex = &string_stack_mutex;
    w_args.string_stack_cond_mutex = &string_stack_cond_mutex;
    w_args.string_stack_condition = &string_stack_condition;
    w_args.int_stack_condition = &int_stack_condition;
    w_args.int_stack_mutex = &int_stack_mutex;

	w_args.read_done = &read_done;
	w_args.read_done_mutex = &read_done_mutex;	
	
	w_args.work_done = &work_done;
	w_args.work_done_mutex = &work_done_mutex;

    pthread_t worker_t[WORKERS];
    for(int i = 0; i< WORKERS; i++)
    {
        pthread_create(&worker_t[i], NULL, worker, (void *)&w_args);
    }


    WriterArgs wr_args;
    wr_args.int_stack = &int_stack;
    wr_args.int_stack_mutex = &int_stack_mutex;
    wr_args.int_stack_condition = &int_stack_condition;
    wr_args.int_stack_cond_mutex = &int_stack_cond_mutex;

	wr_args.work_done = &work_done;
	wr_args.work_done_mutex = &work_done_mutex;

    pthread_t writer_t;
    pthread_create(&writer_t, NULL, writer, (void *)&wr_args);


	if(!pthread_join(reader_t, NULL))
	{
		printf("Reader stopped!\n");
        pthread_mutex_lock(&read_done_mutex);
		read_done = 1;
        pthread_mutex_unlock(&read_done_mutex);
	}
    sleep(3);
}
