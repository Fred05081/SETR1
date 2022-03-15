#include<stdio.h>
#include<stdlib.h>


int vAvg(int *array)
{
int sum=0;
double media=0.0;

for(int i=0;i< length(array);i++){
sum=sum+array[i];
}

media= (double) (sum/lenght(array));
return media;
}
int vSum(int * vect){
  int vSum = 0;
for( int i = 0; i <sizeof(vect) ;i++)
  {
    vSum += vect [i];
  }
  return vSum;

}

int vInit(int *array){

	array[10]={0 1 2 3 4 5 6 7 8 9};


}

int main (void)
{
  printf("Ola mundo!!!\n");
  printf("Este e um programa escrito em linguagem C\n");
    int i=0;
  
  return 0;
}
