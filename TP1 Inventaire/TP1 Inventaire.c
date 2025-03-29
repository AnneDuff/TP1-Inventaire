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

void* allocate(size_t size);

typedef struct Items {
	char* name;
	int valeur;
}Items;

typedef struct Node Node;

struct Node {
	Items* data;
	Node* prev;
	Node* next;
};

typedef Node bagOfHolding;

int itemsCount = 0;

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

void delete(Node* head, Items* name) {
	Node* temp = head;
	int wasFound = 0;
	while (temp != head->prev) {
		if ((int)temp->data == name) { wasFound = 1; break; }
		temp = temp->next;
	}
	if (!wasFound) {
		return; //Error couldn’t find value
	}
	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	printf("\nThe item %s has been removed from the bag of holding.\n", name->name);
}

void displayInventory(bagOfHolding* head) {
	printf("\nInventory:\n-----\n");
	Node* temp = head;


	while (temp != head->prev) {
		/*if (temp->data == NULL) {
			printf("This spot is empty or has been deleted.\n");
		}*/
		printf("Item: %s\nValue: %d\n", temp->data->name, temp->data->valeur);
		temp = temp->next;
		printf("-----\n");
	}
	printf("\nNombre d'items: %d\n", itemsCount);
}

void sort(Node *head) {
	if (head == NULL) {
		return;
	}
	Node* currNode = head;
	bool swapped = false;
	for (int i = 0; i < itemsCount - 2; i++) {
		if (currNode->data->valeur > currNode->next->data->valeur) {
			Items* swap = currNode->next->data;
			currNode->next->data = currNode->data;
			currNode->data = swap;
			swapped = true;
		}
		if (i >= itemsCount - 1 && swapped == true) {
			swapped = false;
			i = -1;
			currNode = head;
			currNode->next = head->next;
		}
		currNode = currNode->next;
	}
}

Node* search(Node* head, int value) {
	Node* temp = head->prev;
	while ((int)temp->data != value) {
		temp = temp->next;
		if (temp == head->prev) {
			temp = NULL;
			break;
		}
	}
	return temp;
}

void* allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}


void main(void) {
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
#define NUM_ITEMS 6
	srand(time(NULL));

	Items fiole = {
		.name = "fiole",
		.valeur = 5
	};
	Items torch = {
		.name = "torch",
		.valeur = 10
	};
	Items map = {
		.name = "map",
		.valeur = 30
	};
	Items staff = {
		.name = "staff",
		.valeur = 100
	};
	Items supplies = {
		.name = "supplies",
		.valeur = 10
	};
	Items spellbook = {
		.name = "spellbook",
		.valeur = 150
	};
	Items ring = {
		.name = "ring",
		.valeur = 1000
	};
	/*Items boots = {
		.name = "boots",
		.valeur = 25
	};
	Items chainmail = {
		.name = "chainmail",
		.valeur = 200
	};
	Items sword = {
		.name = "sword",
		.valeur = 50
	};
	Items potion = {
		.name = "potion",
		.valeur = 10
	};
	Items shield = {
		.name = "shield",
		.valeur = 100
	};
	Items apple = {
		.name = "apple",
		.valeur = 1
	};*/
	Items possible_items[NUM_ITEMS] = {torch, map, staff, supplies, spellbook, ring};
	bagOfHolding head = { 0 };
	
	insert(&head, &fiole);
	
	for (int i = 0; i < (rand() % 10) + 1; i++) {
		insert(&head, &possible_items[rand() % NUM_ITEMS]);
	}

	displayInventory(&head);
	sort(&head);
	//delete(&head, &staff);
	displayInventory(&head);
}
