#pragma once
#include "object.h"
#include "table.h"
#include "value.h"

typedef struct {
 ObjString* key;
 Value value;
} Entry;

typedef struct {
 int count;
 int capacity;
 Entry* entries;
} Table;

void initTable(Table* table);
void markTable(Table* table);
void freeTable(Table* table);
void tableRemoveWhite(Table* table);
bool tableSet(Table* table, ObjString* key, Value value);
void tableAddAll(Table* from, Table* to);
bool tableGet(Table* table, ObjString* key, Value* value);
bool tableDelete(Table* table, ObjString* key);
ObjString* tableFindString(Table* table, const char* chars,
 int length, uint32_t hash);