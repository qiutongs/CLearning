#define RETURN_SIZE 128
#define STACK_INITIALIZER {.pElements = {0}, .size = 0}

typedef struct Stack {
    void * pElements[RETURN_SIZE];
    int size;
} Stack_t;

void push(Stack_t * pStack, void * pElement);
void * poll(Stack_t * pStack);

void push(Stack_t * pStack, void * pElement) {
    pStack->pElements[pStack->size++] = pElement;
}
void * poll(Stack_t * pStack) {
    return pStack->pElements[--pStack->size];
}