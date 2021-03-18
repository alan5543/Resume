/*
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your group information here.
 *
 * Group No.: 16 (Join a project group in Canvas)
 * Creator: Yang Kai Yam
 * Project Concept: Create and Design a pzip operation in linux system
 */

// add/remove header files as you need
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdbool.h> //define boolean flag

int thread_number; //define how many threads will be used
int page_size; //general page size is 4096 Bytes
int files_number; //total number of files
int page_number;
int queue_head = 0; //define the circular queue
int queue_tail = 0;
int queue_size = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER, filelock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER, fill = PTHREAD_COND_INITIALIZER;
int* pages_per_file;
bool finish = false; //Initially, define the finish flag to be false

#define queue_capacity 10

struct buffer { //define the buffer for use
	char* address;
	int f_num;
	int p_num;
	int last_psize;
}buf[queue_capacity];

struct buffer get() { //add to queue
	struct buffer a1 = buf[queue_tail];
	int temp = queue_tail + 1;
	queue_tail = temp % queue_capacity;
	queue_size = queue_size - 1;
	return a1;
}

struct result { //the information of compressed result
	char* info;
	int* counter;
	int capacity;
}*res;

struct specific {
	int capacity;
	char* address;
}*files;

void position(struct buffer a1) { //remove from queue
	buf[queue_head] = a1;
	int temp = queue_head + 1;
	queue_head = temp % queue_capacity;
	queue_size = queue_size + 1;
}

struct result Compress(struct buffer temp) {
	struct result done;
	int count = 0;
	done.counter = malloc(temp.last_psize * sizeof(int));
	char* tString = malloc(temp.last_psize);
	int i = 0, end = temp.last_psize;
	while (i < end) { //loop from the starting point to the last page of size
		tString[count] = temp.address[i];
		done.counter[count] = 1;
		while ((temp.address[i] == temp.address[i + 1]) && (temp.last_psize > i + 1)) {
			done.counter[count]++;
			i++;
		}
		count++;
		i++;
	}                //the buffer object is compressed
	done.capacity = count;
	done.info = realloc(tString, count);
	return done;
}
//the control of the consumer threads
void *threadConsumer() {
	bool first_arrive = true;
	while (!(finish == true && queue_size == 0) || (first_arrive == true)) {
		pthread_mutex_lock(&lock);
		while (queue_size == 0 && finish == false) {
			pthread_cond_signal(&empty);
			pthread_cond_wait(&fill, &lock); //call the producer wake up
		}                                    //start to fill up the buffer queue
		if (finish == true && queue_size == 0) {
			pthread_mutex_unlock(&lock);     //when finish flag is true, mapping is done
			return NULL;
		}
		struct buffer temp = get();

		if(finish == false)
			pthread_cond_signal(&empty);

		pthread_mutex_unlock(&lock);

		int pos = 0; //calculate the position of the buffer
		int num = 0, end = temp.f_num;
		while (num < end) {
			pos = pos + pages_per_file[num];
			num++;
		}
		pos = pos + temp.p_num;

		res[pos] = Compress(temp); //call the compression of buffer
		first_arrive = false; //undefine the first do of the first arrive
	}
	return NULL;
}

void* threadProducer(void *arg) {
	char** file_name = (char **)arg;
	char* mapper;
	int file;
	struct stat op;

	//int i = 0;
	for (int file_count = 0; file_count < files_number; file_count++) {
		file = open(file_name[file_count], O_RDONLY);
		int last_psize = 0;
		int pages_num = 0;

		if (file == -1) {  //Not Found
			printf("Error: File didn't open\n");
			exit(1);
		}
		if (fstat(file, &op) == -1) { //Not retrieve the file stats
			close(file);
			printf("Error: Couldn't retrieve file stats");
			exit(1);
		}
		if (op.st_size == 0)  //file is empty
			continue;

		pages_num = op.st_size / page_size;
		double extra_pages_num = ((double)op.st_size / page_size);

		if (pages_num == extra_pages_num) { //determine whether need an extra page or not
			last_psize = page_size;
		}
		else if (pages_num < extra_pages_num) { //extra page is needed
			pages_num ++;
			last_psize = op.st_size%page_size;
		} 

		page_number = page_number + pages_num;
		pages_per_file[file_count] = pages_num;
		mapper = mmap(NULL, op.st_size, PROT_READ, MAP_SHARED, file, 0); //mapping to the entire file

		if (mapper == MAP_FAILED) {
			close(file);
			printf("Error mmapping the file\n");
			exit(1);
		}

		for (int k = 0; k < pages_num; k++) { //create the buffer
			pthread_mutex_lock(&lock);
			while (queue_size == queue_capacity) { //forcus on the queue
				pthread_cond_broadcast(&fill);
				pthread_cond_wait(&empty, &lock); //call the consumer thread to work
			}
			pthread_mutex_unlock(&lock);
			struct buffer tp;
			int endpage = pages_num - 1;
			if (k == endpage)
				tp.last_psize = last_psize;
			else
				tp.last_psize = page_size;
			
			tp.address = mapper;
			tp.p_num = k; //determine the page number
			tp.f_num = file_count; //determine the number of file
			mapper = mapper + page_size; //walk to the next page

			pthread_mutex_lock(&lock);
			position(tp);
			pthread_mutex_unlock(&lock);
			pthread_cond_signal(&fill);
		}
		close(file);
	} //exit the loop

	finish = true; //the mapping of memory is done already
	pthread_cond_broadcast(&fill); //broadcast to other consumer threads
	return 0;
}


int main(int argc, char** argv)
{
	if (argc < 2) { //first check whether the argc is less than 2 arguments or not
		printf("pzip: file1 [file2 ...]\n");
		exit(1);
	}
	page_size = 10000000; //4096 bytes, define the paga size
	files_number = argc - 1; //define the number of file
	thread_number = get_nprocs(); //define the process consumer threads
	int filenum = sizeof(int)*files_number;
	pages_per_file = malloc(filenum);

	res = malloc(sizeof(struct result) * 512000 * 2);
	pthread_t pid, cid[thread_number];
	pthread_create(&pid, NULL, threadProducer, argv + 1);
	int cthread_num = 0, wait_PC = 0;

	while (cthread_num < thread_number) {
		pthread_create(&cid[cthread_num], NULL, threadConsumer, NULL);
		cthread_num++;
	}
	while (wait_PC < thread_number) {
		pthread_join(cid[wait_PC], NULL);
		wait_PC++;
	}
	pthread_join(pid, NULL);

	//printing process, for generating output
	char* output = malloc(page_number*page_size*(sizeof(int) + sizeof(char))); //determine the output variable
	char* start = output; //set up the starting point
	int ps = 0; //position
	while (ps < page_number) {
		if (ps < page_number - 1) {
			if (res[ps].info[res[ps].capacity - 1] == res[ps + 1].info[0])
			{
				res[ps + 1].counter[0] = res[ps + 1].counter[0] + res[ps].counter[res[ps].capacity - 1];
				res[ps].capacity--;
			}
		}
		int ps2 = 0;
		while (ps2 < res[ps].capacity) {

			*((int*)output) = res[ps].counter[ps2]; //determine the number
			output = output + sizeof(int);

			*((char*)output) = res[ps].info[ps2];
			output = output + sizeof(char);
			ps2++;
		}
		ps++;
	}
	fwrite(start, output - start, 1, stdout);

}