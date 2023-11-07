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
### List of Assumptions:

1. We reassign the value of MEMS virtual address every time a process node becomes free to cover all possible edge cases that may arise.

2. We do not set the total number of pages to 0 after using mems finish since mems finish is used at the end to deallocate memory. Therefore, the pages used by our code in one run will remain the same.
Introduction:

3. Made some additional functions for better code flow. 

The MeMS system manages memory allocation and deallocation within a virtual address space.
It maintains a free list of memory segments (HOLEs) and allocates memory when requested by user programs.

----------------------------------------------------------------------------------------------------
### Github Repository
https://github.com/Niketiiitd/OS_ASS3.git

----------------------------------------------------------------------------------------------------

### Approach :

1. mems_init() initializes the MeMS system. It initializes the free list and the starting virtual address for the MeMS heap using the mmap system call.

2. mems_finish() is responsible for cleaning up the MeMS system. It unmaps all allocated memory regions using the munmap system call, including MainChainNodes and SubChainNodes. It also unmaps the free list head and the MeMS heap starting address.

3. mems_malloc(size_t size) allocates memory of the specified size using the MeMS system. It searches the free list for a suitable hole segment with enough space. If a suitable hole is found, it can split the hole into a PROCESS segment and a new HOLE segment. If not, it requests memory from the operating system using mmap and creates a new MainChainNode and SubChainNodes to manage the allocated memory. It returns the MeMS virtual address for the allocated memory.

4. mems_print_stats() prints statistics about the MeMS system. It displays the total number of pages used, the total unused memory (memory in the free list), the length of the main chain, and the lengths of each sub-chain.

5. mems_get(void* v_ptr) takes a MeMS virtual address as input and returns the corresponding MeMS physical address by searching the MeMS data structures. It iterates through the MainChainNodes and SubChainNodes to find the match and calculate the offset.

6.mems_free(void *v_ptr) frees the memory associated with a given MeMS virtual address. It marks the corresponding SubChainNode as a HOLE and may merge adjacent HOLEs if they exist.

----------------------------------------------------------------------------------------------------
## Error Handling 
1. If the mmap and unmap fails to execute , then the code prints an error message.
2. Handle cases when the required node is not found while traversing the free list


