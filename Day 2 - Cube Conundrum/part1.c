#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_SET_DELIMITER(value)(value == ',' || value == ';' || value == '\n')
#define MAX_RED     12
#define MAX_GREEN   13
#define MAX_BLUE    14

typedef struct {
    int id;
    int red;
    int green;
    int blue;
    int status;
    void *next;
} Set;

typedef struct {
    int id;
    Set *child;
    void *next;
    int red;
    int green;
    int blue;
} Node;

int is_valid(Set *n) {

    return (n->red <= MAX_RED &&
        n->green <= MAX_GREEN &&
        n->blue <= MAX_BLUE);
}

Set* init_set() {
    Set *set;
    set = malloc(sizeof(Set));
    set->id = 1;
    set->red = 0;
    set->green = 0;
    set->blue = 0;
    set->next = NULL;
    set->status = 0;

    return set;
}

int main(int argc, char **argv)
{
    FILE *f = fopen("./input.txt", "r");
    char c, *buffer = NULL;
    Set *current = NULL;
    Node *ghead = NULL, *gcurrent = NULL;

    while((c = getc(f)) != EOF) {
        
        // Finding first ':' for game id
        if (c == ':') {
            int id;            
            Node *tmp;

            sscanf(buffer, "Game%d", &id);
            
            tmp = malloc(sizeof(Node));
            tmp->next = NULL;
            tmp->child = NULL;
            tmp->id = id;

            if (gcurrent) {
                gcurrent->next = tmp;
            }
            gcurrent = tmp;

            Set *child;
            child = init_set();
            tmp->child = child;
            current = child;

            if (!ghead) {
                ghead = gcurrent;
            }

            goto reset;
        }

        if (buffer && IS_SET_DELIMITER(c)) {
            
            char name[32];
            int value;
            
            sscanf(buffer, "%d%s", &value, &name);
            
            if (!strcmp("red", name)) {
                current->red = value;
            }
            else if (!strcmp("green", name)) {
                current->green = value;
            }
            else if (!strcmp("blue", name)) {
                current->blue = value;
            }
            else {
                printf("Not found: %s\n", buffer);
            }

            if (c == ';') {
                Set *tmp;
                tmp = init_set();
                tmp->id = current->id + 1;
                current->next = tmp;
                current = tmp;
            }
            
            goto reset;
        }   
        else if (c != '\n' && c != ' ') {
            if (!buffer) {
                buffer = malloc(sizeof(c) + 1);
                *buffer = '\0';
            }
            else {
                buffer = realloc(buffer, strlen(buffer) + sizeof(c) + 1);
            }
            
            sprintf(buffer, "%s%c", buffer, c);
        }

        else if (c == '\n') {
reset:      
            if (buffer) {
                memset(buffer, '\0', strlen(buffer));
                free(buffer);
                buffer = NULL;
            }
        }
    }

    int total = 0;
    gcurrent = ghead;
    Set *child = NULL;

    while(gcurrent) {
   
        if (gcurrent) {

            child = gcurrent->child;
            int valid = 1;
            
            while(child && valid) {
            
                // printf("Game %d: %d red, %d green, %d blue\n", gcurrent->id, child->red, 
                //       child->green, child->blue);

                if (!is_valid(child)) {
                    valid = 0;
                }
                child = child->next;

            }

            if (valid) {
                printf("Game %d\n", gcurrent->id);
                total += gcurrent->id;
            }
        }
        
        gcurrent = gcurrent->next;
    }
    printf ("Total: %d\n", total);
    fclose(f);

    return 0;
}
