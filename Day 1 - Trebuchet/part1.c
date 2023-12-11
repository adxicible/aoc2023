#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_DIGIT(value) (value >= 49 && value <= 58)

typedef struct {
	char *start;
	char *end;
	void *next;
} Number;

int main(int argc, char **argv)
{
	FILE *f = fopen("./input.txt", "r");
	char c;
	int lines = 0;
	Number *head = NULL, *current = NULL;

	if (f) {
		Number *number = NULL;

		while((c = getc(f)) != EOF) {
			printf("%c", c);

			if (IS_DIGIT(c)) {
				
				if (!number) {
					number = malloc(sizeof(Number));
					number->start = malloc(sizeof(char) + 1);
					number->end = malloc(sizeof(char) + 1);
					number->next = NULL;

					if (current) {
						current->next = number;
						current = NULL;
					}
					

					*(number->start + 1) = '\0';
					*(number->end + 1) = '\0';

					memcpy(number->start, &c, sizeof(c));
					memcpy(number->end, &c, sizeof(c));

					if (!head) {
						head = number;
					}
				}
				else {
					memcpy(number->end, &c, sizeof(c));
				}
			}

			if (c == '\n') {
				printf("%d: %s,%s\n",lines, number->start, number->end);
				current = number;				
				number = NULL;
				fflush(stdout);
				lines++;
			}
		}
	}

	current = head;
	int total = 0;

	do {
		char *value = malloc(strlen(current->start) + strlen(current->end) + 1);
		strcpy(value, current->start);
		strcat(value, current->end);
		
		*(value+strlen(value)+1) = '\0';

		total += atoi(value);
		
		Number *old = current;
		
		current = old->next;
		free(old);
		free(value);
	} 
	while(current != NULL);

	printf("Total: %d", total);

	fflush(stdout);
	free(head);
	fclose(f);

	head = NULL;
	return 0;
}
