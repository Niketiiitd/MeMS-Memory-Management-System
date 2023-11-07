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



