#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <ctype.h>

using namespace std;

void usage( char * pcError = 0 )
{
    if ( pcError )
        printf( "Error: %s\n", pcError );

    printf( "text file difference\n" );
    printf( "  usage: mydiff [-c:X] file1 file2\n" );
    printf( "  arguments:  -c:X   Number of differences before stopping, default is 10\n" );
    exit( 1 );
} //usage

int main( int argc, char * argv[] )
{
    char * pA = 0;
    char * pB = 0;
    long long count = 10;

    for ( int i = 1; i < argc; i++ )
    {
        char *parg = argv[i];
        char c = *parg;

        if ( '-' == c || '/' == c )
        {
            char ca = tolower( parg[1] );

            if ( 'c' == ca )
            {
                if ( ':' == parg[2] )
                    count = _strtoui64( parg + 3 , 0, 10 );
                else
                    usage( "colon required after c argument" );
            }
            else if ( '?' == ca )
                usage();
            else
                usage( "invalid argument specified" );
        }
        else
        {
            if ( 0 == pA )
                pA = parg;
            else if ( 0 == pB )
                pB = parg;
            else
                usage( "too many arguments" );
        }
    }

    if ( !pA || !pB )
        usage( "two files not specified\n" );

    FILE * fpA = fopen( pA, "r" );
    if ( !fpA )
        usage( "can't open first file" );

    FILE * fpB = fopen( pB, "r" );
    if ( !fpB )
        usage( "can't open second file" );

    static char acA[1000];
    static char acB[1000];
    long long differences = 0;
    long long lines = 0;
    
    do
    {
        char * pca = fgets( acA, _countof( acA ), fpA );
        char * pcb = fgets( acB, _countof( acB ), fpB );

        if ( !pca && !pcb && !differences )
        {
            printf( "files are identical; looked at %llu lines\n", lines );
            exit( 0 );
        }

        if ( !pca )
        {
            printf( "out of lines in first file; looked at %llu\n", lines );
            exit( 1 );
        }

        if ( !pcb )
        {
            printf( "out of lines in second file; looked at %llu\n", lines );
            exit( 1 );
        }

        lines++;

        if ( strcmp( acA, acB ) )
        {
            printf( "line %llu\n", lines );
            printf( "  a: %s", acA );
            printf( "  b: %s", acB );

            differences++;
            if ( differences > count )
            {
                printf( "stopping after %llu differences\n", count );
                exit( 1 );
            }
        }
    } while( true );

    fclose( fpB );
    fclose( fpA );

    return 0;
} //main

