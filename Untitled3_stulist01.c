#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
	int id;
	char name[20];
	float score;
	struct Node *next;
};

struct Node* createNode(int stu_id, const char *stu_name, float stu_score) {
	struct Node *stu = (struct Node*)malloc(sizeof(struct Node));
	if (stu == NULL) {
		printf("malloc fail\n");
		return NULL;
	}
	stu->id = stu_id;
	snprintf(stu->name, sizeof(stu->name), "%s", stu_name);
	stu->score = stu_score;
	stu->next = NULL;
	return stu;
}

struct Node* addNode(struct Node *head, struct Node *n) {
	if (n == NULL) {
		printf("add fail\n");
		return head;
	}
	if (head == NULL) {
		return n;
	}
	struct Node *cur = head;
	while (cur->next != NULL) {
		cur = cur->next;
	}
	cur->next = n;
	return head;
}

void printList(struct Node *head) {
	if (head == NULL) {
		printf("no students\n");
		return;
	}
	printf("%-8s%-19s%-8s\n", "ID", "NAME", "SCORE");
	struct Node *cur = head;
	while (cur != NULL) {
		printf("%-8d%-19s%-8.1f\n", cur->id, cur->name, cur->score);
		cur = cur->next;
	}
	return;
}

void saveList(struct Node *head, const char *filename) {
	FILE *fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("cannot open %s\n", filename);
		return;
	}
	struct Node *cur = head;
	while (cur != NULL) {
		fwrite(cur, sizeof(struct Node), 1, fp);
		cur = cur->next;
	}
	fclose(fp);
	printf("saved to %s\n", filename);
}

struct Node* loadList(const char *filename) {
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("no save file: %s\n", filename);
		return NULL;
	}
	struct Node *head = NULL;
	struct Node tmp;
	while (fread(&tmp, sizeof(struct Node), 1, fp) == 1) {
		head = addNode(head, createNode(tmp.id, tmp.name, tmp.score));
	}
	fclose(fp);
	return head;
}

void freeList(struct Node *head) {
	if (head == NULL) {
		printf("no list to free\n");
		return;
	}
	while (head != NULL) {
		struct Node *temp = head;
		head = head->next;
		free(temp);
	}
	printf("list is freed\n");
	return;
}

struct Node* deleteId(struct Node *head, int stu_id) {
	if (head == NULL) {
		printf("no list to delete\n");
		return head;
	}
	if (head->id == stu_id) {
		struct Node *temp = head;
		head = head->next;
		free(temp);
	} else {
		struct Node *prev = head;
		while (prev->next != NULL && prev->next->id != stu_id) {
			prev = prev->next;
		}
		if (prev->next == NULL) {
			printf("id:%d not found\n", stu_id);
			return head;
		}
		struct Node *temp = prev->next;
		prev->next = temp->next;
		free(temp);
	}
	printf("id:%d is deleted:\n", stu_id);
	printList(head);
	return head;
}

int main(void) {
	struct Node *head = NULL;
	struct Node *s;
	int    choice;
	int    id;
	char   name[20];
	float  score;
	while (1) {
		printf("\n===== Student Management System =====\n");
		printf("1. Add student\n");
		printf("2. Show all\n");
		printf("3. Delete by id\n");
		printf("4. Save to file\n");
		printf("5. Load from file\n");
		printf("0. Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		if (choice == 0) {
			break;
		}
		switch (choice) {
			case 1:
				printf("Enter id name score: ");
				scanf("%d %19s %f", &id, name, &score);
				s = createNode(id, name, score);
				if (s == NULL) {
					break;
				}
				head = addNode(head, s);
				printf("student added\n");
				break;
			case 2:
				printList(head);
				break;
			case 3:
				printf("Enter id to delete: ");
				scanf("%d", &id);
				head = deleteId(head, id);
				break;
			case 4:
				saveList(head, "students.dat");
				break;
			case 5:
				freeList(head);
				head = NULL;
				head = loadList("students.dat");
				break;
			default:
				printf("invalid choice\n");
		}
	}
	freeList(head);
	head = NULL;
	printf("bye\n");
	return 0;
}

