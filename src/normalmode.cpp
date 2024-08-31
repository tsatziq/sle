#include "sle/normalmode.h"
#include "sle/insertmode.h"
#include "sle/mainscreen.h"

namespace sle
{

EditLoop::NormalMode::NormalMode(
    EditLoop* parent)
    :
    parent_(parent),
    c_(parent->c_)
{}

bool EditLoop::NormalMode::handle()
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

CmdState EditLoop::NormalMode::parse(
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
}

void EditLoop::NormalMode::execute(
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
    case Motion::BEGINLN:
    case Motion::ENDLN:
    case Motion::MIDDLELN:
    {
        auto cur = Point::make(0, c_->buf->cursor()->y());
        std::regex pattern(R"(\S)");
        auto endX = c_->buf->lineLen(cur) - 1;

        auto str = c_->buf->getLine(cur);
        auto revStr = str;
        std::reverse(revStr.begin(), revStr.end());

        auto firstCh = c_->buf->find(pattern, cur->y(), str);
        auto lastCh = c_->buf->find(pattern, cur->y(), revStr);

        if (!firstCh || !lastCh)
            break;

        int revPos = str.size() - lastCh->x() - 1;
        int mid = (revPos - firstCh->x()) / 2;

        if (cmd_.motion == Motion::BEGINLN)
            target = c_->scr->toScrCoord(firstCh);
        else if (cmd_.motion == Motion::MIDDLELN)
            target = c_->scr->toScrCoord(Point::make(mid, cur->y()));
        else if (cmd_.motion == Motion::ENDLN)
            target = c_->scr->toScrCoord(Point::make(revPos, cur->y()));
        break;
    }
    case Motion::BOTTOM:
    case Motion::MIDDLE:
    case Motion::TOP:
        // SEURAAVAKS: naa H,L,M ja horisontaalisesti samat.
    {
        PointPtr cur;

        if (cmd_.motion == Motion::TOP)
            cur = c_->scr->toBufCoord(Point::make(0, 0));
        else if (cmd_.motion == Motion::BOTTOM)
            cur = c_->scr->toBufCoord(Point::make(0, c_->scr->height() - 1));
        else if (cmd_.motion == Motion::MIDDLE)
            cur = c_->scr->toBufCoord(Point::make(0, c_->scr->height() / 2));

        std::regex pattern(R"(\S)");
        auto endX = c_->buf->lineLen(cur) - 1;
        auto ln = Range::make(cur, Point::make(endX, cur->y()));
        auto res = c_->buf->find(pattern, ln);
        if (!res)
            cur->setX(0);

        c_->buf->setCursor(res);
        target = c_->scr->toScrCoord(res);
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
    case Action::CHANGEEOL:
    {
        auto eol = Point::make(c_->buf->cursor());
        eol->setX(c_->buf->lineLen() - 1);
        if (eol->y() != c_->buf->size() - 1)
            eol->decX();
        parent_->changeMode(
            Mode::INSERT,
            new InsertModeData(InsertType::CHANGE, eol));

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

bool EditLoop::NormalMode::isPrimaryAct(
    const Action a) const
{
    switch (a)
    {
        case Action::APPEND:
        case Action::CHANGEEOL:
        case Action::INSERT:
        case Action::DELCHAR:
        case Action::TEST:
        case Action::QUIT:
            return true;
        default:
            return false;
    }
}

bool EditLoop::NormalMode::isPendingCmd(
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

bool EditLoop::NormalMode::isMotion(
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

bool EditLoop::NormalMode::isDoubleCmd(
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

Motion EditLoop::NormalMode::toMotion(
    const char c)
{
    switch (c)
    {
    case 0x04: return Motion::SCRFWD;
    case 0x08: return Motion::BEGINLN;
    case 0x0C: return Motion::ENDLN;
    case 0x02: return Motion::MIDDLELN;
    case 0x15: return Motion::SCRBCK;
    case 'f': return Motion::TO;
    case 'F': return Motion::TOBCK;
    case 'h': return Motion::LEFT;
    case 'H': return Motion::TOP;
    case 'j': return Motion::DOWN;
    case 'k': return Motion::UP;
    case 'l': return Motion::RIGHT;
    case 'L': return Motion::BOTTOM;
    case 'B': return Motion::MIDDLE;
    case 't': return Motion::TILL;
    case 'T': return Motion::TILLBCK;
    default: return Motion::NONE;
    }
}

Action EditLoop::NormalMode::toAction(
    const char c)
{
    switch (c)
    {
    case 'a': return Action::APPEND;
    case 'c': return Action::CHANGE;
    case 'C': return Action::CHANGEEOL;
    case 'd': return Action::DELETE;
    case 'i': return Action::INSERT;
    case 'x': return Action::DELCHAR;
    case 'z': return Action::QUIT;
    case '[': return Action::TEST;
    default: return Action::NONE;
    }
}

Command EditLoop::NormalMode::prevCmd_ = {};

}
