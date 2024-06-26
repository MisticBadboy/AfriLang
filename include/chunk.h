#pragma once
#include "common.h"
#include "value.h"

typedef enum
{
    OP_RETURN,
    OP_CONSTANT,
    OP_GET_GLOBAL,
    OP_NIL,
    OP_TRUE,
    OP_PRINT,
    OP_FALSE,
    OP_NOT,
    OP_EQUAL,
    OP_GREATER,
    OP_SET_GLOBAL,
    OP_GET_LOCAL,
    OP_JUMP_IF_FALSE,
    OP_JUMP,
    OP_SET_LOCAL,
    OP_LESS,
    OP_NEGATE,
    OP_DEFINE_GLOBAL,
    OP_ADD,
    OP_POP,
    OP_INHERIT,
    OP_LOOP,
    OP_GET_SUPER,
    OP_SUPER_INVOKE,
    OP_SUBTRACT,
    OP_CALL,
    OP_CLOSURE,
    OP_GET_UPVALUE,
    OP_CLASS,
    OP_GET_PROPERTY,
    OP_SET_PROPERTY,
    OP_INVOKE,
    OP_SET_UPVALUE,
    OP_CLOSE_UPVALUE,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_METHOD
} OpCode;

typedef struct
{
    int count;
    int *lines;
    int capacity;
    uint8_t *code;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);
void freeChunk(Chunk *chunk);