#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../gcollect/gc.h"
#include "algae.h"

extern int tests();

int MAX_LOOPS = 512;

int main( int argc, char** argv )  {

	volatile struct GC* gc = initGC( 100 );
	
	
	if( !strcmp(argv[1],"all") )	{
		
		tests();
		return 0;
	}
	
	char* A;
	char* B;
	char* OP;
	
	if( argc == 1 )	{
	
		OP = "mul";
		A = getstring( "1234" );
		B = getstring( "4321" );
	}
	
	if( argc == 2 )	{
		
		OP = argv[1];
		A = getstring( "1234" );
		B = getstring( "4321" );
	}
	
	if( argc == 3 )	{
		
		OP = argv[1];
		A = argv[2];
		B = getstring( "17" );
	}

	if( argc == 4 )	{
		
		OP = argv[1];
		A = argv[2];
		B = argv[3];
	}
	
	
	char* _;

	if( !strcmp(OP,"mul") || !strcmp(OP,"MUL") )
		_ = MUL( A,B );

	if( !strcmp(OP,"add") || !strcmp(OP,"ADD") || !strcmp(OP,"+") )
		_ = ADD( A,B );


	if( !strcmp(OP,"div") || !strcmp(OP,"DIV") )
		_ = DIV( A,B );

	if( !strcmp(OP,"sub") || !strcmp(OP,"SUB") || !strcmp(OP,"-") )
		_ = SUB( A,B );


	printf( "Result: '%s'\n", _ );
	
	cleanUp();
	
	return 0;
}

char* SUB( char* A, char* B )	{

	// SUB Algorithm:
	// The subtraction of a real number (the subtrahend [B]) from another (the
	// minuend [A]) can be defined as the addition of the minuend [A] and the
	// additive inverse of the subtrahend [B].
	
	
	char* B2;
	int flag = 2;
	//flipSign(B);
	if( B[0]=='+' )	{
		
		B[0] = '-';
		flag = 1;
		B2 = B;
	}
	else if( B[0] == '-' )	{

		B[0] = '+';
		flag = 0;
		B2 = B;
	}

	if( flag==2 )	{
	
		char* b = (char*) g( malloc( strlen(B) + 2 ) );
		b[0] = '-';
		strcat( b,B );
		
		B2 = b;
	}

	
	char* _ = ADD( A, B2 );
	
	if( flag==1 )
		B[0] = '+';
	else if( flag==0 )
		B[0] = '-';

	return _;
}
 
char* DIV( char* A_, char* B_ )	{
	
	char* A = A_;
	char* B = B_;
	int lenA = strlen( A );
	int lenB = strlen( B );
	
	int LOOPS = 0;
	
	int longerOperand = (lenA<lenB)?lenB:lenA;
	
	int INIT_ASSUMPT = longerOperand * 2;
	
	char* B_rvalue = getstring( "0" );

	char* R;
	char* R2;
	char* C = (char*) g( malloc( INIT_ASSUMPT ) );
	char* V;
	
	char t;
	char dd;
	int carat;

	t = A[ lenB ];
	A[ lenB ] = '\0';
	R = getstring( A );
	A[ lenB ] = t;


	if( cmp_dstr( R,B ) < 0 )	{
		
		t = A[ lenB+1 ];
		A[ lenB+1 ] = '\0';
		
		freeRef( R );
		R = getstring( A );		
		A[ lenB+1 ] = t;
	}
	
	char* dd_str = getstring( "0" );
	carat = strlen( R ) - 1;
	int i;
	while( 1 )	{
		
		for( i=0; i<9; i++ )	{
			
			B_rvalue[0] = '0' + i;
			
			V = MUL( R, B_rvalue );
			
			if( cmp_dstr( V, R ) > 0 )	{

				i--;
				break;
			}
			
			freeRef( V );
		}
		
		freeRef( V );
		
		
		B_rvalue[0] = '0' + i;		
		V = MUL( R, B_rvalue );
		
		R2 = SUB( R, V );
		
		freeRef( V );
		freeRef( R );
		R = R2;
		
		if( cmp_dstr( R, "0" ) == 0 )
			if( carat >= lenA )
				break;
		
		C[ carat++ ] = '0' + i;
		
		if( carat >= INIT_ASSUMPT )	{
			
			char* _ = (char*) g( malloc( INIT_ASSUMPT * 2 + 1 ) );
			INIT_ASSUMPT *= 2;
			strcpy( _, C );
			freeRef( C );
			
			C = _;
		}
		if( carat >= lenA )
			dd = '0';
		else
			dd = A[ carat ];

		dd_str[0] = dd;
		
		char* _temp_ = (char*) g( malloc( strlen(R) + 2 ) );		
		strcpy( _temp_, R );
		strcat( _temp_, dd_str );
		
		freeRef( R );
		R = _temp_;
		
		LOOPS++;
		
		if( LOOPS>MAX_LOOPS )
			break;
	}

	C[ carat ] = '\0';
	
	return C;
}

char* MUL( char* A_, char* B_ )	{

	char asign = '+';
	char bsign = '+';
	
	char* A = A_;
	char* B = B_;
	
	if( (A_[0]=='+') || (A_[0]=='-') )	{
		
		asign = A_[0];
		A++;
	}
	
	if( (B_[0]=='+') || (B_[0]=='-') )	{
	
		bsign = B_[0];
		B++;
	}
	
	
	int lenA = strlen( A );
	int lenB = strlen( B );

	char** resultRows = (char**) g( malloc( sizeof(char*) * lenB ) );
	
	int lenC = lenA + 1;
	int maxTrailingDigits = lenB - 1;
	lenC += maxTrailingDigits;
	
	int lenT = lenC + 1;	
	
	int x;
	int y;
	int z;
	int t;
	int r = 0;
	
	char a;
	char b;
	char c;
	int trailing_zeroes = 0;
	
	char* C = (char*) g( malloc( lenC + 1 ) );
	
	char* T = (char*) g( malloc( lenT + 1 ) );
	memset( T,'0', lenT );
	
	t = lenT-1;

	for( y=lenB-1; y>=0; y-- )	{

		memset( C, '0', lenC );

		z = lenC-1-trailing_zeroes;

		b = B[ y ] - '0';

		for( x=lenA-1; x>=0; x--, z-- )	{
	
			a = A[ x ] - '0';
		
			c = b * a;
			c += C[ z ] - '0';
			
			if( c > 9 )	{
		
				C[ z ] = (c % 10) + '0';
		
				while( (c-=10) >= 0 )
					C[ z-1 ] += 1;
			}
			else	{
				
				C[ z ] = c + '0';
			}
		}

		trailing_zeroes++;
		resultRows[ r++ ] = getstring( C );
	}

	if( r==1 )
		resultRows[ r++ ] = getstring( "0" );
	
	
	// now we add all result rows together.
	if( r%2 != 0 )	{
			// an odd number of result rows (0-based!).
		resultRows[ r++ ] = getstring( "0" );
	}

	for( y=0; y<r; y+=2 )	{

		signed za = strlen( resultRows[y] ) - 1;
		signed zb = strlen( resultRows[y+1]) - 1;

		t = lenT - 1;

		while( 1 )	{
			
			if( za<0 )
				a = 0;
			else
				a = resultRows[y][za] - '0';
			
			if( zb<0 )
				b = 0;
			else
				b = resultRows[y+1][zb] - '0';
			
			c = a + b;
			c += T[ t ];
			

			if( c > '9' )	{
				
				c -= '0';
				T[ t ] = '0' + (c % 10);
				
				while( (c-=10) >= 0 )
					T[ t-1 ] += 1;
			}
			else	{
				
				T[ t ] = c;
			}
			
			t--;
			za--;
			zb--;
			
			//if( (za<0) && (zb<0) )
				if( t==0 )
					break;
		}
	}
	
	// check each entry in T for >9 overflow
	for( t=lenT-1; t > 0; t-- )	{
	
		if( T[ t ] > '9' )	{

			c = T[ t ] - '0';
			T[ t ] = '0' + (c % 10);

			while( (c-=10) >= 0 )
				T[ t-1 ] += 1;
		}
	}
	
	char* T_ = galloc( strlen(T) + 2 );
	T_[0] = ( (asign==bsign) ? '+' : '-' );
	
	strcat( T_,T );
	freeRef( T );
	T = T_;
	
	return T;
}

char* ADD( char* A_, char* B_ )	{
	
	char* A = A_;
	char* B = B_;
	char* C;
	
	int negb = 1;
	int nega = 1;

	char sign = '+';
	
	if( *A == '-' )	{

		nega = -1;
	}
	
	if( *B == '-' )	{
	
		negb = -1;
	}

	if( nega!=negb )	{
		
		if( (A[0]=='+') || (A[0]=='-') )
			A++;
		
		if( (B[0]=='+') || (B[0]=='-') )
			B++;
		
		int cs;
		if( (cs=cmp_dstr( A,B ))<0 )
			sign = (negb==-1?'-':'+');
		else
			sign = (nega==-1?'-':'+');
		
		if( cs<0 )	{
			
			// sub the smaller from the larger.
			char* ttt;
			ttt = A;
			A = B;
			B = ttt;
		}
		
		
		C = SUB( A,B );
		
		switch( C[0] )	{
			
			break;
			case '+':
			case '-':
				C[0] = sign;
				break;
			
			break;
			default:
				char* D = (char*) galloc( strlen(C)+2 );
				D[0] = sign;
				D[1] = '\0';
				strcat( D,C );
				freeRef( C );
				C = D;
				break;
		}
		
		return C;
	}

	if( (A[0]=='+') || (A[0]=='-') )
		A++;
	
	if( (B[0]=='+') || (B[0]=='-') )
		B++;

	int lenA = strlen( A );
	int lenB = strlen( B );
	
	sign = (nega==-1)?'-':'+';
	
	int lenC = lenA>lenB?lenA:lenB;
	lenC += 1;
	
	C = (char*) g( malloc( lenC + 1 ) );
	memset( C, '0', lenC );
	

	
	char a;
	char b;
	char c;
	
	int x = lenA - 1;
	int y = lenB - 1;
	int z = lenC - 1;
	
	while( 1 )	{

		if( z<1 )
			break;


		if( x<0 )
			a = 0;
		else
			a = A[ x ] - '0';
		

		if( y<0 )
			b = 0;
		else
			b = B[ y ] - '0';
		

		//b *= negb;
		a += C[ z ] - '0';
		
		loop:
		
		//a *= nega;
		c = a + b;
		
		if( c < 0 )	{
			
			C[ z-1 ] -= 1;
			
			//a *= nega;
			a += 10;
			goto loop; //c = a + b;
		}
		
		if( c > 9 )	{
			
			C[ z ] = '0' + (c % 10);
			
			while( (c-=10) >= 0 )
				C[ z-1 ] += 1;
		}
		else	{
			
			C[ z ] = '0' + c;
		}


		x--;
		y--;
		z--;
	}

	char* temp;

	if( C[ 0 ] < '0' )	{
	
		// result is negative.
		c = '0' - C[ 0 ];
		C[ 0 ] = '0' + c;
		
		//sign = '-';
	}

	loop2:

	if( C[ 0 ] > '9' )	{
		
		c = C[ 0 ] - '0';
		// 1. left-extend C
		
		temp = (char*) g( malloc( lenC + 2 ) );
		lenC += 1;
		
		temp[0] = '0';
		z = 1;
		while( *C != '\0' )
			temp[z++] = *(C++);

		temp[z] = '\0';
		freeRef( C );
		C = temp;
		// ...
		// then...
		
		C[ 1 ] = (c % 10) + '0';

		while( (c-=10) >= 0 )
			C[ 0 ] += 1;

		goto loop2;
	}
	
	char* t;
	char* _ = (char*) g( malloc( lenC + 2 ) );
	_[0] = sign;
	
	t = C;
	z = 1;
	while( *C != '\0' )
		_[z++] = *(C++);
	
	_[z] = '\0';
	
	freeRef( t );
	
	return _;
	
}


char* getstring( char* _ )	{

	char* str = (char*) g( malloc( strlen(_) ) );
	char* temp = str;

	while( *_ != '\0' )	{
		
		*temp = *_;
		temp++;
		_++;
	}
	
	*temp = '\0';
	
	return str;
}

int cmp_dstr( char* a, char* b )	{

	if( a == NULL )
		printf( "Warning. Arg 'a' in cmp_dstr (\"%s\":%d) is a NULL ptr.\n", __FILE__, __LINE__ );
	if( b == NULL )
		printf( "Warning. Arg 'b' in cmp_dstr (\"%s\":%d) is a NULL ptr.\n", __FILE__, __LINE__ );
	
	fflush( stdout );
	
	int i = 0;
	
	while( a[i++]=='0' )
		++a;
	
	i = 0;
	while( b[i++]=='0' )
		++b;
	
	int len_a = strlen( a );
	int len_b = strlen( b );
	
	if( len_a<len_b )
		return -1;

	if( len_a>len_b )
		return +1;

	for( int test=0; test<len_a; test++ )
		if( a[test]>b[test] )
			return +1;
		else if( a[test]<b[test] )
			return -1;
	
	return 0;	
}

