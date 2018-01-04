
/**< ����:            LS */

/**< �޸�����:        2017/5/3 */
/***< �޸�ԭ��: ���win8�����°汾��, ����̨���д˳���, �ػ�ʱ��᲻���������ƶ�������*/

/**< ��Ҫ˵��:        ���������ھ�ȷ�Ķ�ʱ�ػ�*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <winver.h>

#define check_leap_year(year) (((!((year)%4)&&(((year)%100)||(!((year)%400)))))?1:2)  /**< ���귵��1, ƽ�귵��2 */

void backspace(int len);
int check(struct tm *target);
void ring(int count);

time_t sys_time = 0x00, input_time = 0x00;
const int iTmFah=1000; /// iTmFah= int Time Flash (ms)
const int iPotMit=2000; ///iPotMit= int Prompt Maintain (ms)
const double time_forbidden = 0, time_warning = 120; /**< ��һ��: ���ڽ�ֹ�趨С��0�����ڵ�ʱ�� */
const double ans = 30, zero = 1e-20;

void CmdPromption(int Num); ///����ϵͳ��ʾ, NumΪ������

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
            printf("����");
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
        len = printf("        �������");
        Sleep(iPotMit);
        backspace(len);
        len = printf("%8c������ͨ��ֱ�ӹرձ�������ȡ����ʱ�ػ�", ' ');
        Sleep(4000);
        CmdPromption(0);
        timeinfo = localtime(&input_time);
        printf("%8c>>�ػ�ʱ��<<  %02d��%02d��  %02dʱ%02d��\n\n", ' ', (timeinfo->tm_mon + 1),
               timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min);
        /**< time.h �й涨: �·ݵ�ȡֵ��Χ�� [0, 11] */

        while(1)
        {
            time(&sys_time);
            timeinfo = localtime(&sys_time);
            len = printf("%8c��ǰʱ��  %02d��%02d��  %02dʱ%02d��%02d��", ' ', (timeinfo->tm_mon + 1),
                         timeinfo->tm_mday, timeinfo->tm_hour,
                         timeinfo->tm_min, timeinfo->tm_sec);
            len += printf("%16c��������������", ' ');
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
        printf("\t����");
        Sleep(iPotMit);
        exit(1);
    }
    free(timeinfo);
    timeinfo = NULL;

    printf("\tʱ�䵽");

    if(1==system("shutdown -s -hybrid -t 0 /f"))    /**< ��ʱʹ�����ַ�ʽ������ٹػ����� */
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

    /**< �������ݵĺϷ��Լ�� ���� */

    /**< �������ݵĺ����Լ�� ��ʼ */

    input_time = mktime(target);    /**<  �����������·ݵĽṹ��ʹ��mktime��������ת��, ��
                                        ת�������������*/

    if (time_forbidden>difftime(input_time, sys_time))
        return (0);

    /**< �������ݵĺ����Լ�� ���� */
    /**< �涨���ܽ��� X �������ڵĶ�ʱ���� */

    return (1);
}


void ring(int count)
{
    int len = 0;
    for(; count; count--)
    {
        len = printf("%c", 0x07);
        len += printf("%20c�����ػ�...", ' ');
        Sleep(2000);
        backspace(len);
    }
    backspace(len);
}


void CmdPromption(int Num)
{
    ///Num=0, ��ִ�й̶����
    system("CLS");
    system("mode con: COLS=100 LINES=30"); ///����cmd�Ĵ��ڴ�С
    system("title ���� ���������澡ͷ");
    system("color 3E");    /**< �趨����, ǰ����ɫ */

    printf("\n%30c�Զ��ػ�����\n%32c�汾 0.3\n\n", ' ', ' ');
    if(1==Num)
        printf("%20c������ػ�ʱ��%2c", ' ', '>');
}
