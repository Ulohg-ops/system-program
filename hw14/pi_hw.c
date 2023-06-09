#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <time.h>

int num_thread;
long total_loopcount;
double global_hit=0.0;
pthread_mutex_t mutex;
long  hit[16];
double pi[16];
int idx  = 0;
int flag = 0;
int enter_handler_count = 0;
struct timespec tt1,tt2;

 //將timespec轉成nanoseconds
long ts_to_long(struct timespec t) {
    return t.tv_sec * 1000000000 + t.tv_nsec;
}

struct param{
	int thread_index;
	int now_loopcount;
	struct drand48_data* rand_buffer;
};

struct param* para;

void calculate_average(void){
	double total_sum = 0.0;
	for(int i = 0 ;i < num_thread ;i++ ){
        pi[i] = 4*((double)hit[i]/para[i].now_loopcount);
		printf("pi[%d] = %lf\n",i,pi[i]);
        total_sum = total_sum+pi[i];
    }     
	printf("Now pi = %2.7lf\n",total_sum/num_thread);
}


void sighandler(int signum){ 
	if (enter_handler_count%2 == 0)
		clock_gettime(CLOCK_MONOTONIC, &tt1);	
	else
		clock_gettime(CLOCK_MONOTONIC, &tt2);
		
	for (int i=0; i<idx; i++){
		printf("now, thread %d hit = %ld\n",i ,hit[i]);
		printf("now, loopcount : %d\n",para[i].now_loopcount);
	}
	calculate_average();
	enter_handler_count++;
	
	long time = ts_to_long(tt2)-ts_to_long(tt1);
	if (time < 1000000000 && time > 0)
		exit(EXIT_SUCCESS);
	
}

void thread(struct param* para) {
	double point_x,point_y;
	double rand_d;
	long i;
	long local_loopcount = total_loopcount/num_thread; 	

	pthread_mutex_lock(&mutex);
	long* local_hit = &hit[idx++];
	pthread_mutex_unlock(&mutex);

	for(i = 0 ;i < local_loopcount ;i++){	
		para->now_loopcount = i;
		drand48_r(para->rand_buffer, &rand_d);
		point_x = rand_d;
		drand48_r(para->rand_buffer, &rand_d);
		point_y = rand_d;
		
		if( (point_x*point_x + point_y*point_y) < 1.0)
			*local_hit+=1;
	}
	//printf("hit = %ld\n", *local_hit);
	
	pthread_mutex_lock(&mutex);
	global_hit += *local_hit;
	pthread_mutex_unlock(&mutex);
}

void main(int argc,char*argv[]) {
	pthread_t id[16];
	int i;
	double pi = 0.0;
	double rand_d;

	total_loopcount=atol(argv[1]);
	num_thread=atoi(argv[2]);
	assert(num_thread < 16);
	para =(struct param*)malloc(sizeof(struct param)*num_thread);

	signal(SIGINT , sighandler);

	pthread_mutex_init(&mutex,NULL);	
	for(i = 0 ;i < num_thread ;i++ ) {
		/*使用aligned_alloc分配記憶體，避免false sharing*/
		/*在這個例子中，「剛好」用malloc也會得到相同效果*/
		para[i].thread_index = i;
		para[i].now_loopcount= 0;
		para[i].rand_buffer = aligned_alloc(64, sizeof(struct drand48_data));
		struct param* para_ptr = para+i;	
	
		srand48_r(rand(), para[i].rand_buffer);
		drand48_r(para[i].rand_buffer, &rand_d);

		printf("@buffer = %p\n",para[i].rand_buffer);
		printf("thread%d's seed = %f\n",i,rand_d);
		
		pthread_create(&id[i],NULL,(void *)thread,(void *)para_ptr);
	}

	for(i=0;i<num_thread;i++)
		pthread_join(id[i],NULL);

	pi = (double)4*(global_hit/total_loopcount);
	printf("pi = %.8lf\n",pi);
}