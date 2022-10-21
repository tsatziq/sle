#include "sle/modeloop.h"
#include "sle/context.h"
#include "sle/screen.h"
#include "sle/screenmanager.h"
#include <memory>
#include <ncurses.h>

namespace sle {

class ModeLoopImpl : public ModeLoop
{
public:
    static std::unique_ptr<ModeLoopImpl> create(const Context* context)
    {
        return std::unique_ptr<ModeLoopImpl>(new ModeLoopImpl(context));
    }

    ~ModeLoopImpl();

    void start() override;

    void normalLoopRun() override;

    void insertLoopRun() override;

    void cmdLoopRun() override;

private:
    ModeLoopImpl(const Context* context);

    const Context* c_;
    Screen* main_ = nullptr;
    Mode mode_ = Mode::normal;
};

ModeLoopImpl::ModeLoopImpl(const Context* context)
    : c_(context)
    , main_(context->screens->screen(ScreenId::main))
{}

ModeLoopPtr ModeLoop::create(const Context* context)
{
    return ModeLoopImpl::create(context);
}

ModeLoopImpl::~ModeLoopImpl()
{}

void ModeLoopImpl::start()
{
    while (mode_ != Mode::quit) {
        switch (mode_) {
            case Mode::normal:
                normalLoopRun();
                break;
            case Mode::insert:
                insertLoopRun();
                break;
            case Mode::cmd:
                cmdLoopRun();
                break;
            default:
                break;
        }
    }

}

void ModeLoopImpl::normalLoopRun()
{
    Screen* scr = c_->screens->screen(ScreenId::main);
    Buffer* buf = c_->buffer.get();

    bool quit = false;

    while (char c = scr->getChar()) {
        switch (c) {
            case 'B':
                c_->cursor->move(QuickMove::B);
                break;
            case 'b':
                //buf->backWord(1);
                break;
            case 'C':
                c_->cursor->move(QuickMove::C);
                break;
            case 'd':
                c_->pager->movePage(1);
                break;
            case 'E':
                c_->cursor->move(QuickMove::E);
                break;
            case 'i':
                mode_ = Mode::insert;
                quit = true;
                break;
            case 'H':
                c_->cursor->move(QuickMove::H);
                break;
            case 'h':
                c_->cursor->leftRight(-1);
                break;
            case 'j':
                c_->cursor->upDown(1);
                break;
            case 'k':
                c_->cursor->upDown(-1);
                break;
            case 'L':
                c_->cursor->move(QuickMove::L);
                break;
            case 'l':
                c_->cursor->leftRight(1);
                break;
            case 'M':
                c_->cursor->move(QuickMove::M);
                break;
            case 'Q':
            case 'q':
                mode_ = Mode::quit;
                quit = true;
            case 'u':
                c_->pager->movePage(-1);
                break;
            case 'w':
            {
                Coord pos = c_->finder->findWord();
                c_->cursor->move(pos);
                break;
            }
            case 'x':
            {
                bool lastLine = c_->cursor->coord().y() + 1 == buf->size();
                if (!buf->eraseLine(1))
                    break;

                if (lastLine)
                    c_->cursor->upDown(-1);
                c_->pager->show();
                c_->sideBar->refresh();
                c_->cursor->redraw();
                break;
            }
            case 'z':
                buf->saveFile();
                break;
            default:
                break;
        }

        if (quit)
            break;
    }
}

void ModeLoopImpl::insertLoopRun()
{
    Screen* scr = c_->screens->screen(ScreenId::main);
    Buffer* buf = c_->buffer.get();

    bool quit = false;

    while (char c = scr->getChar())
    {

        int numLines = buf->size();

        if ((c > 31) && (c < 127))
        {
            buf->addChar(c);
            c_->pager->show();
            c_->cursor->leftRight(1);
        }
        else
        {
            switch (c)
            {
            case 27:
                mode_ = Mode::normal;
                quit = true;
                break;

            case 10:
                buf->addChar('\n');
                //buf->moveCursor(0,1);
                //buf->setX(0);
                c_->pager->show();
                break;

            case 127:
                if (buf->eraseChar(-1))
                {
                    c_->pager->show();
                    c_->cursor->leftRight(-1);
                }
                break;
            }

        }

        if (quit)
            break;

        if (buf->size() != numLines)
            c_->sideBar->refresh();
    }
}

void ModeLoopImpl::cmdLoopRun()
{}

}
