/**
 *
 * @file ini.hpp
 * @author William Reinhardt
 *
 * Contains functionality for reading and writing ini files.
 */

#ifndef INI_H
#define INI_H

#include "main.h"

class Ini_file{
    public:
        Ini_file(std::string path);
        Ini_file(const char * path);
        std::string get(std::string section, std::string key, std::string default_value);
        void set(std::string section, std::string key, std::string new_value);
        void save();
    private:
        const char * file_path;                                                 //The path that the ini file is located
        std::map<std::string, std::map<std::string, std::string> > contents;    //The contents of the ini file
        void parse_file();
        std::vector<std::string> split_string(std::string str, char delim);
};

#endif
