#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "bitset.h"

/*
struct bitset {
  unsigned int * bits;
  int size_in_bits;
  int size_in_words;
  int bits_in_word;
};
*/

// create a new, empty bit vector set with a universe of 'size' items
struct bitset * bitset_new(int size) {
  struct bitset * result;
  int size_in_words, bits_in_word;

  result = malloc(sizeof(struct bitset));

  bits_in_word = sizeof(unsigned int) * 8;
  size_in_words = size / bits_in_word;

  if (size % bits_in_word != 0) {
    size_in_words++;
  }

  result -> data = malloc(bits_in_word * size_in_words);

  for (int i = 0; i < size_in_words; i++) {
    result -> data[i] = 0;
  }

  result -> bits_in_word = bits_in_word;
  result -> size_in_bits = size_in_words * bits_in_word;
  result -> size_in_words = size_in_words;

  return result;
}

// set a specified bit in a given bitset
void set_bit(struct bitset * this, int k) {
  int word_index = k / this -> bits_in_word;
  int bit_index = k % this -> bits_in_word;

  unsigned int mask = 1;
  mask = mask << bit_index;

  this -> data[word_index] = this -> data[word_index] | mask;
}

// get the size of the universe of items that could be stored in the set
int bitset_size(struct bitset * this) {
  int size = 0;
  size = this -> size_in_words * this -> bits_in_word;
  return size;
}

// get the number of items that are stored in the set
int bitset_cardinality(struct bitset * this) {
  int cardinality = 0;

  int currentItem = 0;

  for (currentItem; currentItem < bitset_size(this); currentItem++) {
    int word_index = currentItem / this -> bits_in_word;
    int bit_index = currentItem % this -> bits_in_word;

    unsigned int mask = 1;

    mask = mask << bit_index;

    if (this -> data[word_index] & mask) {
      cardinality++;
    }
  }
  
  return cardinality;
}

// check to see if an item is in the set
int bitset_lookup(struct bitset * this, int item) {
  int word_index = item / this -> bits_in_word;
  int bit_index = item % this -> bits_in_word;
  unsigned int mask = 1;

  mask = mask << bit_index;

  if (this -> data[word_index] & mask) {
    return 1;
  } else {
    return 0;
  }
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set
void bitset_add(struct bitset * this, int item) {
  assert(item >= 0 && item < this -> size_in_bits);

  if (!bitset_lookup(this, item)) {
    //int word_index = item / this -> bits_in_word;

    //this -> data[word_index] = set_bit(this, item);
    set_bit(this, item);
  }
}

// remove an item with number 'item' from the set
void bitset_remove(struct bitset * this, int item) {
  int word_index = item / this -> bits_in_word;
  int bit_index = item % this -> bits_in_word;
  unsigned int mask = 1;

  mask = mask << bit_index;
  mask = ~mask;

  this -> data[word_index] = this -> data[word_index] & mask;
}

// place the union of src1 and src2 into dest
void bitset_union(struct bitset * dest, struct bitset * src1, struct bitset * src2) {
  assert(src1 -> size_in_bits == src2 -> size_in_bits);

  for (int i = 0; i < src1 -> size_in_words; i++) {
    dest -> data[i] = src1 -> data[i] | src2 -> data[i];
  }
}

// place the intersection of src1 and src2 into dest
void bitset_intersect(struct bitset * dest, struct bitset * src1, struct bitset * src2) {
  assert(src1 -> size_in_bits == src2 -> size_in_bits);

  for (int i = 0; i < src1 -> size_in_words; i++) {
    dest -> data[i] = src1 -> data[i] & src2 -> data[i];
  }
}

/* Test methods */
// print the contents of the bitset
void bitset_print(struct bitset * this) {
  int i;
  int size = bitset_size(this);

  for (i = 0; i < size; i++) {
    if (bitset_lookup(this, i) == 1) {
      printf("%d ", i);
    }
  }

  printf("\n");
}