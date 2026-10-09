#include <iostream>
#include <fstream>

#include "vector.h"
#include "stack.h"

using namespace std;

struct Interpreter {
    Stack* stack1;
    Stack* stack2;
    Stack* workingStack;
    Stack* altStack;
    int registerK;
    const int* program;
    size_t ip;
    size_t programLength;
};

Interpreter *interpreterCreate(const int* program, size_t length) {
    Interpreter* interpreter = new Interpreter;
    interpreter->stack1 = stack_create();
    interpreter->stack2 = stack_create();

    interpreter->workingStack = interpreter->stack1;
    interpreter->altStack = interpreter->stack2;

    interpreter->registerK = 0;
    interpreter->program = program;
    interpreter->programLength = length;
    interpreter->ip = 0;
    return interpreter;
}

void interpreterRun(Interpreter *machine) {
    while (machine->ip < machine->programLength) {

        bool skipIncrement = false;

        int cmd = machine->program[machine->ip];

        if (cmd == ' ' || cmd == '\n' || cmd == '\t' || cmd == '\r') {
            machine->ip++;
            continue;
        }
        switch (cmd) {
            case 'u':
            case '^':
                if (stack_empty(machine->workingStack)) {
                    return;
                }
                machine->registerK = stack_get(machine->workingStack);
                stack_pop(machine->workingStack);
                break;
            case 'd':
            case 'v':
                stack_push(machine->workingStack, machine->registerK);
                break;
            case 's':
            case '/': {
                if (stack_empty(machine->workingStack)) {
                    return;
                }
                int a = stack_get(machine->workingStack);
                stack_pop(machine->workingStack);
                if (stack_empty(machine->workingStack)) {
                    stack_push(machine->workingStack, a);
                    return;
                }
                int b = stack_get(machine->workingStack);
                stack_pop(machine->workingStack);
                stack_push(machine->workingStack, a);
                stack_push(machine->workingStack, b);
                break;
            }
            case 'S':
            case ';': {
                Stack* temp = machine->workingStack;
                machine->workingStack = machine->altStack;
                machine->altStack = temp;
                break;
            }
            case 'l':
            case '1':
                stack_push(machine->workingStack, 1);
                break;
            case 'm':
            case '-': {
                if (stack_empty(machine->workingStack)) {
                    return;
                }
                int a = stack_get(machine->workingStack);
                stack_pop(machine->workingStack);
                if (stack_empty(machine->workingStack)) {
                    stack_push(machine->workingStack, a);
                    return;
                }
                int b = stack_get(machine->workingStack);
                stack_pop(machine->workingStack);
                stack_push(machine->workingStack, b - a);
                break;
            }
            case 'i':
            case '[': {
                int num;
                cin >> num;
                stack_push(machine->workingStack, num);
                break;
            }
            case 'o':
            case ']': {
                if (stack_empty(machine->workingStack)) {
                    return;
                }
                int num = stack_get(machine->workingStack);
                stack_pop(machine->workingStack);
                cout << num;
                break;
            }
            case 'I':
            case '{': {
                char ch;
                cin >> ch;
                stack_push(machine->workingStack, (int)ch);
                break;
            }
            case 'O':
            case '}': {
                if (stack_empty(machine->workingStack)) {
                    return;
                }
                int code = stack_get(machine->workingStack);
                stack_pop(machine->workingStack);
                cout << (char)code;
                break;
            }
            case 'r':
            case '%': {
                Stack* tempStack = stack_create();
                while (!stack_empty(machine->workingStack)) {
                    int val = stack_get(machine->workingStack);
                    stack_pop(machine->workingStack);
                    stack_push(tempStack, val);
                }
                if (machine->workingStack == machine->stack1) {
                    stack_delete(machine->stack1);
                    machine->stack1 = tempStack;
                    machine->workingStack = tempStack;
                } else if (machine->workingStack == machine->stack2) {
                    stack_delete(machine->stack2);
                    machine->stack2 = tempStack;
                    machine->workingStack = tempStack;
                }
                break;
            }
            case 'f':
            case '*':
                break;
            case 'g':
            case '>': {
                if (stack_empty(machine->workingStack)) {
                    return;
                }
                int n = stack_get(machine->workingStack);
                stack_pop(machine->workingStack);

                if (n == 0) {
                    break;
                }

                if (n > 0) {
                    int count = 0;
                    while (count < n) {
                        machine->ip++;
                        if (machine->ip >= machine->programLength) {
                            return;
                        }
                        int nextCmd = machine->program[machine->ip];
                        if (nextCmd == 'f' || nextCmd == '*') {
                            count++;
                        }
                    }
                } else {
                    int count = 0;
                    int abs_n = -n;
                    while (count < abs_n) {
                        if (machine->ip == 0) {
                            return;
                        }
                        machine->ip--;
                        int nextCmd = machine->program[machine->ip];
                        if (nextCmd == 'f' || nextCmd == '*') {
                            count++;
                        }
                    }
                }
                skipIncrement = true;
                break;
            }
            default:
                break;
        }
        if (!skipIncrement) {
            machine->ip++;
        }
    }
}

void interpreterDelete(Interpreter *interpreter) {
    if (interpreter) {
        stack_delete(interpreter->stack1);
        stack_delete(interpreter->stack2);
        delete interpreter;
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Верный формат: " << argv[0] << " (файлСкрипта.txt) (файлДанных.txt)\n";
        return 1;
    }

    const char* scriptFile = argv[1];
    const char* inputFile = argv[2];

    ifstream script_file(scriptFile);
    if (!script_file) {
        cerr << "Не удалось открыть файл скрипта\n";
        return 1;
    }
    if (freopen(inputFile, "r", stdin) == nullptr) {
        cerr << "Не удалось открыть файл с входными данными: " << inputFile << "\n";
        return 1;
    }

    Vector *programCode = vector_create();
    char ch;
    while (script_file.get(ch)) {
        size_t currentSize = vector_size(programCode);
        vector_resize(programCode, currentSize + 1);
        vector_set(programCode, currentSize, (int)ch);
    }
    script_file.close();

    Interpreter *machine = interpreterCreate(programCode->Data, programCode->currLength);
    interpreterRun(machine);

    printStack(machine->stack1);
    printStack(machine->stack2);

    interpreterDelete(machine);
    vector_delete(programCode);

    return 0;
}