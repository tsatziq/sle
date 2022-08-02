#include "sle/modeloop.h"
#include "sle/screen.h"
#include "sle/screenmanager.h"
#include <memory>
#include <ncurses.h>

namespace sle {

class ModeLoopImpl : public ModeLoop
{
public:
    static std::unique_ptr<ModeLoopImpl> create(
        ContextPtr context, const std::string& file)
    {
        return std::unique_ptr<ModeLoopImpl>(new ModeLoopImpl(context, file));
    }

    ~ModeLoopImpl();

    void start() override;

    void normalLoopRun() override;

    void insertLoopRun() override;

    void cmdLoopRun() override;

private:
    ModeLoopImpl(ContextPtr context, const std::string& file);

    ContextPtr c_;
    std::string file_;
    Mode mode_ = Mode::normal;
};

ModeLoopImpl::ModeLoopImpl(ContextPtr context, const std::string& file)
    : c_(context)
    , file_(file)
{}

ModeLoopPtr ModeLoop::create(ContextPtr context, const std::string& file)
{
    return ModeLoopImpl::create(context, file);
}

ModeLoopImpl::~ModeLoopImpl()
{}

void ModeLoopImpl::start()
{
    c_->buffer->readFile(file_);

    c_->sideBar->refresh();

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
                buf->backWord(1);
                break;
            case 'I':
            case 'i':
                mode_ = Mode::insert;
                quit = true;
                break;
            case 'Q':
            case 'q':
                mode_ = Mode::quit;
                quit = true;
            case 'w':
                //buf->fwdWord(1);
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
    Screen* scr = c_->screens->getScreen(ScreenId::main);
    Buffer* buf = c_->buffer.get();

    bool quit = false;

    while (char c = scr->getChar()) {

        int numLines = buf->getSize();

        if ((c > 31) && (c < 127)) {
            buf->addChar(c);
            buf->moveCursor(1, 0);
            buf->show(scr);
        } else {
            switch (c) {
                case 27:
                    mode_ = Mode::normal;
                    quit = true;
                    break;

                case 10:
                    buf->addChar('\n');
                    buf->moveCursor(0,1);
                    buf->setX(0);
                    buf->show(scr);
                    break;

                case 127:
                    buf->eraseChar(-1);
                    buf->show(scr);
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
