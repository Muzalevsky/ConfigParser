#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <vector>
#include <unistd.h>
#include <config_parser_api.h>

int test_case = 1;
void parse_options( int argc, char **argv )
{
    int c;
    opterr = 0;
    while ( (c = getopt( argc, argv, "t:" )) != -1 )
        switch ( c )
        {
            case 't':
                test_case = strtoul( optarg, NULL, 0 );
                break;
            case '?':
            default:
                break;
        }
}

int main( int argc, char **argv )
{
    parse_options(argc, argv);
    printf("\tTest case: %d\n", test_case);

    switch ( test_case ) {
        case 1:
        {
            /*
             * First case: create testing filter.cfg, get section, check its size
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
                printf("\tfailed\n");
                return 1;
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
            return 0;
            break;
        }

        default:
        {
            printf("\tfailed\n");
            return 1;
            break;
        }
    }
}
