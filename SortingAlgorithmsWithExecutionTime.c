#include<stdio.h>
#include<time.h>

/*This program basically has all the popular sorting algorithms in one place (shell, quick, heap, merge, insertion).
It runs each of the algorithms on a set of integer data given by the user, and then measures the execution time of each algorithm for the given data.

The code for the sorting is not explained, as the concepts are readily available to be understood from the internet.

*/

void swap (int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

//O(NlogN)
void shellSort (int a[], int n) {
	int i,j,k;
	for (i = n; i > 0; i /= 2)
		for (j = i; j < n; j++)
			for (k = j - i; k >= 0; k-= i)
				if (a[k+i] >= a[k]) break;
				else swap(&a[k+i], &a[k]);
}

void heapify (int a[], int n, int i) {
	int l = 2*i + 1, r = 2*i + 2, largest = i;
	if (l < n && a[l] > a[largest])
		largest = l;
	if (r < n && a[r] > a[largest])
		largest = r;

	if (largest != i) {
		swap (&a[i], &a[largest]);
		heapify (a, n, largest);
	}
}

//O(NlogN) avg.
void heapSort (int a[], int n) {
	int i;
	for (i = n/2; i >= 0; i--)
		heapify(a, n, i);

	for (i = n - 1; i >= 0; i--) {
		swap(&a[0], &a[i]);
		heapify(a, i, 0);
	}
}

void merge (int a[], int l, int m, int r) {
	int n1 = m - l + 1, n2 = r - m, i, j, k;

	int L[n1], R[n2];
	for (i = 0; i < n1; i++)
		L[i] = a[l + i];
	for (j = 0; j < n2; j++)
		R[j] = a[m + j + 1];

	k = l;
	i = j = 0;

	while (i < n1 && j < n2) {
		if (L[i] <= R[j])
			a[k++] = L[i++];
		else a[k++] = R[j++];
	}
	while (i < n1)
		a[k++] = L[i++];
	while (j < n2)
		a[k++] = R[j++];
}

//O(NlogN)
void mergesort (int a[], int start, int end) {
	int m = (start+end)/2;

	if (start >= end) return;

	mergesort(a, start, m);
	mergesort(a, m+1, end);

	merge(a, start, m, end);
}

int partition (int a[], int low, int high) {
	int i, j = low;
	int pivot = a[high];
	for (i = low; i < high; i++)
		if (a[i] <= pivot)
			swap (&a[i], &a[j++]);
	swap (&a[j], &a[high]);
	return j;
}

//O(NlogN) avg.
void quicksort(int a[], int low, int high) {
	if (low >= high) return;

	int p = partition(a, low, high);
	quicksort(a, low, p-1);
	quicksort(a, p+1, high);
}

//O(N^2)
void insertionsort (int a[], int n) {
	int i,j, tmp;
	for (i = 1; i < n; i++) {
		tmp = a[i];
		for (j = i - 1; a[j] > tmp && j >= 0; j--)
				a[j+1] = a[j];
		a[j+1] = tmp;
	}
}

void display(int a[], int n) {
	int i;
	for (i = 0; i < n; i++)
		printf("%d ", a[i]);
}

int main() {
	int n, i, ch;
	printf("\nEnter size of array: ");
	scanf("%d", &n);
	clock_t begin, end;
	int a[n], b[n];
	printf("\nEnter the array: ");
	for (i = 0; i < n; i++)
		scanf("%d", &a[i]);
	printf("\n\n1 - Insertion sort\n2 - Shell sort\n3 - Merge Sort\n4 - Heap Sort\n5 - Quick Sort\n6 - Exit.");

	for (i = 0; i < n; i++)
		b[i] = a[i];
	begin = clock(); //Get start timestamp as clock_t. Difference of two clock_t's, divided by a scale factor gives elapsed time in microsec.
	insertionsort(b, n);
	end = clock(); //Get end timestamp after finishing insertionsort()
	printf("\n\nInsertion-sorted array: ");
	display(b, n);
	printf("\nExecution time: %lf ms", (double)(end-begin)*1000/CLOCKS_PER_SEC);

	for (i = 0; i < n; i++)
		b[i] = a[i];

	begin = clock();
	shellSort(b, n);
	end = clock();
	printf("\n\nShell-sorted array: ");
	display(b, n);
	printf("\nExecution time: %lf ms", (double)(end-begin)*1000/CLOCKS_PER_SEC);

	for (i = 0; i < n; i++)
		b[i] = a[i];

	begin = clock();
	mergesort(b, 0, n - 1);
	end = clock();
	printf("\n\nMerge-sorted array: ");
	display(b, n);
	printf("\nExecution time: %lf ms", (double)(end-begin)*1000/CLOCKS_PER_SEC);

	for (i = 0; i < n; i++)
		b[i] = a[i];

	begin = clock();
	heapSort(b, n);
	end = clock();
	printf("\n\nHeap-sorted array: ");
	display(b, n);
	printf("\nExecution time: %lf ms", (double)(end-begin)*1000/CLOCKS_PER_SEC);

		for (i = 0; i < n; i++)
			b[i] = a[i];

		begin = clock();
		quicksort(b, 0, n - 1);
		end = clock();
		printf("\n\nQuick-sorted array: ");
		display(b, n);
		printf("\nExecution time: %lf ms\n\n", (double)(end-begin)*1000/CLOCKS_PER_SEC);

	return 0;
}
