#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct bitset {
    unsigned int * bits;
    int size_in_bits;
    int size_in_words;
    int bits_per_word;
};

// create a new, empty bit vector set with a universe of 'size' items
struct bitset * bitset_new(int size) {
	struct bitset * result;
    int size_in_words, bits_per_word;

    result = malloc(sizeof(struct bitset));

    bits_per_word = sizeof(unsigned int) * 8;
    size_in_words = size / bits_per_word;

	if (size % bits_per_word != 0) {
        size_in_words++;
    }

    result -> bits = malloc(bits_per_word * size_in_words);

    for (int i = 0; i < size_in_words; i++) {
        result -> bits[i] = 0;
    }

    result -> bits_per_word = bits_per_word;
    result -> size_in_bits = size_in_words * bits_per_word;
    result -> size_in_words = size_in_words;

    return result;
}

// set a specified bit in a given bitset
int set_bit(struct bitset * this, int k) {
	int word_index = k / this -> bits_per_word;
	int bit_index = k % this -> bits_per_word;

	unsigned int mask = 1;
	mask = mask << bit_index;

	this -> bits[word_index] = this -> bits[word_index] | mask;
}

// get the size of the universe of items that could be stored in the set
int bitset_size(struct bitset * this) {
	int size = 0;
	size = this -> size_in_words * this -> bits_per_word;
	return size;
}

// get the number of items that are stored in the set
int bitset_cardinality(struct bitset * this) {
	int cardinality = 0;

	int currentItem = 0;

	for (currentItem; currentItem < bitset_size(this); currentItem++) {
		int word_index = currentItem / this -> bits_per_word;
		int bit_index = currentItem % this -> bits_per_word;

		unsigned int mask = 1;

		mask = mask << bit_index;

		if (this -> bits[word_index] & mask) {
			cardinality++;
		}
	}
	
	return cardinality;
}

// check to see if an item is in the set
int bitset_lookup(struct bitset * this, int item) {
	int word_index = item / this -> bits_per_word;
	int bit_index = item % this -> bits_per_word;
	unsigned int mask = 1;

	mask = mask << bit_index;

	if (this -> bits[word_index] & mask) {
		return 1;
	} else {
		return 0;
	}
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set
int bitset_add(struct bitset * this, int item) {
	assert(item >= 0 && item < this -> size_in_bits);

	if (!bitset_lookup(this, item)) {
		int word_index = item / this -> bits_per_word;

    	this -> bits[word_index] = set_bit(this, item);
	}
}

// remove an item with number 'item' from the set
int bitset_remove(struct bitset * this, int item) {
	int word_index = item / this -> bits_per_word;
	int bit_index = item % this -> bits_per_word;
	unsigned int mask = 1;

	mask = mask << bit_index;
	mask = ~mask;

	this -> bits[word_index] = this -> bits[word_index] & mask;
}

// place the union of src1 and src2 into dest
struct bitset * bitset_union(struct bitset * dest, struct bitset * src1, struct bitset * src2) {
    assert(src1 -> size_in_bits == src2 -> size_in_bits);

    for (int i = 0; i < src1 -> size_in_words; i++) {
        dest -> bits[i] = src1 -> bits[i] | src2 -> bits[i];
    }
}

// place the intersection of src1 and src2 into dest
void bitset_intersect(struct bitset * dest, struct bitset * src1, struct bitset * src2) {
	assert(src1 -> size_in_bits == src2 -> size_in_bits);

    for (int i = 0; i < src1 -> size_in_words; i++) {
        dest -> bits[i] = src1 -> bits[i] & src2 -> bits[i];
    }
}

/* Test methods */
// print the contents of the bitset
void bitset_print(struct bitset * this) {
  int i;
  int size = bitset_size(this);

  for (i = 0; i < size; i++) {
    if (bitset_lookup(this, i) == 1) {
      printf("%c ", i);
    }
  }

  printf("\n");
}
  
// add the characters from a string to a bitset
void add_chars_to_set(struct bitset * this, char * s) {
  int i;

  for (i = 0; s[i] != 0; i++) {
    unsigned char temp = s[i];
    bitset_add(this, temp);
  }
}

// small routine to test a bitset
void mytest() {
  struct bitset * a = bitset_new(256);
  struct bitset * b = bitset_new(256);
  struct bitset * c = bitset_new(256);
  char * string1 = "What can you hear";
  char * string2 = "Nothing but the rain";

  add_chars_to_set(a, string1);
  add_chars_to_set(b, string2);

  // print the contents of the sets
  printf("Bitset A: \n");
  bitset_print(a);
  printf("Bitset B: \n");
  bitset_print(b);

  // compute and print the union of sets
  bitset_union(c, a, b);
  printf("Union: \n");
  bitset_print(c);

  // compute and print the intersection of sets
  bitset_intersect(c, a, b);
  printf("Intersection: \n");
  bitset_print(c);
}

int main() {
	mytest();
	return 0;
}