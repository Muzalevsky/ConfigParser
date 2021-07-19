#include <config_parser_api.h>
#include <filterparser.h>

#define CLASS_FILTER_CONFIG_PATH "/data/config/filter.conf"
#define CORE_CONFIG_PATH         "/data/config/core.conf"
#define FILTER_CONF_PREFIX       "[filter_config_api]: "

static std::string class_filter_get_path_string()
{
    std::string filterConfigPath = CLASS_FILTER_CONFIG_PATH;
    return filterConfigPath;
}

int class_filter_exists()
{
    ConfigParser parser( class_filter_get_path_string(), ios_base::in, false );
    return (int)parser.loaded;
}

std::vector<int> class_filter_read_sector( std::string classifier_name )
{
    ConfigParser parser( class_filter_get_path_string(), ios_base::in, false );
    parser.readFile();

    for( auto it = parser.sections.begin(); it != parser.sections.end(); it++ ) {
        if ( (*it).name == classifier_name ) {
            return (*it).classIdx;
        }
    }

    std::vector<int> empty;
    empty.resize(0);
    return empty;
}

void class_filter_write_header()
{
    ConfigParser parser( class_filter_get_path_string(), ios_base::out | ios_base::trunc, false );
    parser.writeNewFilter();
}

void class_filter_write_sector( std::string classifier_name, std::string type, std::vector<int> classIndex )
{
    ConfigParser parser( class_filter_get_path_string(), ios_base::out | ios_base::app, false );
    parser.writeSector( classifier_name, type, classIndex );
}

void class_filter_write_footer()
{
    ConfigParser parser( class_filter_get_path_string(), ios_base::out | ios_base::app, false );
    parser.finishNewFilter();
}

size_t class_filter_get_classes_size( const char *classifier_name )
{
    std::vector<int> indexes = class_filter_read_sector( std::string(classifier_name) );
    return indexes.size();
}

int class_filter_get_classes_data( const char *classifier_name, int *array )
{
    std::vector<int> indexes = class_filter_read_sector( std::string(classifier_name) );
    if ( indexes.empty() )
        return ENODATA;

    for ( size_t i = 0; i < indexes.size(); i++ )
        array[i] = indexes[i];

    return EOK;
}

/*
 * The following section was created for core.conf parsing
 */
#if 1

static std::string core_config_get_path_string()
{
    std::string filterConfigPath = CORE_CONFIG_PATH;
    return filterConfigPath;
}

int core_config_exists()
{
    ConfigParser parser( core_config_get_path_string(), ios_base::in, true );
    return (int) parser.loaded;
}

static int config_section_get_value( std::string section_name, std::string key, char *out_val, size_t size )
{
    if ( !out_val )
        return EINVAL;

    ConfigParser parser( core_config_get_path_string(), ios_base::in, true );
    parser.readFile();
    std::string optval;
    parser.getValue( section_name, std::string(key), optval );
    if ( optval.size() > size )
        return ENOMEM;

    if ( !optval.empty() ) {
        sprintf( out_val, "%s", optval.c_str() );
        return EOK;
    }

    return ENODATA;
}

int core_config_get_value( const char *key, char *out_val, size_t size )
{
    return config_section_get_value( std::string("core"), std::string(key), out_val, size );
}

int core_config_get_driver_value( const char *driver_name, const char *key, char *out_val, size_t size )
{
    return config_section_get_value( std::string(driver_name), std::string(key), out_val, size );
}

int core_config_set_driver_value( const char *driver_name, const char *key, const char *out_val)
{
    if ( !out_val )
        return EINVAL;

    ConfigParser parser( core_config_get_path_string(), ios_base::in, true );
    return parser.updateValue(std::string(driver_name), std::string(key), std::string(out_val));
}

#endif
