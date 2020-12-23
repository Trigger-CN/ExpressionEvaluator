#include "StackHandler.h"

StackElemNode* stackListInit()
{
    StackElemNode* headNode = (StackElemNode*)MM_Alloc(sizeof(StackElemNode));
    if (headNode)
    {
        MM_Set(headNode, 0, sizeof(StackElemNode));
    }

    return headNode;
}

StackElemNode* Push(StackElemNode* headNode, int elemType, void* elem, int prio)
{

    StackElemNode* newNode = (StackElemNode*)MM_Alloc(sizeof(StackElemNode));
    if (newNode)
    {
        newNode->type = elemType;
        switch (elemType)
        {
        case Int:
        {
            if (elem)
                newNode->elemI = *(int*)elem;
            break;
        }
        case Float:
        {
            if (elem)
                newNode->elemF = *(float*)elem;
            break;
        }
        case Char:
        {
            if (elem && *(char*)elem)
                newNode->elemC = *(char*)elem;
            break;
        }
        default:
            break;
        }
        newNode->prio = prio;
        if (!headNode->next)
        {
            headNode->next = newNode;
            newNode->prev = headNode;
            newNode->next = NULL;
        }
        else
        {
            StackElemNode* oldNode = headNode->next;

            headNode->next = newNode;
            newNode->prev = headNode;

            newNode->next = oldNode;
            oldNode->prev = newNode;
        }

    }
    return newNode;
}

StackElemNode* PushNode_Copy(StackElemNode* headNode, StackElemNode* targetNode)
{

    StackElemNode* newNode = (StackElemNode*)MM_Alloc(sizeof(StackElemNode));
    if (newNode)
    {
        newNode->type = targetNode->type;
        newNode->elemC = targetNode->elemC;
        newNode->elemF = targetNode->elemF;
        newNode->elemI = targetNode->elemI;
        newNode->prio = targetNode->prio;
        if (!headNode->next)
        {
            headNode->next = newNode;
            newNode->prev = headNode;
            newNode->next = NULL;
        }
        else
        {
            StackElemNode* oldNode = headNode->next;

            headNode->next = newNode;
            newNode->prev = headNode;

            newNode->next = oldNode;
            oldNode->prev = newNode;
        }

    }
    return newNode;
}

StackElemNode* PushNode_Cut(StackElemNode* headNode, StackElemNode* targetNode)
{

    StackElemNode* newNode = targetNode;
    if (newNode)
    {
        if (!headNode->next)
        {
            headNode->next = newNode;
            newNode->prev = headNode;
            newNode->next = NULL;
        }
        else
        {
            StackElemNode* oldNode = headNode->next;

            headNode->next = newNode;
            newNode->prev = headNode;

            newNode->next = oldNode;
            oldNode->prev = newNode;
        }

    }
    return newNode;
}

StackElemNode* Pop(StackElemNode* headNode)
{
    if (headNode->next)
    {
        StackElemNode* popNode = headNode->next;
        if (popNode->next)
        {
            headNode->next = popNode->next;
            popNode->next->prev = headNode;
        }
        else
            headNode->next = NULL;
        return popNode;
    }
    return 0;
}

void FreeStack(StackElemNode* headNode)
{
    StackElemNode* nowNode = headNode;
    while (nowNode)
    {
        if (nowNode->prev)
        {
            MM_Free(nowNode->prev);
            nowNode->prev = NULL;
        }
        if (nowNode->next)
            nowNode = nowNode->next;
        else
        {
            MM_Free(nowNode);
            return;
        }

    }
}

void StackScan(StackElemNode* headNode)
{
    StackElemNode* nowNode = headNode;
    while (nowNode->next)
    {
        nowNode = nowNode->next;
        printf("%d->", nowNode->type);
    }
    printf("\n");
    nowNode = headNode;
    while (nowNode->next)
    {
        nowNode = nowNode->next;
        switch (nowNode->type)
        {
        case Int:
        {
            printf("[%d]", nowNode->elemI);
            break;
        }
        case Float:
        {
            printf("<%f>", nowNode->elemF);
            break;
        }
        case Char:
        {
            printf("{%c}", nowNode->elemC);
            break;
        }
        default:
            break;
        }
    }
    printf("\n");
}

void StackScanRev(StackElemNode* headNode)
{
    StackElemNode* nowNode = headNode;
    while (nowNode->next)
    {
        nowNode = nowNode->next;
    }
    while (nowNode->prev)
    {

        switch (nowNode->type)
        {
        case Int:
        {
            printf("[%d]", nowNode->elemI);
            break;
        }
        case Float:
        {
            printf("<%f>", nowNode->elemF);
            break;
        }
        case Char:
        {
            printf("{%c}", nowNode->elemC);
            break;
        }
        default:
            break;
        }
        nowNode = nowNode->prev;
    }
    printf("\n");
}