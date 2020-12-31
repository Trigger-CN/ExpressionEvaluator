#include "EE.h"
#include "MM.h"
#include "StackHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define EE_MEM_ALLOC(size)     MM_Alloc(size)
#define EE_MEM_FREE(p)         MM_Free(p)
#define EE_MEM_SET(p,val,size) MM_Set(p,val,size)

static StackElemNode* ExprToMidForm(const char* expr)
{
    StackElemNode* stackTop = stackListInit();
    int index = 0;
    int bracketsCont = 0;
    bool isFloat = false;
    bool wasChar = false;
    bool wasNum = false;
    bool hadDePoint = false;
    char* str = (char*)EE_MEM_ALLOC(sizeof(char)*100);
    if (str)
    {
        EE_MEM_SET(str, 0, 100);
        do
        {
            if (expr[index] >= '0' && expr[index] <= '9' || expr[index] == '.')
            {
                if (expr[index] == '.')
                {
                    if (hadDePoint)//检查单个数字中存在多个小数点
                    {
                        printf("Error : There are multiple decimal points in a single number\n");
                        return 0;
                    }
                    isFloat = true;
                    hadDePoint = true;
                }   
                if (str && *str)
                    sprintf(str, "%s%c", str, expr[index]);
                else
                    sprintf(str, "%c", expr[index]);
                wasChar = false;
                wasNum = true;
            }
            else if (expr[index] == '*' || expr[index] == '/' || expr[index] == '+' || expr[index] == '-' || expr[index] == '(' || expr[index] == ')' || expr[index] == '\0')
            {
                if (!wasChar && wasNum)//{+123-}:[+]<-[123]>[-]
                {
                    float elemInt = (float)atof(str);
                    Push(stackTop, Float, &elemInt, 0);
                    EE_MEM_SET(str, 0, 100);
                    hadDePoint = false;
                    isFloat = false;
                }
                else if (expr[index - 1] != '(' && expr[index - 1] != ')')//检查多个相邻运算符
                {
                    if (expr[index] == '*' || expr[index] == '/' || expr[index] == '+' || expr[index] == '-')
                    {
                        printf("Error : Multiple adjacent operators\n");
                        return 0;
                    }
                    else if (expr[index] == '\0')
                    {
                        printf("Error : no end\n");
                        return 0;
                    }
                }
                if (expr[index] != '\0')
                {
                    if (expr[index] == '(')
                    {
                        bracketsCont++;
                        Push(stackTop, Char, (char*)&expr[index], 3);    
                    }
                    else if (expr[index] == ')')
                    {
                        bracketsCont--;
                        Push(stackTop, Char, (char*)&expr[index], 3);
                    } 
                    else if (expr[index] == '*' || expr[index] == '/')
                        Push(stackTop, Char, (char*)&expr[index], 2);
                    else
                        Push(stackTop, Char, (char*)&expr[index], 1);
                }
                wasChar = true;
            }
            else
            {
                printf("Error : Contains illegal characters\n");
                return 0;
            }
            index++;
        } while (expr[index - 1] != '\0');
        if (bracketsCont)
        {
            printf("Error : Incomplete brackets\n");
            return 0;
        }
    }
    StackScanRev(stackTop);
    EE_MEM_FREE(str);
    return stackTop;
}

static StackElemNode* MidToAfter(StackElemNode* midStackHead)
{
    if (!midStackHead)
    {
        return 0;
    }
    StackElemNode* tempStackTop = stackListInit();
    StackElemNode* resultStackTop = stackListInit();
    StackElemNode* nowNode = midStackHead;

    while (nowNode->next)
    {
        nowNode = nowNode->next;
    }
    while (nowNode)
    {
        if (nowNode->prio > 0)
        {
            if (!tempStackTop->next || nowNode->prio < tempStackTop->next->prio)
            {
                if (nowNode->next && nowNode->next->prio <= 0)
                    PushNode_Copy(resultStackTop, nowNode->next);
                while (tempStackTop->next && tempStackTop->next->elem.elemC != '(')
                {
                    PushNode_Cut(resultStackTop, Pop(tempStackTop));
                }
                PushNode_Copy(tempStackTop, nowNode);
            }
            else if (nowNode->type == Char && nowNode->elem.elemC == ')')
            {
                if (nowNode->next && nowNode->next->prio <= 0)
                    PushNode_Copy(resultStackTop, nowNode->next);
                while (tempStackTop->next->elem.elemC != '(')
                {
                    PushNode_Cut(resultStackTop, Pop(tempStackTop));
                }
                EE_MEM_FREE(Pop(tempStackTop));
            }
            else
            {
                if (nowNode->next && nowNode->next->prio <= 0)
                    PushNode_Copy(resultStackTop, nowNode->next);
                PushNode_Copy(tempStackTop, nowNode);
            }
        }
        else if (!nowNode->prev)
        {
            if (nowNode->next && nowNode->next->prio <= 0)
                PushNode_Copy(resultStackTop, nowNode->next);
            while (tempStackTop->next && tempStackTop->next->elem.elemC != '(')
            {
                PushNode_Cut(resultStackTop, Pop(tempStackTop));
            }
            PushNode_Copy(tempStackTop, nowNode);
        }
        nowNode = nowNode->prev;
    }
    FreeStack(tempStackTop);
    StackScanRev(resultStackTop);
    return resultStackTop;
}

static float Compute(StackElemNode* target)
{
    if (!target)
    {
        return 0;
    }
    StackElemNode* computeStackTop = stackListInit();
    StackElemNode* nowNode = target;
    while (nowNode->next)
    {
        nowNode = nowNode->next;
    }
    while (nowNode->prev)
    {
        if (nowNode->prio <= 0)
            PushNode_Copy(computeStackTop, nowNode);
        else
        {
            StackElemNode* tmp2 = Pop(computeStackTop);
            StackElemNode* tmp1 = Pop(computeStackTop);
            float tmp = 0;
            if (nowNode->elem.elemC == '+')
            {
                tmp = tmp1->elem.elemF + tmp2->elem.elemF;
            }
            else if (nowNode->elem.elemC == '-')
            {
                tmp = tmp1->elem.elemF - tmp2->elem.elemF;
            }
            else if (nowNode->elem.elemC == '*')
            {
                tmp = tmp1->elem.elemF * tmp2->elem.elemF;
            }
            else if (nowNode->elem.elemC == '/')
            { 
                tmp = tmp1->elem.elemF / tmp2->elem.elemF;
            }
            Push(computeStackTop, Float, &tmp, 0);
            EE_MEM_FREE(tmp1);
            EE_MEM_FREE(tmp2);
        }
        nowNode = nowNode->prev;
    }
    float result = computeStackTop->next->elem.elemF;
    FreeStack(computeStackTop);
    return result;
}
void CharFilter(const char* inStr, char* outStr, char c)
{
    int inStrIndex = 0;
    int outStrIndex = 0;
    while (inStr[inStrIndex]!='\0')
    {
        if (inStr[inStrIndex] != c)
        {
            outStr[outStrIndex] = inStr[inStrIndex];
            outStr[outStrIndex + 1] = '\0';
            outStrIndex++;
        }
        inStrIndex++;
    }
}

void DelFloatZero(char* strInput)
{
    int index = 0;
    while (strInput[index+1] != '\0')
    {
        index++;
    }
    while (strInput[index] == '0')
    {
        strInput[index] = 0;
        index--;
    }
    if(strInput[index] == '.')
        strInput[index] = 0;
    else
        strInput[index+1] = 0;
}

float EE_CalculatorToFloat(char* strInput)
{
    StackElemNode* mid = ExprToMidForm(strInput);
    float reslut = 0;
    if (mid)
    {
        StackElemNode* back = MidToAfter(mid);
        reslut = Compute(back);
        FreeStack(back);
        FreeStack(mid);
    }
    return reslut;
}

void EE_CalculatorToStr(const char* strInput,char* strOutput,int strSize)
{
    StackElemNode* mid = ExprToMidForm(strInput);
    float reslut = 0;
    if (mid)
    {
        StackElemNode* back = MidToAfter(mid);
        reslut = Compute(back);
        printf("%d\r\n", MM_Occupation());
        FreeStack(back);
        FreeStack(mid);
    }
    snprintf(strOutput, strSize,"%f", reslut);
    DelFloatZero(strOutput);
}

//const char* prevExpr = "(1+2)*7";
//const char* prevExpr = "1 +( 1- 2)";
//const char* prevExpr = "1+2*3+(4/5+6)*7";
int EE_example()
{
    char* strInput = (char*)EE_MEM_ALLOC(100);
    do
    {
        scanf("%s", strInput);
        //CharFilter(prevExpr, strInput, ' ');
        //printf("%s\n", strInput);
        StackElemNode* mid = ExprToMidForm(strInput);
        if (mid)
        {
            StackElemNode* back = MidToAfter(mid);
            printf("result: %f\n", Compute(back));
            printf("%d\n", MM_Occupation());
            FreeStack(back);
        }
        else
        {
            printf("Calculation not started\n");
        }
        printf("%d\n", MM_Occupation());
        FreeStack(mid);
        printf("\n");
        printf("%d\n",MM_Occupation());
    } while (1);

    printf("over\n");
}


