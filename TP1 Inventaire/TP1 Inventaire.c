#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

#define HEAP_SIZE 2048 * 2048 * 4
static uint8_t* heap = NULL;
static size_t heap_top = 0;

typedef struct Items {
	char* name;
	int value;
}Items;

typedef struct Node Node;

struct Node {
	Items* data;
	Node* prev;
	Node* next;
};

typedef Node bagOfHolding;

int itemsCount = 0;

void* allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}

void insert(Node* head, Items* newData) {
	Node* currNode = head;
	if (currNode->data == NULL) {
		currNode->data = newData;
		itemsCount += 1;
		printf("The item %s has been added to the bag of holding.\n", newData->name);
		return;
	}
	else {
		Node* n = allocate(sizeof(Node));
		n->data = newData;
		n->prev = NULL;
		n->next = NULL;
		if (currNode->next != NULL) {
			n->next = currNode->next;
			currNode->next->prev = n;
		}
		currNode->next = n;
		n->prev = currNode;
		itemsCount += 1;
		printf("The item %s has been added to the bag of holding.\n", newData->name);
	}
}

//will not work if node to remove is the tail
void delete(Node* head, char* name) {
	Node* currNode = head;
	bool wasFound = false;
	while (currNode != head->prev) {
		if (currNode->data->name == name) {
			wasFound = true; 
			break; 
		}
		currNode = currNode->next;
	}
	if (!wasFound) {
		printf("The item hasn't been found :(\n");
		return;
	}
	if (currNode == head) {
		Node* swap = currNode->data;
		currNode->data = currNode->next->data;
		currNode->next->data = swap;
		currNode = currNode->next;
	}
	currNode->prev->next = currNode->next;
	currNode->next->prev = currNode->prev;
	itemsCount -= 1;
	printf("\nThe item %s has been removed from the bag of holding.\n", currNode->data->name);
}

void displayInventory(Node* head) {
	printf("\nInventory:\n-----\n");
	Node* currNode = head;
	while (currNode != head->prev) {
		if (currNode->data == NULL) {
			printf("This spot is empty or has been deleted.\n");
		}
		printf("Item: %s\nValue: %dGP\n", currNode->data->name, currNode->data->value);
		currNode = currNode->next;
		printf("-----\n");
	}
	printf("\nNombre d'items: %d\n", itemsCount);
}

//sorts items according to their values
void sort(Node* head) {
	if (head == NULL) {
		return;
	}
	Node* currNode = head;
	bool swapped = true;
	while (swapped) {
		swapped = false;
		currNode = head;
		while (currNode->next != NULL) {
			if (currNode->data->value > currNode->next->data->value) {
				Items* swap = currNode->data;
				currNode->data = currNode->next->data;
				currNode->next->data = swap;
				swapped = true;
			}
			currNode = currNode->next;
		}
	}
	printf("The bag of holding has been sorted.\n");
}

//finds items relative to their positions
void findItemPos(Node* position) {
	printf("Item found! It is: %s\n", position->data->name);
}

//finds items relative to their names
void findItemName(Node* head, char* itemName) {
	Node* currNode = head;
	while (currNode != head->prev) {
		if (currNode->data->name == itemName) {
			printf("Item found! It's name is: %s\n", currNode->data->name);
			break;
		}
	}
}

void main(void) {
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
#define NUM_ITEMS 10
	srand(time(NULL));

	Items fiole = {
		.name = "fiole",
		.value = 5
	};
	Items map = {
		.name = "map",
		.value = 30
	};
	Items staff = {
		.name = "staff",
		.value = 100
	};
	Items supplies = {
		.name = "supplies",
		.value = 10
	};
	Items spellbook = {
		.name = "spellbook",
		.value = 150
	};
	Items ring = {
		.name = "ring",
		.value = 1000
	};
	Items boots = {
		.name = "boots",
		.value = 25
	};
	Items chainmail = {
		.name = "chainmail",
		.value = 200
	};
	Items sword = {
		.name = "sword",
		.value = 50
	};
	Items shield = {
		.name = "shield",
		.value = 100
	};
	Items apple = {
		.name = "apple",
		.value = 1
	};
	Items possible_items[NUM_ITEMS] = {
		map, staff, supplies, spellbook, ring,
		boots, chainmail, sword, shield, apple}; 

	bagOfHolding head = { 0 };
	
	insert(&head, &fiole);
	
	for (int i = 0; i < (rand() % 5) + 5; i++) {
		insert(&head, &possible_items[rand() % NUM_ITEMS]);
	}

	displayInventory(&head);
	delete(&head, "fiole");
	sort(&head);
	displayInventory(&head);



	//findItemPos(&head);
	//findItemName(&head, "fiole");
}
