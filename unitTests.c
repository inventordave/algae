// UNITTESTS_C

/** //RECORD

OP	a > b
		+a +b
		-a +b
		+a -b
		-a -b
		
	a == b
		+a +b
		-a +b
		+a -b
		-a -b
		
	a < b
		+a +b
		-a +b
		+a -b
		-a -b
*/

#include <stdio.h>
#include <string.h>
#include "../gcollect/gc.h"
#include "algae.h"

int tests()	{
	
	char* A = galloc( 6 );
	strcpy( A, "+9999" );
	char* B = galloc( 6 );
	strcpy( B, "+98" );
	char* C;
	
	int i = 0;
	char c = 'a';
	char o = '+';
	
	i += 1;

		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );
		
		A[0] = '-'; B[0] = '+';
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );
		
		A[0] = '+'; B[0] = '-';
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );

		A[0] = '-';
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );

	i += 1;
	c = 'a';
	strcpy( A, "+888");
	strcpy( B, "+888");;	
		
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );
		
		A[0] = '-'; B[0] = '+';
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );
		
		A[0] = '+'; B[0] = '-';
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );
		
		A[0] = '-';
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );

	i += 1;
	c = 'a';
	strcpy( A,"+634" );
	strcpy( B,"+567" );
		
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );
		
		A[0] = '+'; B[0] = '-';
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );
		
		A[0] = '-'; B[0] = '+';
		C = ADD( A,B );
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );
		
		C = ADD( A,B );
		B[0] = '-';
		printf( "Test %d%c: %s %c %s = %s\n", i, c++, A, o, B, C );

	return 0;
}

