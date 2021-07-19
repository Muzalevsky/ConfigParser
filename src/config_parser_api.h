#ifndef CONFIG_PARSER_API_H
#define CONFIG_PARSER_API_H

#ifdef __cplusplus

#include <vector>
#include <string>

std::vector<int>    class_filter_read_sector( std::string classifier_name );

void                class_filter_write_header();
void                class_filter_write_sector( std::string classifier_name, std::string type, std::vector<int> classIndex );
void                class_filter_write_footer();

#endif

#ifdef __cplusplus
extern "C" {
#endif

int     core_config_exists();
int     core_config_get_driver_value( const char *driver_name, const char *key, char *out_val, size_t size );
int     core_config_get_value( const char *key, char *out_val, size_t size );

int     core_config_set_driver_value( const char *driver_name, const char *key, const char *out_val );

int     class_filter_exists();
size_t  class_filter_get_classes_size( const char *classifier_name );
int     class_filter_get_classes_data( const char *classifier_name, int *array );

#ifdef __cplusplus
}
#endif

#endif // CONFIG_PARSER_API_H
