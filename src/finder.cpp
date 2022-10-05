#include "sle/cursor.h"
#include "sle/types.h"
#include <memory>

namespace sle {

class FinderImpl : public Finder
{
public:
    static std::unique_ptr<FinderImpl> create(
        const Text* txt)
    {
        return std::unique_ptr<FinderImpl>(new FinderImpl(txt));
    }

    ~FinderImpl();

private:
    FinderImpl(const Text* txt);

    const Text* txt_ = nullptr;
};

FinderImpl::FinderImpl(const Text* txt)
    : txt_(txt)
{}

FinderPtr Finder::create(const Text* txt)
{
    return FinderImpl::create(txt);
}

FinderImpl::~FinderImpl()
{}

}

