#include <stdio.h>
#include <omp.h>

int main()
{

#pragma omp parallel
	{
		// cada thread tem uma variável específica da sua thread
		int ID = omp_get_thread_num();
		printf(" hello(%d) ", ID);
		printf(" world(%d) \n", ID);
	}

	return 0;
}
