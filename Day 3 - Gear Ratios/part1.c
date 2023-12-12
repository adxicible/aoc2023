#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#define IS_SYMBOL(v) (!isdigit(v) && v != '.')

typedef struct {
    int value;
    int x, length, y;
    bool used;
    void *next;
} Number;

void append(char **buffer, char *c) {

    if (!*buffer) {
        *buffer = malloc(sizeof(char) + 1);
        **buffer = '\0';
    }
    else {
        *buffer = realloc(*buffer, strlen(*buffer) + sizeof(char) + 1);
        *(*buffer + strlen(*buffer)) = '\0';
    }

    sprintf(*buffer, "%s%c", *buffer, *c);
}

Number* init_number() {

    Number *tmp;
    tmp = malloc(sizeof(Number));
    tmp->value = 0;
    tmp->x = 0;
    tmp->y = 0;
    tmp->length = 0;
    tmp->used = false;
    tmp->next = NULL;

    return tmp;
}

void mark_used(Number **head, int x)
{
    Number *data;
    data = (*head);

    while(data) {
    
        if (data->x <= x && (data->length-1 + data->x) >= x && !data->used) {
            data->used = true;
        }

        data = data->next;
    }
}

void main() {

    FILE *f;
    char c, *buffer = NULL;
    int width=0, height=0, x=0, y=0;

    f = fopen("./input.txt", "r");

    while((c = getc(f)) != EOF) {
        if(c == '\n') {
            if (width < x) {
                width = x;
            }
            x = 0;
            y++;

            continue;
        }
        else {
            append(&buffer, &c);
        }

        x++;
    }

    height = y-1;
    Number *num_list = NULL, *current = NULL;
    int size;

    for(y = 0; y < height; y++) {
        for(x = y*width; x < width*y+width;x++) {
            
            if (isdigit(*(buffer+x))) {
                size = 0;
                while(isdigit(*(buffer+x+size)) && size+x < width*y+width) {
                    size++;
                }

                char newstr[size + 1];
                memset(newstr, '\0', size+1);
                strncpy(newstr, buffer+x, size);

                Number *tmp;
                tmp = init_number();

                tmp->x = x;
                tmp->y = y;
                tmp->length = size;
                tmp->value = atoi(newstr);

                if (current) {
                    current->next = tmp;
                } 
                
                current = tmp;

                if (!num_list) {
                    num_list = current;
                }

                //printf("y:%d,x:%d,size: %d %d\n", current->y, current->x, 
                //        current->length, current->value);
                x = x+size; 
            }
        }
    }
    
    for (y = 0; y < height; y++) {
        for(x = y*width; x < width*y+width; x++) {
            
            if (IS_SYMBOL(*(buffer+x))) {
                //printf("%d,%d :%c\n",y, x-y*width, *(buffer + x));

                if (y > 0 && isdigit(*(buffer+x-width-1))) { // top left
                    mark_used(&num_list, x-width-1);
                }

                if (y > 0 && isdigit(*(buffer+x-width))) { // top 
                    mark_used(&num_list, x-width);
                }

                if (y > 0 && isdigit(*(buffer+x-width+1))) { // top right 
                    mark_used(&num_list, x-width+1);
                }
                
                if (y+1 <= height && isdigit(*(buffer+x+width-1))) { // bottom left
                    mark_used(&num_list, x+width-1);
                }
                
                if (y+1 <= height && isdigit(*(buffer+x+width))) { // bottom
                    mark_used(&num_list, x+width);
                }

                if (y+1 <= height && isdigit(*(buffer+x+width+1))) { // bottom right
                    mark_used(&num_list, x+width+1);
                }
                
                if (x > 0 && isdigit(*(buffer+x-1))) { // left
                    mark_used(&num_list, x-1);
                }

                if (x+1 < (width*y+width) && isdigit(*(buffer+x+1))) { // right
                    mark_used(&num_list, x+1);
                }

            }
        }
    }

    current = num_list;
    int total = 0;
    while(current) {
        
        if (current->used) {
            //printf("%d\n", current->value);
            total += current->value;
        }

        current = current->next;
    }

    printf("Total: %d\n", total);

    free(buffer);
    buffer = NULL;
    fclose(f);
}
