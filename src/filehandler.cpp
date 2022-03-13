#include "sle/filehandler.h"
#include <fstream>
#include <vector>
#include <string>

namespace sle {

class FileHandlerImpl : public FileHandler {
public:
    static std::unique_ptr<FileHandlerImpl> create()
    {
        return std::unique_ptr<FileHandlerImpl>(new FileHandlerImpl());
    }

    virtual ~FileHandlerImpl() = default;

    StrPacket getContents(const std::string& path) const override;

    void saveFile(const std::string& path) const override;

private:
    FileHandlerImpl()
    {}
};

FileHandlerPtr FileHandler::create()
{
    return FileHandlerImpl::create();
}

StrPacket FileHandlerImpl::getContents(const std::string& path) const
{
    std::ifstream fin;
    fin.open(path);

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

void FileHandlerImpl::saveFile(const std::string& path) const
{ (void)path; }

}
