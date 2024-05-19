#include "sle/editloop.h"
#include "sle/mainscreen.h"
#include <memory>

namespace sle 
{

class EditLoop::NormalMode
    : public EditLoop::ModeBase
{
public:
    explicit NormalMode(
        EditLoop* parent)
        :
        parent_(parent),
        c_(parent->c_)
    {}

    bool handle() override
    {
        auto scr = c_->scr;
        bool endLoop = false;
        bool quitProg = false;

        while (true)
        {
            char ch = scr->getCh();

            switch (ch)
            {
            case 'z':
                endLoop = true;
                quitProg = true;
                break;
            case 'i':
                parent_->changeMode(Mode::INSERT);
                endLoop = true;
                quitProg = false;
                break;
            case 'h':
            {
                auto cursor = c_->buf->move(Direction::LEFT);
                c_->scr->moveCursor(c_->scr->toScrCoord(cursor));
                break;
            }
            case '[':
                c_->scr->test();
                break;
            default:
                break;
            }
        
            if (endLoop)
               return quitProg;
        }
    }

private:
    EditLoop* parent_ = nullptr;
    ContextPtr c_ = nullptr;
};

class EditLoop::InsertMode
    : public EditLoop::ModeBase
{
public:
    explicit InsertMode(
        EditLoop* parent)
        :
        parent_(parent),
        c_(parent->c_)
    {}

    bool handle() override
    {
        auto scr = c_->scr;
        bool endLoop = false;

        while (true)
        {
            char ch = scr->getCh();
            
            switch (ch)
            {
            case 'q':
                parent_->changeMode(Mode::NORMAL);
                endLoop = true;
                break;
            case '\n':
                c_->buf->addCh(ch);
                c_->scr->paintCh(ch);
                break;
            default:
                if (c_->scr->paintCh(ch))
                    c_->buf->addCh(ch);
                break;
            }

            if (endLoop)
                return false;
        }
    }

private:
    EditLoop* parent_ = nullptr;
    ContextPtr c_ = nullptr;
};

EditLoop::EditLoop(
    const ContextPtr& context)
    :
    c_(context)
{}

void EditLoop::init()
{
    modePool_[Mode::NORMAL] = std::make_shared<NormalMode>(this);
    modePool_[Mode::INSERT] = std::make_shared<InsertMode>(this);
    changeMode(Mode::NORMAL);
    while (!mode_->handle())
        mode_->handle();
}

void EditLoop::changeMode(
    const Mode& newMode)
{
    mode_ = modePool_.at(newMode);
}
}
