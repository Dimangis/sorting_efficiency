#include <locale.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <omp.h>
#define MAX 10000
void Vibor(int B[], int K, int I[]); // сортировка выбором
void Shaker(int B[], int K, int I[]); // шейкерная сортировка
void Hoar(int B[], int lef, int rig, int I[]); // сортировка Хоара
void manual(int B[], int K); // ручной ввод массива
void RAND(int B[], int K, int min, int max); // создание рандомного массива
void swap(int *a, int *b);
void RF1(int AR[], int KK); // чтение файла потоков
void RF2(int AR[], int KK); // чтение файла квантов
int bynarysearch(int x, int B[], int K); // бинарный поиск
void Tabla(int CTQ[], int B[], int I[], int K); // вывод таблицей
void FTabla(int CTQ[], int B[], int I[], int K); // запись таблицы в файл
void check(int B[], int K, int min, int max); // проверка ввода
//int sortchek(int B[], int K); // проверка сортировки

int srav = 0;
int iter = 0;
int sel = 0;

int main() {
  int A[MAX], C[MAX],Index[MAX], N;
  srand((unsigned int)time(0));
  printf("Enter kol-vo\n");
  scanf_s("%d", &N);  
L1:  
  printf("select an action number\n");
  printf("1 - manual input\n");
  printf("2 - generation of random\n");
  printf("3 - read from file\n");
  scanf_s("%d", &sel);
  switch (sel) {
  default:
    printf("Error!\n");
    printf("Enter again\n");
    goto L1;
  case 1:
    manual(A, N);
    check(A, N, 1, 255);
    break;
  case 2:
    RAND(A, N, 1, 255);
    break;
  case 3:
    RF1(A, N);
    break;
  }
  for (int i = 0; i < N; ++i) {
    Index[i] = i + 1;
  }
  printf("Original array:\n");
  for (int i = 0; i < N; ++i) printf("%d\t%d\n", A[i], Index[i]);
  L2:
  for (int i = 0; i < N; ++i) C[i] = A[i];
  printf("select an action number\n");
  printf("1 - Sorting by choice\n");
  printf("2 - Hoare sorting\n");
  printf("3 - Shaker sorting\n");
  scanf_s("%d", &sel);
  double st, fn;
  iter = 0; srav = 0;
  switch (sel) {
  default:
    printf("Error!\n");
    printf("Enter again\n");
    goto L2;
  case 1:
    st = omp_get_wtime();
    Vibor(C, N, Index);
    fn = omp_get_wtime();
    break;
  case 2:
    st = omp_get_wtime();
    Hoar(C, 0, N-1, Index);
    fn = omp_get_wtime();
    break;
  case 3:
    st = omp_get_wtime();
    Shaker(C, N, Index);
    fn = omp_get_wtime();
    break;
  }
 // int lol = sortcheck(C, N);
  //if (lol > -1) printf("ERROR OF SORTING\n");
  for (int i = 0; i < N; ++i) {
    printf("%d\t%d\n", C[i], Index[i]);
  }
  printf("\tTime = %f(ms)\n\tComparisons=%d\n\tIterations=%d\n", (fn - st) * 1000, srav, iter);

  
  L3:
  printf("select an action number\n");
  printf("1 - Set new array\n");
  printf("2 - Sort the array again\n");
  printf("3 - continue\n");
  printf("4 - end\n");
  scanf_s("%d", &sel);
  switch (sel) {
  default:
      printf("Error!\n");
    printf("Enter again\n");
    goto L3;
  case 1:
    goto L1;
  case 2:
    goto L2;
  case 3:
    break;
  case 4:
    goto L5;
  }
  int CPUtime[MAX];
  L4:
  printf("Select how to set the CPU time quantum\n");
  printf("1 - manual input\n");
  printf("2 - generation of random\n");
  printf("3 - read from file\n");
  scanf_s("%d", &sel);
  switch (sel) {
  default:
    printf("Error!\n");
    printf("Enter again\n");
    goto L4;
  case 1:
   manual(CPUtime, N);
   check(CPUtime, N, 0, 1000);
    break;
  case 2:
    RAND(CPUtime, N, 0, 1000);
    break;
  case 3:
    RF2(CPUtime, N);
    break;
  }
  if (N > 20) {
      st = omp_get_wtime();
      FTabla(CPUtime, C, Index, N);
      fn = omp_get_wtime();
      printf("Information search time: %f(ms)", (fn - st) * 1000);
  } else {
    st = omp_get_wtime();
    Tabla(CPUtime, C, Index, N);
    fn = omp_get_wtime();
    printf("Information search time: %f(ms)", (fn - st) * 1000);
  }

  L5:
  system("pause");
  return 0;
}

void Vibor(int B[], int K, int I[]) { // сортировка выбором
  for (int i = 0; i < K; ++i) {
    int min = i;
    for (int j = i+1; j < K; ++j) {
      if (B[min] > B[j]) {
        min = j;  
        
      }
      srav = srav+1;
    }
    swap(&B[min], &B[i]);
    swap(&I[min], &I[i]);
    iter = iter+1;
  }
  return;  
}

void Shaker(int B[], int K, int I[]) { // шейкерная сортировка
  int left = 1;
  int right = K - 1;  
  while (left <= right) {
    for (int i = right; i >= left; i--) {
      if (B[i - 1] > B[i]) {
        swap(&B[i - 1], &B[i]);
        swap(&I[i - 1], &I[i]);
        iter = iter+1;
      }
      srav = srav+1;
    }
    left++;
    for (int i = left; i <= right; i++) {
      if (B[i - 1] > B[i]) {
        swap(&B[i - 1], &B[i]);
        swap(&I[i - 1], &I[i]);        
        iter = iter+1;
      }
      srav = srav+1;
    }
    right--;
  }
  return;
}

void Hoar(int B[], int lef, int rig, int I[]) { // сортировка Хоара
  int i = lef;
  int j = rig;
  int x = B[(lef + rig) / 2];
  do {
    while (B[i] < x) {
      i++;
      srav = srav + 1;
    }
    while (B[j] > x) {
      j--;
      srav = srav + 1;
    }
    if (i <= j) {
      swap(&B[i], &B[j]);
      swap(&I[i], &I[j]);
      iter = iter + 1;
      i++;
      j--;
    }    
  } while (i <= j);
  if (lef < j) {
    Hoar(B, lef, j, I);
  }
  if (i < rig) {
    Hoar(B, i, rig, I);
  }
  return;
}

void manual(int B[], int K) {
  for (int i = 0; i < K; ++i) {
    scanf_s("%d", &B[i]);
  }
  return;
}

void RAND(int B[], int K, int min, int max) { // создание рандомного массива
  int j = 0;
  srand((unsigned int)time(0));
  for (int i = 0; i < K; ++i) {
    B[i] = (max - min) * ((double)rand() / RAND_MAX) + min;      
  }
  printf("\n");
  return;
}

void RF1(int AR[], int KK) {
  FILE* f;
  errno_t err;
  err = fopen_s(&f, "C:\\Users\\mustafaev.d\\source\\repos\\Project1\\Project1\\Laba\\Array.txt", "r");
  if (err == 0) {
    printf("file open");

    for (int i = 0; i < KK; i++)
      fscanf_s(f, "%d", &AR[i]);
    printf("\n");
  }
  else {
    printf("error");
  }
}

void RF2(int AR[], int KK) {
  FILE* f;
  errno_t err;
  err = fopen_s(&f, "CC:\\Users\\mustafaev.d\\source\\repos\\Project1\\Project1\\Laba\\Quant.txt", "r");
  if (err == 0) {
    printf("file open");

    for (int i = 0; i < KK; i++)
      fscanf_s(f, "%d", &AR[i]);
    printf("\n");
  }
  else {
    printf("error");
  }
}

void swap(int *a, int *b) {
  int tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
  return;
}

int bynarysearch(int x, int B[], int K) {  
  int low, high, middle, u = K/2;
  low = 0;
  high = K - 1;
  if (x > B[K - 1]) return K - 1; 
  else if (x < B[0]) return -1;
  else {
    while (low < high)
    {
      u--;
      middle = (low + high) / 2;
      if ((x < B[middle]) && (u == 0))
        return middle-1;
      if ((x == B[middle]) || (u == 0))    
      return middle;
      else if (x < B[middle])
        high = middle;
      else if (x > B[middle])
        low = middle;      
      else if (middle == 0)
        return -1;
      else return middle - 1;
    }
    return low;
  }
}

void Tabla(int CTQ[], int B[], int I[], int K) {
  int ind, j, temp = 0;
  ind = bynarysearch(CTQ, B, K);
  printf("_______________________________________________________________________________________\n");
  printf("|Number of   |CPU time     |Total available |Number of    |Time of     |Remaining CPU |\n");
  printf("|Iteration   |quantum      |CPU time        |flow         |flow        |time          |\n");
  for (int i = 0; K > 0; ++i) {
    printf("|____________|_____________|________________|_____________|____________|______________|\n");
    printf("|   %5d    |", i + 1); // Number of Iteration
    printf("   %5d     |", CTQ[i]); //CPU time quantum
    CTQ[i] += temp; // CPU time quantum = Total available CPU time
    for (j = 0; bynarysearch(CTQ[i], B, K) > -1; ++j) {
      if (j > 0) {
        printf("|____________|_____________|________________|_____________|____________|______________|\n");
        printf("|            |             |");
      }
      ind = bynarysearch(CTQ[i], B, K);
      printf("    %5d       |", CTQ[i]); //Total
      printf("  %5d      |", I[ind]); // Number of flow
      printf("    %5d   |", B[ind]); // Time of flow
      CTQ[i] -= B[ind];
      for (int ni = ind; ni < K - 1; ++ni) {
        B[ni] = B[ni + 1];
        I[ni] = I[ni + 1];
      }
      K--;
      printf("   %5d      |\n", CTQ[i]); // Remaining CPU time
    }
    
    if (j == 0)
      printf("  %5d   |  ---     |  ---     |  %5d   |\n", CTQ[i], CTQ[i]); // There is not flow time
    temp = CTQ[i];
  }
  printf("|____________|_____________|________________|_____________|____________|______________|\n");
  return;
}

void FTabla(int CTQ[], int B[], int I[], int K) {
  int ind, j, temp = 0;
  ind = bynarysearch(CTQ, B, K);
  FILE* f;
  errno_t err;
  err = fopen_s(&f, "C:\\Users\\mustafaev.d\\source\\repos\\Project1\\Project1\\Laba\\Result.txt", "wb");
  if (err == 0) {
    printf("file open\n");
    fprintf(f,"_______________________________________________________________________________________    \n");
    fprintf(f,"|Number of   |CPU time     |Total available |Number of    |Time of     |Remaining CPU |    \n");
    fprintf(f,"|Iteration   |quantum      |CPU time        |flow         |flow        |time          |    \n");
    for (int i = 0; K > 0; ++i) {
      fprintf(f, "|____________|_____________|________________|_____________|____________|______________|     \n");
      fprintf(f, "|   %5d    |", i + 1); // Number of Iteration
      fprintf(f, "   %5d     |", CTQ[i]); //CPU time quantum
      CTQ[i] += temp; // CPU time quantum = Total available CPU time
      for (j = 0; bynarysearch(CTQ[i], B, K) > -1; ++j) {
        if (j > 0) {
          fprintf(f, "|____________|_____________|________________|_____________|____________|______________|    \n");
          fprintf(f, "|            |             |");
        }
        ind = bynarysearch(CTQ[i], B, K);
        fprintf(f, "    %5d       |", CTQ[i]); //Total
        fprintf(f, "  %5d      |", I[ind]); // Number of flow
        fprintf(f, "    %5d   |", B[ind]); // Time of flow
        CTQ[i] -= B[ind];
        for (int ni = ind; ni < K - 1; ++ni) {
          B[ni] = B[ni + 1];
          I[ni] = I[ni + 1];
        }
        K--;
        fprintf(f, "   %5d      |    \n", CTQ[i]); // Remaining CPU time
      }

      if (j == 0)
        fprintf(f, "  %5d   |  ---     |  ---     |  %5d   |    \n", CTQ[i], CTQ[i]); // There is not flow time
      temp = CTQ[i];
    }
    fprintf(f, "|____________|_____________|________________|_____________|____________|______________|    \n");
  } else {
    printf("error");
  }
  return;
}

void check(int B[], int K, int min, int max) {
  for (int i = 0; i < K; ++i) {
    if ((B[i] < min) || (B[i] > max)) {
      printf("ERROR\n");
      printf("%d - index of wrong element\n", i + 1);
      scanf_s("%d", &B[i]);
    }
  }
}
/*int sortchek(int B[], int K) {
  for (int i = 1; i < K; ++i) {
    if (B[i-1] > B[i]) return i;
    else return -1;
  }
}*/