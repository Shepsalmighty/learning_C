#include "GF.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int main ( void )
{ 
    char * c_filename = "GF_lookup_tables.c";
    char * h_filename = "GF_lookup_tables.h";

    uint8_t GF_array[256][256];
    uint8_t unGF_array[256][256];

    for( size_t drive = 0; drive < 256; drive ++)
    {   
        for( size_t byte = 0; byte < 256; byte ++)
        {   
            uint8_t GF_value = GF(drive, byte);
            GF_array[drive][byte] = GF_value;
            unGF_array[drive][GF_value] = byte;
        }
    }
    
    FILE * file = fopen( c_filename, "w");
    if( ! file )
    {   
        perror( c_filename );
        exit(EXIT_FAILURE);
    }
    //write Galois Feild values to GF_array
    fprintf( file, "#include \"%s\"\n" , h_filename );
    fprintf( file, "uint8_t GF_array[256][256]=\n{\n" );
    for( size_t drive = 0; drive < 256; drive ++)
    {   
        fprintf( file, "\t{");
        
        for( size_t byte = 0; byte < 256; byte ++)
        {
            fprintf( file, "0x%02X, ", GF_array[drive][byte] );
        }
        fprintf(file, "},\n");
    }
    fprintf( file, "};\n");
    
    //write un_Galois Feilded values to unGF_array for easy look up later
    fprintf( file, "uint8_t unGF_array[256][256]=\n{\n" );
    for( size_t drive = 0; drive < 256; drive ++)
    {   
        fprintf( file, "\t{");
        for( size_t GF_value = 0; GF_value < 256; GF_value ++)
        {
            fprintf( file, "0x%02X, ", unGF_array[drive][GF_value] );
        }
        fprintf(file, "},\n");
    }
    fprintf( file, "};\n");

    fclose( file );

    FILE * h_file = fopen( h_filename, "w");
    if( ! h_file )
    {   
        perror( h_filename );
        exit(EXIT_FAILURE);
    }
    
    fprintf( h_file, "#ifndef GF_LOOKUP_TABLES_H\n");
    fprintf( h_file, "\t#define GF_LOOKUP_TABLES_H\n");
    fprintf( h_file, "\t#include<stdint.h>\n");
    fprintf( h_file, "\textern uint8_t GF_array[256][256];\n");
    fprintf( h_file, "\textern uint8_t unGF_array[256][256];\n");
    fprintf( h_file, "#endif // ! GF_LOOKUP_TABLES_H\n");

    fclose( h_file );

    exit( EXIT_SUCCESS );


}
