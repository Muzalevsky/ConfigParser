#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <vector>
#include <unistd.h>

#include <config_parser_api.h>

int main( int argc, char **argv )
{
    {
        /*
         * First case: create testing filter.conf, get section, check its size
         */
        const int test_size = 10;
        const char *test_section = "Test-section";
        const char *test_type = "Test-type";

        bool filter_exist = class_filter_exists();
        if ( !filter_exist ) {
            printf("Cannot restore filter file. I will create it by myself!\n");

            std::vector<int> indexes;
            for ( int i = 0; i < test_size; i++ ) {
                indexes.push_back(i);
            }

            class_filter_write_header();
            class_filter_write_sector( std::string(test_section), std::string(test_type), indexes );
            class_filter_write_footer();
        } else {
            printf("Filter file already exists!\n");
        }

        filter_exist = class_filter_exists();
        if ( !filter_exist ) {
            printf("Failed to find class filter. It's strange...\n");
            printf("\tfailed\n");
            return 1;
        }

        std::vector<int> indexes = class_filter_read_sector(test_section);
        if ( indexes.size() != test_size ) {
            printf("Classes count differs.\n");
            printf("\tfailed\n");
            return 1;
        }

        printf("\tsuccess\n");
    }
    {
        /*
         * Second case: trying to read some value from core.conf
         */
        bool isCoreConfig = core_config_exists();
        if ( !isCoreConfig )
        {
            printf("Cannot find core config!\n");
            printf("\tfailed\n");
            return 1;
        }

        char option_value[PATH_MAX];
        int status = core_config_get_value( "nReadThreads", &option_value[0], PATH_MAX );
        if ( status != 0 )
        {
            printf("Error while reading( status = %d )\n", status);
            printf("\tfailed\n");
            return 2;
        }

        printf( "nReadThreads = %d\n", atoi(option_value) );

        printf("\tsuccess\n");
    }

    return 0;
}
