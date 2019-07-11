#pragma once

//#include <map>
#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace ui
{
	class Coloris;
}

struct IAction
{
	ui::Coloris* coloris = nullptr;

	virtual void make() = 0;
	virtual void unmake() = 0;

	virtual bool validate() = 0;

	std::string name = "";
	bool refresh = false;
	bool run = true;
};

/*struct Action
{
	std::function<void(void)> make;
	std::function<void(void)> unmake;
	std::string name = "";
	bool refresh = false;
	bool run = true;
};*/

class History
{
public:
	History(ui::Coloris* c)
	{
		coloris = c;
	};

	void addAction(std::shared_ptr<IAction>);
	//bool removeAction();

	std::string toString() const;

	void clear();

	void redo();
	void undo();

private:
	ui::Coloris* coloris = nullptr;

	//std::map<std::string, IAction> actions;
	std::vector<std::shared_ptr<IAction>> actions;

	size_t carret = 0;
};