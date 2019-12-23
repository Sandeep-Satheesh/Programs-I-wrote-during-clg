#include<stdio.h>
#include<stdlib.h> // for rand() function
#include<limits.h> //for the value of INT_MAX (maximum supported integer value)
#include<time.h> // to initialize the random number generator with the current system time.
#define INTEGERS_PER_FILE 1000 //control how many random integers you want to generate in one input file.

/*This program attempts to sort a bulk amount of numbers using the external merge sort algorithm, using the input data as randomly generated numbers in 3 files.

    Logic:
        1. Generate 3 files input1.txt, input2.txt and input3.txt, each with INTEGERS_PER_FILE random numbers in them.

      -To sort:
        2. Open each file, and merge-sort every set of 100 integers and create 10 such temporary files (EACH WITH INTGERS_PER_FILE/10 integers in them) each containing 100 sorted integers.
        3. Open the 10 files and merge each of the 10 temporary files into one sorted file (input1sorted, input2sorted, input3sorted).
        4. Again merge the 3 sorted input files into 1 sortedoutput file.
*/

//The following two functions are part of the standard mergesort routine, the code is available readily on the Internet.
void merge(int arr[], int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1+ j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
	if (l < r)
	{
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l+(r-l)/2;

		// Sort first and second halves
		mergeSort(arr, l, m);
		mergeSort(arr, m+1, r);

		merge(arr, l, m, r);
	}
}

//Function to generate sample data (random numbers). Uses one FILE object to sequentially create 3 text files containing integer data to be sorted.
void generateSampleData() {
	FILE *f;
	//Repeatedly generate 3 files with INTEGERS_PER_FILE integers each.
	int filesGenerated = 1, ct = 1, t;
	char filename[] = "input1.txt";
    srand(time(0));

	while (filesGenerated++ <= 3) {
		f = fopen (filename, "w");
		if (!f) {
			printf("\n\nAccess not granted to create file '%s' ! Exiting...\n\n", filename);
			return;
		}
		ct = 1;
		while (ct++ <= INTEGERS_PER_FILE) {
			t = rand()%INT_MAX;
            fprintf(f, "%d ", t); //Write the random number to the file (as text).
		}
		fclose(f);
		printf("\nSample file '%s' generated successfully.", filename);

		filename[5] = '0' + filesGenerated;
	}
    printf("\n\nGenerated sample data files successfully.Restart program to sort the files.\n\n");
}

//Bulk-writes one whole array a[] with n integers into a text file pointed to by fo.
void outputArrToFile(int a[], int n, FILE *fo) {
    int ct = 0;
    while (ct < n)
        fprintf(fo, "%d ", a[ct++]);
}

//Routine to remove the 10 temporarily generated sorted files for each input file (after mergesorting them in memory).
void removeTempFiles() {
    char sortedFileName[] = "sorted_pt01.txt";
    int i;
    FILE *chunkFiles[10];
    for (i = 1; i <= 10; i++) {
        sortedFileName[9] = '0' + i/10;
        sortedFileName[10] = '0' + i%10;

        remove(sortedFileName);
    }
}

//Bulk-reads in n integers from a FILE *f into an array a[].
void fillUpArrFromFile(int a[], int n, FILE *f) {
    int i = 0;
    while (i < n && !feof(f))
        fscanf(f, "%d ", &a[i++]);
}

//Returns the smallest element in an array through linear search. The index of the smallest element is captured in the pointer *idx.
int min (int a[], int *idx, int n) {
    int i, min = a[0];
    *idx = 0;
    for (i = 1; i < n; i++)
        if (a[i] < min) {
            min = a[i];
            *idx = i;
        }
    return min;
}

//Merges the sorted chunks of files into one single output file. Requires the chunks to be open, passed as an array of FILE* pointers. No. of such files are given by n, and fileNo is a flag.
//fileNo is X when we are merging the sorted i/p files into the FINAL o/p file (it will be renamed later from inputX_sorted.txt to output.txt). Otherwise it is an integer.
void mergeFiles (FILE *chunks[], int n, char fileNo) {
    char outputFileName[] = "input1_sorted.txt";
    outputFileName[5] = fileNo;

    FILE *fo = fopen (outputFileName, "w");

    int a[n], i = 0, flg = 0, t, minIdx;

    for (i = 0; i < n; i++)
        fscanf(chunks[i], "%d ", &a[i]);

    while (1) {
        for (i = 0; i < n; i++)
            if (feof(chunks[i]))
                a[i] = INT_MAX;
        flg = 0;
		for (i = 0; i < n; i++)
			if (a[i] != INT_MAX) {
				flg = 1;
				break;
			}
		if (flg == 0) break;
        t = min (a, &minIdx, n);
        fprintf(fo, "%d ", t);

        fscanf(chunks[minIdx], "%d ", &a[minIdx]);
    }
    for (i = 0; i < n; i++) {
        while (!feof(chunks[i])) {
            fscanf(chunks[i], "%d ", &t);
            fprintf(fo, "%d ", t);
        }
        fclose(chunks[i]);
    }
    fclose(fo);

    printf("\nRemoving temporary files...");
    removeTempFiles();
}

//Reads in sets of INTEGERS_PER_FILE integers from the input file (f) and sorts them using mergesort function.
void sortFile (FILE *f, char fileNo) {
    int a[INTEGERS_PER_FILE/10], i = 0;

    char sortedFileName[] = "sorted_pt01.txt";
    FILE *chunkFiles[10];

    for (i = 1; i <= 10;) {
        fillUpArrFromFile(a, INTEGERS_PER_FILE/10, f);
        mergeSort(a, 0, 99);
        //Create 10 sorted temporary files in write mode. These will be merged later.
        chunkFiles[i-1] = fopen(sortedFileName, "w");

        if (!chunkFiles[i-1]) {
            printf("\nCan't create file '%s'!", sortedFileName);
            return;
        }

        outputArrToFile(a, INTEGERS_PER_FILE/10, chunkFiles[i-1]);
        fclose(chunkFiles[i-1]);
        i++;

        sortedFileName[9] = '0' + i/10;
        sortedFileName[10] = '0' + i%10;
    }
    //Open all the files once again in read mode, for merging.
    for (i = 1; i <= 10; i++) {
        sortedFileName[9] = '0' + i/10;
        sortedFileName[10] = '0' + i%10;

        chunkFiles[i-1] = fopen (sortedFileName, "r");
        if (!chunkFiles[i-1]) {
            printf("\nCan't create file '%s'!", sortedFileName);
            return;
        }
    }

    mergeFiles(chunkFiles, 10, fileNo);
}

//"Brain" function that co-ordinates the calling of other functions to perform the external sorting process step-by-step.
void externalSort() {
    /*Algorithm:
        1. Sort each of the 3 files.
        2. Merge the sorted 3 files into 1 single file.
    */
    int i;
    //Step 1:
        //Open the input files.
        FILE *f[3];
        f[0] = fopen("input1.txt", "r");
        f[1] = fopen("input2.txt", "r");
        f[2] = fopen("input3.txt", "r");

        //Sort each of the input files.
        for (i = 0; i < 3; i++) {
            if (!f[i]) {
                printf("\nOne of the input files does not exist! Terminating app...");
                return;
            }
            printf("\n\nSorting file %d...", i+1);
            sortFile(f[i], '1' + i);
            fclose(f[i]);
        }

        //Merge the sorted input files.
        f[0] = fopen("input1_sorted.txt", "r");
        f[1] = fopen("input2_sorted.txt", "r");
        f[2] = fopen("input3_sorted.txt", "r");

        printf("\n\nMerging the sorted files into a single file...");
        mergeFiles(f, 3, 'X');

        for (i = 0; i < 3; i++) {
            fclose(f[i]);
        }
        //remove ("inputX_sorted.txt"); //if it already exists we can't rename it...
        rename ("inputX_sorted.txt", "merged_output.txt");
    printf("\n\nCompleted sorting all the files successfully.\n\n");
}

//main() function to generate sample data (if required) or call the externalSort() function...
int main() {
	printf("\n\nMenu\n1 - Generate sample data (3 files with 1,000 integers each).\n2 - Sort and merge the generated sample data into 1 file using external sort. ");
	int ch;
	printf("\n\nMake your choice: ");
	scanf("%d", &ch);

    if (ch == 1) generateSampleData();
	else if (ch == 2) externalSort();
	else printf("\n\nInvalid input! Exiting...");

	return 0;
}
