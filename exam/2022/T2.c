#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

#define CAPACITY 4      

typedef struct {
    int data[CAPACITY];
    int in;            
    int out;          
    pthread_mutex_t mutex;
    pthread_cond_t wait_empty_buffer;
    pthread_cond_t wait_full_buffer;
} buffer_t;

void buffer_init(buffer_t *buffer)
{
    buffer->in = 0;
    buffer->out = 0;

    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->wait_empty_buffer, NULL);
    pthread_cond_init(&buffer->wait_full_buffer, NULL);
}

int buffer_is_empty(buffer_t *buffer)
{
    return buffer->in == buffer->out;
}

int buffer_is_full(buffer_t *buffer)
{
    return (buffer->in + 1) % CAPACITY == buffer->out;
}

// Todo
void buffer_put(buffer_t *buffer, int item)
{

}

// Todo
int buffer_get(buffer_t *buffer)
{

}

void *thread_entry(void *arg)
{
	buffer_t *buffer = (buffer_t *)arg;
	int item;
   	for(int i=0;i<3;i++){
		item = buffer_get(buffer);
		printf("%d\n", item);
		item = item+1;
		buffer_put(buffer, item);
   	}
	 return NULL;
}

#define N 3

int main()
{
    buffer_t buffers[N];
    pthread_t pthreads[N];

    int i;
    for (i = 0; i < N; i++) {
        buffer_t *buffer = buffers + i;
        buffer_init(buffer);
    }

    puts("END");
    return 0;
}
