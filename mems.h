/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions 
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/
// add other headers as required
#include<stdio.h>
#include<stdlib.h>
#include <stdint.h> 
#include <sys/mman.h>



/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify this 
macro to make the output of all system same and conduct a fair evaluation. 
*/
#define PAGE_SIZE 4096

// struct Node {
//     int data;
//     struct Node* right;
//     struct Node* down;
// };
typedef struct SubChainNode {
    size_t size;
    void* VA;

    int type;  // 0 for HOLE, 1 for PROCESS
    struct SubChainNode* right;
    struct SubChainNode* left;
    void* mem_ptr; // MeMS physical address
} SubChainNode;

typedef struct MainChainNode {
    struct MainChainNode* up;
    struct MainChainNode* down;
    SubChainNode* sub_chain; // Doubly linked list for sub-chain
} MainChainNode;

/*
Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the free list
2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
3. any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing
Returns: Nothing
*/
MainChainNode* free_list_head = NULL;
void* mems_heap_start = NULL;
    

void mems_init(){
    
    int mems_virtual_address=0;
    free_list_head = (struct MainChainNode*)mmap(NULL, sizeof(struct MainChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (free_list_head == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    

    // Initialize the starting virtual address of the MeMS heap
    mems_heap_start = mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mems_heap_start == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

}


/*
This function will be called at the end of the MeMS system and its main job is to unmap the 
allocated memory using the munmap system call.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_finish(){
	MainChainNode* current_main = free_list_head;

    while (current_main != NULL) {
    SubChainNode* current_sub = current_main->sub_chain;

    while (current_sub != NULL) {
        if (current_sub->type == 1) {
            if (current_sub->mem_ptr != NULL) {
                if (munmap(current_sub->mem_ptr, current_sub->size) != 0) {
                    perror("munmap failed");
                    // Handle error gracefully or exit
                }
            }
        }

        SubChainNode* temp = current_sub;
        current_sub = current_sub->right;
        
        // Deallocate any dynamically allocated memory within SubChainNode
        // Example: if (temp->data != NULL) {
        //     free(temp->data);
        // }

        // Unmap the SubChainNode
        if (munmap(temp, sizeof(SubChainNode)) != 0) {
            perror("munmap failed");
            // Handle error gracefully or exit
        }
    }

    MainChainNode* temp_main = current_main;
    current_main = current_main->down;

    // Deallocate any dynamically allocated memory within MainChainNode
    // Example: if (temp_main->data != NULL) {
    //     free(temp_main->data);
    // }

    // Unmap the MainChainNode
    if (munmap(temp_main, sizeof(MainChainNode)) != 0) {
        perror("munmap failed");
        // Handle error gracefully or exit
    }
	}

    if (free_list_head != NULL) {
        munmap(free_list_head, sizeof(MainChainNode));
        free_list_head = NULL;
    }

    if (mems_heap_start != NULL) {
        munmap(mems_heap_start, sizeof(char));
        mems_heap_start = NULL;
    }   
}

/*
Allocates memory of the specified size by reusing a segment from the free list if 
a sufficiently large segment is available. 

Else, uses the mmap system call to allocate more memory on the heap and updates 
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from mapping
by adding it to the free list.
Parameter: The size of the memory the user program wants
Returns: MeMS Virtual address (that is created by MeMS)
*/ 


int give_vir(void* i) {
    int mems_of = 1000;
    MainChainNode* current_main = free_list_head;
    MainChainNode* prev_node = NULL;

    while (current_main != NULL) {
        SubChainNode* current_sub = current_main->sub_chain;
        SubChainNode* previous_sub = NULL;

        while (current_sub != NULL) {
            if (current_sub->mem_ptr == i) {
                int mems_addr = (mems_of);
                mems_of += current_sub->size;
                return mems_addr;
            }

            mems_of += current_sub->size;
            previous_sub = current_sub;
            current_sub = current_sub->right;
        }

        prev_node = current_main;
        current_main = current_main->down;
    }

    // If the provided 'i' was not found, you might want to return NULL or some other appropriate value.
    //return (int*) NULL;
}

void* give_vir2(void* i) {
    uintptr_t mems_of = 1000;
    MainChainNode* current_main = free_list_head;
    MainChainNode* prev_node = NULL;

    while (current_main != NULL) {
        SubChainNode* current_sub = current_main->sub_chain;
        SubChainNode* previous_sub = NULL;

        while (current_sub != NULL) {
            if (current_sub->mem_ptr == i) {
                void* mems_addr = (mems_of);
                mems_of += current_sub->size;
                return mems_addr;
            }

            mems_of += current_sub->size;
            previous_sub = current_sub;
            current_sub = current_sub->right;
        }

        prev_node = current_main;
        current_main = current_main->down;
    }

    // If the provided 'i' was not found, you might want to return NULL or some other appropriate value.
    return NULL;
}

int give_last(void*i){
    MainChainNode* current_main = free_list_head;
    MainChainNode* prev_node = NULL;

    while (current_main != NULL) {
        SubChainNode* current_sub = current_main->sub_chain;
        SubChainNode* previous_sub = NULL;

        while (current_sub != NULL) {
            if (current_sub->mem_ptr == i) {
            int lastpoint=give_vir(i)+current_sub->size;
            lastpoint--;
            return lastpoint;
            }

            
            previous_sub = current_sub;
            current_sub = current_sub->right;
        }

        prev_node = current_main;
        current_main = current_main->down;
    }
    

}


void* give_last2(void* i) {
    MainChainNode* current_main = free_list_head;
    MainChainNode* prev_node = NULL;

    while (current_main != NULL) {
        SubChainNode* current_sub = current_main->sub_chain;
        SubChainNode* previous_sub = NULL;

        while (current_sub != NULL) {
            if (current_sub->mem_ptr == i) {
                int lastpoint = give_vir(i) + current_sub->size;
                lastpoint--;
                return (void*) lastpoint;
            }

            previous_sub = current_sub;
            current_sub = current_sub->right;
        }

        prev_node = current_main;
        current_main = current_main->down;
    }

    // If 'i' is not found in the data structures, you should return something (e.g., NULL).
    return NULL;
}





size_t calcExistHoleSize() {
    size_t total_size = 0;
    MainChainNode* current_main = free_list_head;

    while (current_main != NULL) {
        SubChainNode* current_sub = current_main->sub_chain;

        while (current_sub != NULL) {
            if (current_sub->type == 0) {
                total_size += current_sub->size;
            }

            current_sub = current_sub->right;
        }

        current_main = current_main->down;
    }
    return total_size;
}
static int nopage=0;
static int pii=0;
static int lp = 0;
static uintptr_t mems_offset = 1000;

void* mems_malloc(size_t size) {
    size_t page_aligned_size =0;
    MainChainNode* current_main = free_list_head;
    MainChainNode* prev_node = NULL;

    while (current_main != NULL) {
        SubChainNode* current_sub = current_main->sub_chain;
        SubChainNode* previous_sub = NULL;

        while (current_sub != NULL) {
            if (current_sub->type == 0 && current_sub->size >= size) {
                // Found a HOLE segment with enough space
                if (current_sub->size > size) {
                    // Split the HOLE into a new HOLE and a PROCESS segment
                    void* mem_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                    void* mem_ptr2 = mmap(NULL, current_sub->size-size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                    SubChainNode* new_hole = (SubChainNode*)mmap(NULL, sizeof(SubChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                    new_hole->size = current_sub->size - size;
                    new_hole->type = 0;
                    new_hole->mem_ptr = mem_ptr2;
                    
                    

                    current_sub->size = size;
                    current_sub->type = 1; // Mark as PROCESS
                    current_sub->right = new_hole;
                    current_sub-> mem_ptr=mem_ptr;
                    new_hole->left = current_sub;


                    // Calculate the virtual address with the offset
                    
                    void* mems_addr = (void*)(mems_offset);
                    mems_offset += size;

                    // Update the virtual address in the data structure
    			current_sub->VA = mems_addr;
                    return mems_addr;
                  
                } else {
                    void* mem_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                    // The HOLE is exactly the right size, so use it as is
                    current_sub->type = 1; // Mark as PROCESS
                    current_sub -> mem_ptr=mem_ptr;

                    // Calculate the virtual address with the offset
                   
                    void* mems_addr = (void*)(mems_offset);
                    current_sub->VA = mems_addr;
                     mems_offset += size;
                    return mems_addr;
                }
            }

            previous_sub = current_sub;
            current_sub = current_sub->right;
        }

        prev_node = current_main;
        current_main = current_main->down;
        
    }
    pii++;
    if (pii>1){
        mems_offset += calcExistHoleSize();
        mems_offset -= lp;
        lp+=calcExistHoleSize();
       }
    int i=1;
    while(size>=(i*PAGE_SIZE)){
        i++;
    }
    nopage+=i;
    page_aligned_size = i*PAGE_SIZE;


    // If no suitable segment was found, request memory from the OS using mmap
    void* mem_ptr = mmap(NULL,size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
     void* mem_ptr2 = mmap(NULL, page_aligned_size-size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mem_ptr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    
    

    // Create a new main chain node and its sub-chain node for the allocated memory
     MainChainNode* new_main_node = (MainChainNode*)mmap(NULL, sizeof(MainChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    SubChainNode* new_process_node = (SubChainNode*)mmap(NULL, sizeof(SubChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    SubChainNode* new_hole_node = (SubChainNode*)mmap(NULL, sizeof(SubChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    new_main_node->sub_chain = new_process_node;
    new_process_node->size = size;
    new_process_node->type = 1; // Mark as PROCESS
    new_process_node->mem_ptr = mem_ptr;
    //void* mem_ptr2 =(char*)new_process_node->mem_ptr+size;
    new_hole_node->size = page_aligned_size - size;
    new_hole_node->type = 0;
    new_hole_node->mem_ptr = mem_ptr2;

    // Update pointers in the sub-chain
    new_process_node->right = new_hole_node;
    new_process_node->left = NULL;
    new_hole_node->right = NULL;
    new_hole_node->left = new_process_node;

    // Update pointers in the main chain
    new_main_node->down = NULL;
    if (prev_node != NULL) {
        prev_node->down = new_main_node;
    } else {
        free_list_head = new_main_node;
    }

    // Calculate the virtual address with the offset
   // mems_offset+=calcExistHoleSize();
    
    
    void* mems_addr = (void*)(mems_offset);
    new_process_node->VA = mems_addr;
    mems_offset += size;
    new_hole_node->VA=mems_offset;
    return mems_addr;
    

}




/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not used.
3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing
Returns: Nothing but should print the necessary information on STDOUT
*/
void mems_print_stats() {
    // Variables to keep track of statistics
    int MainChainLength = 0;
    size_t totalUnusedMemory = 0;
    int arr[1000000];
    int index=0;
    int count=0;
    MainChainNode* currentMain = free_list_head;

    while (currentMain != NULL) {
        currentMain = currentMain->down;
        if (currentMain==NULL){
            break;
        }

        int d=0;
        SubChainNode* previous_sub = NULL;
        SubChainNode* currentSub = currentMain->sub_chain;
        SubChainNode* currentSub1 = currentMain->sub_chain;
        while (currentSub1 != NULL) {
            previous_sub=currentSub1;
            currentSub1 = currentSub1->right;
           
        
        }
        
        printf("MAIN[%u,%u] -> ", give_vir(currentSub->mem_ptr), give_last(previous_sub->mem_ptr));
        while (currentSub != NULL) {
            if (currentSub->type == 1) {
                printf("P[%u,%u] <-> ", give_vir(currentSub->mem_ptr),give_last(currentSub -> mem_ptr));
            }else{
                printf("H[%u,%u] <-> ", give_vir(currentSub->mem_ptr),give_last(currentSub -> mem_ptr));
            }
            
            if (currentSub->type == 0) {
                // Calculate unused memory
                totalUnusedMemory += currentSub->size;

            }
            count++;
            
           
            currentSub = currentSub->right;
        }
    
        printf("NULL \n");
        arr[index]=count;
        
        index++;
        count=0;
        MainChainLength++;
        
    }

    // Print the overall statistics
    printf("Total pages used: %d\n", nopage);
    printf("Total Unused Memory (in bytes): %zu\n", totalUnusedMemory);
    printf("Main chain length: %d\n", MainChainLength);
    printf("Sub-chain Length arr:");
    printf("[");
    for (int u=0;u<index;u++){
        printf("%d",arr[u]);
        printf(",");
    }
    printf("]\n");

}


/*
Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
*/
void* mems_get(void* v_ptr) {
    // Start from the free_list_head
    MainChainNode* mainC_node = free_list_head;
    
    int i = 0;
    while (mainC_node != NULL) {
        mainC_node= mainC_node->down;
        if (mainC_node==NULL){
            break;
        }
        SubChainNode* sub_chain_node = mainC_node->sub_chain;
        while (sub_chain_node != NULL) {
            
            if (sub_chain_node->VA <= v_ptr && sub_chain_node->right->VA > v_ptr){
                
                int diff = v_ptr - sub_chain_node->VA;
                
                return sub_chain_node->mem_ptr + diff ;
            }
            sub_chain_node = sub_chain_node->right;
            i++;
        }

        
    }

    // MeMS virtual address not found, return NULL
    return NULL;
}





/*
this function free up the memory pointed by our virtual_address and add it to the free list
Parameter: MeMS Virtual address (that is created by MeMS) 
Returns: nothing
*/
void mems_free(void *v_ptr) {
    MainChainNode* currentMain = free_list_head;

    while (currentMain != NULL) {
        SubChainNode* currentSub = currentMain->sub_chain;

        while (currentSub != NULL) {
            if (currentSub->VA == v_ptr && currentSub->type == 1) {
                // Found the matching MeMS virtual address with type PROCESS

                // Mark the sub-chain node as HOLE
                currentSub->type = 0; // Mark as HOLE
                int flag = 0;

                // Merge with the previous HOLE if it exists
                if (currentSub->left != NULL && currentSub->left->type == 0) {
                    currentSub->size += currentSub->left->size;
                    if (currentSub->left->left != NULL) {
                        currentSub->left->left->right = currentSub;
                        currentSub->left = currentSub->left->left;
                    } else {
                        currentMain->sub_chain = currentSub;
                        currentSub->left = NULL;
                    }

                    flag = 1;

                    // Remove the current node from the sub-chain
                    free(currentSub->left); // Free the current node
                }

                // Merge with the next HOLE if it exists
                if (currentSub->right != NULL && currentSub->right->type == 0) {
                    currentSub->size += currentSub->right->size;
                    if (currentSub->right->right != NULL) {
                        currentSub->right->right->left = currentSub;
                        currentSub->right = currentSub->right->right;
                    } else {
                        currentSub->right = NULL;
                    } // Update right pointer
                    flag = 1;

                    // Remove the next node from the sub-chain
                    free(currentSub->right); // Free the next node
                }

                // Exit the loop if we found and processed the node
                break;
            }

            currentSub = currentSub->right;
        }

        currentMain = currentMain->down;
    }
}
