// code for a huffman coder


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "huff.h"
#include "bitfile.h"

const int MAX_INT = 2147483647;

// create a new huffcoder structure
struct huffcoder *  huffcoder_new() {
  // Creates return struct
  struct huffcoder * newHuffCoder;
  newHuffCoder = malloc(sizeof(struct huffcoder));

  // Initialises the arrays in the huffcoder struct
  int i;
  for (i = 0; i < NUM_CHARS; i++){
    newHuffCoder -> freqs[i] = 0;
    newHuffCoder -> code_lengths[i] = 0;
    newHuffCoder -> codes[i] = 0;
  }

  newHuffCoder -> tree = malloc(sizeof(struct huffchar));
  return newHuffCoder;
}

// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder * this, char * filename) {
  unsigned char c;

  // Opens the file from the command line
  FILE * file;
  file = fopen(filename, "r");
  assert(file != NULL);

  // Reads each character
  c = fgetc(file);
  while (!feof(file)) {
    // Increments the frequency for the current character
    this -> freqs[c]++;
    // Gets the next character
    c = fgetc(file);
  }

  fclose(file);

  // Ensures each frequency is set to at least 1
  int i;
  for (i = 0; i < NUM_CHARS; i++) {
    if (this -> freqs[i] == 0) {
      this -> freqs[i] = 1;
    }
  }
}

struct huffchar * makeCompound(struct huffchar * smallest1, struct huffchar * smallest2,
	int currentSeqNo) {
  // Creates the return object with the two smaller objects
  struct huffchar * returnCompound;
  returnCompound = malloc(sizeof(struct huffchar));

  returnCompound -> freq = smallest1 -> freq + smallest2 -> freq;
  returnCompound -> is_compound = 1;
  returnCompound -> seqno = currentSeqNo;  
  returnCompound -> u.compound.left = smallest1;
  returnCompound -> u.compound.right = smallest2;

  return returnCompound;
}

// Selection sort
void selectionSort(struct huffchar * array[], int n) { 
  int count1, count2;
  int currentMin;
  int sequenceNo;
  int minFrequency; 

  for (count1 = 0; count1 < 2; count1++) { 
      currentMin = -1;
      sequenceNo = MAX_INT;
      minFrequency = MAX_INT;

      // Loop through the list to find the two smallest elements
      for (count2 = count1; count2 < n ; count2++) {
        if ((array[count2] != NULL && array[count2] -> freq < minFrequency) ||
        (array[count2] != NULL && array[count2] -> freq == minFrequency &&
        array[count2] -> seqno < sequenceNo)) {
          sequenceNo = array[count2] -> seqno;
          minFrequency = array[count2] -> freq;
          currentMin = count2; 
        }
      }

   struct huffchar * temp;
   temp = array[count1];
   array[count1] = array[currentMin];
   array[currentMin] = temp;
  }
} 

// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this) {
  // Makes a frequency list
  struct huffchar ** characterList = malloc(sizeof(struct huffchar *) * NUM_CHARS);

  int i;
  for (i = 0; i < NUM_CHARS; i++) {
    characterList[i] = malloc(sizeof(struct huffchar));

    characterList[i] -> freq = this -> freqs[i];
    characterList[i] -> is_compound = 0; 
    characterList[i] -> seqno = i;
    characterList[i] -> u.compound.left = NULL;
    characterList[i] -> u.compound.right = NULL;
    characterList[i] -> c = i;
  }

  // Makes tree
  int currentSeqNo = NUM_CHARS;
  for (i = 0; i < NUM_CHARS -1 ; i++) {
    // Gets the two characters with the smallest frequencies and returns them from the list
    selectionSort(characterList, NUM_CHARS);
    struct huffchar * smallest1 = characterList[0]; 
    struct huffchar * smallest2 = characterList[1];

    currentSeqNo++;

    // Compounds the two smallest and adds it to the tree
    struct huffchar * compound = makeCompound(smallest1, smallest2, currentSeqNo);
    characterList[0] = compound;
    characterList[1] = NULL;
  }

  this -> tree = characterList[0];
}

// Recursive function which converts the tree into a table of codes
void tree2table_recursive(struct huffcoder * this, struct huffchar * node,
     unsigned long path, int depth) { 
  if (node -> is_compound) {
    // Adds a 0 to the end of the character code
    path = path << 1;
    // Calls the method for the left tree
    tree2table_recursive(this, node -> u.compound.left, path, depth + 1);

    // Adds a 1 to the end of the character code
    path = path | 1;
    // Calls the method for the right tree
    tree2table_recursive(this, node -> u.compound.right, path, depth + 1);
  } else {
    // Saves the path
    unsigned char index = node -> c;
    this -> codes[index] = path;
    // Saves the length of the character code
    this -> code_lengths[index] = depth;
  }
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder * this) {
  // Declares an array to store the path variables
  struct huffchar * root = this -> tree;
  unsigned long path = 0;
  int depth = 0;
  tree2table_recursive(this, root, path, depth);
}

// print the Huffman codes for each character in order
void huffcoder_print_codes(struct huffcoder * this)
{
  int i, j;
  char buffer[NUM_CHARS];

  for ( i = 0; i < NUM_CHARS; i++ ) {
    // put the code into a string
    assert(this->code_lengths[i] < NUM_CHARS);
    for (j = this->code_lengths[i]-1; j >= 0; j--) {
      buffer[this -> code_lengths[i] - j - 1] = ((this -> codes[i] >> j) & 1) + '0';
    }
    // don't forget to add a zero to end of string
    buffer[this -> code_lengths[i]] = '\0';

    // print the code
    printf("char: %d, freq: %d, code: %s\n", i, this->freqs[i], buffer);;
  }
}

unsigned char getCode(struct bitfile * this, struct huffchar * node) {
  int bitRead;

  // Checks to see if a character has not been found
  if (node -> is_compound) {
    bitRead = bitfile_read_bit(this);
    printf("%d", bitRead);

    if (bitRead == 0) {
      return getCode(this, node -> u.compound.left);
    } else if (bitRead == 1) {
      return getCode(this, node -> u.compound.right);
    }
  } else {
    return node -> c;
  }
}

// encode the input file and write the encoding to the output file
void huffcoder_encode(struct huffcoder * this, char * input_filename,
		      char * output_filename) {
  FILE * inputFile;
  inputFile = fopen(input_filename, "r");
  assert(inputFile != NULL);

  // Creates a bitfile for the output file
  struct bitfile * outputFile;
  outputFile = bitfile_open(output_filename, "w");

  // Reads each character
  char c;
  c = fgetc(inputFile);
  while (!feof(inputFile)) {
    // Writes the characters
    int j;
    for (j = this -> code_lengths[c] - 1; j >= 0; j--) {
      // Gets the characters
      int buffer = ((this -> codes[c] >> j) & 1);
      bitfile_write_bit(outputFile, buffer);
    }
    
    // Gets the next character
    c = fgetc(inputFile);
  }

  // Adds to the end of the file
  int counter;
  for (counter = this -> code_lengths[4] - 1; counter >= 0; counter--) {
    int buffer = ((this -> codes[4] >> counter) & 1);
    bitfile_write_bit(outputFile, buffer);
  }

  fclose(inputFile);
  bitfile_close(outputFile);
}

// decode the input file and write the decoding to the output file
void huffcoder_decode(struct huffcoder * this, char * input_filename,
		      char * output_filename) {
  struct bitfile * inputFile;
  inputFile = bitfile_open(input_filename, "r");

  FILE * outputFile;
  outputFile = fopen(output_filename, "w");
  assert(outputFile != NULL);

  int count = 0;
  unsigned char character;
  while (!bitfile_end_of_file(inputFile) && count < 10000) {
    printf(" %c\n", character);

    character = getCode(inputFile, this -> tree);
    if (character == 4) {
      inputFile -> is_EOF = 1;
      printf("\n\n\n***REACHED END OF FILE***\n\n\n");
    } else {
      if (fputc(character, outputFile) == 4) {
        inputFile -> is_EOF = 1;
      }
    }
    count++;
  }
  
  bitfile_close(inputFile);
  fclose(outputFile);
}