#include <string.h>
#include "global.h"

void itoa(int n, char s[]);
void dwtoa(DWORD n, char s[]);
void reverse(char s[]);

 void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* ���������� ���� */
         n = -n;          /* ������ n ������������� ������ */
     i = 0;
     do {       /* ���������� ����� � �������� ������� */
         s[i++] = n % 10 + '0';   /* ����� ��������� ����� */
     } while ((n /= 10) > 0);     /* ������� */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
 
 void dwtoa(DWORD n, char s[])
 {
     int i;
 
     i = 0;
     do {       /* ���������� ����� � �������� ������� */
         s[i++] = n % 10 + '0';   /* ����� ��������� ����� */
     } while ((n /= 10) > 0);     /* ������� */
     
     s[i] = '\0';
     reverse(s);
 }
 
 /* reverse:  �������������� ������ s �� ����� */
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }