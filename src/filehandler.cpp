#include "sle/filehandler.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace sle
{

FileHandler::FileHandler(
    const ContextPtr& context)
    :
    c_(context)
{}

void FileHandler::init()
{
}

void FileHandler::readFile(
    const std::string& path,
    Buffer* buf)
{
    std::filesystem::path p(path);

    if (!std::filesystem::exists(p))
        return;

    if (!std::filesystem::is_regular_file(p))
        return; 

    std::ifstream file(path);

    if (!file.is_open())
        return;

    std::vector<std::string> txt;
    std::string line;
    
    while (std::getline(file, line))
        txt.push_back(line.append(1, '\n'));
    
    file.close();

    buf->addText(txt);
}

}
