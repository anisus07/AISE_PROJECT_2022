
// C program to demonstrate segmentation
// fault when value is passed to scanf
#include <stdio.h>
 
int main ()
{
    float a = 0 ;
    long b = 0 ;
    long c = 0 ;
 
    a = ((float)b / (float)c) * 100 ;
    printf("la valeur de a est %f\n",a);
    return 0 ;
}
