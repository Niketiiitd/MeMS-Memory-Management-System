# MeMS: Memory Management System [CSE231 OS Assignment 3]
[Documentation](https://docs.google.com/document/d/1Gs9kC3187lLrinvK1SueTc8dHCJ0QP43eRlrCRlXiCY/edit?usp=sharing)
---

### How to run the exmaple.c
After implementing functions in mems.h follow the below steps to run example.c file
```
$ make
$ ./example
```
----------------------------------------------------------------------------------------------------
List of Assumptions:

1. We reassign the value of MEMS virtual address every time a process node becomes free to cover all possible edge cases that may arise.

2. We do not set the total number of pages to 0 after using mems finish since mems finish is used at the end to deallocate memory. Therefore, the pages used by our code in one run will remain the same.
Introduction:

3. Made some new functions for clear understanding of code. 

The MeMS system manages memory allocation and deallocation within a virtual address space.
It maintains a free list of memory segments (HOLEs) and allocates memory when requested by user programs.

----------------------------------------------------------------------------------------------------
Function: mems_init()

Description:
Initializes the MeMS system, creating the initial data structures.\n
Input:
None.
Output:
None.
---------------------------------------------------------------------------------------------------
Function: mems_finish()

Description:
Releases all memory used by the MeMS system and performs cleanup.
Input:
None.
Output:
None.
---------------------------------------------------------------------------------------------------
Function: mems_malloc(size_t size)

Description:
Allocates memory of the specified size in the MeMS system. If no suitable segment is found in the free list, it requests memory from the OS.
Input:
size (type: size_t) - The size of memory to allocate.
Output:
MeMS virtual address.
---------------------------------------------------------------------------------------------------
Function: mems_print_stats()

Description:
Prints statistics about the MeMS system, including the number of pages used, unused memory, and details of nodes in the main chain and sub-chain.
Input:
None.
Output:
None (prints information to the standard output).
---------------------------------------------------------------------------------------------------
Function: mems_get(void* v_ptr)

Description:
Returns the MeMS physical address mapped to a MeMS virtual address.
Input:
v_ptr (type: void*) - MeMS virtual address.
Output:
MeMS physical address.
---------------------------------------------------------------------------------------------------
Function: mems_free(void* v_ptr)

Description:
Frees memory pointed to by a MeMS virtual address and adds it to the free list. Merges adjacent HOLEs for efficiency.
Input:
v_ptr (type: void*) - MeMS virtual address to be freed.
Output:
None.
---------------------------------------------------------------------------------------------------
Global Variables:

Detail the global variables utilized in your code, such as free_list_head and mems_heap_start, explaining their significance.
---------------------------------------------------------------------------------------------------


