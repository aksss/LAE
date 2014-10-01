/* created by Akshay on 09/24/2014*/

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>

void forward_Elimination(int n,float **A,float *X);
void back_substitution(int n,float **A,float *X);

int main()
{
        float **A,*X,**orig_mat,*approx_res,*error;
        int i,j,n;
	clock_t start, end;
   	double total_time;
	FILE *input_ptr,*output_ptr;
	//input_ptr = fopen("in_10*10.txt","r");
        //output_ptr = fopen("out_10*10.txt","w");
	input_ptr = fopen("in_random_matrix_1000*1000.txt","r");
        output_ptr = fopen("out_random_matrix_1000*1000.txt","w");
	//input_ptr = fopen("in_Hilbert_matrix_5*5.txt","r");
        //output_ptr = fopen("out_Hilbert_matrix_5*5.txt","w");
        if(input_ptr==NULL) exit(1);
	fprintf(output_ptr,"\n\n*Welcome in Gauss Elimination method using partial_pivoting  to solve equations of form A.X = B\n\n");
/////////////////////Input the Order of Matrix//////////////////////////////////////////////////////
    	fprintf(output_ptr,"*Enter the order of matrix :\n");
    	fscanf(input_ptr,"%d",&n);
	fprintf(output_ptr,"N(matrix size)=%d*%d",n,n);
//////////////////////Memory Allocation for matrix and right hand side vector//////////////////////////
        A = (float**)malloc(n*sizeof(float*));
	orig_mat = (float**)malloc(n*sizeof(float*));
        for(i = 0; i < n; i++)
	{
            A[i] = (float*)malloc(n*sizeof(float));
	    orig_mat[i] = (float*)malloc(n*sizeof(float));
	}
        X = (float*)malloc(n*sizeof(float));
	approx_res = (float*)malloc(n*sizeof(float));
	error = (float*)malloc(n*sizeof(float));
//////////////////////Input coefficient of matrix and constant terms from User//////////////////////////
 	printf("\n\n*Enter the coefficients of matrix :\n");
	for(i=0; i<n; i++)
    	{
        	for(j=0; j<n; j++)
       		{
            		fscanf(input_ptr,"%f",&A[i][j]);
			orig_mat[i][j] = A[i][j];
        	}
    	}
    	printf("\n\n*Enter the Right hand side vector :\n");
    	for(i=0; i<n; i++)
    	{
        	fscanf(input_ptr,"%f",&A[i][n]);
		orig_mat[i][n] = A[i][n];
    	}
/////////////////////////////////////Display Entered Matrix//////////////////////////////////////
    	fprintf(output_ptr,"\n\n*Entered Matrix is :\n");
    	for(i=0; i<n; i++)
    	{
        	for(j=0; j<=n; j++)
       	 	{
            		fprintf(output_ptr," %.3f\t ",A[i][j]);
        	}
        	fprintf(output_ptr,"\n\n");
    	}
////////////////////////////////////////generation of scalar matrix//////////////////////////////
	start = clock();        
	forward_Elimination(n,A,X);
/////////////////////////Augmented form of the matrix///////////////////////////////
	fprintf(output_ptr,"\n\n*The Augmented form of the matrix is :\n");
        for(i=0;i<n;i++)
	{
            for(j=0;j<=n;j++)
            { 
		fprintf(output_ptr,"%.3f\t",A[i][j]);
	    }
            fprintf(output_ptr,"\n");
        }
/////////////////////////////////backward substitution Method///////////////////////////////
        back_substitution(n,A,X);
	end = clock();
////////////////////////////////display the solution of Equations///////////////////////////////
        fprintf(output_ptr,"\n\n*The solution of above equations are :\n\n");
        for(i=0;i<n;i++)
	{
        	fprintf(output_ptr,"X[%d] = %.4f\n", i, X[i]);
	}
////////////////////////////////Error Identification///////////////////////////////////////////
	fprintf(output_ptr,"\n*Using above solutions,the given linear algebraic equations evaluates to(Approximate):\n\n");
	for(i=0;i<n;i++)
	{
        	for(j=0;j<n;j++)
		{
			approx_res[i] = approx_res[i] + X[j]*orig_mat[i][j] ;
			fprintf(output_ptr,"(%f)*(%f) + ",X[j],orig_mat[i][j]);
		}
		fprintf(output_ptr,"= %lf",approx_res[i]);
		fprintf(output_ptr,"\n");
	}
	/*Display Error grnerated*/
	fprintf(output_ptr,"\n*Error= ((Actual - Approximate)/Actual)*100\n\n");
	for(i=0;i<n;i++)
	{	
		//error[i] = ((res[i] - orig_mat[i][n])/res[i])*100;
		error[i] = ((orig_mat[i][n] - approx_res[i])/(orig_mat[i][n]))*100;
		fprintf(output_ptr,"%d.(%lf) - (%lf) = %-.2le\n",i+1,orig_mat[i][n],approx_res[i], fabs(error[i]));
	}
	/*Display Execution time */
	total_time = ((double)(end-start))/(CLOCKS_PER_SEC);
	fprintf(output_ptr,"\n\nTotal Execution time = %f sec\n",total_time);
////////////////////////////////free allocated memory////////////////////////////////////////
        for(i = 0; i < n; i++)
        {
		free(A[i]);
		free(orig_mat[i]);
	}
        free(A);
	free(orig_mat);
        free(X);
	free(approx_res);
	free(error);
	fflush(output_ptr);
	fclose(input_ptr);
	fclose(output_ptr);
    	return 0;
}

void forward_Elimination(int n,float **A,float *X)
{
	float app,mult;
	int i,j,k,p;
	for(i=0;i<n;i++)
	{
            app = A[i][i];
            p = i;
/*find largest no of the columns and row no. of largest no.*/
            for(k = i+1; k < n; k++)
            {
		if(fabs(app) < fabs(A[k][i]))
	   	{
               		app = A[k][i] ;
               		p = k;
            	}
	    }
/*swaping the elements of diagonal row and row containing largest no*/
            for(j = 0; j <= n; j++)
            {
                X[j] = A[p][j];
                A[p][j] = A[i][j];
                A[i][j] = X[j];
            }
/*calculating triangular matrix using Forward Elimination method*/
            for(j=i+1;j<n;j++)
	    {
                mult = A[j][i]/A[i][i];
                for(k=i;k<=n;k++)
		{		
                    A[j][k] = A[j][k] - mult * A[i][k];
        	}    
	    }
        } 
}

/*for calculating value of unknows using backward substitution method*/
void back_substitution(int n,float **A,float *X)
{
	int i,j;
	float sum;
        for(i=n-1;i>=0;i--)
        {
            sum = 0;
            for(j=i+1;j<n;j++)
            {
		sum = sum + A[i][j]*X[j];
	    }
            X[i] = (A[i][n]-sum)/A[i][i];
        }
}
