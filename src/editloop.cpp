#include "sle/editloop.h"
#include "sle/mainscreen.h"
#include <memory>

namespace sle 
{

// SEURAAVAKS: f ja w/b komennot ainaki.. siin on isompi homma.
// ehk tee joku Finder classi ja contextii se?
// VAI bufferin sisal ja siin on funktiota vaan sit paljo?
// vai voisko sit saada tehtya jollain regex funkkari noi kaikki?

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
            case 'a':
            {
                auto cursor = c_->buf->cursor();
                cursor->incX();
                cursor = c_->buf->setCursor(cursor);
                c_->scr->moveCursor(c_->scr->toScrCoord(cursor));

                parent_->changeMode(Mode::INSERT);
                endLoop = true;
                quitProg = false;
                break;
            }
            case 'f':
            {
                auto res = c_->buf->findCh('k');
                if (res)
                {
                    c_->buf->setCursor(res);
                    c_->scr->moveCursor(res);
                }
                break;
            }
            case 'h':
            {
                auto cursor = c_->buf->move(Direction::LEFT);
                c_->scr->moveCursor(c_->scr->toScrCoord(cursor));
                break;
            }
            case 'i':
                parent_->changeMode(Mode::INSERT);
                endLoop = true;
                quitProg = false;
                break;
            case 'j':
            {
                auto cursor = c_->buf->move(Direction::DOWN);
                auto& range = c_->visibleRange;

                if (!range->contains(cursor))
                {
                    int diff = cursor->y() - range->end()->y();
                    range->start()->setY(range->start()->y() + diff); 
                    range->end()->setY(cursor->y());

                    c_->scr->paint(c_->buf->getRange(range));
                }

                c_->scr->moveCursor(c_->scr->toScrCoord(cursor));
                break;
            }
            case 'k':
            {
                auto cursor = c_->buf->move(Direction::UP);
                auto& range = c_->visibleRange;

                if (!range->contains(cursor))
                {
                    int diff = range->start()->y() - cursor->y();
                    range->end()->setY(range->end()->y() - diff);
                    range->start()->setY(cursor->y());

                    c_->scr->paint(c_->buf->getRange(range));
                }
                
                c_->scr->moveCursor(c_->scr->toScrCoord(cursor));
                break;
            }
            case 'l':
            {
                auto cursor = c_->buf->move(Direction::RIGHT);
                c_->scr->moveCursor(c_->scr->toScrCoord(cursor));
                break;
            }
            case 'x':
            {
                auto cur = c_->buf->cursor();
                c_->buf->erase(Range::make(cur, {cur->x() + 1, cur->y()}));
                c_->scr->delCh();
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
            {
                auto cursor = c_->buf->move(Direction::LEFT);
                c_->scr->moveCursor(c_->scr->toScrCoord(cursor));
                parent_->changeMode(Mode::NORMAL);
                endLoop = true;
                break;
            }
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
