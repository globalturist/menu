#include "Menu.h"
#include "Console.h"

void a() { std::cout << 'a'; }
void b() { std::cout << 'b'; }
void c() { std::cout << 'c'; }
void d() { std::cout << 'd'; }

int main()
{
	CompositeMenu menu{ 5,5,true }; 
	//std::shared_ptr<CompositeMenu> menu{ std::make_shared<CompositeMenu>(5, 5, true) };
	std::shared_ptr<ContainerComponent> submenu{ std::make_shared<CompositeMenu>(5, 5) };
	std::dynamic_pointer_cast<CompositeMenu>(submenu)->addHeader("Sub menu");
	menu.addHeader("Main menu");
	std::shared_ptr<ContainerComponent> optA{ std::make_shared<Action>("a", a) };
	std::shared_ptr<ContainerComponent> optB{ std::make_shared<Action>("b", b) };
	std::shared_ptr<ContainerComponent> optC{ std::make_shared<Action>("c", c) };
	std::shared_ptr<ContainerComponent> optD{ std::make_shared<Action>("d", d) };
	
	menu.addChild(optA);
	menu.addChild(optB);
    menu.addChild(optC);
	menu.addChild(optD);
    menu.removeChild(optB);
	
	std::dynamic_pointer_cast<CompositeMenu>(submenu)->addChild(optA);
	std::dynamic_pointer_cast<CompositeMenu>(submenu)->addChild(optB);
	std::dynamic_pointer_cast<CompositeMenu>(submenu)->addChild(optC);
	std::dynamic_pointer_cast<CompositeMenu>(submenu)->addChild(optD);
	
	menu.addChild(submenu);
	menu.call();
	return 0;
}