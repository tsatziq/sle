#include "sle/finder.h"
#include "sle/types.h"
#include <memory>

namespace sle {

class FinderImpl : public Finder
{
public:
    static std::unique_ptr<FinderImpl> create(const Context* context)
    {
        return std::unique_ptr<FinderImpl>(new FinderImpl(context));
    }

    ~FinderImpl();

private:
    FinderImpl(const Context* context);

    const Context* c_ = nullptr;
};

FinderImpl::FinderImpl(const Context* context)
    : c_(context)
{}

FinderPtr Finder::create(const Context* context)
{
    return FinderImpl::create(context);
}

FinderImpl::~FinderImpl()
{}

}

