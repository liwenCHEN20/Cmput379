/*
 * Cmput379 - Assignment 1
 * Maxime Convert
 * Student ID: 1297078
 */

struct memchunk 
{
	void *start;
	unsigned long length;
	int RW;
};

int get_mem_layout (struct memchunk *chunk_list, int size);
