#pragma once
#include "common.h"
#include "chunk.h"
#include "value.h"
#include "table.h"

typedef enum
{
    OBJ_BOUND_METHOD,
    OBJ_CLASS,
    OBJ_NATIVE,
    OBJ_FUNCTION,
    OBJ_STRING,
    OBJ_INSTANCE,
    OBJ_CLOSURE,
    OBJ_UPVALUE
} ObjType;

struct Obj_t
{
    ObjType type;
    bool isMarked;
    Obj *next;
};

typedef Value (*NativeFn)(int argCount, Value *args);
typedef struct
{
    Obj obj;
    NativeFn function;
} ObjNative;

struct ObjString_t
{
    Obj obj;
    int length;
    char *chars;
    uint32_t hash;
};

typedef struct ObjUpvalue
{
    Obj obj;
    Value *location;
    struct ObjUpvalue *next;
    Value closed;
} ObjUpvalue;

typedef struct
{
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString *name;
    int upvalueCount;
} ObjFunction;

typedef struct
{
    Obj obj;
    ObjFunction *function;
    ObjUpvalue **upvalues;
    int upvalueCount;
} ObjClosure;

typedef struct
{
    Obj obj;
    ObjString *name;
    Table methods;
} ObjClass;

typedef struct
{
    Obj obj;
    ObjClass *klass;
    Table fields;
} ObjInstance;

typedef struct
{
    Obj obj;
    Value receiver;
    ObjClosure *method;
} ObjBoundMethod;

static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjClass *newClass(ObjString *name);
ObjClosure *newClosure(ObjFunction *function);
ObjFunction *newFunction();
ObjInstance *newInstance(ObjClass *klass);
ObjUpvalue *newUpvalue(Value *slot);
ObjNative *newNative(NativeFn function);
ObjString *copyString(const char *chars, int length);
ObjString *takeString(char *chars, int length);
ObjBoundMethod *newBoundMethod(Value receiver,
                               ObjClosure *method);

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_BOUND_METHOD(value) isObjType(value, OBJ_BOUND_METHOD)
#define IS_INSTANCE(value) isObjType(value, OBJ_INSTANCE)
#define IS_CLASS(value) isObjType(value, OBJ_CLASS)
#define IS_CLOSURE(value) isObjType(value, OBJ_CLOSURE)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_BOUND_METHOD(value) ((ObjBoundMethod *)AS_OBJ(value))
#define AS_INSTANCE(value) ((ObjInstance *)AS_OBJ(value))
#define AS_CLASS(value) ((ObjClass *)AS_OBJ(value))
#define AS_CLOSURE(value) ((ObjClosure *)AS_OBJ(value))
#define AS_NATIVE(value) \
    (((ObjNative *)AS_OBJ(value))->function)
#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)