#pragma once
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cmath>

using namespace std;

enum ReplacementPolicy
{
	FIFO,
	LRU
};


struct frames { //Linked List

	int page_number;

	//int num_used; //LRU


};

unsigned int swap_number = 0;
int leastRU = 0;

class virtualMemoryManagerInterface
{
public:
	virtualMemoryManagerInterface(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) : policy(p), N(pS), numFrames(nF), virtualAddressSpaceSize(vA) {
		assert(virtualAddressSpaceSize > N);
		assert(pow(2, virtualAddressSpaceSize) > numFrames * pow(2, N));


		frame = new frames*[nF]();

	}
	//Policy = FIFO or LRU
	//N = page size
	//NumFrames = Total number of frames

	/** This is the method the test bench will drive in emulating memory management.
	* Your memory manager should return the physical address corresponding to the given virtual
	* address. This function must NOT return until any page swapping is completed, if necessary.
	* This function is to effect page swaps by calling the other key function (defined below)
	*/
	virtual unsigned long long memoryAccess(unsigned long long address) { //this is a virtual address

																		  //return this virtual address's physical address
																		  //this function calls swap function
																		  //Address is the page number.

		if (pow(2, virtualAddressSpaceSize) >= address) { //Address is legit
														  //First check if this address already exist
			if (policy == FIFO) {  //First in first out

				for (int i = 0; i<numFrames;i++) {

					if ((frame[i] != NULL) && (frame[i]->page_number == (address / (pow(2, N)))))
					{ //Address already exists
						return ((i*(pow(2, N))) + (address % static_cast<int>(pow(2, N))));
					}

				}

				for (int i = 0; i<numFrames;i++) {

					if (frame[i] == NULL) { //Frames empty
						frame[i]->page_number = (address / (pow(2, N)));
						return ((i*(pow(2, N))) + (address % static_cast<int>(pow(2, N))));

					}

				}
				//Swap frames frame[swap_number] with address, swap number ++
				swap(swap_number, (address / (pow(2, N))));
				int save = swap_number;
				if ((swap_number + 1) < numFrames) {
					swap_number++;
				}

				else
					swap_number = 0;

				return ((save*(pow(2, N))) + (address % static_cast<int>(pow(2, N))));

			}
			else {  //Least Recently Used

				for (int i = 0; i<numFrames;i++) {

					if ((frame[i] != NULL) && (frame[i]->page_number == (address / (pow(2, N)))))
					{ //Address already exists
						return ((i*(pow(2, N))) + (address % static_cast<int>(pow(2, N))));
					}

				}

				if (frame[leastRU] == NULL) {
					frame[leastRU]->page_number = (address / (pow(2, N)));
					int safe = leastRU;
					if ((leastRU + 1) < numFrames) {
						leastRU++;
					}

					else
						leastRU = 0;

					return ((safe*(pow(2, N))) + (address % static_cast<int>(pow(2, N))));

				}

				if (frame[leastRU] != NULL) {
					swap(leastRU, (address / (pow(2, N))));
					int safe = leastRU;
					if ((leastRU + 1) < numFrames) {
						leastRU++;
					}
					else
						leastRU = 0;

					return ((safe*(pow(2, N))) + (address % static_cast<int>(pow(2, N))));

				}

			}

		}

		else {
			cout << "Adress is larger than Available Virtual Address Space" << endl;
			return 0;
		}


	};




	/** This is the method your memory manager should call to swap pages.
	* This function has been instrumented by the TAs to report memory system performance.
	* @param frameNumber the physical frame to write to swap file (write is emulated)
	* @param pageNumber the (virtual) page number to read from swap file into the given frame
	*/
	void swap(unsigned int frameNumber, unsigned int pageNumber) {
		///@todo t! a instrumentation to go here
		///students should instrument memorymanager performance in their own class for their internal verification or may
		///modify this code for their testing purpose; however the TAs instrumentation will used for determining grade basis.

		frame[frameNumber]->page_number = pageNumber;

		numSwaps++;
	}





	/** Report the to-date number of page swaps, used by test benches to quantify performance.
	* @returns the number of page swaps
	*/
	unsigned long long& numberPageSwaps()
	{

		return numSwaps;

	}



protected:
	ReplacementPolicy policy;

	unsigned int N; /// N = Page Address bits size, 2^ N = The physical frame/virtual page size to manage (bytes)
	unsigned int numFrames; /// total number of physical frames. Phy mem size = numFrames * 2^N
	unsigned int virtualAddressSpaceSize; ///  2^virtualAddressSpaceSize = total virtual address space (bytes)
	unsigned long long numSwaps = 0; ///total number of page swaps performed
	frames **frame;

};



