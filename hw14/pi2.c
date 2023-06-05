#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include<time.h>
#include<stdatomic.h>
#include <stdbool.h>
#include<errno.h>
int num_thread;
long total_loopcount;
long local_loopcount[16];//代表我接收ctlc時總共丟了幾筆資料在正方形中
double global_hit=0.0;
long loopCountAThreadNeed=0;
pthread_mutex_t mutex;
long hit[16];
int idx = 0;
double pi = 0.0;
struct timespec start_time, end_time;
int ctrlc_state;
time_t start_t, end_t;
bool isFirstTimeClick = true;

void thread(void* rand_buffer) {
	int threadID=-1;
	double point_x,point_y;
	long i;
	int j;
	double rand_d;
	pthread_mutex_lock(&mutex);
	threadID=idx;
	long* local_hit = &hit[idx];
	long* loopcount=&local_loopcount[idx++];
	pthread_mutex_unlock(&mutex);

	for(i=0;i<loopCountAThreadNeed;i++) {	
		drand48_r(rand_buffer, &rand_d);
		point_x = rand_d;
		drand48_r(rand_buffer, &rand_d);
		point_y = rand_d;
		local_loopcount[threadID]++;//此thread目前跑了幾次迴圈
		if( (point_x*point_x + point_y*point_y) < 1.0)
			*local_hit+=1;
	}
	printf("hit = %ld\n", *local_hit);
	pthread_mutex_lock(&mutex);
	global_hit += *local_hit;
	pthread_mutex_unlock(&mutex);
}


void sigint_handler(int signum){
	if(isFirstTimeClick){
		time(&start_t);
		isFirstTimeClick=false;
	}else{
		time(&end_t);
		double diff_t=difftime(start_t,end_t);
		if(diff_t<1.0){
			double hitTotal=0;
			long loopTotal=0;
			for(int k=0;k<idx;k++){
				hitTotal+=hit[k];
				loopTotal+=local_loopcount[k];
			}
			pi = (double)4*(hitTotal/loopTotal);
			printf("click detect:pi = %.8lf\n",pi);
			start_t=end_t;
		}	
	}
	
}

void main(int argc,char*argv[]) {
	struct drand48_data* rand_buffer[16];
	pthread_t id[16];
	double rand_d;
	int i;
	total_loopcount=atol(argv[1]);
	num_thread=atoi(argv[2]);
	assert(num_thread < 16);
	signal(SIGINT, sigint_handler);

	pthread_mutex_init(&mutex,NULL);
	loopCountAThreadNeed=total_loopcount/num_thread;
	for(i=0;i<num_thread;i++) {
		/*使用aligned_alloc分配記憶體，避免false sharing*/
		/*在這個例子中，「剛好」用malloc也會得到相同效果*/
		rand_buffer[i] = aligned_alloc(64, sizeof(struct drand48_data));
		srand48_r(rand(), rand_buffer[i]);
		drand48_r(rand_buffer[i], &rand_d);
		printf("@buffer = %p\n", rand_buffer[i]);
		printf("thread%d's seed = %f\n", i, rand_d);
		pthread_create(&id[i],NULL,(void *)thread,rand_buffer[i]);
	}

	for(i=0;i<num_thread;i++)
		pthread_join(id[i],NULL);
	pi = (double)4*(global_hit/total_loopcount);
	printf("pi = %.8lf\n",pi);

}