#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* arithmetic functions */
double add(double a, double b) {
  return a + b;
}

double subtract(double a, double b) {
  return a - b;
}

double multiply(double a, double b) {
  return a * b;
}

double divide(double a, double b) {
  return a / b;
}

/* linked list stack */
struct node {
    char data;
    struct node* next;
};

void initLinkedList(struct node* head) {
    head = NULL;
}

struct node* pushToStack(struct node* head, char data) {
    struct node* tmp = (struct node*)malloc(sizeof(struct node));
    if(tmp == NULL) {
        exit(0);
    }

    tmp->data = data;
    tmp->next = head;
    head = tmp;

    return head;
}

struct node* popFromStack(struct node *head, char *element) {
    struct node* tmp = head;
    *element = head->data;
    head = head->next;
    free(tmp);

    return head;
}

void display(struct node* head) {
    struct node *current;
    current = head;
    if(current!= NULL) {
        do {
            printf("%d ",current->data);
            current = current->next;
        }
        while (current!= NULL);
        printf("\n");
    }
    else {
        printf("The Stack is empty\n");
    }
 
}

FILE * open_file(char filename[]) {
  FILE * file;

  file = fopen(filename, "r");
  if (file == NULL) {
    printf("FATAL: Error opening file %s. Aborting program.\n", filename);
    exit(1);
  }

  return file;
}

int main(int argc, char ** argv) {
  char * filename;

  if (argc == 1) {
    printf("Error: No input filename provided\n");
    printf("Usage: %s <input filename>\n", argv[0]);
    exit(1);
  } else if (argc > 2) {
    printf("Error: Too many command line parameters\n");
    printf("Usage: %s <input filename>\n", argv[0]);
    exit(1);
  } else {
    filename = argv[1];
  }

  FILE *file = open_file(filename);
  char *str = malloc(sizeof(char) * 1024);
  fgets(str, 1024, file);

  struct node* head = NULL;
  initLinkedList(head);

  double a;
  double b;
  double result;

  char *ch;
  ch = strtok(str, " ");

  int char1;

  while (ch != NULL) {
    switch (*ch) {
      case '+':
        // pop the top two items off the stack
        head = popFromStack(head, ch);
        a = *ch;
        head = popFromStack(head, ch);
        b = *ch;
        // apply addition to those items
        result = add(a, b);
        // push the result onto the stack
        head = pushToStack(head, result);
        break;
      case '-':
        // pop the top two items off the stack
        head = popFromStack(head, ch);
        a = *ch;
        head = popFromStack(head, ch);
        b = *ch;
        // apply subtraction to those items
        result = subtract(a, b);
        // push the result onto the stack
        head = pushToStack(head, result);
        break;
      case 'X':
        // pop the top two items off the stack
        head = popFromStack(head, ch);
        a = *ch;
        head = popFromStack(head, ch);
        b = *ch;
        // apply multiplication to those items
        result = multiply(a, b);
        // push the result onto the stack
        head = pushToStack(head, result);
        break;
      case '/':
        // pop the top two items off the stack
        head = popFromStack(head, ch);
        a = *ch;
        head = popFromStack(head, ch);
        b = *ch;
        // apply division to those items
        result = divide(a, b);
        // push the result onto the stack
        head = pushToStack(head, result);
        break;
      case '^':
        // pop the top two items off the stack
        head = popFromStack(head, ch);
        a = *ch;
        head = popFromStack(head, ch);
        b = *ch;
        // apply power to those items
        result = pow(a, b);
        // push the result onto the stack
        head = pushToStack(head, result);
        break;
      default:
        char1  = atoi(ch);
        head = pushToStack(head, char1);
        break;
    } 
    ch = strtok(NULL, " ,");
  }
  display(head);
  fclose(file);
  return 0;
}