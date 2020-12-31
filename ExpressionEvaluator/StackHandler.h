#ifndef _STACKHANDLER_H
#define _STACKHANDLER_H

#include "MM.h"

extern enum TypeEnum {
    Int,
    Float,
    Char
};

typedef struct StackElemNode_TypeDef {
    struct StackElemNode_TypeDef* prev;
    int elemI;
    float elemF;
    char elemC;
    int type;
    int prio;
    struct StackElemNode_TypeDef* next;
}StackElemNode;

StackElemNode* stackListInit();
StackElemNode* Push(StackElemNode* headNode, int elemType, void* elem, int prio);
StackElemNode* PushNode_Copy(StackElemNode* headNode, StackElemNode* targetNode);
StackElemNode* PushNode_Cut(StackElemNode* headNode, StackElemNode* targetNode);
StackElemNode* Pop(StackElemNode* headNode);
void FreeStack(StackElemNode* headNode);
void StackScan(StackElemNode* headNode);
void StackScanRev(StackElemNode* headNode);

#endif // !_STACKHANDLER_H

