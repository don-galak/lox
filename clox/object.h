#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value)     (AS_OBJ(value)->type)

#define IS_STRING(value)    isObjType(value, OBJ_STRING)

#define AS_STRING(value)    ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)   (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_STRING,
} ObjType;

struct Obj {
    ObjType type;
};

struct ObjString {
    Obj obj;
    int length;
    char* chars;
};

/**
 * Why not just put the body of this function right in the macro?
 * What’s different about this one compared to the others? It’s because the body uses value twice.
 * A macro is expanded by inserting the argument expression every place the parameter name appears in the body.
 * If a macro uses a parameter more than once, that expression gets evaluated multiple times.
 *
 * That’s bad if the expression has side effects.
 * If we put the body of isObjType() into the macro definition and then you did, say:
 * `IS_STRING(POP())`
 * then it would pop two values off the stack! Using a function fixes that.
 */
static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif