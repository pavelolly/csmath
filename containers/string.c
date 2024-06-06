#include <string.h>
#include <stdlib.h>

#include "array.h"
#include "string.h"


void DStringResize(DString *string, size_t new_capacity) {
    DArrayResize_Impl(string, sizeof(char) /*1*/, new_capacity);
    DStringAppend(string, '\0');
}

void DStringAppend(DString *string, char item) {
    // do nothing if there is null terminator already
    if (item == '\0' && string->count < string->capacity && string->items[string->count + 1] == '\0') {
        return;
    }

	while (string->capacity < string->count + 2) {
        DStringResize(string, string->capacity ? string->capacity * 1.5f + 1 : DARRAY_INIT_CAPACITY);
    }

    if (item != '\0') {
        string->items[string->count++] = item;
    }
    string->items[string->count] = '\0';
}

void DStringExtend(DString *string, const char *str) {
    DStringExtendBuffer(string, str, strlen(str));
}

void DStringExtendBuffer(DString *string, const char *buffer, size_t count) {
	while (string->capacity < string->count + count) {
        DStringResize(string, string->capacity ? string->capacity * 1.5f + 1 : DARRAY_INIT_CAPACITY);
    }

    for (size_t i = 0; i < count; ++i) {
        string->items[string->count++] = buffer[i];
    }

    string->items[string->count] = '\0';
}

void DStringConcat(DString *string, const DString *other) {
    DStringExtendBuffer(string, other->items, other->count);
}

void DStringFree(DString *string) {
    free(string->items);
}

void DStringClear(DString *string) {
    free(string->items);
    string->items = NULL;
    string->count = 0;
    string->capacity = 0;
}