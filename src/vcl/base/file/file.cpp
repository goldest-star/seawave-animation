#include "file.hpp"

#include <fstream>
#include <iostream>
#include <cassert>

#include "../error/error.hpp"

namespace vcl
{

void assert_file_exist(const std::string& filename)
{
    // Open file
    std::ifstream stream(filename);

    // Abort program if the file cannot be opened
    if( !stream.is_open() ) {

        std::string msg =
                "Error cannot access the following file path \n    \""+filename+"\"\n\n"
                "    => If the file exists, you may need to adapt the path from which you run the program \n"
                "       Reminder: The program is exprected to run form the root directory\n"
                "       (In QtCreator: set projects/Run directory)\n"
                "       (In Command Line: change the directory such that you can access the file)\n"
                "       (In Windows: you need to copy the directory containing this data at the same place than your executable)";

        error_vcl(msg);
    }

    stream.close();
}

std::string read_file_text(const std::string& path)
{
    assert_file_exist(path);

    // Open file with pointer at last position
    std::ifstream stream(path, std::ios::ate);
    assert(stream.is_open());

    std::string full_text;

    // get position of the current pointer (last position)
    std::streampos const s = stream.tellg();

    // resize the buffer to handle the entire text
    full_text.resize( static_cast<size_t>(s) );

    // goes back to the beginning of the file and read the entire text
    stream.seekg(0,std::ios::beg);
    stream.read(&full_text[0],s);

    stream.close();
    assert(!stream.is_open());


    return full_text;

}

}
