/*
Documentation Template

Author:				author's name
Date:				date
Version:			which version
Project ID:			number or name of project
CS Class:			CS xxxx
Programming Language:		C or Scheme or Java or ...
OS/Hardware dependencies:	how does the program rely on the operating system,
				   or on the hardware?

Problem Description:		a statement of the requirements

Overall Design:			main design decisions about the program
	System structure
	Data representation
	Algorithms

Program Assumptions
      and Restrictions:		what needs to be true for the program to work

Interfaces:			how the program interacts with users, data or programs
	User
	File/D-B
	Program/Module

Implementation Details:
	Data			implementation details of data representation
	Variables		key variables and their scopes
	Algorithm		implementation details of algorithm(s) used

How to build the program:	the command line or the makefile

Program Source:			listing of the source code

Additional Files:		names of other files used

Results:			listing of sample run(s) of the program

Test Procedures:		how the program was tested
Test Data:			test cases

Performance Evaluation:		how well the program performs
	Time/Space
	User Interface

References:			https://www.learn-c.org/en/Linked_lists

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

// TODO: Add more fields to this struct
struct job {
    int id;
    int arrival;
    int length;
    struct job *next;
};

/*** Globals - We have access to global variables throughout all functions in this C file ***/
int seed = 100;

//This is the start of our linked list of jobs, i.e., the job list
struct job *head = NULL;

/*** Globals End ***/

/*Function to append a new job to the list*/
void append(int id, int arrival, int length){
  // create a new struct and initialize it with the input data
  struct job *tmp = (struct job*) malloc(sizeof(struct job));

  //tmp->id = numofjobs++;
  tmp->id = id;
  tmp->length = length;
  tmp->arrival = arrival;

  // the new job is the last job
  tmp->next = NULL;

  // Case: job is first to be added, linked list is empty
  if (head == NULL){
    head = tmp;
    return;
  }

  struct job *prev = head;

  //Find end of list
  while (prev->next != NULL){
    prev = prev->next;
  }

  //Add job to end of list
  prev->next = tmp;
  return;
}


/*Function to read in the workload file and create job list (the job list is a linked list of job structs)*/
void read_workload_file(char* filename) {
  int id = 0;
  FILE *fp;
  size_t len = 0;
  ssize_t read;
  char *line = NULL,
       *arrival = NULL,
       *length = NULL;

  //struct job **head_ptr = malloc(sizeof(struct job*));

  if( (fp = fopen(filename, "r")) == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) > 1) {
    arrival = strtok(line, ",\n");
    length = strtok(NULL, ",\n");

    // Make sure neither arrival nor length are null.
    assert(arrival != NULL && length != NULL);

    append(id++, atoi(arrival), atoi(length));
  }

  fclose(fp);

  // Make sure we read in at least one job
  assert(id > 0);

  return;
}


void policy_FIFO(struct job *head) {
  // TODO: Fill this in
	struct job * currententry = head;
	int currenttimecounter = 0;

	printf("Execution trace with FIFO:\n");

	while(currententry != NULL)
	{
		printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n", currenttimecounter, currententry->id, currententry->arrival, currententry->length);
		currenttimecounter += currententry->length;
		currententry = currententry->next;
	}

	printf("End of execution with FIFO.\n");
	printf("%s","");
  return;
}

void analyze_FIFO(struct job *head) {
  // TODO: Fill this in

  return;
}

int main(int argc, char **argv) {

 if (argc < 4) {
    fprintf(stderr, "missing variables\n");
    fprintf(stderr, "usage: %s analysis-flag policy workload-file slice-duration\n", argv[0]);
		exit(EXIT_FAILURE);
  }

  int analysis = atoi(argv[1]); //atoi returns an integer from a string argument
  char *policy = argv[2],
       *workload = argv[3];
  //int slice_duration = atoi(argv[4]);

  // Note: we use a global variable to point to
  // the start of a linked-list of jobs, i.e., the job list
  read_workload_file(workload); //reads in the workload file (one of the test files that is run) and places its contents into the global lsit of jobs to be accessed later

  if (strcmp(policy, "FIFO") == 0 ) { //strcmp compares string 1 to string 2 to see if they are equal; if string 1 is equal to string 2, then 0 is returned.
    policy_FIFO(head);
    if (analysis) { //if analysis is 1 (1 = true, 0 = false for analysis), then we should analyze the performance of our job scheduler
      printf("Begin analyzing FIFO:\n");
      analyze_FIFO(head);
      printf("End analyzing FIFO.\n");
    }

    exit(EXIT_SUCCESS);
  }

  // TODO: Add other policies

	exit(EXIT_SUCCESS);
}

