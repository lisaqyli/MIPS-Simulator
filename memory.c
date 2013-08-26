#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"


/* Pointer to simulator memory */
uint8_t *mem;
/* Create variable to keep track of heap status here 
   You may change this to use your own implementation. */
uint8_t *heap_status;


/* Called by program loader to initialize memory. */
uint8_t* init_mem() {
  assert (mem == NULL);
  mem = (uint8_t*) calloc(MEM_SIZE, sizeof(uint8_t)); // allocate zeroed memory
  if (mem==NULL) fprintf(stderr, "%s: Allocation Failed\n", __FUNCTION__);

  //you may change the following line if using your own implementation to maintain the heap
  heap_status = (uint8_t*) init_heap_status();
  return (uint8_t*) mem;
}

/* Called in init_mem() to initialize your implementation of
   how you keep track of heap allocation status.*/
uint8_t* init_heap_status(){

  heap_status = (uint8_t*) calloc((HEAP_TOP - HEAP_BOTTOM), sizeof(uint8_t)); //allocate zeroed memo
  if (heap_status == NULL) fprintf(stderr, "%s, ALLocation Failed\n", __FUNCTION__);
  return heap_status;
}

/* Returns 1 if memory access is ok, otherwise returns 0. 
The parameter write_permission is the macro READ if this is
a read request, and WRITE if it is a write request. See memory.h. */

int access_ok(uint32_t mipsaddr, mem_unit_t size, uint8_t write_permission) {
   
   uint8_t* hs_pointer = heap_status;    
  /* TODO YOUR CODE HERE */
  
  if (write_permission == READ) {
    if (mipsaddr < CODE_BOTTOM || mipsaddr % size != 0 || mipsaddr >= MEM_SIZE )
      return 0;   
  
    if (*(hs_pointer + mipsaddr) == 0) {
	     return 0;
    } 
  } else {
    
    if (mipsaddr < CODE_TOP || mipsaddr % size != 0 || mipsaddr >= MEM_SIZE)
      return 0;
    if (*(hs_pointer + mipsaddr) == 0) {
      return 0;
  } 

  }

  return 1;
  
}

/* Writes size bytes of value into mips memory at mipsaddr */
void store_mem(uint32_t mipsaddr, mem_unit_t size, uint32_t value) {
  if (!access_ok(mipsaddr, size, WRITE)) {
    fprintf(stderr, "%s: bad write=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  }
  
  int i;
  for (i = 0; i < size; i++) {
    *(mem + mipsaddr + i) = value & 0X000000FF; //do I need to cast here???
    value = value >> 8;   // what if size is too big and not enough value??


  }
  /*
   int little = 0;
  while (little < size) {
      *(mem + mipsaddr + little) = (uint8_t)(value & 0xFF);
      little += 1;
      value = value >> 8;
  }
  */
  
  /* TODO YOUR CODE HERE */


}

/* Returns zero-extended value from mips memory */
uint32_t load_mem(uint32_t mipsaddr, mem_unit_t size) {

  if (!access_ok(mipsaddr, size, READ)) {
    fprintf(stderr, "%s: bad read=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  }
  /*
   int shift = 0;
  int value = 0;
  while (shift < size) {
      value += ((uint32_t) *(mem + mipsaddr + shift) << (shift * 8));
      shift += 1;
  }
  return value;
  */

  if (size  == SIZE_BYTE) {
    return *(uint8_t*)(mem + mipsaddr);
  }
  if (size == SIZE_HALF_WORD) {
    return *(uint16_t*)(mem + mipsaddr);
  }
 
    return *(uint32_t*)(mem+mipsaddr);

 
  
  /* TODO YOUR CODE HERE */

  // incomplete stub to let mipscode/simple execute
  // (only handles size == SIZE_WORD correctly)
  // feel free to delete and implement your own way
}

/*Uses the first fit algorithm to allocate a block of size bytes 
in the heap. Returns the address of the start of the block 
if allocation is successful, returns 0 on failure.*/

uint32_t first_fit_malloc(uint32_t size){
  /* TODO YOUR CODE HERE */
  uint8_t *hs_pointer = heap_status;
  int i;
  for (i = 0; i < (HEAP_TOP - HEAP_BOTTOM); i = i+4) { //int i might cause prob here, coz comparing int with uint32
    if (*(hs_pointer+i) != 0) { //in if statement, does pointer increment???
    } else {
      if (check_heap_size(size, hs_pointer+i) == 0)  {
      } else {
        *(hs_pointer+i) = 2; //the beginning of a new block is 2.
        int k;
        for (k = 1; k < size; k++) {   //assign every of the memo addr to 1, meaning they are occupied
          *(hs_pointer+i+k) = 1;
        }
      return  HEAP_BOTTOM +  i;
      }
    }
  }
  //NOT YET IMPLEMENTED, YOU MAY NEED TO CHANGE RETURN VALUE
  return 0;
}

/*check if that block of heap is free to fit, used only for first_fit malloc()*/
int check_heap_size(uint32_t size, uint8_t* hs_pointer) {
  int j;
  for (j = 0; j < size; j++) {
    if (*(hs_pointer + j) != 0)
      return 0;
  }
  return 1; 
}

/* check if th address in the heap is free. return true if free, false otherwise. */
/**
int check_heap_free(uint32_t addr) {
  uint8_t* hs_pointer = heap_status;
  if (*(hs_pointer + (addr - HEAP_BOTTOM)) == 0) {
    return 1;
  }
  return 0;
  }
**/
/* check if the adress is the beginning of a block that was allocated on the help。 
bool check_heap_begin(uint32_t addr) (
  uint8_t* hs_pointer =  heap_status;
  if ((*(hs_pointer-1) != 0) && (*hs_pointer ==0)) {
    return true;
  }
  return false;
  )
  */

/*Free the allocated block of memory in the heap beginning at
mipsaddr. In order to successfully free a block, mipsaddr must 
be the beginning of a block that was allocated on the heap. 
If block_free() is called on an unallocated memory address or an address
that is not the beginning of a block, bad_free() should be called.
*/

void block_free(uint32_t mipsaddr){
  /* TODO YOUR CODE HERE */
  if (*(heap_status + mipsaddr - HEAP_BOTTOM) != 2)
    bad_free(mipsaddr);
  *(heap_status +mipsaddr - HEAP_BOTTOM) = 0;
  mipsaddr++;
  while (*(heap_status + mipsaddr - HEAP_BOTTOM) ==1) {
  *(heap_status + mipsaddr - HEAP_BOTTOM) = 0;
  mipsaddr++;
  }
 
}



/*Implementation of this function is optional but may be useful for debugging.
  This function will not be graded and is for your benefit only. To call this 
  function in MIPS, use syscall with $v0=64.*/

void print_heap_status(){
  //YOUR CODE HERE
  //OPTIONAL FOR DEBUGGING
}

/*Called when trying to free an invalid address. Do NOT
  change this function.*/
void bad_free(uint32_t mipsaddr){
  fprintf(stderr, "%s: bad free=%08x\n", __FUNCTION__, mipsaddr);
  exit(-1);
}


/**Extra Credit**/
/*Wait until completing the rest of the project to work on this part; it
  requires significantly more work than first_fit_malloc().
  You may need to declare new variables and/or modify functions you 
  have already completed.*/


uint32_t next_fit_malloc(uint32_t size){
  /* TODO YOUR CODE HERE */
  return 0;
}


