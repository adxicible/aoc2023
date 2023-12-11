#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 1024
#define IS_DIGIT(v) (v >= 49 && v <= 58)

typedef struct {
	char *start;
	char *end;
	void *next;
} Number;

#define MIN_NUM_LEN 3
#define MAX_NUM_LEN 5

const char* WORDS[] = { 
	"one", "two", "three", "four",
	"five", "six", "seven", "eight", 
	"nine" 
};

int wtoi (char *word) {

	for (int i = 0; i < 9; i++) 
	{
		if(strcmp(word, *(WORDS + i)) == 0) {
			return i + 49;
		}
	}

	return 48;
}

int search_start_by_index(char *buffer, int start_index) {

	int subsLength[]  = { 3,4,5 };

	if (IS_DIGIT(*(buffer + start_index))) {
		return *(buffer + start_index);
	}

	for(int l = 3; l <= 5; l++) {
		char substr[l+1];
		memset(substr, 0, l+1);
		strncpy(substr, buffer + start_index, l);
			
		if (wtoi(substr) != '0') {
			return wtoi(substr);
		}
	}

	return 48;
}

int main(int argc, char **argv)
{
	FILE *f = fopen("./input.txt", "r");
	char c;
	int lines = 1;
	Number *head = NULL, *current = NULL;
	char *linebuffer = NULL;

	if (f) {
		Number *number = NULL;
		
		while((c = getc(f)) != EOF) {
			if(c != '\n') {

				if (linebuffer) {
					linebuffer = realloc(linebuffer, strlen(linebuffer) + 2);
					sprintf(linebuffer, "%s%c", linebuffer, c);
				}
				else {
					linebuffer = malloc(sizeof(c)+1);
					memcpy(linebuffer, &c, sizeof(c)); 
					*(linebuffer+1) = '\0';
				}

				continue;
			}

			printf("%d: %s\n", lines, linebuffer);
			
			for(int i = 0; i < strlen(linebuffer); i++) {
				
				char cc = search_start_by_index(linebuffer, i);

				if (cc == '0') continue;

				if (!number) {
					number = malloc(sizeof(Number));
					number->start = malloc(sizeof(char)+1);
					number->end = malloc(sizeof(char)+1);
					number->next = NULL;

					if (current) {
						current->next = number;
						current = NULL;
					}

					*(number->start + 1) = '\0';
					*(number->end + 1) = '\0';

					memcpy(number->start, &cc, sizeof(c));
					memcpy(number->end, &cc, sizeof(c));

					if (!head) {
						head = number;
					}
				}
				else {
					memcpy(number->end, &cc, sizeof(c));
				}

			}
			
			if (number) {
				printf ("%d: Sets: %c %c\n",lines, *number->start, *number->end);
				current = number;
				number = NULL;
			}

			free(linebuffer);
			linebuffer = NULL;
			lines++;
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
	fflush(stdin);
	free(head);
	fclose(f);

	head = NULL;
	return 0;
}
