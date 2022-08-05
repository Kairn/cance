/*
 * An implementation of a ring buffer (circular queue) that can grow dynamically
 * in size when needed (up to a max limit).
 *
 * Concepts:
 * Dynamic memory allocation.
 * Create efficient data structure leveraging pointers.
 * How to use switch statement.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 32
#define INITIAL_QUEUE_SIZE 4

/* Global variables are generally not recommended for larger projects. */
int queue_capacity = INITIAL_QUEUE_SIZE;
int element_id = 1;

int get_user_action();
int *enqueue(int *queue_ref, int **queue_head_ptr, int **queue_tail_ptr);
int get_queue_size(int *queue_ref, int *queue_head, int *queue_tail);
void print_buffer(int *queue_ref, int *queue_head, int *queue_tail);
void print_buffer_reverse(int *queue_ref, int *queue_head, int *queue_tail);

int main(int argc, char const *argv[]) {
  /* Where is queue is allocated. */
  int *queue_ref = NULL;
  int *queue_head = NULL;
  int *queue_tail = NULL;

  /* Initialize the queue with some memory. */
  queue_ref = calloc(INITIAL_QUEUE_SIZE, sizeof(int));
  queue_head = queue_ref;
  queue_tail = queue_ref;
  printf("Ring buffer of size %d has been created successfully.\n",
         INITIAL_QUEUE_SIZE);

  printf("\nActions you can perform:\n");
  printf("1: Enqueue an element.\n");
  printf("2: Dequeue, remove an element from the tail.\n");
  printf("3: Get the queue size.\n");
  printf("4: Get the head of the queue.\n");
  printf("5: Get the tail of the queue.\n");
  printf("6: Print the ring buffer (head to tail).\n");
  printf("7: Print the ring buffer (tail to head).\n");
  printf("8: Clear the entire buffer.\n");
  printf("9: Quit.\n\n");

  int has_quitted = 0;
  int *temp_ref = NULL;

  while (!has_quitted) {
    int action = get_user_action();

    switch (action) {
      case -1:
        has_quitted = 1;
        printf("Program aborted.\n");
        break;
      case 1:
        temp_ref = enqueue(queue_ref, &queue_head, &queue_tail);
        if (temp_ref == NULL) {
          printf("Enqueue failed, capacity limit is reached.\n");
        } else {
          printf("Successfully added %d to the buffer.\n", element_id - 1);
          queue_ref = temp_ref;
        }
        break;
      case 2:
        break;
      case 3:
        printf("The buffer has %d elements.\n",
               get_queue_size(queue_ref, queue_head, queue_tail));
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        print_buffer(queue_ref, queue_head, queue_tail);
        break;
      case 8:
        break;
      case 9:
        has_quitted = 1;
        printf("Bye.\n");
        break;
      default:
        printf("Unrecognized action, try again.\n");
        break;
    }
  }

  /* Free the memory occupied by the queue. */
  free(queue_ref);
  return 0;
}

int get_user_action() {
  char input[16];
  int input_action;

  printf("Enter the action [1-9]: ");

  if (fgets(input, sizeof input, stdin)) {
    if (sscanf(input, "%d", &input_action) == 1) {
      return input_action;
    } else {
      return 255;
    }
  } else {
    printf("Unexpected EOF encountered.\n");
    return -1;
  }
}

/* Attempts to add an element to the ring buffer. If the array is full while
 * empty slots are available at the front due to deletion, elements are slotted
 * to the beginning. If the entire queue is occupied, the array is expanded with
 * elements copied, unless the maximum allowed capacity is reached. A new
 * pointer will be returned if a new array is allocated. */
int *enqueue(int *queue_ref, int **queue_head_ptr, int **queue_tail_ptr) {
  int next_element_id = element_id++;

  /* In the beginning, nothing is at the head. */
  /* This should only happen when the queue is empty. */
  if (*(*queue_head_ptr) == 0) {
    *(*queue_head_ptr) = next_element_id;
    return queue_ref;
  }

  int *temp_head = *queue_head_ptr;
  /* Move the head to the next potential empty slot. */
  *queue_head_ptr += 1;
  /* Out of bounds check. */
  if (*queue_head_ptr - queue_ref >= queue_capacity) {
    /* Move the head to the beginning of the queue. */
    *queue_head_ptr = queue_ref;
  }

  /* Check if the queue is full. */
  if (*(*queue_head_ptr) == 0) {
    /* Empty slot is available. */
    *(*queue_head_ptr) = next_element_id;
    return queue_ref;
  }

  /* Check capacity limit. */
  if (queue_capacity >= MAX_QUEUE_SIZE) {
    /* Reset the head. */
    *queue_head_ptr = temp_head;
    return NULL;
  }

  /* Allocate an expanded the array. */
  int *queue_ref_expanded = calloc(queue_capacity * 2, sizeof(int));

  /* Copy elements. */
  int i = 0;
  for (; i < queue_capacity; ++i) {
    *(queue_ref_expanded + i) = *(*queue_tail_ptr);
    *queue_tail_ptr += 1;
    if (*queue_tail_ptr - queue_ref >= queue_capacity) {
      *queue_tail_ptr = queue_ref;
    }
  }

  /* Set new queue values. */
  *queue_head_ptr = queue_ref_expanded + queue_capacity;
  *(*queue_head_ptr) = next_element_id;
  *queue_tail_ptr = queue_ref_expanded;
  queue_capacity *= 2;

  free(queue_ref);
  return queue_ref_expanded;
}

int get_queue_size(int *queue_ref, int *queue_head, int *queue_tail) {
  if (queue_head == queue_tail) {
    return *queue_head == 0 ? 0 : 1;
  } else if (queue_head < queue_tail) {
    return queue_capacity - (queue_tail - queue_head) + 1;
  } else {
    return (queue_head - queue_ref) + 1;
  }
}

void print_buffer(int *queue_ref, int *queue_head, int *queue_tail) {
  int i = 0;
  int size = get_queue_size(queue_ref, queue_head, queue_tail);

  if (size == 0) {
    printf("The buffer is empty.\n");
  } else {
    printf("Listing the buffer from tail to head (FIFO):\n");
  }

  for (; i < size; ++i) {
    printf("%d", *queue_tail);
    if (i != size - 1) {
      printf(", ");
    }

    ++queue_tail;
    if (queue_tail - queue_ref >= queue_capacity) {
      queue_tail = queue_ref;
    }
  }

  printf("\n");
}
