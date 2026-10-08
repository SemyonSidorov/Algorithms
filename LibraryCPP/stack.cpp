#include "stack.h"
#include "vector.h"
#include <iostream>

using namespace std;

Stack *stack_create()
{
    Stack* stack = new Stack;
    stack->vec = vector_create();
    return stack;
}

void stack_delete(Stack *stack)
{
    if (stack) {
        vector_delete(stack->vec);
        delete stack;
    }
}

void stack_push(Stack *stack, Data data)
{
    vector_push_back(stack->vec, data);
}

Data stack_get(const Stack *stack)
{
    if (!stack_empty(stack)) {
        return vector_get(stack->vec, vector_size(stack->vec) - 1);
    }
    return (Data)0;
}

void stack_pop(Stack *stack)
{
    if (!stack_empty(stack)) {
        stack->vec->currLength--;
    }
}

bool stack_empty(const Stack *stack)
{
    return vector_size(stack->vec) == 0;
}

void printStack(Stack* stack) {
    if (stack_empty(stack)) {
        cout << endl;
        cout << "Пусто";
    } else {
        for (size_t i = 0; i < stack->vec->currLength; i++) {
            cout << stack->vec->Data[i];
            if (i != stack->vec->currLength - 1) {
                cout << " ";
            }
        }
    }
    cout << endl;
}