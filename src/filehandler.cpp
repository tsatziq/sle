#include "sle/filehandler.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace sle
{

// conffiin sittenki suoraan maarita hdr/src path pareja
// kirjota ome flex parseri conffille? ehk json kuiteski

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

    buf->appendTxt(txt);
    path_ = path;
}

const std::string FileHandler::getFilePair()
{
    static constexpr const char hdrs[] = "/Users/tsatziq/code/sle/include/sle/";
    static constexpr const char srcs[] = "/Users/tsatziq/code/sle/src/";

    auto pos = path_.find('.');
    if (pos == std::string::npos)
        return {};

    auto name = path_.substr(0, pos);
    auto ext = path_.substr(pos + 1);

    std::string pair;
    if (ext == "cpp")
        pair = hdrs + name + ".h";
    else if (ext == "h")
        pair = srcs + name + ".cpp";
    else
        return {};

    if (std::filesystem::exists(pair))
    {
        path_ = pair;
        return pair;
    }
    else
        return {};
}

}
