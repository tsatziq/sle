#include "sle/editloop.h"
#include "sle/customtypes.h"
#include "sle/mainscreen.h"
#include "sle/normalmodelexer.h"
#include <memory>
#include <sstream>
#include <string>
#include <cctype>
#include <queue>

namespace sle 
{

bool EditLoop::ModeBase::quit_ = false;

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
        while (true)
        {
            char ch = c_->scr->getCh();
            input_ += ch;

            switch (parse(ch))
            {
            case CmdState::FINISHED:
                prevPart_ = CmdPart::NONE;
                execute(cmd_);

                if (quit_)
                    break;
                if (!exitMode_)
                    prevCmd_ = cmd_;
                cmd_ = Command{};
                input_.clear();
                break;
            case CmdState::INVALID:
                prevPart_ = CmdPart::NONE;
                input_.clear();
                break;
            case CmdState::UNFINISHED:
            default:
                break;
            }            

            if (quit_)
                return true;
            else if (exitMode_)
            {
                exitMode_ = false;
                return false;
            }
        }
    }

private:
    enum class CmdPart
    {
        NONE,
        COUNT,
        ACTION,
        MOTION,
        TO,
    };

    /// Parse the current input string.
    /// \param input The string to parse.
    /// \returns CmdState enum indicating if command was finished.
    CmdState parse(
        const char c)
    {
        auto scr = c_->scr;
        CmdState ret = CmdState::UNFINISHED;

        switch (prevPart_)
        {
        case CmdPart::NONE:
        {
            if (toAction(c) != Action::NONE)
            {
                cmd_.action = toAction(c);
                if (isPrimaryAct(cmd_.action))
                    ret = CmdState::FINISHED;
                else
                    prevPart_ = CmdPart::ACTION;
            }
            else if (toMotion(c) != Motion::NONE)
            {
                cmd_.motion = toMotion(c);
                if (isPendingCmd(cmd_.motion))
                    prevPart_ = CmdPart::MOTION;
                else
                    ret = CmdState::FINISHED;
            }
            else if (std::isdigit(c))
            {
                cmd_.count = c;
                prevPart_ = CmdPart::COUNT;
            }
            else
                ret = CmdState::INVALID;
            break;
        }
        case CmdPart::COUNT:
        {
            if (toAction(c) != Action::NONE)
            {
                cmd_.action = toAction(c);
                if (isPrimaryAct(cmd_.action))
                    ret = CmdState::FINISHED;
                else
                    prevPart_ = CmdPart::ACTION;
            }
            else if (toMotion(c) != Motion::NONE)
            {
                cmd_.motion = toMotion(c);
                prevPart_ = CmdPart::MOTION;
            }
            else if (std::isdigit(c))
                cmd_.count += c;
            else
                ret = CmdState::INVALID;
            break;
        }
        case CmdPart::ACTION:
        {
            if (isDoubleCmd(cmd_.action) && (toAction(c) == cmd_.action))
            {
                cmd_.motion = Motion::LINE;
                ret = CmdState::FINISHED;
            }
            else if (toMotion(c) != Motion::NONE)
            {
                cmd_.motion = toMotion(c);
                prevPart_ = CmdPart::MOTION;
            }
            else if (std::isdigit(c))
            {
                cmd_.motion = Motion::TOLINE;
                cmd_.to += c;
                prevPart_ = CmdPart::MOTION;
            }
            break;
        }
        case CmdPart::MOTION:
        {
            switch (cmd_.motion)
            {
            case Motion::TOLINE:
            {
                if (std::isdigit(c))
                    cmd_.to += c;
                else if (c == 'G')
                    ret = CmdState::FINISHED;
                else
                    ret = CmdState::INVALID; 
                break;
            }
            case Motion::TILL:
            case Motion::TILLBCK:
            case Motion::TO:
            case Motion::TOBCK:
            {
                cmd_.to += c;
                ret = CmdState::FINISHED;
                break;
            }
            default:
                ret = CmdState::INVALID;
                break;
            }
            break;
        } 
        default:
            ret = CmdState::INVALID;
            break;
        }

        return ret;

        /*
        if (input.size() == 1)
        {
            ret = CmdState::FINISHED;

            switch (input.at(0))
            {
            case 'z':
                exitMode_ = true;
                quit_ = true;
                break;
            case 'a':
            {
                auto cursor = c_->buf->cursor();
                cursor->incX();
                cursor = c_->buf->setCursor(cursor);
                c_->scr->moveCursor(c_->scr->toScrCoord(cursor));

                parent_->changeMode(Mode::INSERT);
                exitMode_ = true;
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
                exitMode_ = true;
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
                // if (std::is_digit(
                // tai sit vaa perakkai caset 0-9...
                ret = CmdState::INVALID;
            }

            return ret;
        }

        return CmdState::FINISHED;
        */
    }

    void execute(
        const Command& cmd)
    {
        auto scr = c_->scr;
        PointPtr target = Point::make(0, 0);

        switch (cmd_.motion)
        {
        case Motion::LEFT:
            target = c_->buf->move(Direction::LEFT);
            break;
        case Motion::DOWN:
        {
            // huom lldb toimii huonosti koska siin nayton koko jotenki huonosti.
            target = c_->buf->move(Direction::DOWN);
            auto& range = c_->visibleRange;

            if (!range->contains(target))
            {
                auto endY = range->end()->y();
                auto startY = range->start()->y();

                int diff = target->y() - range->end()->y();

                range->start()->setY(range->start()->y() + diff); 
                range->end()->setY(target->y());

                c_->scr->paint(c_->buf->getRange(range));
            }
            break;
        }
        case Motion::RIGHT:
            target = c_->buf->move(Direction::RIGHT);
            break;
        case Motion::SCRFWD:
        case Motion::SCRBCK:
        {
            int yNow = c_->buf->cursor()->y();

            if (cmd_.motion == Motion::SCRFWD)
                c_->scr->scrollScr(c_->scr->height() / 2, Direction::DOWN);
            else if (cmd_.motion == Motion::SCRBCK)
                c_->scr->scrollScr(c_->scr->height() / 2, Direction::UP);
            
            auto cur = c_->scr->toBufCoord(c_->scr->cursor());

            if (cur->y() == yNow)
            {
                if (cmd_.motion == Motion::SCRFWD)
                    cur->setY(yNow + c_->scr->height() / 2);
                if (cmd_.motion == Motion::SCRBCK)
                    cur->setY(yNow - c_->scr->height() / 2);

                if (cur->y() < 0)
                    cur->setY(0);
                if (cur->y() > c_->buf->size())
                    cur->setY(c_->buf->size() - 1);
            }

            std::regex pattern(R"(\S)");
            auto endX = c_->buf->lineLen(cur) - 1;
            auto ln = Range::make(cur, Point::make(endX, cur->y()));
            auto res = c_->buf->find(pattern, ln);
            if (!res)
                cur->setX(0);

            target->set(cur->x(), cur->y());
            c_->scr->moveCursor(c_->scr->toScrCoord(cur));
            c_->buf->setCursor(cur);
            break;
        }
        case Motion::TILL:
        case Motion::TILLBCK:
        case Motion::TO:
        case Motion::TOBCK:
        {
            if (cmd_.to.empty())
                break;

            switch (cmd_.motion)
            {
            case Motion::TO:
            case Motion::TILL:
                target = c_->buf->findCh(cmd_.to.front()); 
                break;
            case Motion::TOBCK:
            case Motion::TILLBCK:
                target = c_->buf->findCh(cmd_.to.front(), Direction::LEFT); 
                break;
            default:
                break;
            }

            if (!target)
                target = c_->buf->cursor();
            else
            {
                switch (cmd_.motion)
                {
                case Motion::TILL:
                    target->decX();
                    break;
                case Motion::TILLBCK:
                    target->incX();
                    break;
                default:
                    break;
                }
                c_->buf->setCursor(target);
            }
            break;
        }
        case Motion::UP:
        {
            target = c_->buf->move(Direction::UP);
            auto& range = c_->visibleRange;

            if (!range->contains(target))
            {
                int diff = range->start()->y() - target->y();

                range->end()->setY(range->end()->y() - diff);
                range->start()->setY(target->y());

                c_->scr->paint(c_->buf->getRange(range));
            }
            break;
        }
        default:
            break;
        }

        switch (cmd_.action)
        {
        case Action::APPEND:
        {
            auto cursor = c_->buf->cursor();
            cursor->incX();
            cursor = c_->buf->setCursor(cursor);
            c_->scr->moveCursor(c_->scr->toScrCoord(cursor));

            parent_->changeMode(Mode::INSERT);
            exitMode_ = true;
            break;
        }
        case Action::DELCHAR:
        {
            auto cur = c_->buf->cursor();
            c_->buf->erase(Range::make(cur, {cur->x() + 1, cur->y()}));
            c_->scr->delCh();
            break;
        }
        case Action::NONE:
        {
            auto p = scr->toScrCoord(target);
            c_->scr->moveCursor(scr->toScrCoord(target));
            break;
        }
        case Action::INSERT:
            parent_->changeMode(Mode::INSERT);
            exitMode_ = true;
            break;
        case Action::TEST:
            c_->scr->test();
            break;
        case Action::QUIT:
            exitMode_ = true;
            quit_ = true;
            break;
        default:
            break;
        }
    }

    bool isPrimaryAct(
        const Action a) const
    {
        switch (a)
        {
            case Action::APPEND:
            case Action::INSERT:
            case Action::DELCHAR:
            case Action::TEST:
            case Action::QUIT:
                return true;
            default:
                return false;
        }
    }

    bool isPendingCmd(
        const Motion m) const
    {
        switch (m)
        {
        case Motion::TO: 
        case Motion::TOBCK: 
        case Motion::TILL:
        case Motion::TILLBCK:
            return true;
        default:
            return false;
        }
    }

    bool isMotion(
        const char c) const
    {
        switch (c)
        {
            case 'b': case 'e': case 'f': case 't': case 'w': 
                return true;
            default:
                return false;
        }
    }

    /// E.g. 'dd' and 'cc'.
    bool isDoubleCmd(
        const Action action) const
    {
        switch (action)
        {
            case Action::DELETE:
                return true;
            default:
                return false;
        }
    }

    Motion toMotion(
        const char c)
    {
        switch (c)
        {
        case 0x04: return Motion::SCRFWD;
        case 0x15: return Motion::SCRBCK;
        case 'f': return Motion::TO;
        case 'F': return Motion::TOBCK;
        case 'h': return Motion::LEFT;
        case 'j': return Motion::DOWN;
        case 'k': return Motion::UP;
        case 'l': return Motion::RIGHT;
        case 't': return Motion::TILL;
        case 'T': return Motion::TILLBCK;
        default: return Motion::NONE;
        }
    }

    Action toAction(
        const char c)
    {
        switch (c)
        {
        case 'a': return Action::APPEND;
        case 'd': return Action::DELETE;
        case 'i': return Action::INSERT;
        case 'x': return Action::DELCHAR;
        case 'z': return Action::QUIT;
        case '[': return Action::TEST;
        default: return Action::NONE;
        }
    }

    EditLoop* parent_ = nullptr;
    ContextPtr c_ = nullptr;
    static NormalModeLexer lex_;

    bool exitMode_ = false;
    std::string input_;
    static Command prevCmd_; ///< Last valid command for repeat.
    Command cmd_; ///< Command that is currently being formed.
    CmdPart prevPart_ = CmdPart::NONE; ///< Which part was parsed last.
};

Command EditLoop::NormalMode::prevCmd_ = {};
NormalModeLexer EditLoop::NormalMode::lex_ = NormalModeLexer();

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
                return false;
            }
            case '\n':
                c_->buf->addCh(ch);
                c_->scr->paintCh(ch);
                return false;
            default:
                if (c_->scr->paintCh(ch))
                    c_->buf->addCh(ch);
                return false;
            }
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

void EditLoop::run()
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
