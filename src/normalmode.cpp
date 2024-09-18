#include "sle/normalmode.h"
#include "sle/insertmode.h"
#include "sle/mainscreen.h"
#include <cmath>

namespace sle
{

EditLoop::NormalMode::NormalMode(
    EditLoop* parent)
    :
    parent_(parent),
    c_(parent->c_),
    b_(c_->buf),
    s_(c_->scr)
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
            ret = CmdState::FINISHED;
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
    ScreenState state = ScreenState::NONE;
    auto scr = c_->scr;
    PointPtr target = Point::make();

    switch (cmd_.motion)
    {
    case Motion::LEFT:
        target = c_->buf->move(Direction::LEFT);
        c_->buf->setCursor(target);
        break;
    case Motion::DOWN:
    {
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

        c_->buf->setCursor(target);
        // NOTE: tama ja monet muut tarvitaa ainaki et lldb debug onnistuu.
        // muuten se teksti liikkuessa jotenki menee sekasin
        c_->scr->refreshScr(ScreenState::REDRAW,
            c_->scr->toScrCoord(target));
        break;
    }
    case Motion::RIGHT:
        target = c_->buf->move(Direction::RIGHT);
        c_->buf->setCursor(target);
        state = ScreenState::NONE;
        break;
    case Motion::SCRFWD:
    case Motion::SCRBCK:
    {
        int yNow = c_->buf->cursor()->y();
        int scroll = c_->scr->height() / 2;

        if (cmd_.motion == Motion::SCRFWD)
            c_->scr->scrollScr(scroll, Direction::DOWN);
        else if (cmd_.motion == Motion::SCRBCK)
            c_->scr->scrollScr(scroll, Direction::UP);
        
        auto cur = c_->scr->toBufCoord(c_->scr->cursor());

        if (cur->y() == yNow || std::abs(cur->y() - yNow) < scroll) 
        {
            if (cmd_.motion == Motion::SCRFWD)
                cur->setY(yNow + scroll);
            if (cmd_.motion == Motion::SCRBCK)
                cur->setY(yNow - scroll);

            if (cur->y() < 0)
                cur->setY(0);
            if (cur->y() > c_->buf->size())
                cur->setY(c_->buf->size() - 1);
        }

        std::regex pattern(R"(\S)");
        auto endX = c_->buf->lineLen(cur) - 1;
        auto ln = Range::make(
            Point::make(0, cur->y()),
            Point::make(endX, cur->y()));
        auto res = c_->buf->find(pattern, ln);
        if (!res)
            cur->setX(0);

        target->set(res->start());
        c_->buf->setCursor(res->start());
        break;
    }
    case Motion::TILL:
    case Motion::TILLBCK:
    case Motion::TO:
    case Motion::TOBCK:
    {
        auto cur = c_->buf->cursor();
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

        if (target)
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

            c_->scr->refreshScr(ScreenState::REDRAW,
                c_->scr->toScrCoord(target));
        }
        break;
    }
    case Motion::BEGINLN:
    case Motion::ENDLN:
    case Motion::MIDDLELN:
    {
        std::regex pattern(R"(\S+.*\S|\S)");
        auto cur = Point::make(c_->buf->cursor());

        auto res = c_->buf->find(
            pattern,
            Range::make(
                Point::make(0, cur->y()),
                Point::make(c_->buf->lineLen(cur) - 1, cur->y())));

        if (!res)
            break;

        int start = res->start()->x();
        int end = res->end()->x();
        int mid = start + ((end - start) / 2);

        if (cmd_.motion == Motion::BEGINLN)
            target = c_->scr->toScrCoord(res->start());
        else if (cmd_.motion == Motion::MIDDLELN)
            target = c_->scr->toScrCoord(Point::make(mid + 1, cur->y()));
        else if (cmd_.motion == Motion::ENDLN)
            target = c_->scr->toScrCoord(res->end());

        c_->buf->setCursor(target);
        break;
    }
    case Motion::BOTTOM:
    case Motion::MIDDLE:
    case Motion::TOP:
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

        c_->buf->setCursor(res->end());
        target = c_->scr->toScrCoord(res->end());
        c_->buf->setCursor(target);
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
        c_->buf->setCursor(target);
        break;
    }
    // SEURAAVAKS HETI: laita B et se pysahtyy kans ekan sanan edes/rivin
    // alus. sillee et saa helpost oikeen koordin delete wordia varten.
    // KS etta del word paivittaa kursorin oikeesee paikkaan.!!!!
    case Motion::WORDBCK:
    case Motion::WORDFWD:
    {
        if (cmd_.motion == Motion::WORDFWD)
            target = c_->buf->moveWord(Direction::RIGHT);
        else if (cmd_.motion == Motion::WORDBCK)
            target = c_->buf->moveWord(Direction::LEFT);
        else
            break;

        if (target)
            c_->scr->refreshScr(ScreenState::REDRAW,
                c_->scr->toScrCoord(target));
        break;
    }
    default:
        break;
    }

    // SEURAAVAKS: DELETE & CHANGE liikkeen kanssa..
    // SEURAAVAKS: laita ][ liikkuu esim scrHeihgt / 5.
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
    // NOTE: jotenki ei ota sita commandia viel ekoil nappailyilla
    case Action::DELETE:
    {
        if (!target)
            break;

        auto cur = c_->buf->cursor();

        if (cur->y() > target->y())
        {
            PointPtr tmp = target;
            target = cur;
            cur = tmp;
        }

        int start = cur->y();
        int end = target->y();

        // If last word of line, need to advance over last line.
        if (target->x() == b_->lineLen(target) - 2)
            target->incX();

        c_->buf->erase(Range::make(cur, target));
        auto txt = c_->buf->getRange(Range::make(
            Point::make(0, start),
            Point::make(0, c_->visibleRange->end()->y() + 1)));

        if (start != end)
            c_->scr->paint(txt, Point::make(0, start));
        else
        {
            s_->clrToEol(Point::make(0, cur->y()));
            s_->paint({b_->getLine()}, Point::make(0, cur->y()));
        }

        if (b_->charAt(cur) == '\n')
            cur->decX();
        c_->scr->moveCursor(c_->scr->toScrCoord(cur));
        c_->buf->setCursor(cur);

        // HETI SEURAAVAKS:
        /* 
        tokavikal rivil lopust dw ei toimi, getRange pitaa kattoo et jos menee
yli size() ni palauta vaa siihe asti ku riveja on.
        2. dw vikan rivin vikal sanal ei toimi..
        */
        auto st = c_->visibleRange->start();
        auto en = c_->visibleRange->end();
        auto i = 1;
        break;
    }
    case Action::NONE:
    {
        if (!target || !target->isFullySet())
            break;

        c_->buf->setCursor(target); // KOSKA dw kursor ei saa liikkua.
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

    //if (target)
    c_->scr->refreshScr(state); 
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

// ehka pitaa vaa teha H,L,K, ja ctrl noi, komplete sit joku muu
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
    case 'b': return Motion::WORDBCK;
    case 't': return Motion::TILL;
    case 'T': return Motion::TILLBCK;
    case 'w': return Motion::WORDFWD;
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
