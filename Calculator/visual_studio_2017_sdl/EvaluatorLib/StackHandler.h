#ifndef STACKHANDLER_H
#define STACKHANDLER_H

#include <stdint.h>

enum TypeEnum {
    Int,
    Float,
    Char
};

typedef struct StackElemNode_TypeDef {
    struct StackElemNode_TypeDef* prev;
    struct StackElemNode_TypeDef* next;

    union 
    {
        int32_t elemI;
        float elemF;
        char elemC;
    }elem;

    uint8_t type : 4;
    uint8_t prio : 4;
}StackElemNode;

StackElemNode* stackListInit();
StackElemNode* Push(StackElemNode* headNode, int elemType, void* elem, int prio);
StackElemNode* PushNode_Copy(StackElemNode* headNode, StackElemNode* targetNode);
StackElemNode* PushNode_Cut(StackElemNode* headNode, StackElemNode* targetNode);
StackElemNode* Pop(StackElemNode* headNode);
void FreeStack(StackElemNode* headNode);
void StackScan(StackElemNode* headNode);
void StackScanRev(StackElemNode* headNode);

#endif // !STACKHANDLER_H

