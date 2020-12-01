// C code file for  a file ADT where we can read a single bit at a
// time, or write a single bit at a time

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "bitfile.h"

// open a bit file in "r" (read) mode or "w" (write) mode
struct bitfile * bitfile_open(char * filename, char * mode) {
	struct bitfile * returnBitfile;
	returnBitfile = malloc(sizeof(struct bitfile));

	if (strcmp(mode,"r") == 0) {
		returnBitfile -> is_read_mode = 1;
	} else if (strcmp(mode,"w") != 0) {
		returnBitfile -> is_read_mode = 0;
	}
	
	// Sets the file to be stored in the bitfile
	returnBitfile -> file = fopen(filename, mode);
	// Checks if a file has actually been passed
	assert(returnBitfile -> file != NULL);
	
	returnBitfile -> index = 0;
	returnBitfile -> buffer = 0;
	returnBitfile -> is_EOF = 0;

	return returnBitfile;
}

// write a bit to a file; the file must have been opened in write mode
void bitfile_write_bit(struct bitfile * this, int bit) {
	if (this -> is_read_mode != 0) {
		fprintf(stderr, "ERROR: The file must be in write mode\n\n");
		assert(0);
	} else {
		// Only writes in bytes
		if (!feof(this -> file)) {
			// Adds a 0
			this -> buffer = this -> buffer << 1;
			if (bit == 1) {
				// Adds a 1
				this -> buffer = this -> buffer | 1;
			}
			this -> index++;
			printf("%d", bit);

			if ((this -> index % 8 == 0) && (this -> index != 0)) {
				// Flushes buffer to the file
				printf("\n%d\n\n", this -> buffer);
				fputc(this -> buffer, this -> file);

				// Resets buffer
				this -> buffer = 0;
			} 
		}
	}
}

// read a bit from a file; the file must have been opened in read mode
int bitfile_read_bit(struct bitfile * this) {
	if (this -> is_read_mode != 1) {
		fprintf(stderr, "ERROR: The file must be in read mode\n\n");
		assert(0);
	} else {
		// Returns the read bit
		int currentIndex = this -> index;

		// Gets a new buffer if required
		if (this -> index % 8 == 0) {
			// Returns a new buffer
			this -> buffer = fgetc(this -> file);
		}

		int bit = 7 - (currentIndex % 8);
		int temp = this -> buffer;
		int returnValue = (temp >> bit) & 1;
		this -> index++;

		return returnValue;
	}
}

// close a bitfile; flush any partially-filled buffer if file is open
// in write mode
void bitfile_close(struct bitfile * this) {
	if (!this -> is_read_mode) {
		// Checks if the buffer needs to be shifted to the left
		int leftShiftNo = (8 - (this -> index % 8));
		unsigned char temp = this -> buffer;
		temp = temp << leftShiftNo;

		// Adds buffer to the file
		fputc(temp, this -> file);

		printf("Buffer:\n");
		printf("\n%d", temp);
	}

	// Refreshes buffer
	this -> buffer = 0;
	// Closes file 
	fclose(this -> file);
}

// check for end of file
int bitfile_end_of_file(struct bitfile * this) {
	return this -> is_EOF;
}