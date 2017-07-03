#include "Menu.h"
#include <string>
#include <algorithm>

namespace
{
	unsigned constexpr char selectionArrow    { 175 };
	constexpr int		    boxedLengthPadding{ 4 };
	constexpr int           boxedHightPadding { 2 };
	constexpr int           yRangePadding     { 2 };
	constexpr int           bareLengthPadding { 1 };
	constexpr int		    xJumpRange        { 0 };

	int findMiddle(int num)
	{
		return ((num / 2) + 1);
	}
}

/*|---COMPOSITE_MENU_FUNCTIONS_START---|*/
/*|---PUBLIC_FUNCTIONS_START---|*/
void CompositeMenu::addChild(const std::shared_ptr<ContainerComponent> child)
{
	if (std::find_if(m_children.begin(),
					 m_children.end(),
		             [&](const std::shared_ptr<ContainerComponent>& cmp)
					 {return cmp->getName() == child->getName(); }) == m_children.end()) 
	{
		if (m_enabledBoxedMenu) updateLengths(child->getName(), boxedLengthPadding);
		else                    updateLengths(child->getName(), bareLengthPadding);
		m_children.emplace_back(std::move(child));
	}//end of if
	else std::cout << "An Action with this name already exists.\n";
}

void CompositeMenu::removeChild(const std::shared_ptr<ContainerComponent> child)
{
	if (std::find_if(m_children.begin(),
					 m_children.end(),
					 [&](const std::shared_ptr<ContainerComponent>& cmp)
					 {return cmp == child; }) != m_children.end())
	{
		m_children.erase(std::remove_if(m_children.begin(),
										m_children.end(),
										[&](const std::shared_ptr<ContainerComponent>& cmp)
										{return cmp == child; }),
			                            m_children.end());
	}//end of if
	else std::cout << "The option doesn't exist.\n";
}

void CompositeMenu::addHeader(const std::string& header)
{
	m_enabledHeader = true;
	m_header        = header;
	if (static_cast<unsigned int>(m_header.length()) + 1 > m_menuLength)
	{
		m_menuLength = static_cast<unsigned int>(m_header.length()) + 1;
	}//end of if
}

void CompositeMenu::enableBoxedOptions(int boxHight)
{//must be an odd number for symmetry reasons
	if ((boxHight >= 3) && (boxHight % 2 != 0))
	{
		m_enabledBoxedMenu = true;
		m_jumpRange        = boxHight;
	}//end of if
	else std::cout << "Invalid box size.\n";
}

void CompositeMenu::disableBoxedOptions()
{
	if (m_enabledBoxedMenu)
	{
		m_enabledBoxedMenu = false;
		m_jumpRange = 1; //default jump range with no boxes
		updateMenuHight();
	}//end of if
}

void CompositeMenu::setMenuXY(int x, int y)
{
	Console::setXY(x, y, m_menuXPos, m_menuYPos);
}

/*|---VIRTUAL_FUNCTIONS_START---|*/
const std::string& CompositeMenu::getName() const
{
	return m_header;
}

void CompositeMenu::call() 
{
	if (static_cast<int>(m_children.size()) == 0)
		std::cout << "No options found, unable to print menu.\n";
	else
	{
		updateMenuHight();
		printMenu(m_menuXPos, m_menuYPos);
		executeOption(Console::navigation(intPair(xJumpRange, m_jumpRange),
			                              intPair(m_menuXPos + 1, m_menuXPos + 1),
			                              intPair(m_menuYPos + getPadding(),
			                              (m_menuYPos + getPadding()) + m_jumpRange * (static_cast<int>(m_children.size()) - 1)),
			                              selectionArrow));
	}//end of else
}
/*|----VIRTUAL_FUNCTIONS_END----|*/
/*|----PUBLIC_FUNCTIONS_END----|*/

/*|---PRIVATE_FUNCTIONS_START---|*/
void CompositeMenu::updateLengths(const std::string& opt, int padding)
{
	if ((static_cast<unsigned int>(opt.length()) + padding) > m_menuLength)
	{
		m_menuLength = static_cast<unsigned int>(opt.length()) + padding;
	}//end of if
}

void CompositeMenu::printBoxedOptions(int& curXPos, int& curYPos) const
{
	for (size_t index{}; index < m_children.size(); ++index)
	{
		Console::drawBox(m_jumpRange - boxedHightPadding, m_menuLength - boxedLengthPadding, curXPos, curYPos);
		Console::gotoxy(curXPos + 2, curYPos + findMiddle(m_jumpRange) - 1);
		std::cout << m_children[index]->getName();
		curYPos += m_jumpRange;
	}//end of for
}

void CompositeMenu::printBareOptions(int& curXPos, int& curYPos) const
{
	++curXPos;
	for (size_t index{}; index < m_children.size(); ++index)
	{
		Console::gotoxy(curXPos, curYPos++);
		std::cout << m_children[index]->getName();
	}//end of for
}

void CompositeMenu::printHeader(int& curXPos, int& curYPos) const
{
	Console::gotoxy(curXPos + 1, curYPos);
	std::cout << m_header;
	Console::gotoxy(curXPos + 1, ++curYPos);
	for (size_t index{}; index < m_header.length(); ++index) std::cout << '-';
	++curYPos;
}

void CompositeMenu::printMenu(int curXPos, int curYPos) const
{
	Console::drawBox(m_menuHight, m_menuLength, curXPos, curYPos);
	curXPos += 2;
	++curYPos;
	if (m_enabledHeader)        printHeader(curXPos, curYPos);
	if (m_enabledBoxedMenu)     printBoxedOptions(curXPos, curYPos);
	else                        printBareOptions(curXPos, curYPos);
}

void CompositeMenu::executeOption(intPair coords) const
{
	int destX, destY;
	if (m_enabledHeader) destX = m_menuXPos + m_menuLength - 1;
	else                 destX = m_menuXPos + m_menuLength;
	if (m_enabledHeader) destY = m_menuYPos + m_menuHight - 3;
	else				 destY = m_menuYPos + m_menuHight;
	Console::clrSec(m_menuXPos, m_menuYPos, destX, destY);
	m_children[(coords.second - (m_menuYPos + getPadding())) / m_jumpRange]->call();
}

void CompositeMenu::updateMenuHight()
{
	if (m_enabledHeader) m_menuHight = m_jumpRange * static_cast<unsigned int>(m_children.size()) + 2;
	else                 m_menuHight = m_jumpRange * static_cast<unsigned int>(m_children.size());
}

int CompositeMenu::getPadding() const 
{
	if (m_enabledHeader) return(findMiddle(m_jumpRange) + yRangePadding);
	else				 return(findMiddle(m_jumpRange));
}
/*|----PRIVATE_FUNCTIONS_END----|*/
/*|----COMPOSITE_MENU_FUNCTIONS_END----|*/

/*|---ACTION_FUNCTIONS_START---|*/
/*|---PUBLIC_FUNCTIONS_START---|*/
/*|---VIRTUAL_FUNCTIONS_START---|*/
const std::string& Action::getName() const
{
	return m_actionName;
}

void Action::call() 
{
	m_action();
}
/*|----VIRTUAL_FUNCTIONS_END----|*/
/*|----PUBLIC_FUNCTIONS_END----|*/
/*|----ACTION_FUNCTIONS_END----|*/