#ifndef MENU_H
#define MENU_H

#include "Console.h"
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

class ContainerComponent
{
public:
	virtual const std::string& getName() const = 0;
	virtual void call() = 0;

	virtual ~ContainerComponent() = default;
};

class CompositeMenu final : public ContainerComponent
{
public:
	void addChild(const std::shared_ptr<ContainerComponent> child);
	void removeChild(const std::shared_ptr<ContainerComponent> child);

	void addHeader(const std::string& header);
	void enableBoxedOptions(int boxHight = 3); 
	void disableBoxedOptions();
	void setMenuXY(int x, int y);

	virtual const std::string& getName() const override;
	virtual void call() override;

	explicit CompositeMenu(int x = 0, int y = 0, bool enabledBoxedMenu = false, int boxHight = 3)
		: m_menuXPos{ x }, m_menuYPos{ y }, m_enabledBoxedMenu{ enabledBoxedMenu }
	{//boxHight = 3 is the minimal size for symmetry
		if (m_enabledBoxedMenu)
		{
			enableBoxedOptions(boxHight);
		}//end of if
	}

private:
	std::vector<std::shared_ptr<ContainerComponent> > m_children;
	std::string					     m_header{};
	bool					         m_enabledBoxedMenu;
	bool                             m_enabledHeader{ false };
	int					             m_menuXPos;
	int					             m_menuYPos;
	unsigned int                     m_menuHight;
	unsigned int                     m_menuLength;
	unsigned int                     m_jumpRange{ 1 };

	void updateLengths(const std::string& opt, int padding);
	void printBoxedOptions(int& curXPos, int& curYPos) const;
	void printBareOptions(int& curXPos, int& curYPos) const;
	void printHeader(int& curXPos, int& curYPos) const;
	void printMenu(int curXPos, int curYPos) const;
	void executeOption(intPair coords) const;
	void updateMenuHight();
	int getPadding() const;
};

class Action final : public ContainerComponent
{
public:
	virtual const std::string& getName() const override;
	virtual void call() override;

	explicit Action(const std::string& actionName,
					const std::function<void()> action)
		: m_actionName{ actionName }, m_action{ action } 
	{
	}

private:
	const std::function<void()> m_action;
	const std::string           m_actionName;
};

#endif

