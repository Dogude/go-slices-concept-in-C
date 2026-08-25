#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Header {
	size_t size;
	struct Header* next;    
} Header;

typedef struct {
	Header* head;       
} root ;

root rt = { NULL };           

void * my_malloc(size_t size) {
	
	Header* block = malloc(sizeof(Header) + size);
	
	if (!block) return NULL;
	
	block->size = size;
	
	block->next = rt.head;
	rt.head = block;

	return (void*)(block + 1);
}

typedef struct {

	int *data;
	size_t len;
	size_t cap;

} Slice;

void release() {
	Header** current = &rt.head;	
	while (*current) {
		
		Header *temp = *current;
		*current = temp->next;
		printf("Freed %zu bytes at %p\n", temp->size, temp);
		free(temp);
			
	}
}

void PrintSlice(Slice s) {
	printf("[");
	for (size_t i = 0; i < s.len; i++) {
		
		i == s.len - 1 ? printf("%d",s.data[i]) : printf("%d ", s.data[i]);
		
	}
	printf("]\n");

}

Slice make(size_t len, size_t cap) {

	Slice s = { 0 };

	if (len > cap) {
		fprintf(stderr, "invalid argument: length and capacity swapped\n");
		release();
		exit(1);
	}

	if (len == 0 && cap == 0) {

		return s;
	}

	size_t size = cap * sizeof(int);
	s.data = my_malloc(size);
	s.cap = cap;
	if (!s.data) {
		fprintf(stderr, "Error: Runtime panic - Allocation Failed\n");
		release();
		exit(1);	
	}

	memset(s.data, 0, size);
	s.len = len;
	
	return s;

}

Slice append(Slice s,int data) {
			
	if (s.len < s.cap) {
		s.data[s.len] = data;
		s.len++;
	}
	else {
		
		size_t size = !s.cap ? 4 * sizeof(int) : s.cap * 2 * sizeof(int);
		
		int* new_data = my_malloc(size);
		
		if (!new_data) {
			fprintf(stderr, "Error: Runtime panic - Allocation Failed\n");			
			release();
			exit(1);

		}

		memset(new_data,0,size);
		
		if (s.cap) {
			memcpy(new_data, s.data, s.cap * sizeof(int));
		}

		s.data = new_data;
		s.cap = !s.cap ? 4 : s.cap * 2;
		s.data[s.len] = data; 
		s.len++;
	}

	return s;

}

Slice slice(Slice s, size_t low, size_t high) {
	
	Slice sub = { 0 };

	if (low > high || high > s.cap) {
		
		fprintf(stderr, "Error: subslice index error\n");
		release();
		exit(1);
	
	}

	sub.data = s.data;
	sub.data += low;
	sub.len = high - low;
	sub.cap = s.cap - low;

	return sub;
}

size_t len(Slice s) {

	return s.len;

}

size_t cap(Slice s) {

	return s.cap;

}

int main() {
	
	Slice s = make(0,0);

	PrintSlice(s);
	printf("%zu %zu\n", len(s), cap(s));

	s = append(s, 12);
	printf("%zu %zu\n", len(s), cap(s));
	s = append(s, 13);
	s = append(s, 139);
	s = append(s, 140);

	Slice s2 = make(4,10);

	s2 = slice(s2, 2, 5);

	printf("%zu %zu\n",len(s2),cap(s2));

	PrintSlice(s);
	PrintSlice(s2);

	release();


}
