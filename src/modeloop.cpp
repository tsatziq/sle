#include "sle/modeloop.h"
#include "sle/screen.h"
#include "sle/screenmanager.h"
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

    void initLineNums() override;

    void normalLoopRun() override;

    void insertLoopRun() override;

    void cmdLoopRun() override;

private:
    ModeLoopImpl(ContextPtr context, const std::string& file);

    ContextPtr c_;
    std::string file_;
    BufferId currentBuf_;
    BufferId lineNumBuf_;
    Mode mode_ = Mode::normal;
};

ModeLoopImpl::ModeLoopImpl(ContextPtr context, const std::string& file)
    : c_(context)
    , file_(file)
    , currentBuf_(context->bufManager->addBuffer())
    , lineNumBuf_(context->bufManager->addBuffer())
{}

ModeLoopPtr ModeLoop::create(ContextPtr context, const std::string& file)
{
    return ModeLoopImpl::create(context, file);
}

ModeLoopImpl::~ModeLoopImpl()
{}

void ModeLoopImpl::start()
{
    Buffer* buf = c_->bufManager->getBuffer(currentBuf_);
    buf->readFile(file_);

    initLineNums();

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

void ModeLoopImpl::initLineNums()
{
    Buffer* buf = c_->bufManager->getBuffer(currentBuf_);
    Buffer* lnNums = c_->bufManager->getBuffer(lineNumBuf_);
    CursePtr mainScr = c_->scrManager->getScreen(ScreenId::main)->getCurse();

    int size = buf->getSize();
    if (size == 0)
        size = 1;

    std::vector<std::string> lineNums;
    std::string line;

    for (int i = 1; i <= size; i++) {
        line = "   " + std::to_string(i) + "\n"; 
        lineNums.push_back(line);
    }

    Screen* side = c_->scrManager->getScreen(ScreenId::side);
    int h = side->getHeight();
    h -= lineNums.size();
    
    for (int i = 1; i <= h; i++) {
        line = "~\n";
        lineNums.push_back(line);
    }

    lnNums->clear();
    lnNums->addLines(lineNums);
    lnNums->show(side);

    Coord cursor = buf->getCursor();
    wmove(mainScr, cursor.y, cursor.x);
    refresh();
}

void ModeLoopImpl::normalLoopRun()
{
    Screen* scr = c_->scrManager->getScreen(ScreenId::main);
    Buffer* buf = c_->bufManager->getBuffer(currentBuf_);

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
    Screen* scr = c_->scrManager->getScreen(ScreenId::main);
    Buffer* buf = c_->bufManager->getBuffer(currentBuf_);

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
            initLineNums();
    }
}

void ModeLoopImpl::cmdLoopRun()
{}

}
