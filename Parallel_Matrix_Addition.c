#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int** int_contigous_2D_matrix(int m, int n)
{
	int** mat = (int**)malloc(m * sizeof(int)), * linear = (int*)malloc(m * n * sizeof(int)), i;
	if (mat != NULL && linear != NULL)
	{
		for (i = 0; i < m; i++)
			mat[i] = &linear[i * n];
		return mat;
	}
	return NULL;
}

void free_mat(int** mat)
{
	free(mat[0]);
	free(mat);
}

int main(int argc,char* argv[])
{
    int** matX,** matY,** matZ, r1, c1, r2 ,c2, i, j ,k;
    printf("Welcome to Matrix Addition program!\n");
    printf("\nPlease enter dimensions of the first matrix: ");
    scanf("%d %d",&r1,&c1);
    if(r1 > 0 && c1 > 0)
    {
        matX = int_contigous_2D_matrix(r1, c1);
        printf("\nPlease enter its elements:\n\n");

        for(i = 0;i < r1;i++)
            for(j = 0;j < c1;j++)
                scanf("%d",&matX[i][j]);

        printf("\nPlease enter dimensions of the second matrix: ");
        scanf("%d %d",&r2,&c2);
        if(r2 <= 0 || c2 <= 0)
            printf("\nMatrix Dimensions are Invalid\n\n");
        else if(r1 != r2 || c1 != c2)
            printf("\nMatrices Dimensions are not same; Matrices can't be added\n\n");
        else
        {
            printf("\nPlease enter its elements:\n\n");
            matY = int_contigous_2D_matrix(r2, c2);
            matZ = int_contigous_2D_matrix(r1, c2);
            for(i = 0;i < r2;i++)
                for(j = 0;j < c2;j++)
                    scanf("%d",&matY[i][j]);
            #pragma omp parallel private(i,j)
            {
                #pragma omp for schedule(static)
                for(k = 0;k < r1*c2;k++)
                {
                    i = k / c1;
                    j = k % c2;
                    matZ[i][j] = matX[i][j] + matY[i][j];
                    printf("\nThread %d Processed matZ[%d][%d] = %d\n", omp_get_thread_num() , i , j , matZ[i][j]);
                }
            }
            printf("\nAddition Result Matrix:\n\n");
            for(i = 0;i < r1;i++)
            {
                for(j = 0;j < c2;j++)
                    printf("%d ",matZ[i][j]);
                printf("\n");
            }

            free_mat(matZ);
            free_mat(matY);
        }

        free_mat(matX);
    }
    else
        printf("\nMatrix Dimensions are Invalid\n\n");

    return 0;
}
