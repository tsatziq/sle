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
    , main_(context->screens->getScreen(ScreenId::main))
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
    Screen* scr = c_->screens->getScreen(ScreenId::main);
    Buffer* buf = c_->buffer.get();

    bool quit = false;

    while (char c = scr->getChar()) {
        switch (c) {
            case 'b':
                //buf->backWord(1);
                break;
            case 'd':
                c_->pager->movePage(1);
                break;
            case 'i':
                mode_ = Mode::insert;
                quit = true;
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
            case 'l':
                c_->cursor->leftRight(1);
                break;
            case 'Q':
            case 'q':
                mode_ = Mode::quit;
                quit = true;
            case 'u':
                c_->pager->movePage(-1);
                break;
            case 'w':
                //buf->fwdWord(1);
                break;
            case 'x':
            {
                bool lastLine = c_->cursor->coord().y() + 1 == buf->getSize();
                if (!buf->eraseLine(1))
                    break;

                if (lastLine)
                    c_->cursor->upDown(-1);
                c_->pager->show();
                c_->sideBar->refresh();
                c_->cursor->redraw();
                break;
            }
            default:
                break;
        }

        if (quit)
            break;
    }
}

void ModeLoopImpl::insertLoopRun()
{
    Screen* scr = c_->screens->getScreen(ScreenId::main);
    Buffer* buf = c_->buffer.get();

    bool quit = false;

    while (char c = scr->getChar())
    {

        int numLines = buf->getSize();

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

        if (buf->getSize() != numLines)
            c_->sideBar->refresh();
    }
}

void ModeLoopImpl::cmdLoopRun()
{}

}
