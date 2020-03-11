#define RETURN_SIZE 128
#define STACK_INITIALIZER {.pTop = NULL, .size = 0}

typedef struct Node {
    void * pElement;
    struct Node * pNext;
} Node_t;

typedef struct Stack {
    Node_t * pTop;
    int size;
} Stack_t;

void push(Stack_t * pStack, void * pElement);
void * poll(Stack_t * pStack);


// stack implementation
void push(Stack_t * pStack, void * pElement) {
    Node_t * pNode = malloc(sizeof(Node_t));
    pNode->pElement = pElement;
    pNode->pNext = pStack->pTop;
    pStack->pTop = pNode;
    pStack->size++;
}
void * poll(Stack_t * pStack) {
    void * pRet = pStack->pTop->pElement;
    Node_t * pNext = pStack->pTop->pNext;
    free(pStack->pTop);
    pStack->pTop = pNext;
    pStack->size--;
    return pRet;
}