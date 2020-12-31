# ExpressionEvaluator
 ExpressionEvaluator简易表达式计算器
 
 配合自制的内存管理器和堆栈管理器，采用中缀表达式转后缀表达式实现计算，并具备表达式错误检查功能。
 
 计算器应用程序是Release文件夹中的ExpressionEvaluator.exe，可手动输入表达式，回车进行计算，可一次性输入多个以空格分隔开的表达式，计算器会依此计算。
 
 ## 示例
 
![IMG_20200928_213432](https://github.com/Trigger-CN/ExpressionEvaluator/blob/main/image/test3.png)
![IMG_20200928_213431](https://github.com/Trigger-CN/ExpressionEvaluator/blob/main/image/test2.png)

```c
//const char* prevExpr = "(1+2)*7";
//const char* prevExpr = "1 +( 1- 2)";
//const char* prevExpr = "1+2*3+(4/5+6)*7";
int main()
{
    char* strInput = (char*)MM_Alloc(100);
    do
    {
        scanf("%s", strInput);
        //CharFilter(prevExpr, strInput, ' ');
        //printf("%s\n", strInput);
        StackElemNode* mid = ExprToMidForm(strInput);//字符串输入接口，可将strInput换成任意表达式字符串
        if (mid)
        {
            StackElemNode* back = MidToAfter(mid);
            printf("result: %f\n", Compute(back));
            FreeStack(back);
        }
        else
        {
            printf("Calculation not started\n");
        }
        FreeStack(mid);
        printf("\n");
    } while (1);

    printf("over\n");
    system("pause");
}
```
