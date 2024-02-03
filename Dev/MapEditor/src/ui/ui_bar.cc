#include "ui_bar.h"
#include <stdexcept>

UI_Bar::UI_Bar(SDL_Color *color,UI_BAR_TYPE barType)
{

    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->barType = barType;
    this->color = color;

	throw std::runtime_error("Not Implemented!");
}


UI_Bar::~UI_Bar()
{
    /*
    UI_Item *temp;

    //Empty the list and free the buttons
	while(items.empty() == false){

		temp = items.front();
		items.pop_front();
        delete temp;
    }
    */
}


void UI_Bar::addItem(IItem * newitem)
{
    items.push_back(newitem);
    updateItems();
}


bool UI_Bar::handleEvents(SDL_Event * event)
{
	// pass event to all bar members
    for (auto i : items)
    {
        i->handleEvents(event);
	}

	// TODO: should only pass event untill
	// one item accepts it

	return true;
}


void UI_Bar::draw()
{
    int cx,cy;


    cx = x;
    cy = y;

    //Background
	FillRect(x, y, x+w, y+h, color);


	for(auto & it : items){

		it->draw();

        if(barType == UI_BAR_HORIZONTAL){
            cx += it->getWidth();
        }
        else{
            cy += it->getHeight();
        }
    }

}


void UI_Bar::setX(int x){ this->x = x; }


int  UI_Bar::getX(){ return x; }


void UI_Bar::setY(int y){ this->y = y; }


int  UI_Bar::getY(){ return y; }


int UI_Bar::getWidth(){ return w; }


int UI_Bar::getHeight(){ return h; }

/*Update the position of all items*/
void UI_Bar::updateItems()
{

    std::list<IItem*>::iterator it;

    w = 0;
    h = 0;

	for(auto & it : items) {

        if(barType == UI_BAR_HORIZONTAL){
            it->setX(x+w);
            it->setY(y);
            w += it->getWidth();

            if(it->getHeight() > h){
                h = it->getHeight();
            }
        }
        else{

            it->setX(x);
            it->setY(y + h);

            h += it->getHeight();

            if(it->getWidth() > w){

                w = it->getWidth();
            }
        }

		// TODO: call update items??
		//it->updateItems();
    }
}
