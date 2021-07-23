/**
 *
 * @file ini.hpp
 * @author William Reinhardt
 *
 * Contains functionality reading and writing ini files.
 */

#include "main.h"


//std::string constructor
Ini_file::Ini_file(std::string path){
    file_path = path.c_str();
    parse_file();
}

//C-string constructor
Ini_file::Ini_file(const char * path){
    file_path = path;
    parse_file();
}

/**
 *
 * Ini_file::get
 *
 * Gets the value found in the passed section and key, or a default value if no
 * value is found.
 *
 * @param section is the section that the value is located
 * @param key is the key that the value is located
 * @param default_value is the value used if no value is defined in the file
 * @return the value found in the section and key if it exists, default_value otherwise
 */
std::string Ini_file::get(std::string section, std::string key, std::string default_value = ""){
    if(contents.find(section) == contents.end() || contents[section].find(key) == contents[section].end())
        return default_value;
    else
        return contents[section][key];
}

/**
 *
 * Ini_file::set
 *
 * Sets the value at the passed section and key to the passed value.
 *
 * @param section is the section that the value is located
 * @param key is the key that the value is located
 * @param new_value is the value you want to set
 */
void Ini_file::set(std::string section, std::string key, std::string new_value){
    contents[section][key] = new_value;
}

/**
 *
 * Ini_file::save
 *
 * Saves the ini file.
 *
 */
void Ini_file::save(){
    std::ofstream file(file_path);
    for(auto it = contents.begin(); it != contents.end(); it++){
        file << '[' << it->first << ']' << std::endl;
        for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++){
            file << it2->first << "=\"" << it2->second << '"' << std::endl;
        }
    }
    file.close();
}

/**
 *
 * Ini_file::parse_file
 *
 * Stores the contents of the ini file into a map
 */
void Ini_file::parse_file(){
    std::string line;
    std::string section;
    std::string temp;
    std::vector<std::string> pair;
    std::ifstream file(file_path);
    if(file){
      while(getline(file, line)){
        if(line.front() == '[')
            section = line.substr(1, line.size() - 2);
        else{
            pair = split_string(line, '=');
            contents[section][pair[0]] = pair[1].substr(1, pair[1].size() - 2);
        }
      }
      file.close();
    }
}

/**
 *
 * Ini_file::split_string
 *
 * Splits a string based on the passed delimiter
 *
 * @param str is the string to be split
 * @param delim is the delimiter.
 */
std::vector<std::string> Ini_file::split_string(std::string str, char delim){
            std::vector<std::string> ret;
            std::stringstream ss(str);
            std::string temp;
            while (std::getline (ss, temp, delim))
                ret.push_back(temp);
            return ret;
}
