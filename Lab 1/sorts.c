/* Program that reads in a text file of integers
   and outputs the numbers in sorted order. */

#include <stdio.h>
#include <stdlib.h>

/* find smallest element between start and end of array */
int findMin(int a[], int size, int start) {
  int min = a[start];
  int result = start;
  int i;

  for ( i = start; i < size; i++ ) {
    if ( a[i] < min ) {
      min = a[i];
      result = i;
    }
  }
  return result;
}

/* selection sort algorithm: repeatedly find smallest
   element and place at start of unsorted section. */
static void selectionSort(int a[], int size) {

}

/* insertion sort algorithm: place unsorted array
   elements into the correct location in an initially
   empty sorted part.*/
void insertionSort(int a[], int size) {

}

/* bubble sort algorithm: repeatedly compare and swap
   adjacent array elements. */
void bubbleSort(int a[], int size) {

}

/* funny sort algorithm: like bubble sort only we
   go back to the start every time we find a pair
   out of order. */
void funnySort(int a[], int size) {
  int j, temp;

  j = 0;
  while ( j < size - 1 ) {
    if ( a[j] > a[j+1] ) {
      temp = a[j];
      a[j] = a[j+1];
      a[j+1] = temp;
      j = 0;
    }
    else {
      j++;
    }
  }
}

/* Open a file. Abort progam if file cannot be opened */
FILE * open_file(char filename[])
{
  FILE * file;

  file = fopen(filename, "r");
  if ( file == NULL ) {
    printf("FATAL: Error opening file %s. Aborting program.\n", filename);
    exit(1);
  }

  return file;
}

/* read a stream of up to 'size' integers from a text file.
   return number of integers */
int read_in(int a[], int size, char filename[])
{
  const int max_line = 1024;
  char line[max_line];
  int i;
  FILE * file;
  char * eof;

  file = open_file(filename);

  /* read in the ints - one per line */
  i = 0;
  eof = fgets(line, max_line, file);
  while ( eof != NULL && i < size ) {     /* eof == NULL => end of file */
    sscanf(line, "%d", &(a[i]));
    i++;
    eof = fgets(line, max_line, file);
  }
  fclose(file);
  return i;
}

/* write out an array of integers up to 'size' */
void write_out(int a[], int size)
{
  int i;

  for ( i = 0; i < size; i++ ) {
    printf("%d\n", a[i]);
  }
}


/* read in a file of numbers, sort them, and
   write them out to the screen */
int main()
{
  const int size = 1024;
  int nums[size];
  int nnums;

  nnums = read_in(nums, size, "numbers.txt");
  funnySort(nums, nnums);
  write_out(nums, nnums);
  /* flush output buffer, so all output appears on screen */
  fflush(stdout);

  return 0;
}
