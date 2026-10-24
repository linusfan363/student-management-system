#include <stdio.h>
#include <stdlib.h>
#define DATA_FILE "students.dat"
#define NAME_LEN 20

struct Node {
	int id;
	char name[NAME_LEN];
	float score;
	struct Node *next;
};

struct Node *g_head = NULL;

void saveList(struct Node *head, const char *filename) {
	FILE *fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("cannot open %s\n", filename);
		return;
	}
	struct Node *cur = head;
	while (cur != NULL) {
		struct Node tmp = *cur;
		tmp.next = NULL;
		if (fwrite(&tmp, sizeof tmp, 1, fp) != 1) {
			printf("write fail: %s\n", filename);
			fclose(fp);
			return;
		}
		cur = cur->next;
	}
	fclose(fp);
	printf("saved to %s\n", filename);
}

void freeList(struct Node *head) {
	if (head == NULL) {
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

void saveOnExit(void) {
	saveList(g_head, DATA_FILE);
	freeList(g_head);
	g_head = NULL;
	printf("bye\n");
}

void flushLine(void) {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {
		//TODO
	}
}

int readInt(const char *prompt) {
	int value;
	while (1) {
		printf("%s", prompt);
		int ret = scanf("%d", &value);
		if (ret == 1) {
			flushLine();
			return value;
		}
		if (ret == EOF) {
			printf("\ninput stream closed, exit.\n");
			exit(1);
		}
		flushLine();
		printf("invalid input!\n");
	}
}

float readFloat(const char *prompt) {
	float value;
	while (1) {
		printf("%s", prompt);
		int ret = scanf("%f", &value);
		if (ret == 1) {
			flushLine();
			return value;
		}
		if (ret == EOF) {
			printf("\ninput stream closed, exit.\n");
			exit(1);
		}
		flushLine();
		printf("invalid input!\n");
	}
}

float readScore(const char *prompt) {
	float value;
	while (1) {
		value = readFloat(prompt);
		if (value >= 0 && value <= 100) {
			return value;
		}
		printf("score must be 0-100!\n");
	}
}

void readString(const char *prompt, char *buf, int cap) {
	char fmt[8];
	snprintf(fmt, sizeof fmt, "%%%ds", cap - 1);
	while (1) {
		printf("%s", prompt);
		int ret = scanf(fmt, buf);
		if (ret == 1) {
			flushLine();
			return;
		}
		if (ret == EOF) {
			printf("\ninput stream closed, exit.\n");
			exit(1);
		}
		flushLine();
		printf("invalid input!\n");
	}
}

struct Node* createNode(int stu_id, const char *stu_name, float stu_score) {
	struct Node *stu = (struct Node*)calloc(1, sizeof(struct Node));
	if (stu == NULL) {
		printf("out of memory\n");
		return NULL;
	}
	stu->id = stu_id;
	snprintf(stu->name, sizeof(stu->name), "%s", stu_name);
	stu->score = stu_score;
	stu->next = NULL;
	return stu;
}

void printList(struct Node *head) {
	if (head == NULL) {
		printf("no students\n");
		return;
	}
	const int idW = 8, nameW = 20, scorW = 8;
	printf("%-*s%-*s%-*s\n", idW, "ID", nameW, "NAME", scorW, "SCORE");
	struct Node *cur = head;
	while (cur != NULL) {
		printf("%-*d%-*s%-*.1f\n", idW, cur->id, nameW, cur->name, scorW, cur->score);
		cur = cur->next;
	}
	return;
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

struct Node* findNode(struct Node *head, int stu_id) {
	struct Node *cur = head;
	while (cur != NULL && cur->id != stu_id) {
		cur = cur->next;
	}
	return cur;
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
		if (findNode(head, tmp.id) != NULL) {
			printf("skip duplicate id:%d in file\n", tmp.id);
			continue;
		}
		head = addNode(head, createNode(tmp.id, tmp.name, tmp.score));
	}
	fclose(fp);
	return head;
}

struct Node* modifyId(struct Node *head, int stu_id) {
	struct Node *target = findNode(head, stu_id);
	if (target == NULL) {
		printf("id:%d not found\n", stu_id);
		return head;
	}
	printf("found: id=%d name=%s score=%.1f\n", target->id, target->name, target->score);
	readString("enter new name: ", target->name, sizeof target->name);
	target->score = readScore("enter new score (0-100): ");
	printf("modified:\n");
	printList(head);
	return head;
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

struct Node* sortId(struct Node *head) {
	struct Node *sorted = NULL;
	struct Node *tail = NULL;
	while (head != NULL) {
		struct Node *min = head;
		struct Node *minprev = NULL;
		struct Node *prev = head;
		while (prev->next != NULL) {
			if (prev->next->id < min->id) {
				min = prev->next;
				minprev = prev;
			}
			prev = prev->next;
		}//find the minimum and its prev
		if (minprev == NULL) {
			head = min->next;
		} else {
			minprev->next = min->next;
		}//unlink the minimum
		min->next = NULL;
		if (sorted == NULL) {
			sorted = tail = min;
		} else {
			tail->next = min;
			tail = min;
		}//append
	}
	return sorted;
}

void showMenu(void) {
	printf("\n===== Student Management System =====\n");
	printf("1. Show all\n");
	printf("2. Sort by id\n");
	printf("3. Add by id\n");
	printf("4. Modify by id\n");
	printf("5. Delete by id\n");
	printf("6. Save to file\n");
	printf("0. Exit\n");
}

int main(void) {
	atexit(saveOnExit);
	g_head = loadList(DATA_FILE);
	printList(g_head);
	int id;
	while (1) {
		showMenu();
		int choice = readInt("please choose an option: ");
		if (choice == 0) {
			break;
		}
		switch (choice) {
			case 1: {
				printList(g_head);
				break;
			}
			case 2: {
				g_head = sortId(g_head);
				printf("sorted by id:\n");
				printList(g_head);
				break;
			}
			case 3: {
				char name[NAME_LEN];
				float score;
				struct Node *adstu;
				id = readInt("enter add id: ");
				if (findNode(g_head, id) != NULL) {
					printf("id:%d already exists\n", id);
					break;
				}
				readString("enter add name: ", name, sizeof name);
				score = readScore("enter add score (0-100): ");
				adstu = createNode(id, name, score);
				if (adstu == NULL) {
					break;
				}
				g_head = addNode(g_head, adstu);
				printList(g_head);
				printf("student added\n");
				break;
			}
			case 4: {
				id = readInt("enter modify id: ");
				g_head = modifyId(g_head, id);
				break;
			}
			case 5: {
				id = readInt("enter delete id: ");
				g_head = deleteId(g_head, id);
				break;
			}
			case 6: {
				saveList(g_head, DATA_FILE);
				break;
			}
			default: {
				printf("invalid choice\n");
			}
		}
	}
	return 0;
}

