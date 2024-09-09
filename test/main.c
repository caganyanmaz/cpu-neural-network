#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main()
{
	srand(23);
	int no_failed = 0;                   
	Suite *s;
	SRunner *runner;                     

	s = matrix_suite();                   
	runner = srunner_create(s);          

	srunner_run_all(runner, CK_NORMAL);  
	no_failed = srunner_ntests_failed(runner); 
	srunner_free(runner);                      
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;  

}
