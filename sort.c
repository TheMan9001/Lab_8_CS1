#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//increase by one every time a malloc is used
int extraMemoryAllocated;


void swap(int* a, int* b){
	
	int temp = *a;
    *a = *b;
    *b = temp;
}

//this function will sort the array into a max Heap order
void heapify(int arr[], int N, int i){
	
    //assume that the largest is already the root
    int largest = i;
 
    //grab the left and right indexs
    int nextLeft = 2 * i + 1;
    int nextRight = 2 * i + 2;
 
    //checks if either of the numbers are greater than the root
	//if so largest is now that index
    if (nextLeft < N && arr[nextLeft] > arr[largest]){
        largest = nextLeft;
	}
	if (nextRight < N && arr[nextRight] > arr[largest]){
        largest = nextRight;
	}
 
    //checks if the root isn't the largest 
    if (largest != i) {
		//swap the root with the largest
        swap(&arr[i], &arr[largest]);
 
        //since we had to switch root with an adjacent number
		//we need recursivly check the newly ordered heap
        heapify(arr, N, largest);
    }
	
	//if index i was the largest then the heap is properly sorted
}
 
// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int N){
 
    // Builds max heap before doing any swaps
    for (int i = N / 2 - 1; i >= 0; i--){
		heapify(arr, N, i);
	}
 
    for (int i = N - 1; i >= 0; i--){
		//now that the array is in max Heap setup arr[0] will always be the max number
		//so if we swap the root with the end of the array that number will be sorted
        swap(&arr[0], &arr[i]);
 
        //remake a max heap exlcuding numbers we already sorted
        heapify(arr, i, 0);
    }
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
	
	//creates and allocates memory for left and right arrays
	int *L;
	int *R;
	L = (int*) malloc(n1 * sizeof(int));
	extraMemoryAllocated += n1;
	R = (int*) malloc(n2 * sizeof(int));
	extraMemoryAllocated += n2;
	//extraMemoryAllocated is also incremented here due to the malloc uses
	
	//split the original array into the two arrays created above
    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
	}
    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1 + j];
	}
  
    //merge the two arrays back into one sorted array
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2){
        if (L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        }else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
  
    //Copies any remaining numbers between the two arrays back into the original array
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int pData[], int l, int r){
	
	if(l < r){
		int mid = (l+r)/2;
		
		mergeSort(pData, l, mid);
		mergeSort(pData, mid+1, r);
		
		merge(pData, l, mid, r);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData){
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile){
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL){
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i){
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz){
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i){
		//makes sure function doesn't try to get numbers that don't exist
		//(used to stop funtion when all numbers are already printed)
		if(i >= dataSz){
			printf("\n\n");
			return;
		}
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i){
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void){
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i){
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0){
			continue;
		}
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}