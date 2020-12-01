/* bloom.h header file for a simple Bloom filter for approximate
   storage of sets of strings

   Author: David Gregg
*/

#ifndef BLOOM_H
#define BLOOM_H

#include "bitset.h"

struct bloom {
  struct bitset * bit_vector;
  int size;
};

extern const int BLOOM_HASH1;
extern const int BLOOM_HASH2;

// create a new, empty Bloom filter with 'size' slots
struct bloom * bloom_new(int size);

// check to see if a string is in the set
int bloom_lookup(struct bloom * this, char * item);

// add a string to the set has no effect if the item is already in the
// set; note that you cannot safely remove items from a Bloom filter
void bloom_add(struct bloom * this, char * item);

// place the union of src1 and src2 into dest
void bloom_union(struct bloom * dest, struct bloom * src1,
		  struct bloom * src2);

// place the intersection of src1 and src2 into dest
void bloom_intersect(struct bloom * dest, struct bloom * src1,
                  struct bloom * src2);

// print out the bits that are 1 in the bit vector of
// a bloom filter
void bloom_print(struct bloom * this);


#endif // BLOOM_H
