#include "History.hpp"
#include "Actions.hpp"

void History::addAction(std::shared_ptr<IAction> a)
{
	if (carret != actions.size())
		actions.erase(actions.begin() + carret, actions.end());
	else if (actions.size() >= 10)
	{
		actions.erase(actions.begin());
		carret--;
	}

	actions.push_back(a);
	carret = actions.size();

	a->coloris = coloris;

	if (a->run)
		a->make();
}

std::string History::toString() const
{
	std::string s = "Carret: " + std::to_string(carret) + '\n';
	for (size_t i = 0; i < actions.size(); ++i)
	{
		/*ActionMove* p = dynamic_cast<ActionMove*>(&*actions[i]);
		s += std::to_string(i+1) + "-) Origin: (" + std::to_string(p->origin.x) + ", " + std::to_string(p->origin.y) + ") Result: (" + 
			std::to_string(p->result.x) + ", " + std::to_string(p->result.y) + ")\n";*/
		s += actions[i]->name + '\n';
	}
	return s;
}

void History::clear()
{
	actions.clear();
	carret = 0;
}

void History::redo()
{
	if (carret < actions.size())
	{
		actions[carret]->make();
		carret++;
	}
}

void History::undo()
{
	if (carret > 0)
	{
		actions[carret-1]->unmake();
		carret--;
	}
}