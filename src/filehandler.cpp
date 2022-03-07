#include "sle/filehandler.h"
#include <fstream>
#include <vector>
#include <string>

#include <iostream>

namespace sle {

class FileHandlerImpl :
    public FileHandler {
public:
    static std::unique_ptr<FileHandlerImpl> create(std::string path)
    {
        return std::unique_ptr<FileHandlerImpl>(new FileHandlerImpl(path));
    }
    
    virtual ~FileHandlerImpl() = default;
    
    StrPacket getContents() override;
    
    void saveFile() override;
    
    void changePath(std::string path) override;

private:
    FileHandlerImpl(std::string path) : path_(path)
    {}
    
    std::string path_;
};

FileHandlerPtr FileHandler::create(std::string path)
{
    return FileHandlerImpl::create(path);
}

StrPacket FileHandlerImpl::getContents()
{

    std::ifstream fin;
    fin.open("testtxt");

    std::string line;
    StrPacket data;

    while (getline(fin, line)) {
	    if (fin.peek() != EOF)
		    line.append("\n");
	    data.push_back(line);
    }

    fin.close();
    
    return data;
}

void FileHandlerImpl::saveFile()
{}

void FileHandlerImpl::changePath(std::string path)
{}

}
