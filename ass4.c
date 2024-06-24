#include <stdio.h>
#include <stdlib.h>

// global variables
int pm_size; 	// size of physical memory
int remaining;	// to store remaining memory

// decalare global variables including linked list of blocks containing
//id, starting address, ending address, link to next node in list
struct node{
	int id;
	int startAddress,endAddress;
	struct node* next;

}*block = NULL;

typedef struct node block_type;


//********************************************************************
void procedure1() {
	// declare/initialize local variables
	//prompt for size of physical memory
	printf("Enter the size of physical memory: ");
	scanf("%d", &pm_size); // error in this you forgot &
	remaining = pm_size;
	
	//allocate memory for linked list of blocks
	block = (block_type *)malloc(sizeof(block_type));
	
	//create "dummy" block with id: -1, starting/ending address of 0 and the link of NULL
	block->id = -1;
	block->startAddress = 0;
	block->endAddress = 0;
	block->next = NULL;		//link of NULL
	
	return;
}


/********************************************************************/
void print() {
	// declare/initialize local variables
	block_type* currentBlock = block->next;
	
	// print header of table containing fields: ID, Start, End
	printf("\n ID\tStart\tEnd");
	printf("\n------------------\n");
	// print each block in list with while-loop, loop until end of list,
	//advancing "current block" pointer
	
	while (currentBlock != NULL){
		printf(" %d\t%d\t%d\n", currentBlock->id, currentBlock->startAddress, currentBlock->endAddress);
		currentBlock = currentBlock->next;
	}
	return;
}


/********************************************************************/
void procedure2() {
	// declare/initialize local variables
	int blockId,blockSize;
	block_type* currentBlock = block;
	block_type* newBlock;
	
	int holeStart, holeEnd, holeSize;
	
	// prompt for block id & block size
	printf("\n Enter block id: ");
	scanf("%d", &blockId); 
	
	printf("\n Enter block size: ");
	scanf("%d", &blockSize); 
	
	// if block size is larger than remaining memory, print message,
	//	return
	if(blockSize > remaining){
		printf("\n Memory overflow!.");
		return;
	}
	
	// allocate memory for new block, set block id
	newBlock = (block_type *)malloc(sizeof(block_type));
	newBlock->id = blockId;
	
	// if block list is "empty"
	if(block->next == NULL) {
		// set fields for new block
		newBlock->startAddress = block->endAddress;
		newBlock->endAddress = newBlock->startAddress + blockSize;
		newBlock->next = NULL;		
		block->next = newBlock;		// link to block list
		remaining -= blockSize;		// reduce remaining memory	
		return;
	}
	
	// while not end of list, loop
	while (currentBlock != NULL){	
		//if duplicate id, print message and return
		if (blockId == currentBlock->id){
			printf("\n Duplicate Id found!.");
			free(newBlock);
			return;	
		}
		currentBlock = currentBlock->next;
	}
	
	// initialize "current block" to compare hole
	currentBlock = block;
	
	while (currentBlock != NULL){
		//set values for start and end of current hole
		holeStart = currentBlock->endAddress;
		
		if (currentBlock->next != NULL){
			holeEnd = currentBlock->startAddress;
		}
		else {
			holeEnd = pm_size;
		}
	
		holeSize = holeEnd - holeStart;
	
		// if block fits in hole, set fields for new block, link to block list, reduce remaining memory, return
		if (blockSize <= holeSize) {
			newBlock->startAddress = holeStart;
			newBlock->endAddress = holeStart + blockSize;
			newBlock->next = currentBlock->next;
			currentBlock->next = newBlock;
			remaining -= blockSize;
			return;
		}
		// advance "current block" pointer
		currentBlock = currentBlock->next;
	}
	
	// if end of list reached, print message no fitting hole
	printf("\n No fitting hole!.");
	return;
}


/***************************************************************/
void procedure3() {
	// declare/initialize local variables
	int blockId,blockSize;
	
	block_type* currentBlock = block;
	block_type* newBlock;
	block_type* bestBlock;
	
	int holeStart, holeEnd, holeSize;
	int atLeastOne = 0;
	int bestSoFar;
	bestSoFar = pm_size;
	int bestStart;
	
	// prompt for block id & block size
	printf("\n Enter block id: ");
	scanf("%d", &blockId); 

	printf("\n Enter block size: ");
	scanf("%d",&blockSize); 
	
	// if block size is larger than remaining memory
	if (blockId > remaining){
		printf("\n Memory Overflow!.");
		return;
	}
	
	// allocate memory for new block, 
	newBlock = (block_type *)malloc(sizeof(block_type));
	newBlock->id = blockId;		// set block id
	
	// if block list is "empty", set fields for new block, link to block
	if(block->next == NULL) {
		//set fields for new block
		newBlock->startAddress = block->endAddress;
		newBlock->endAddress = newBlock->startAddress + blockSize;
		newBlock->next = NULL;	
		block->next = newBlock; 	// link to block list
		remaining -= blockSize;		// reduce remaining memory,
		return;
	}
	
	// while not end of list, loop
	while (currentBlock != NULL){
		//if duplicate id, print message, return
		if(blockId == currentBlock->id){
			printf("Duplicate ID...");
			free(newBlock);
			return;
		}
		currentBlock = currentBlock->next;
	}
	
	// initialize "current block" to compare hole
	currentBlock = block;
	
	// while not end of list, loop
	while (currentBlock != NULL){
		//set values for start and end of current hole
		holeStart = currentBlock->endAddress;
		
		if (currentBlock->next != NULL){
			holeEnd = currentBlock->next->startAddress;
		}
		else {
			holeEnd = pm_size;
		}
		
		holeSize = holeEnd - holeStart;
		
		// if block fits in hole,
		if (blockSize <= holeSize) {
			//set flag "at least one" fitting hole found
			atLeastOne = 1;
			
			// if current hole is smaller than best so far
			if(holeSize < bestSoFar) {
				// set new value for "best so far", "best start" 
				bestSoFar = holeSize;
				bestStart = holeStart;
				bestBlock = currentBlock;		// copy "best block" to current block
			}
		}
		currentBlock = currentBlock->next;		// advance "current block" pointer
	}
	
	// if no hole was found large enough, print message, return
	if(atLeastOne == 0){
		printf("There is no fitting hole");
		free(newBlock);
		free(bestBlock);
		return;
	}
	
	newBlock->startAddress = bestStart;
	newBlock->endAddress = newBlock->startAddress + blockSize;
	newBlock->next = bestBlock->next;
	bestBlock->next = newBlock;
	remaining -= blockSize;
	
	return;
}


/********************************************************************/
void procedure4() {
	// declare/initialize local variables
	block_type* prevBlock;
	int blockSize,blockId;
	
	// prompt for block id
	printf("\n Enter block id: ");
	scanf("%d",&blockId);
	
	// initialize "current block"
	block_type* currentBlock = block;
	
	// while id not found and end of block list not reached
	while((currentBlock != NULL) && (blockId != currentBlock->id)){
		// advance "previous block" pointer and "current block" pointer
		prevBlock = currentBlock;
		currentBlock = currentBlock->next;
	}

	// if "current block" is NULL, print message id not found
	if(currentBlock == NULL){
		printf("\n Id is not found!.");
		return;
	}
	// else remove block, adjusting "previous block" pointer around it,
	prevBlock->next = currentBlock->next;
	blockSize = currentBlock->endAddress - currentBlock->startAddress;
	
	remaining += blockSize;		// increment remaining memory with block size
	free(currentBlock);				// free block with matching id
	
	return;
}


/********************************************************************/
void procedure5() {
	// declare/initialize local variables
	int blockSize;
	int prevBlockEndAddress = block->endAddress;
	
	// initialize "current block"
	block_type* currentBlock = block;
	
	// while end of block list not reached
	while(currentBlock != NULL){
		// adjust start and end fields of each block, compacting
		//together
		blockSize = currentBlock->endAddress - currentBlock->startAddress;
		currentBlock->startAddress = prevBlockEndAddress;
		
		currentBlock->endAddress = currentBlock->startAddress + blockSize;
		
		prevBlockEndAddress = currentBlock->endAddress;
		currentBlock = currentBlock->next;
	}
	return;
}


/********************************************************************/
void procedure6(block_type *node) {
	
	// if node is NULL return
	if(node == NULL){
		return;
	}
	// else call self on link field of node, free node
	procedure6(node->next);
	free(node);

	return;
}
//*************************************************************
int main() {
	// declare local vars
	int choice;
	
	// while user has not chosen to quit
	do{
		// print menu of options
		printf(" Memory Allocation\n");
		printf(" -----------------\n");
		printf(" 1) Enter parameters\n"
				" 2) Allocate memory for block using First-fit\n"
			 	" 3) Allocate memory for block using Best-fit\n"
				" 4) Deallocate memory for block\n"
				" 5) Defragment memory\n"
				" 6) Quit program\n");
		
		// prompt for menu selection
		printf("Enter selection: ");
		scanf("%d", &choice);
		
		// call appropriate procedure based on choice--use switch
		//statement or series of if, else if, else statements
		if(choice==1){
			procedure1();
		}
		else if(choice==2){
			procedure2();
			print();
		}
		else if(choice==3){
			procedure3();
			print();
		}
		else if(choice==4){
			procedure4();
			print();
		}
		else if(choice==5){
			procedure5();
			print();
		}
		else if(choice==6){
			procedure6(block);
			printf("\n Quitting program...");
		}
		else{
			printf("|n You entered a wrong choice!.");
		}
	}while(choice!=6); 	 // while loop
	return 1; // indicates success
} // end of procedure
