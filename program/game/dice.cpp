#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int s,n,i;

    srand((unsigned)time(NULL));    // 乱数の仕組みを初期化

    printf("サイコロを降る回数")
    scanf("%d,&n");

    for (i=1;i<=n;i=i+1)    // n回繰り返す
    {
        s=(rand()%6)+1; // 乱数を6で割った余り+1(1から6のランダムの値が代入)
        printf("%d\n",s);
    }
    return (0);
}
