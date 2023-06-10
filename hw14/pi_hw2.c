#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdatomic.h>

int num_thread = 0;
long total_loopcount = 0;
atomic_long global_hit = 0;
int idx = 0, first_ctrl = 0;
struct timeval record;

typedef struct th_pack {	// sizeof 40 bytes
    long volatile hit;	// sizeof 8 bytes
    long volatile loopcount;	// sizeof 8 bytes
	struct drand48_data rand_buf;   // sizeof 24 bytes
} Thread_Pack;

Thread_Pack* th_data[16];

void sighandler(int signum){
    long cur_hit = 0, cur_loopcount = 0;
    struct timeval cur;
    gettimeofday(&cur, NULL);

	for (int i=0; i<num_thread; i++) {
        cur_hit += th_data[i]->hit;
        cur_loopcount += th_data[i]->loopcount;
    }
    double pi = (double)4*((double)cur_hit/cur_loopcount);
	printf("hit: %ld, loop: %ld, pi = %.8lf\n", cur_hit, cur_loopcount,pi);

    if(first_ctrl == 0) {
        record = cur;
        first_ctrl = 1;
    }
    else {
        struct timeval diff;
        timersub(&cur, &record, &diff);
        double time_used = diff.tv_sec + (double) diff.tv_usec / 1000000.0;
		printf("time used: %lf\n", time_used);
        if(time_used < 1) 
            _exit(0);
        else
            record = cur;
    }
}

void thread(void* th_data) {
	double point_x,point_y;
	long local_loopcount = total_loopcount/num_thread;
	double rand_d;
    Thread_Pack *pack = (Thread_Pack *)th_data;

	for(long i=0;i<local_loopcount;i++) {	
		drand48_r(&(pack->rand_buf), &rand_d);
		point_x = rand_d;
		drand48_r(&(pack->rand_buf), &rand_d);
		point_y = rand_d;
		if( (point_x*point_x + point_y*point_y) < 1.0)
			pack->hit += 1;
        pack->loopcount += 1;
	}
	atomic_fetch_add(&global_hit, pack->hit);
}

void main(int argc,char*argv[]) {
	pthread_t id[16];
	double rand_d;
	total_loopcount=atol(argv[1]);
	num_thread=atoi(argv[2]);
	assert(num_thread <= 16);
	signal(SIGINT, sighandler);

	for(int i=0;i<num_thread;i++) {
        th_data[i] = aligned_alloc(40 , sizeof(struct th_pack));
		srand48_r(rand(), &(th_data[i]->rand_buf));
		drand48_r(&(th_data[i]->rand_buf), &rand_d);
		pthread_create(&id[i],NULL,(void *)thread, th_data[i]);
	}

	for(int i=0;i<num_thread;i++)
		pthread_join(id[i],NULL);

	double pi = (double)4*((double)global_hit/total_loopcount);
	printf("pi = %.8lf\n",pi);
}
