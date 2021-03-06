#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include <errno.h>
#include <locale.h>
#include <string.h>
#include <sys/ioctl.h> 

struct fincore_result 
{
    long cached_size;
};

void fincore(char* path, struct fincore_result *result ) 
{

	int fd;
	struct stat file_stat;
	void *file_mmap;

	// vector result from mincore
	unsigned char *mincore_vec;

	// default page size of this OS
	size_t page_size = getpagesize();
	size_t page_index;

	// the number of pages that we have detected that are cached 
	size_t cached = 0;

	// by default the cached size is zero so initialize this member.
	result->cached_size = 0;

	fd = open( path, O_RDONLY );

	if ( fd == -1 ) 
	{
		char buff[1024];
		sprintf( buff, "Could not open file: %s", path );
		perror( buff );
		exit(EXIT_FAILURE);
	}

	if ( fstat( fd, &file_stat ) < 0 ) 
	{
		char buff[1024];
		sprintf( buff, "Could not stat file: %s", path );
		perror( buff );
		exit(EXIT_FAILURE);
	}

	file_mmap = mmap((void *)0, file_stat.st_size, PROT_NONE, MAP_SHARED, fd, 0 );
    
	if ( file_mmap == MAP_FAILED ) 
	{
		char buff[1024];
		sprintf( buff, "Could not mmap file: %s", path );
		perror( buff );
		exit(EXIT_FAILURE);
	}

	size_t calloc_size = (file_stat.st_size+page_size-1)/page_size;

	mincore_vec = calloc(1, calloc_size);

	if ( mincore_vec == NULL ) 
	{
		perror( "Could not calloc" );
		exit(EXIT_FAILURE);
	}

	if ( mincore(file_mmap, file_stat.st_size, mincore_vec) != 0 ) 
	{
		char buff[1024];
		sprintf( buff, "%s: Could not call mincore on file", path );
		perror( buff );
		exit(EXIT_FAILURE);
	}

	for (page_index = 0; page_index <= file_stat.st_size/page_size; page_index++) 
	{
		if (mincore_vec[page_index]&1) 
		{
			++cached;
		}

	}



	size_t cached_size = (long)cached * (long)page_size;

	// update structure members when done.
	result->cached_size = cached_size;

}

int main(int argc, char *argv[]) 
{

    long division_factor;
    long total_cached_size = 0;
    if (argc < 2) 
    {
        printf("ERROR - no arguments passed. Please pass in output units and path to the database files - e.g. /data/db/myDB.*.\n-b or -B for bytes\n-k or -K for KB\n-m or -M for MB\n-g or -G for GB\n");
	exit(EXIT_FAILURE);
    }    

    // Sets units to Bytes, KB, MB, GB
    if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-B") == 0) division_factor = 1;
    if (strcmp(argv[1], "-k") == 0 || strcmp(argv[1], "-K") == 0) division_factor = 1024; 
    if (strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "-M") == 0) division_factor = 1024 * 1024; 
    if (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "-G") == 0) division_factor = 1024 * 1024 * 1024 ;

    int arg_number;
    for (arg_number=2; arg_number < argc; arg_number++) {
        char* path = argv[arg_number];
        struct fincore_result result;

        fincore( path, &result );

        total_cached_size += result.cached_size;
    }

   	printf( "%'ld\n", total_cached_size / division_factor );

	return EXIT_SUCCESS;

}

