//DON'T FORGET TO SET WORKING DIRECTORY TO THE MAIN ONE! (where all txts are)

//format: #include "YYYY/DD.h"

#include "2025/06.h"

int main() {
	clock_t start = clock();
	Task task;
	task.run();
	clock_t task1 = clock();
	double elapsed = (double) (task1 - start) / CLOCKS_PER_SEC;
	printf("Task1 executed in: %.05f\n\n", elapsed);

	task.runPart2();
	clock_t task2 = clock();
	elapsed = (double) (task2 - task1) / CLOCKS_PER_SEC;
	printf("Task2 executed in: %.05f\n\n", elapsed);
	return 0;
}