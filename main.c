
/**< 作者:            LiShuang */

/**< 修改日期:        2017/5/3 */
/***< 修改原因: 解决win8及以下版本中, 控制台运行此程序, 关机时间会不断像向下移动的问题*/

/**< 简要说明:        不可以用于精确的定时关机*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <winver.h>

#define check_leap_year(year) (((!((year)%4)&&(((year)%100)||(!((year)%400)))))?1:2)  /**< 闰年返回1, 平年返回2 */

void backspace(int len);
int check(struct tm *target);
void ring(int count);

time_t sys_time = 0x00, input_time = 0x00;
const int iTmFah=1000; /// iTmFah= int Time Flash (ms)
const int iPotMit=2000; ///iPotMit= int Prompt Maintain (ms)
const double time_forbidden = 0, time_warning = 120; /**< 第一个: 用于禁止设定小于0分钟内的时间 */
const double ans = 30, zero = 1e-20;

void CmdPromption(int Num); ///设置系统提示, Num为命令码

int main(void)
{
    int len = 0, ring_flag=1;
    int tmp=0, flag=0;
    struct tm *timeinfo=NULL;
    time(&input_time);
    timeinfo = localtime(&input_time);

    CmdPromption(1);
    for(flag=0; flag<4; flag++)
    {
        if(0==scanf("%d", &tmp))
        {
            fflush(stdin);
            printf("错误");
            Sleep(2000);
            exit(1);
        }
        if(0==flag&&tmp)
            timeinfo->tm_mon = --tmp;

        else if(1==flag&&tmp)
            timeinfo->tm_mday = tmp;
        else if(2==flag)
            timeinfo->tm_hour = tmp;
        else if(3==flag)
            timeinfo->tm_min = tmp;
    }
    timeinfo->tm_sec = 0;
    flag = check(timeinfo);
    /***printf("\n%d %d %d %d\n", timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour
                , timeinfo->tm_min);***/

    if(flag)
    {
        printf("\n");
        fflush(stdin); ///while(('\n'!=(tmp=getchar()))&&(tmp!=EOF));
        backspace(len);
        len = printf("        输入完成");
        Sleep(iPotMit);
        backspace(len);
        len = printf("%8c您可以通过直接关闭本窗口来取消定时关机", ' ');
        Sleep(4000);
        CmdPromption(0);
        timeinfo = localtime(&input_time);
        printf("%8c>>关机时间<<  %02d月%02d日  %02d时%02d分\n\n", ' ', (timeinfo->tm_mon + 1),
               timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min);
        /**< time.h 中规定: 月份的取值范围是 [0, 11] */

        while(1)
        {
            time(&sys_time);
            timeinfo = localtime(&sys_time);
            len = printf("%8c当前时间  %02d月%02d日  %02d时%02d分%02d秒", ' ', (timeinfo->tm_mon + 1),
                         timeinfo->tm_mday, timeinfo->tm_hour,
                         timeinfo->tm_min, timeinfo->tm_sec);
            len += printf("%16c程序正在运行中", ' ');
            Sleep(iTmFah);
            backspace(len);

            if(time_warning > difftime(input_time, sys_time)&&ring_flag)
            {
                ring_flag=0;
                ring(2);
            }

            if(ans > difftime(input_time, sys_time))
                break;
        }
    }
    else
    {
        printf("\t错误");
        Sleep(iPotMit);
        exit(1);
    }
    free(timeinfo);
    timeinfo = NULL;

    printf("\t时间到");

    if(1==system("shutdown -s -hybrid -t 0 /f"))    /**< 暂时使用这种方式解决快速关机问题 */
        system("shutdown -s -t 0 /f");

    return 0;
}


void backspace(int len)
{
    int i=0;
    for(i=0; i<len; i++)
        printf("\b");

    for(i=0; i<len; i++)
        printf("%c", ' ');

    for(i=0; i<len; i++)
        printf("\b");
}


int check(struct tm *target)
{
    int leap_year[13]= {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                       common_year[13]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    time(&sys_time);

    /***printf("%d %d %d %d", target->tm_mon, target->tm_mday, target->tm_hour
                , target->tm_min);***/

    if(target->tm_mon<0||target->tm_mon>11)
        return (0);

    if(1==check_leap_year(target->tm_year))
    {
        if(target->tm_mday<1||target->tm_mday>leap_year[target->tm_mon])
            return (0);
    }
    else if(2==check_leap_year(target->tm_year))
    {
        if(target->tm_mday<1||target->tm_mday>common_year[target->tm_mon])
            return (0);
    }
    if(target->tm_hour<0||target->tm_hour>23)
        return (0);
    if(target->tm_min<0||target->tm_min>59)
        return (0);

    /**< 输入数据的合法性检查 结束 */

    /**< 输入数据的合理性检查 开始 */

    input_time = mktime(target);    /**<  如果将错误的月份的结构体使用mktime函数进行转换, 会
                                        转换出错误的数据*/

    if (time_forbidden>difftime(input_time, sys_time))
        return (0);

    /**< 输入数据的合理性检查 结束 */
    /**< 规定不能进行 X 分钟以内的定时设置 */

    return (1);
}


void ring(int count)
{
    int len = 0;
    for(; count; count--)
    {
        len = printf("%c", 0x07);
        len += printf("%20c即将关机...", ' ');
        Sleep(2000);
        backspace(len);
    }
    backspace(len);
}


void CmdPromption(int Num)
{
    ///Num=0, 仅执行固定输出
    system("CLS");
    system("mode con: COLS=100 LINES=30"); ///设置cmd的窗口大小
    system("title 作者 漫步到宇宙尽头");
    system("color 3E");    /**< 设定背景, 前景颜色 */

    printf("\n%30c自动关机程序\n%32c版本 0.3\n\n", ' ', ' ');
    if(1==Num)
        printf("%20c请输入关机时间%2c", ' ', '>');
}
