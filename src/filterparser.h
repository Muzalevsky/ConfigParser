#ifndef FILTERPARSER_H
#define FILTERPARSER_H

/*
 * MuzAl 11/09/2020
 * This is implementation of the class filtration config file parser
 */

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

typedef std::pair<std::string, std::string> KeyVal;

class Section {
public:
    Section() {}
    void _showDebugInfo();
    void checkServiceOptions(std::string& optname, std::string& optval);

    std::string name;
    std::string type;
    std::vector<int>    classIdx;

    std::vector<KeyVal> options;
};

class ConfigParser
{
public:
    ConfigParser(std::string filepath, std::ios_base::openmode mode, bool useCoreConfig );
    ~ConfigParser();

    void                    finishNewFilter();
    void                    readFile();
    int                     writeNewFilter();
    int                     writeSector(std::string clasifier_name, std::string type, std::vector<int>& classIdx );

    void                    getValue( std::string section_name, std::string optname, std::string& optval );

    bool                    loaded;
    std::vector<Section>    sections;

private:
    void checkServiceTags( std::string& line );
    bool hasLineServiceTag(Section& section, std::string& line);

    int readLine( std::string& line );
    void readSection();
    bool parseValue(std::string &from, std::string find, std::string &to );
    bool parseClasses(std::string &line, std::string find, std::vector<int> &vector);
    void trimSpaces( std::string &str );

    bool            _coreMode;
    bool            _debug;
    bool            _begin_filter;
    bool            _begin_sector;
    std::fstream    _config_file;

    std::string     _endConfigMark;
    std::string     _endSectorMark;
    std::string     _filePath;
    std::string     _startConfigMark;
    std::string     _startSectorMark;
};


#endif // FILTERPARSER_H