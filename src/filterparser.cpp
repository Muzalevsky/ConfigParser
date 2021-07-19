#include "filterparser.h"

#include <algorithm>
#include <iterator>
#include <sstream>

//using namespace std;

ConfigParser::ConfigParser(std::string filepath, std::ios_base::openmode mode, bool useCoreConfig = false ) :
    _coreMode(useCoreConfig),
    _debug(false),
    _begin_filter(false),
    _begin_sector(false),
    _filePath(filepath)
{
    _config_file.open( _filePath, mode );
    loaded = _config_file.is_open();
    if ( !loaded ) {
        std::cout << "Failed to open file " << filepath << std::endl;
        return;
    }

    if ( _coreMode ) {
        _endConfigMark = "end core";
        _endSectorMark = "end driver";
        _startConfigMark = "begin core";
        _startSectorMark = "begin driver";
    } else {
        _endConfigMark = "end filter";
        _endSectorMark = "end sector";
        _startConfigMark = "begin filter";
        _startSectorMark = "begin sector";
    }
}

ConfigParser::~ConfigParser()
{
    if (_config_file.is_open())
        _config_file.close();
}

int ConfigParser::readLine( std::string& line )
{
    getline(_config_file, line);

    while ( line.find_first_of('#') != std::string::npos ) {
        if ( _debug )
            std::cout << "Comment line: " << line << std::endl;

        getline(_config_file, line);
    }

    while ( line.empty() ) {
        if ( _debug )
            std::cout << std::endl;

        getline(_config_file, line);
    }

    if ( _config_file.peek() == EOF )
        return ENODATA;

    checkServiceTags(line);

    return EOK;
}

void ConfigParser::readFile()
{
    std::string line;

    while ( readLine( line ) == EOK ) {
        if ( _begin_filter ) {
            if ( _begin_sector ) {
                readSection();
            }
        }
    }
}

void ConfigParser::readSection()
{
    Section section;
    std::string line;

    while ( readLine( line ) == EOK ) {
        /*
         * Fill class filtering tags
         */
        if ( !hasLineServiceTag(section, line) ) {
            size_t tokenPos = line.find('=');
            if ( tokenPos != std::string::npos ) {
                std::string optname = line.substr(0, tokenPos);
                std::string optval = line.substr( tokenPos + 1 );

                /*
                 * Remove extra spaces
                 */
                optname.erase(std::remove_if(optname.begin(), optname.end(), isspace), optname.end());
                optval.erase(std::remove_if(optval.begin(), optval.end(), isspace), optval.end());


                section.options.emplace_back( KeyVal( optname, optval ) );
            }
        }

        if ( !_begin_sector )
            break;
    }

    sections.emplace_back(section);

    if ( _debug ) {
        std::cout << "New section " << section.name << std::endl;
        section._showDebugInfo();
    }

}

bool ConfigParser::parseValue( std::string& from, std::string find, std::string& to )
{
    std::size_t found = from.find(find);
    if (found != std::string::npos) {
        to = from.substr(found + find.length(), std::string::npos);
        trimSpaces( to );
        return true;
    }
    return false;
}

bool ConfigParser::parseClasses(std::string& line, std::string find, std::vector<int>& vector )
{
    std::size_t found = line.find(find);
    if (found != std::string::npos) {
        std::string clear_str = line.substr(found + find.length(), std::string::npos );

        std::string token;
        std::istringstream iss(clear_str);
        while ( getline(iss, token, ',') ) {
            vector.push_back(stoi(token));
        }

        return true;
    }
    return false;
}

void ConfigParser::getValue( std::string section_name, std::string optname, std::string& optval )
{
    for ( auto section : sections ) {
        if ( section.name == section_name ) {
            for ( auto option : section.options ) {
                if ( option.first == optname ) {
                        optval = option.second;
                }
            }
        }
    }
}

void Section::_showDebugInfo()
{
    std::cout << "Name " << name << std::endl << "Type " << type << std::endl << "Include: ";
    for ( auto it = classIdx.begin(); it != classIdx.end(); it++ )
        std::cout << *it << " ";

    std::cout << std::endl;

    for ( auto entry : options ) {
        std::cout << entry.first << "_=_" << entry.second << std::endl;
    }
}

void ConfigParser::checkServiceTags( std::string& line )
{
    if ( line.find(_startConfigMark) != std::string::npos ) {
        _begin_filter = true;
        if ( _debug ) { std::cout << _startConfigMark << std::endl; }
    }

    if ( line.find(_startSectorMark) != std::string::npos ) {
        _begin_sector = true;
        if ( _debug ) { std::cout << _startSectorMark << std::endl; }
    }

    if ( line.find(_endConfigMark) != std::string::npos ) {
        _begin_filter = false;
        if ( _debug ) { std::cout << _endConfigMark << std::endl; }
    }

    if (line.find(_endSectorMark) != std::string::npos) {
        _begin_sector = false;
         if ( _debug ) { std::cout << _endSectorMark << std::endl; }
    }
}

bool ConfigParser::hasLineServiceTag(Section& section, std::string& line)
{
    return ( parseValue( line, std::string("name = "), section.name ) ||
             parseValue( line, std::string("type = "), section.type ) ||
             parseClasses( line, std::string("include = "), section.classIdx ) );
}


int ConfigParser::writeNewFilter()
{
    _config_file << "# This is class filter file" << std::endl;
    _config_file << "# for map converting. Do not modify it manually." << std::endl;
    _config_file << "# Use filter-generator instead." << std::endl;

    _config_file << _startConfigMark << std::endl;
}

int ConfigParser::writeSector( std::string clasifier_name, std::string type, std::vector<int>& classIdx )
{
    _config_file << _startSectorMark << std::endl;

    _config_file << "\tname = " << clasifier_name  << std::endl;
    _config_file << "\ttype = " << type  << std::endl;
    _config_file << "\tinclude = ";

    std::string classes;
    bool firstIdx = true;
    for ( auto it = classIdx.begin(); it !=classIdx.end(); it++ ) {
        if (!firstIdx)
            classes += ", ";

        classes += std::to_string(*it);
        firstIdx = false;
    }

    _config_file << classes << std::endl;

    _config_file << _endSectorMark << std::endl;
    return EOK;
}

void ConfigParser::finishNewFilter()
{
    _config_file << _endConfigMark << std::endl;
}

void ConfigParser::trimSpaces( std::string &str )
{
    str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
}
