#include "scheduling_simulator.h"

static int simulation_time = 0;
static int current_process_id = 0;
static priority_list ready_list;
static priority_list vrr_ready_list;
process *running_process = NULL;
process *all_processes[MAX_NUMBER_OF_PROCESSES];

strategy scheduling_strategy;
int rr_current_time_slice = RR_TIME_SLICE;

void set_scheduling_strategy(int argc, char *argv[]) {
	scheduling_strategy = FIRSTCOMEFIRSTSERVED;
	if (argc > 1) {
		if (strcmp("FCFS", argv[1]) == 0)
			scheduling_strategy = FIRSTCOMEFIRSTSERVED;
		else if (strcmp("RR", argv[1]) == 0)
			scheduling_strategy = ROUNDROBIN;
		else if (strcmp("VRR", argv[1]) == 0)
			scheduling_strategy = VIRTUALROUNDROBIN;
		else
		  fprintf(stderr, "%s: First argument must be one of [FCFS, RR, VRR]. Proceeding with FCFS.\n", __FILE__);
	}
}

void initialize_system() {
	initialize_scheduler();
	initiate_all_processes();
}

int main(int argc, char *argv[]) {
	set_scheduling_strategy(argc, argv);
	initialize_system();
	for (simulation_time = 0; simulation_time < TIME_LIMIT; simulation_time++) {
		set_arriving_ready();
		perform_scheduling();
		print_status(simulation_time);
		proceed();
	}

	cleanup_memory();
	return EXIT_SUCCESS;
}

void initialize_scheduler() {
	ready_list.position_to_insert = 0;
	ready_list.position_to_retrieve = 0;
	vrr_ready_list.position_to_insert = 0;
	vrr_ready_list.position_to_retrieve = 0;
	for (int i = 0; i < MAX_NUMBER_OF_PROCESSES; i++) {
		all_processes[i] = NULL;
	}
}

void initiate_all_processes() {
	FILE *ptr = fopen("scenario.txt", "r");
	int a, b, c;
#define LINE_LEN 64
	char line_buffer[LINE_LEN];

  if (ptr == NULL) {
		fprintf(stderr, "%s: Couldn't open scenario.txt. Make sure it exists and has the correct read-permissions.\n", __FILE__);
		exit(EXIT_FAILURE);
	}

	while (fgets(line_buffer, LINE_LEN, ptr)) {
		int assigned = sscanf(line_buffer, "%d %d %d", &a, &b, &c);

		if (assigned == EOF) {
			break;
		} else if (assigned != 3 && assigned > 0) {
			fprintf(stderr, "%s: scenario.txt contains row with less than three columns.\n", __FILE__);
			exit(EXIT_FAILURE);
		} else if (assigned != 3 && assigned != EOF) {
			fprintf(stderr, "%s: Failed to parse scenario.txt; unknown error.\n", __FILE__);
			exit(EXIT_FAILURE);
		}

		parameters params = { .arrival_time = a, .cpu_burst = b, .io_burst = c };

		process *new_process = create_process(params);

		if (!insert_process(new_process))
			free(new_process);
	}
}

void add_to_ready_list(priority_list *list, process *process_to_add) {
	int index = list->position_to_insert % MAX_NUMBER_OF_PROCESSES;
	list->processes[index] = process_to_add;
	list->position_to_insert++;
}

void set_ready(priority_list *list, process *ready_process) {
	ready_process->status = STATUS_READY;
	if (scheduling_strategy == ROUNDROBIN) {
		add_to_ready_list(&ready_list, ready_process);
	} else {
		add_to_ready_list(list, ready_process);
	}
}

void schedule_process(process *process_to_schedule) {
	running_process = process_to_schedule;
	running_process->status = STATUS_RUNNING;
}

void fcfs() {
	get_next(&ready_list);
}

void round_robin() {
	get_next(&ready_list);
	rr_current_time_slice = RR_TIME_SLICE;
}

void virtual_round_robin() {
	bool success = get_next(&vrr_ready_list);
	if (success) {
		rr_current_time_slice = running_process->vrr_remaining;
	} else {
		get_next(&ready_list);
		rr_current_time_slice = RR_TIME_SLICE;
	}
}

//new
bool is_ready_process_available(priority_list *list) {
	return list->position_to_retrieve < list->position_to_insert;
}

void avoid_scheduling_same_process_twice(priority_list *list,
		process *next_process) {
	if (list->position_to_insert > list->position_to_retrieve) {
		int index = list->position_to_retrieve % MAX_NUMBER_OF_PROCESSES;
		process *next_next_process = list->processes[index];
		if (next_process->id == next_next_process->id) {
			list->position_to_retrieve++;
		}
	}
}

process* identify_next_process(priority_list *list) {
	int index = list->position_to_retrieve % MAX_NUMBER_OF_PROCESSES;
	process *next_process = list->processes[index];
	list->position_to_retrieve++;
	return next_process;
}

bool get_next(priority_list *list) {
	if (!is_ready_process_available(list)) {
		running_process = NULL;
		return false;
	} else {
		process *next_process = identify_next_process(list);
		schedule_process(next_process);
		avoid_scheduling_same_process_twice(list, next_process);
		return true;
	}
}

void select_next_process() {
	if (scheduling_strategy == FIRSTCOMEFIRSTSERVED)
		fcfs();
	else if (scheduling_strategy == ROUNDROBIN)
		round_robin();
	else if (scheduling_strategy == VIRTUALROUNDROBIN)
		virtual_round_robin();
}

void perform_scheduling() {
	if (running_process == NULL) {
		select_next_process();
		return;
	}
	if (running_process->status == STATUS_BLOCKED || running_process->status == STATUS_READY || running_process->status == STATUS_NOT_ARRIVED) {
		select_next_process();
	}
}

void proceed_with_task(process *running_process) {
	if (running_process == NULL) {
		return;
	}
	if (running_process->status == STATUS_RUNNING) {
		if (running_process->cpu > 0) {
			running_process->cpu--;
			if (scheduling_strategy == ROUNDROBIN || scheduling_strategy == VIRTUALROUNDROBIN) {
				rr_current_time_slice--;
				if (running_process->vrr_remaining > 0) {
					running_process->vrr_remaining--;
				}
			}
		}

		if (running_process->cpu == 0) {
			running_process->status = STATUS_BLOCKED;
			running_process->cpu = running_process->params.cpu_burst;
			if (scheduling_strategy == VIRTUALROUNDROBIN) {

				if (rr_current_time_slice > 0) {
					running_process->vrr_remaining = rr_current_time_slice;
				}
			}
		} else if (rr_current_time_slice == 0) {
			set_ready(&ready_list, running_process);
		}
	}
}

void proceed_io_tasks() {
	process *current_process;
	for (int i = 0; i < MAX_NUMBER_OF_PROCESSES; i++) {
		current_process = all_processes[i];
		if (current_process == NULL) {
			continue;
		}
		if (current_process->status == STATUS_BLOCKED) {
			assert(current_process->io > 0);
			current_process->io--;
			if (current_process->io == 0) {
				if (current_process->vrr_remaining > 0) {
					set_ready(&vrr_ready_list, current_process);
				} else {
					set_ready(&ready_list, current_process);
				}
				current_process->io = current_process->params.io_burst;
			}
		}
	}
}

void proceed() {
	proceed_io_tasks();
	proceed_with_task(running_process);
}

bool insert_process(process *task_to_insert) {
	process *current_process;
	for (int i = 0; i < MAX_NUMBER_OF_PROCESSES; i++) {
		current_process = all_processes[i];
		if (current_process == NULL) {
			all_processes[i] = task_to_insert;
			return true;
		}
	}
	return false;
}

process* create_process(parameters params) {
	process *new_process = (process*) malloc(sizeof(process));
	new_process->params = params;
	new_process->id = current_process_id++;
	new_process->cpu = params.cpu_burst;
	new_process->io = params.io_burst;
	new_process->vrr_remaining = 0;
	new_process->status = STATUS_NOT_ARRIVED;
	return new_process;
}

void set_arriving_ready() {
	process *process;
	for (int i = 0; i < MAX_NUMBER_OF_PROCESSES; i++) {
		process = all_processes[i];

		if (process != NULL && process->status == STATUS_NOT_ARRIVED && process->params.arrival_time <= simulation_time) {
			set_ready(&ready_list, process);
		}
	}
}

void cleanup_memory() {
	for (int i = 0; i < MAX_NUMBER_OF_PROCESSES; i++)
		free(all_processes[i]);
}

void print_status(int time) {
	if (running_process == NULL) {
		printf("%4d: Running process: NONE\n", time);
	} else {
		printf("%4d: Running process: %d (%d/%d)\n", time, running_process->id,
				running_process->cpu, running_process->io);
	}
	fflush(stdout);
}
