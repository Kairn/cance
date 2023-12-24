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
#define DEBUG_CODE 777

/* Global variables are generally not recommended for larger projects. */
int queue_capacity = INITIAL_QUEUE_SIZE;
int element_id = 1;

int get_user_action();
void debug_buffer(int *queue_ref, int *queue_head, int *queue_tail);
int *enqueue(int *queue_ref, int **queue_head_ptr, int **queue_tail_ptr);
int dequeue(int *queue_ref, int *queue_head, int **queue_tail_ptr);
int get_queue_size(int *queue_ref, int *queue_head, int *queue_tail);
void print_buffer(int *queue_ref, int *queue_head, int *queue_tail);
void print_buffer_reverse(int *queue_ref, int *queue_head, int *queue_tail);
void clear_buffer(int *queue_ref, int **queue_head_ptr, int **queue_tail_ptr);

int main(int argc, char const *argv[]) {
  /* Where is queue is allocated. */
  int *queue_ref = NULL;
  int *queue_head = NULL;
  int *queue_tail = NULL;

  /* Initialize the queue with some memory. */
  queue_ref = calloc(INITIAL_QUEUE_SIZE, sizeof(int));
  if (queue_ref == NULL) {
    printf("Fatal error, memory allocation failed.\n");
    exit(1);
  }

  queue_head = queue_ref;
  queue_tail = queue_ref;
  printf("Ring buffer of size %d has been created successfully.\n", INITIAL_QUEUE_SIZE);

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
  int dequeued = 0;

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
          /* Set back the ID since enqueue failed. */
          --element_id;
          printf("Enqueue failed, capacity limit is reached.\n");
        } else {
          printf("Successfully added <%d> to the buffer.\n", element_id - 1);
          queue_ref = temp_ref;
        }
        break;
      case 2:
        dequeued = dequeue(queue_ref, queue_head, &queue_tail);
        if (dequeued == 0) {
          printf("Nothing to dequeue.\n");
        } else {
          printf("Dequeued element <%d>.\n", dequeued);
        }
        break;
      case 3:
        printf("The buffer has %d elements.\n", get_queue_size(queue_ref, queue_head, queue_tail));
        break;
      case 4:
        if (*queue_head == 0) {
          printf("There is nothing at the head.\n");
        } else {
          printf("The head is <%d>\n", *queue_head);
        }
        break;
      case 5:
        if (*queue_tail == 0) {
          printf("There is nothing at the tail.\n");
        } else {
          printf("The tail is <%d>\n", *queue_tail);
        }
        break;
      case 6:
        print_buffer_reverse(queue_ref, queue_head, queue_tail);
        break;
      case 7:
        print_buffer(queue_ref, queue_head, queue_tail);
        break;
      case 8:
        clear_buffer(queue_ref, &queue_head, &queue_tail);
        printf("The buffer has been cleared.\n");
        break;
      case 9:
        has_quitted = 1;
        printf("Bye.\n");
        break;
      case DEBUG_CODE:
        debug_buffer(queue_ref, queue_head, queue_tail);
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

/* Prints the buffer from the beginning to end (in memory order) including 0
 * values, head and tail will be marked. */
void debug_buffer(int *queue_ref, int *queue_head, int *queue_tail) {
  int i = 0;
  for (; i < queue_capacity; ++i) {
    if (queue_ref + i == queue_head) {
      if (queue_ref + i == queue_tail) {
        printf("HT<%d>", *(queue_ref + i));
      } else {
        printf("H<%d>", *(queue_ref + i));
      }
    } else if (queue_ref + i == queue_tail) {
      printf("T<%d>", *(queue_ref + i));
    } else {
      printf("%d", *(queue_ref + i));
    }

    if (i != queue_capacity - 1) {
      printf(", ");
    }
  }

  printf("\n");
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
  if (queue_ref_expanded == NULL) {
    printf("Fatal error, memory allocation failed.\n");
    free(queue_ref);
    exit(1);
  }

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
    return (queue_head - queue_tail) + 1;
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

void print_buffer_reverse(int *queue_ref, int *queue_head, int *queue_tail) {
  int i = 0;
  int size = get_queue_size(queue_ref, queue_head, queue_tail);

  if (size == 0) {
    printf("The buffer is empty.\n");
  } else {
    printf("Listing the buffer from head to tail (LIFO):\n");
  }

  for (; i < size; ++i) {
    printf("%d", *queue_head);
    if (i != size - 1) {
      printf(", ");
    }

    --queue_head;
    if (queue_head < queue_ref) {
      queue_head = queue_ref + queue_capacity - 1;
    }
  }

  printf("\n");
}

void clear_buffer(int *queue_ref, int **queue_head_ptr, int **queue_tail_ptr) {
  int i = 0;
  /* Set all values to 0. */
  for (; i < queue_capacity; ++i) {
    *(queue_ref + i) = 0;
  }

  /* Reset the head and tail. */
  *queue_head_ptr = queue_ref;
  *queue_tail_ptr = queue_ref;
}

int dequeue(int *queue_ref, int *queue_head, int **queue_tail_ptr) {
  if (*(*queue_tail_ptr) == 0) {
    return 0;
  }

  int dequeued = *(*queue_tail_ptr);
  /* Set the tail to 0. */
  *(*queue_tail_ptr) = 0;
  if (*queue_tail_ptr == queue_head) {
    return dequeued;
  }

  /* Advance the tail pointer. */
  *queue_tail_ptr += 1;
  if (*queue_tail_ptr - queue_ref >= queue_capacity) {
    *queue_tail_ptr = queue_ref;
  }

  return dequeued;
}
