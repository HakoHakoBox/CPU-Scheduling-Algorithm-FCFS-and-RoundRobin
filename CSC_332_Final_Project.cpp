#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class process {
public:
	int processName; // Will always be one more than the index in eventQueue
	int arrivalTime;
	int turnAroundTime;
	int startTime;
	int completionTime = 0;
	int executionTime;
	int originalExecutionTime;
	//int finished=0; //1 = true. 0=false //not used
private:

};

void fcfs(process array[], int number_of_processes) {
	int timer = 0;
	int switchTime = 5;
	int maxBurstTime = 0;
	double totalSwitchTime = 0;
	double averageTat = 0;

	timer += array[0].arrivalTime; //checks if the first process doesn't arrive at 0

	//Finds the total burst time to calculate efficiency
	for (int i = 0; i < number_of_processes; i++) {

		maxBurstTime += array[i].executionTime;

	}

	for (int i = 0; i < number_of_processes; i++) { //goes through the whole array
		array[i].startTime = timer; //adds the burst time to the timer
		timer += array[i].executionTime;
		array[i].completionTime = timer;
		array[i].turnAroundTime = array[i].completionTime - array[i].arrivalTime;
		averageTat += array[i].turnAroundTime;
		timer += switchTime;			//adds switch time to the timer
		totalSwitchTime += switchTime;  //adds to total switch time for effeciency calculation
	}

	cout << "First Come First Serve (FCFS):" << endl;
	cout << "Total Time required is " << timer << " time units" << endl;
	cout << "Average waiting time is " << averageTat / number_of_processes << " time units" << endl;
	cout << "CPU Efficiency is " << (int)((maxBurstTime / (maxBurstTime + totalSwitchTime)) * 100) << "%" << endl; //(formula: Efficiency = Total Execution Time / (Total Execution time + Total Switching time))

	for (int i = 0; i < 3; i++) {
		cout << "Process " << array[i].processName << ":" << endl;
		cout << "Service time = " << array[i].executionTime << endl;
		cout << "Turnaround time = " << array[i].turnAroundTime << endl;
		cout << endl;
	}
}

void roundRobin(process array[], int number_of_processes, int quantiumTime) {
	int timer = 0;
	int switchTime = 5;
	double totalSwitchTime = 0;
	double averageTat = 0;
	int maxBurstTime = 0;

	for (int i = 0; i < number_of_processes; i++) { //calculates the entire burst time of all the processes together
		maxBurstTime += array[i].executionTime;
		//cout << maxBurstTime << endl; //TESTING
		//cout << array[i].completionTime << endl; //TESTING
	}

	//True if one or more of the processes still have execution time remaining. False if all processes are done.
	bool timeRemaining = true;

	while (timeRemaining) { //while processes still have execution time remaining

		for (int i = 0; i < number_of_processes; i++) { //goes through the entire eventQueue
			if ((array[i].completionTime == 0) && (array[i].executionTime > 0)) { //if the process hasn't been fully completed yet and the burst time is greater than 0
				if (array[i].executionTime > quantiumTime) { //if the process burst time is greater than the quantium time
					array[i].executionTime = array[i].executionTime - quantiumTime; //then we lower the buster time by the quantiumTime
					timer += quantiumTime; //and advance the timer
					//cout << timer << endl; //TESTING
				}
				else { //otherwise the process burst time is smaller than the quantium time, then the process is finished
					timer += array[i].executionTime; //advance the timer
					array[i].executionTime = 0; //set the process to be finisheh
					array[i].completionTime = timer; //set the process to be finisheh
					array[i].startTime = array[i].completionTime - array[i].originalExecutionTime; //calculating startTime 
					array[i].turnAroundTime = array[i].completionTime - array[i].arrivalTime; //calculating turnArroundTime
					averageTat += array[i].turnAroundTime;
					//cout << timer << endl; //TESTING

				}
			}

			timer += switchTime;				//adds switch time to the timer
			totalSwitchTime += switchTime;		//adds switch time to the totalSwitchTime for efficiency calculation 

			//Loop through all processes
			for (int i = 0; i < number_of_processes; i++) {

				/*
				* If a process is found that still has execution time remaining, then the while loop at line 77 continues for another iteration.
				* If no processes are found with execution time > 0,  the loop will not break and time remaining will be false causing the while loop to stop.
				*/
				if (array[i].executionTime > 0) {

					timeRemaining = true;
					break;

				}
				else {

					timeRemaining = false;

				}

			}

		}
	}

	cout << "Round Robin (Quantum " << quantiumTime << "): " << endl;
	cout << "Total Time required is " << timer << " time units" << endl;
	cout << "Average waiting time is " << averageTat / number_of_processes << " time units" << endl;
	cout << "CPU Efficiency is " << (int)((maxBurstTime / (maxBurstTime + totalSwitchTime)) * 100) << "%" << endl; //(formula: Efficiency = Total Execution Time / (Total Execution time + Total Switching time))

	for (int i = 0; i < 3; i++) {
		cout << "Process " << array[i].processName << ":" << endl;
		cout << "Service time = " << array[i].originalExecutionTime << endl;
		cout << "Turnaround time = " << array[i].turnAroundTime << endl;
		cout << endl;
	}
}

//Resets the completion time and execution time to default so the round robin algorithms can run.
void reset(process array[], int executionReset[], int number_of_processes) {

	for (int i = 0; i < number_of_processes; i++) {

		array[i].completionTime = 0;
		array[i].executionTime = executionReset[i];

	}

}

int main() {
	string line;
	ifstream myfile;
	myfile.open("input_SchedulingAlgorithms.txt");
	process eventQueue[50]; //everything is stored in here
	int executionReset[50]; //Stores execution times to reset them after each round robin call.
	int index = 0;
	int quantiumTen = 10;
	int quantiumFifty = 50;
	int quantiumHundo = 100;

	if (!myfile.is_open()) {
		exit(EXIT_FAILURE);
	}

	getline(myfile, line); //grabs the first line of the txt, which is the number of procecesses
	int number_of_processes = stoi(line); //converts string to int

	int process_number, arrival_time, cpu_time;
	while (myfile >> process_number >> arrival_time >> cpu_time) { //takes each line in the txt and assigns the numbers to their variables
		eventQueue[index].processName = process_number;
		eventQueue[index].arrivalTime = arrival_time;
		eventQueue[index].executionTime = cpu_time; // in roundRobin I use executionTime to see how long the cpu has worked on the process
		executionReset[index] = cpu_time;
		eventQueue[index].originalExecutionTime = cpu_time; // buest time of a process that does not get touched
		index += 1;
		cout << process_number << " " << arrival_time << " " << cpu_time << endl; //outputting the original txt file

	}
	cout << endl << "The number of processes is: " << number_of_processes << endl << endl;

	fcfs(eventQueue, number_of_processes);
	reset(eventQueue, executionReset, number_of_processes);

	roundRobin(eventQueue, number_of_processes, quantiumTen);
	reset(eventQueue, executionReset, number_of_processes);

	roundRobin(eventQueue, number_of_processes, quantiumFifty);
	reset(eventQueue, executionReset, number_of_processes);

	roundRobin(eventQueue, number_of_processes, quantiumHundo);
	reset(eventQueue, executionReset, number_of_processes);
}