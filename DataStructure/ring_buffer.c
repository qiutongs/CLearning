struct Buffer {
    uint8_t * data;
    size_t size, capacity;
    size_t head, tail;
};

int init(Buffer * buffer, size_t capacity) {
    if (buffer == NULL || capacity <= 0) {
        return ERROR;
    }
    
    buffer->data = malloc(capacity * sizeof(uint8_t));
    if (buffer->data == NULL) {
        return ERROR;
    }
    
    buffer->size = 0;
    buffer->capacity = capacity;
    buffer->head = 0;
    buffer->tail = 0;
}

void deinit(Buffer * buffer) {
    free(buffer->data);
    buffer->size = 0;
    buffer->capacity = 0;
    buffer->head = 0;
    buffer->tail = 0;
}

int write(Buffer * buffer, uint8_t val) {
    if (buffer->size == buffer->capacity) {
        return ERROR;
    }
    buffer->data[buffer->tail] = val;
    buffer->tail++;
    buffer->tail %= buffer->capacity;
    buffer->size++;
}

int read(Buffer * buffer, uint8_t * val) {
    if (buffer->size == buffer->capacity) {
        return ERROR;
    }
    *val = buffer->data[head];
    buffer->head++;
    buffer->head %= buffer->capacity;
    buffer->size--;
}