#include "vector.h"

Vector *vector_create()
{
    Vector* vector = new Vector;
    vector->Data = nullptr;
    vector->maxCapacity = 0;
    vector->currLength = 0;
    return vector;
}

void vector_delete(Vector *vector)
{
    if (vector) {
        delete[] vector->Data;
        delete vector;
    }
}

Data vector_get(const Vector *vector, size_t index)
{
    if (index < vector->currLength) {
        return vector->Data[index];
    }
    return (Data)0;
}

void vector_set(Vector *vector, size_t index, Data value)
{
    if (index < vector->currLength) {
        vector->Data[index] = value;
    }
}

size_t vector_size(const Vector *vector)
{
    return vector->currLength;
}

void vector_resize(Vector *vector, size_t size)
{
    int* temp = new int[size];
    size_t oldCurrLength = vector->currLength;

    for (size_t i = 0; i < oldCurrLength; i++) {
        temp[i] = vector->Data[i];
    }
    if (vector->Data) {
        delete[] vector->Data;
    }
    vector->Data = temp;

    vector->maxCapacity = size;

    if (size < vector->currLength) {
        vector->currLength = size;
    }
}
void vector_push_back(Vector* vector, Data value) {
    if (vector->maxCapacity == 0) {
        vector_resize(vector, 4);
    }
    if (vector->currLength == vector->maxCapacity) {
        vector_resize(vector, vector->maxCapacity * 2);
    }
    vector->Data[vector->currLength] = value;
    vector->currLength++;
}
