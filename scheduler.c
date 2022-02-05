/*
 Documentation Template

 Author:			Matthew McAlarney and Ali McNicholas

 References:			https://www.learn-c.org/en/Linked_lists\
					https://en.wikipedia.org/wiki/Insertion_sort#List_insertion_sort_code_in_C
 https://www.geeksforgeeks.org/linked-list-set-2-inserting-a-node/

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
	int finished;
	struct job *next;
};

/*** Globals - We have access to global variables throughout all functions in this C file ***/
int seed = 100;

//This is the start of our linked list of jobs, i.e., the job list
struct job *head = NULL;

/*** Globals End ***/
void printList(struct job *head);

/*Function to append a new job to the list*/
void append(int id, int arrival, int length) {
	// create a new struct and initialize it with the input data
	struct job *tmp = (struct job*) malloc(sizeof(struct job));

	//tmp->id = numofjobs++;
	tmp->id = id;
	tmp->length = length;
	tmp->arrival = arrival;

	// the new job is the last job
	tmp->next = NULL;

	// Case: job is first to be added, linked list is empty
	if (head == NULL) {
		head = tmp;
		return;
	}

	struct job *prev = head;

	//Find end of list
	while (prev->next != NULL) {
		prev = prev->next;
	}

	//Add job to end of list
	prev->next = tmp;
	return;
}

/*Function to read in the workload file and create job list (the job list is a linked list of job structs)*/
void read_workload_file(char *filename) {
	int id = 0;
	FILE *fp;
	size_t len = 0;
	ssize_t read;
	char *line = NULL, *arrival = NULL, *length = NULL;

	//struct job **head_ptr = malloc(sizeof(struct job*));

	if ((fp = fopen(filename, "r")) == NULL)
		assert(0);

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
	struct job *currententry = head;
	int currenttimecounter = 0;

	printf("Execution trace with FIFO:\n");

	//Loop though all entries and schedule the jobs by their time of arrival
	while (currententry != NULL) {
		printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n",
				currenttimecounter, currententry->id, currententry->arrival,
				currententry->length);
		currenttimecounter += currententry->length;
		currententry = currententry->next;
	}

	//End of execution
	printf("End of execution with FIFO.\n");
	printf("%s", "");
	return;
}

struct job* getlistsortedbyshortestjoblength(struct job *unsortedarrivals) {

	//Check to see if head is NULL
	if (head == NULL || head->next == NULL) {
		return head;
	}

	struct job *sortedList = NULL;
	struct job *current = head;

	while(current != NULL) {
		if(sortedList == NULL) { //Check to see if the sorted list is empty
			// empty list
			sortedList = (struct job*) malloc(sizeof(struct job));
			memcpy(sortedList, current, sizeof(struct job));
			sortedList->next = NULL;
		}
		else //If the sorted list is not empty, sort the list by shortest job length
		{
			struct job *currentSorted = head;
			struct job *prev = NULL;
			struct job *node = (struct job*) malloc(sizeof(struct job));
			int inserted = 0;

			//while the current node in the linklist is not empty, create a memory copy and determine if the current node needs to be inserted
			while(currentSorted != NULL)
			{
				if(currentSorted->arrival > current->arrival)
				{
					//Id the previous node is null, insert the current node before the previous node
					if(prev == NULL) {
						memcpy(node, current, sizeof(struct job));
						node->next = sortedList;
						sortedList = node;
						inserted = 1;
						break;
					}
					else //else, insert the current node after the previous node
					{
						memcpy(node, current, sizeof(struct job));
						prev->next = node;
						node->next = current;
						inserted = 1;
						break;
					}
				}
				prev = currentSorted;
				currentSorted = currentSorted->next;
			}
			if(!inserted)
			{
				memcpy(node, current, sizeof(struct job));
				currentSorted->next = node;
				node->next = NULL;
			}
			current = current->next;
		}

	}

	//return the list sorted by shortest job length
	return sortedList;
}

void policy_SJF(struct job *head) {

	struct job *listsortedbyshortestjoblength;

	printf("Execution trace with SJF:\n");
	listsortedbyshortestjoblength = getlistsortedbyshortestjoblength(NULL);

	//Obtain the list of jobs sorted by the shortest length that have arrived
	struct job * currentEntry = listsortedbyshortestjoblength;
	int currenttimecounter = 0;

	//Loop through the list of jobs sorted by the shortest length and schedule them
	while(currentEntry != NULL) {
		printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n",
				currenttimecounter, currentEntry->id, currentEntry->arrival,
				currentEntry->length);
		currenttimecounter += currentEntry->length;
		currentEntry = currentEntry->next;
	}

	//End of execution
	printf("End of execution with SJF.\n");
	printf("%s", "");
	return;
}

void analyze_FIFO(struct job *head) {
	// TODO: Fill this in
	return;
}

void analyze_SJF(struct job *head)
{
	return;
}

void
policy_RR (struct job *head, int analysis, int slice)
{
//initializing many of the different variables
  struct job *currententry = head;
  int currenttimecounter = 0;
  int full = 0;
  int count = 0;
  int completed = -1;
  printf ("Execution trace with RR:\n");
  fflush (stdout);
  int wait[count];
  //counts how many jobs need to be schedules
  while (currententry != NULL)
    {
      count++;
      currententry = currententry->next;
    }
    //make arrays for turnaround and response
  int turnaround[count];
  int response[count];
  for (int i = 0; i < count; i++)
    {
      turnaround[i] = 0;
      response[i] = 0;
      wait[i] = 0;
    }
  currententry = head;
  //keep the loop going until all jobs are completed
  while (completed < count)
    {
    //interates through the loop going to each job
      for (int i = 0; i < count; i++)
	{
	//tests to see if we should move on to the next job before completing the current one
      if(currententry->arrival - response[i-1] > slice && currententry != NULL){
        if(currententry->arrival-currenttimecounter < slice+1){
      if(currenttimecounter < currententry->arrival){
     currenttimecounter = currententry->arrival;
     }
     //prints out the end
      printf ("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n",
		      currenttimecounter, i,
		      currententry->arrival, slice);
		       currenttimecounter += slice;
      }
      //resets the current entry
if(currententry->id == count-1){
currententry = head;
}
else{
currententry = currententry->next;
}
}
//if no length skip over it
	  if (currententry->length == 0)
	    {
	      completed++;
	      currententry = currententry->next;
	    }
	    //if lower that the slice, then set length to 0 and
	  else if (currententry->length < slice)
	    {
	      if (full < count)
		{
		  response[i] = currenttimecounter;
		  wait[i] = currententry->length;
		  full++;
		}
             turnaround[i] = currenttimecounter + slice-1;
             if(currententry->arrival == currenttimecounter){
		response[i] = 0;
		}
	      printf ("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n",
		      currenttimecounter, i ,
		      currententry->arrival, currententry->length);
	      currenttimecounter += currententry->length;
		      currententry->length = 0;
	      currententry = currententry->next;

	    }
	    //if any other length (when subtracting length)
	  else
	    {
	      if (currententry->arrival > currenttimecounter)
		{
		  currenttimecounter = currententry->arrival;
		}
//counts to see if completed
	      if (full < count){
		  response[i] = currenttimecounter;
		  wait[i] = currententry->length;
		  full++;
		}
	      printf ("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n",
		      currenttimecounter, currententry->id,
		      currententry->arrival, slice);
	      currenttimecounter += slice;
	      turnaround[i] = currenttimecounter;
	      currententry->length = currententry->length - slice;
	      currententry = currententry->next;
	      fflush (stdout);
	    }
	}
      fflush (stdout);
      if (completed < count)
	{
	  currententry = head;
	  completed = 0;
	}
      else
	{
	  completed = count;
	}

    }
  printf ("End of execution with RR.\n");
  fflush (stdout);
  printf ("%s", "");
//starts analysis

  fflush (stdout);
  currententry = head;
  double avg1 = 0;
  double avg2 = 0;
  double avg3 = 0;
  if (analysis)
    {
      printf ("Begin analyzing RR:\n");
      fflush (stdout);
      for (int i = 0; i < count; i++)
	{
	  printf ("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n",
		  i, response[i], turnaround[i], turnaround[i] - wait[i]);
		  //finds averages
	  avg1 += response[i];
	  avg2 += turnaround[i];
	  avg3 += (turnaround[i] - wait[i]);
	}
      printf ("Average -- Response: %.2f  Turnaround %.2f  Wait %.2f\n",
	      avg1 / count, avg2 / count, avg3 / count);
      printf ("End analyzing RR.\n");
    }
return;
}

void
analyze_RR (struct job *head)
{
  struct job *current = head;
//printf("%d\n", current->length);
  current = current->next;
//printf("%d\n", current->length);
  current = current->next;
  fflush (stdout);

  return;
}

//Used for testing output of SJF
void printList(struct job *head) {
	struct job *current = head;
	while (current != NULL) {
		printf("%d %d\n", current->id, current->arrival);
		current = current->next;
	}
}

int main(int argc, char **argv) {

	if (argc < 4) {
		fprintf(stderr, "missing variables\n");
		fprintf(stderr,
				"usage: %s analysis-flag policy workload-file slice-duration\n",
				argv[0]);
		assert(0);
	}

	int analysis = atoi(argv[1]); //atoi returns an integer from a string argument
	char *policy = argv[2], *workload = argv[3];
	int slice_duration = atoi(argv[4]);

	// Note: we use a global variable to point to
	// the start of a linked-list of jobs, i.e., the job list
	read_workload_file(workload); //reads in the workload file (one of the test files that is run) and places its contents into the global lsit of jobs to be accessed later

	if (strcmp(policy, "FIFO") == 0) { //strcmp compares string 1 to string 2 to see if they are equal; if string 1 is equal to string 2, then 0 is returned.
		policy_FIFO(head);
		if (analysis) { //if analysis is 1 (1 = true, 0 = false for analysis), then we should analyze the performance of our job scheduler
			printf("Begin analyzing FIFO:\n");
			analyze_FIFO(head);
			printf("End analyzing FIFO.\n");
		}
	} else if (strcmp(policy, "SJF") == 0) {
		policy_SJF(head);
		if (analysis) {
			printf("Begin analyzing SJF:\n");
			//analyze_SJF(head);
			printf("End analyzing SJF.\n");
		}
	}
	else if (strcmp (policy, "RR") == 0)
	    {
	      policy_RR (head, analysis, slice_duration);
	      if (analysis && (strcmp (policy, "RR") == 0))
		{			//if analysis is 1 (1 = true, 0 = false for analysis), then we should analyze the performance of our job scheduler
		  //  printf ("Begin analyzing RR:\n");
		  //  analyze_RR(head);
		  // printf ("End analyzing RR.\n");
		}
	    }

	// TODO: Add other policies

	exit(EXIT_SUCCESS);
}
