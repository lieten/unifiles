#ifndef SCHEDULING_SIMULATOR_H_
#define SCHEDULING_SIMULATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NUMBER_OF_PROCESSES 100
#define TIME_LIMIT 100

#define RR_TIME_SLICE 2

typedef enum status {
	STATUS_NOT_ARRIVED,
	STATUS_RUNNING,
	STATUS_READY,
	STATUS_BLOCKED,
} status;

typedef enum strategy {
	FIRSTCOMEFIRSTSERVED,
	ROUNDROBIN,
	VIRTUALROUNDROBIN,
} strategy;

typedef struct parameters {
	int arrival_time;
	int cpu_burst;
	int io_burst;
} parameters;

typedef struct process {
	int id;
	status status;
	parameters params;
	int cpu;
	int io;
	int vrr_remaining;
} process;

typedef struct priority_list {
	process *processes[MAX_NUMBER_OF_PROCESSES];
	int position_to_insert;
	int position_to_retrieve;
} priority_list;


void set_scheduling_strategy(int argc, char *argv[]);
void initialize_system();
void initialize_scheduler();
void initiate_all_processes();
void add_to_ready_list(priority_list*, process*);
void set_ready(priority_list*, process*);
void schedule_process(process*);
void fcfs();
void round_robin();
void virtual_round_robin();
bool is_ready_process_available(priority_list*);
void avoid_scheduling_same_process_twice(priority_list*, process*);
process* identify_next_process(priority_list*);
bool get_next(priority_list*);
void select_next_process();
void perform_scheduling();
void proceed_with_task(process*);
void proceed_io_tasks();
void proceed();
bool insert_process(process*);
process* create_process(parameters);
void set_arriving_ready();
void cleanup_memory();
void print_status(int);

#endif /* SCHEDULING_SIMULATOR_H_ */
