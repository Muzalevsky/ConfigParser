#include <config_parser_api.h>
#include <filterparser.h>

#define FILTER_CONF_PREFIX    "[filter_config_api]: "

static std::string class_filter_get_path_string()
{
    std::string filterConfigPath = "/data/config/" + CLASS_FILTER_CONFIG_FNAME;
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

    std::vector<int> empty;

    for( auto it = parser.sections.begin(); it != parser.sections.end(); it++ ) {
//        cout << "(*it).name " << (*it).name << " classifier_name " << classifier_name << endl;
        if ( (*it).name == classifier_name ) {
            cout << "Found " << classifier_name << endl;
            return (*it).classIdx;
        }
    }

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
    if ( indexes.empty() ) {
        return ENODATA;
    }

    for ( size_t i = 0; i < indexes.size(); i++ ) {
        array[i] = indexes[i];
    }

    return EOK;
}

/*
 * The following section was created for core.conf parsing
 */
#if 0

static int config_section_get_value( config_parser_t ctx, std::string section_name, std::string key, char *out_val, size_t size )
{
    if ( !out_val || !ctx ) {
        return EINVAL;
    }

    ConfigParser *parser = (ConfigParser*)ctx;
    std::string optval;
    parser->getValue( section_name, std::string(key), optval );
    if ( optval.size() > size ) {
        return ENOMEM;
    }

    if ( !optval.empty() ) {
        sprintf( out_val, "%s", optval.c_str() );
        return EOK;
    }

    return ENODATA;
}

int config_core_init( config_parser_t *ctx )
{
    ConfigParser *parser = new ConfigParser( core_config_get_path_string(), ios_base::in, true );
    if ( parser->loaded ) {
        parser->readFile();
        *ctx = parser;
        return EOK;
    }

    return EINVAL;
}

bool config_core_exists( config_parser_t ctx )
{
    ConfigParser *parser = (ConfigParser*)ctx;
    return parser->loaded;
}

int config_core_get_value( config_parser_t ctx, const char *key, char *out_val, size_t size )
{
    return config_section_get_value( ctx, std::string("core"), std::string(key), out_val, size );
}

int config_driver_get_value( config_parser_t ctx, const char *driver_name, const char *key, char *out_val, size_t size )
{
    return config_section_get_value( ctx, std::string(driver_name), std::string(key), out_val, size );
}


void config_core_free( config_parser_t *ctx )
{
    if ( ctx ) {
        ConfigParser *parser = (ConfigParser*)ctx;
        delete parser;
        *ctx = NULL;
    }
}

#endif


