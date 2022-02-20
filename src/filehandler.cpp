#include "sle/filehandler.h"
#include "sle/eventaction.h"
#include "sle/mainscreen.h"
#include "sle/buffer.h"
#include <fstream>
#include <vector>
#include <string>

namespace sle {

class FileHandler::Impl {
public:
    Impl(const DispatcherPtr& dispatcher, std::string path);

protected:
    class State {
    public:
        State(Impl* parent);

        virtual ~State() = default;

    protected:
        template <class NewState>
        void switchState()
        {
            parent_->changeState(new NewState(parent_));
        }

        Impl* parent_;
        const DispatcherPtr dispatcher_;
        EventActionContainer actions_;
    };

    void changeState(State* newState) { currentState_.reset(newState); }

    // List of FSM States
    class Idle;
    class OpenFile;
    class SaveFile;

    using StateUPtr = std::unique_ptr<State>;

    const DispatcherPtr dispatcher_;
    std::string path_;
    BufferPtr buf_;
    StateUPtr currentState_;
};

FileHandler::FileHandler(const DispatcherPtr& dispatcher, std::string path)
    : impl_(new Impl(dispatcher, path))
{}

FileHandler::~FileHandler()
{}

FileHandler::Impl::State::State(Impl* parent)
    : parent_(parent), dispatcher_(parent->dispatcher_)
    , actions_(parent->dispatcher_)
{}

class FileHandler::Impl::Idle : public State {
public:
    Idle(Impl* parent) : State(parent)
    {
		std::ifstream fin;
		fin.open("testtxt");

		std::string line;
		std::vector<std::string> data;

		while (getline(fin, line)) {
			if (fin.peek() != EOF)
				line.append("\n");
			data.push_back(line);
		}

		fin.close();

		parent_->buf_ = 
            std::make_unique<Buffer>(dispatcher_, StrPacket({data}));
    }
};

class FileHandler::Impl::OpenFile : public State {
public:
    OpenFile(Impl* parent) : State(parent)
    {}
};
    

FileHandler::Impl::Impl(const DispatcherPtr& dispatcher, std::string path)
    : dispatcher_(dispatcher), path_(path)
    , buf_(nullptr)
    , currentState_(new Impl::Idle(this))
{}

}
