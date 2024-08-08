int g;
int* gp = &g;
int vec[]= {1,2,3};
int* intptr;

int f0(int a){
    int tmp = a;
    tmp = a + *gp;
    return tmp;
}

int f1(int a)
{
  int tmp;
  tmp = a + vec[0];
  return tmp;
}