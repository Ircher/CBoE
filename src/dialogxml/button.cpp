
/*
 *  button.cpp
 *  BoE
 *
 *  Created by Celtic Minstrel on 11/05/09.
 *
 */

#include "button.hpp"
#include <vector>
#include <map>
#include <stdexcept>

#include "dialog.hpp"
#include "graphtool.hpp"

#include <cmath>
#include <climits>

#include "restypes.hpp"

extern sf::Texture bg_gworld;

void cButton::attachFocusHandler(focus_callback_t) throw(xHandlerNotSupported){
	throw xHandlerNotSupported(true);
}

void cButton::attachClickHandler(click_callback_t f) throw(){
	onClick = f;
}

bool cButton::triggerClickHandler(cDialog& me, std::string id, eKeyMod mods){
	if(onClick) return onClick(me,id,mods);
	return false;
}

cButton::cButton(sf::RenderWindow& parent) :
	cControl(CTRL_BTN,parent),
	wrapLabel(false),
	type(BTN_REG),
	textClr(sf::Color::Black),
	fromList("none") {}

cButton::cButton(cDialog& parent) :
	cControl(CTRL_BTN,parent),
	wrapLabel(false),
	type(BTN_REG),
	textClr(parent.getDefTextClr()),
	fromList("none") {}

cButton::cButton(cDialog* parent,eControlType t) :
	cControl(t,*parent),
	fromList("none"),
	wrapLabel("true") {/* This constructor is only called for LEDs. TODO: Should wrapLabel be true for LEDs? */}

bool cButton::isClickable(){
	return true;
}

void cButton::draw(){
	rectangle from_rect, to_rect;
	
	inWindow->setActive();
	
	if(visible){
		TextStyle style;
		if(type == BTN_TINY) style.pointSize = 9;
		else if(type == BTN_PUSH) style.pointSize = 10;
		else style.pointSize = 12;
		from_rect = btnRects[type][depressed];
		to_rect = frame;
		if(type == BTN_TINY) {
			to_rect.right = to_rect.left + 14;
			to_rect.bottom = to_rect.top + 10;
		}
		rect_draw_some_item(buttons[btnGW[type]],from_rect,*inWindow,to_rect,sf::BlendAlpha);
		style.colour = sf::Color::Black;
		style.lineHeight = 8;
		eTextMode textMode = eTextMode::CENTRE;
		if(type == BTN_TINY) {
			textMode = eTextMode::LEFT_TOP;
			to_rect.left += 18;
			style.colour = textClr;
		} else if(type == BTN_PUSH) {
			to_rect.top += 34;
		}
		win_draw_string(*inWindow,to_rect,lbl,textMode,style);
		// TODO: Adjust string location as appropriate
		// Tiny button string location should be shifted right 20 pixels (or possibly 18)
		// Push button string should be centred below the button
		// Others may need adjustments too, not sure
		// TODO: How is it supposed to know it's a default button when this fact is stored in the dialog, not the button?
		if(key.spec && key.k == key_enter) drawFrame(2,frameStyle); // frame default button, to provide a visual cue that it's the default
	} else if(parent) {
		tileImage(*inWindow,frame,bg[parent->getBg()]);
	}
}

void cButton::setFormat(eFormat prop, short val) throw(xUnsupportedProp){
	if(prop == TXT_WRAP) wrapLabel = val;
	else if(prop == TXT_FRAMESTYLE) frameStyle = val;
	else throw xUnsupportedProp(prop);
}

short cButton::getFormat(eFormat prop) throw(xUnsupportedProp){
	if(prop == TXT_WRAP) return wrapLabel;
	else if(prop == TXT_FRAMESTYLE) return frameStyle;
	else throw xUnsupportedProp(prop);
}

void cButton::setColour(sf::Color clr) throw(xUnsupportedProp) {
	textClr = clr;
}

sf::Color cButton::getColour() throw(xUnsupportedProp) {
	return textClr;
}

// Indices within the buttons array.
size_t cButton::btnGW[14] = {
	0, // BTN_SM
	1, // BTN_REG
	2, // BTN_LG
	4, // BTN_HELP
	1, // BTN_LEFT
	1, // BTN_RIGHT
	1, // BTN_UP
	1, // BTN_DOWN
	5, // BTN_TINY
	1, // BTN_DONE
	3, // BTN_TALL
	3, // BTN_TRAIT
	6, // BTN_PUSH
	5, // BTN_LED
};

sf::Texture cButton::buttons[7];
rectangle cButton::btnRects[13][2];

void cButton::init(){
	static const char*const buttonFiles[7] = {
		"dlogbtnsm",
		"dlogbtnmed",
		"dlogbtnlg",
		"dlogbtntall",
		"dlogbtnhelp",
		"dlogbtnled",
		"dlgbtnred"
	};
	for(int i = 0; i < 7; i++)
		buttons[i].loadFromImage(*ResMgr::get<ImageRsrc>(buttonFiles[i]));
	btnRects[BTN_SM][0] = {0,0,23,23};
	btnRects[BTN_REG][0] = {0,0,23,63};
	btnRects[BTN_LEFT][0] = {23,0,46,63};
	btnRects[BTN_RIGHT][0] = {46,0,69,63};
	btnRects[BTN_UP][0] = {69,0,92,63};
	btnRects[BTN_DOWN][0] = {92,0,115,63};
	btnRects[BTN_DONE][0] = {115,0,138,63};
	btnRects[BTN_LG][0] = {0,0,23,102};
	btnRects[BTN_HELP][0] = {0,0,13,16};
	btnRects[BTN_TINY][0] = {0,42,10,56};
	btnRects[BTN_TALL][0] = {0,0,40,63};
	btnRects[BTN_TRAIT][0] = {40,0,80,63};
	btnRects[BTN_PUSH][0] = {0,0,30,30};
	for(int j = 0; j < 12; j++)
		btnRects[j][1] = btnRects[j][0];
	btnRects[BTN_SM][1].offset(23,0);
	btnRects[BTN_REG][1].offset(63,0);
	btnRects[BTN_LEFT][1].offset(63,0);
	btnRects[BTN_RIGHT][1].offset(63,0);
	btnRects[BTN_UP][1].offset(63,0);
	btnRects[BTN_DOWN][1].offset(63,0);
	btnRects[BTN_DONE][1].offset(63,0);
	btnRects[BTN_LG][1].offset(102,0);
	btnRects[BTN_HELP][1].offset(16,0);
	btnRects[BTN_TINY][1].offset(0,10);
	btnRects[BTN_TALL][1].offset(63,0);
	btnRects[BTN_TRAIT][1].offset(63,0);
	btnRects[BTN_PUSH][1].offset(30,0);
}

rectangle cLed::ledRects[3][2];

void cLed::init(){
	rectangle baseLed = {0,0,10,14};
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 2; j++){
			ledRects[i][j] = baseLed;
			ledRects[i][j].offset(i * 14, j * 10);
		}
}

cLed::cLed(cDialog* parent) :
	cButton(parent,CTRL_LED),
	state(led_off),
	textFont(FONT_BOLD),
	textSize(10) {
	type = BTN_LED;
}

void cLed::attachClickHandler(click_callback_t f) throw(){
	onClick = f;
}

void cLed::attachFocusHandler(focus_callback_t f) throw(){
	onFocus = f;
}

bool cLed::triggerFocusHandler(cDialog& me, std::string id, bool losing){
	if(onFocus != NULL) return onFocus(me,id,losing);
	return true;
}

bool cLed::triggerClickHandler(cDialog& me, std::string id, eKeyMod mods){
	bool result;
	eLedState oldState = state;
	if(onClick != NULL) result = onClick(me,id,mods);
	else{ // simple state toggle
		switch(state){
			case led_red:
			case led_green:
				state = led_off;
				break;
			case led_off:
				state = led_red;
		}
		result = true;
	}
	if(!triggerFocusHandler(me,id, oldState != led_off)){
		result = false;
		state = oldState;
	}
	return result;
}

void cLed::setFormat(eFormat prop, short val) throw(xUnsupportedProp){
	if(prop == TXT_FONT) textFont = (eFont) val;
	else if(prop == TXT_SIZE) textSize = val;
	else throw xUnsupportedProp(prop);
}

short cLed::getFormat(eFormat prop) throw(xUnsupportedProp){
	if(prop == TXT_FONT) return textFont;
	else if(prop == TXT_SIZE) return textSize;
	else throw xUnsupportedProp(prop);
}

void cLed::draw(){
	rectangle from_rect, to_rect;
	
	inWindow->setActive();
	
	if(visible){
		TextStyle style;
		style.pointSize = textSize;
		style.lineHeight = textSize - 1;
		style.font = textFont;
		from_rect = ledRects[state][depressed];
		to_rect = frame;
		to_rect.right = to_rect.left + 14;
		to_rect.bottom = to_rect.top + 10;
		rect_draw_some_item(buttons[btnGW[BTN_LED]],from_rect,*inWindow,to_rect);
		style.colour = textClr;
		to_rect.right = frame.right;
		to_rect.left = frame.left + 18; // Possibly could be 20
		win_draw_string(*inWindow,to_rect,lbl,eTextMode::LEFT_TOP,style);
	} else if(parent) {
		tileImage(*inWindow,frame,bg[parent->getBg()]);
	}
}

cControl::storage_t cLed::store() {
	storage_t storage = cButton::store();
	storage["led-state"] = getState();
	return storage;
}

void cLed::restore(storage_t to) {
	cButton::restore(to);
	if(to.find("led-state") != to.end())
		setState(boost::any_cast<eLedState>(to["led-state"]));
	else setState(led_off);
}

cLedGroup::cLedGroup(cDialog* parent) :
	cControl(CTRL_GROUP,*parent),
	fromList("none") {}

cButton::~cButton() {}

cLed::~cLed() {}

cLedGroup::~cLedGroup(){
	ledIter iter = choices.begin();
	while(iter != choices.end()){
		delete iter->second;
		iter++;
	}
}

void cLedGroup::recalcRect(){
	ledIter iter = choices.begin();
	frame = {INT_MAX, INT_MAX, 0, 0};
	while(iter != choices.end()){
		rectangle otherFrame = iter->second->getBounds();
		if(otherFrame.right > frame.right)
			frame.right = otherFrame.right;
		if(otherFrame.bottom > frame.bottom)
			frame.bottom = otherFrame.bottom;
		if(otherFrame.left < frame.left)
			frame.left = otherFrame.left;
		if(otherFrame.top < frame.top)
			frame.top = otherFrame.top;
		iter++;
	}
	frame.inset(-6,-6);
}

void cLedGroup::attachClickHandler(click_callback_t f) throw() {
	onClick = f;
}

void cLedGroup::attachFocusHandler(focus_callback_t f) throw() {
	onFocus = f;
}

void cLed::setState(eLedState to){
	state = to;
}

eLedState cLed::getState(){
	return state;
}

void cLedGroup::addChoice(cLed* ctrl, std::string key) {
	choices[key] = ctrl;
	if(ctrl->getState() != led_off)
		setSelected(key);
}

bool cLedGroup::handleClick(location where) {
	std::string which_clicked;
	ledIter iter = choices.begin();
	while(iter != choices.end()){
		if(iter->second->isVisible() && where.in(iter->second->getBounds())){
			if(iter->second->handleClick(where)) {
				which_clicked = iter->first;
				break;
			}
		}
		iter++;
	}
	
	if(which_clicked == "") return false;
	
	clicking = which_clicked;
	return true;
}

bool cLedGroup::triggerClickHandler(cDialog& me, std::string id, eKeyMod mods){
	std::string which_clicked = clicking;
	clicking = "";
	
	if(choices[which_clicked]->triggerClickHandler(me,which_clicked,mods)){
		if(onClick && !onClick(me,id,mods)) return false;
		if(!curSelect.empty()) {
			choices[curSelect]->setState(led_off);
			if(!choices[curSelect]->triggerFocusHandler(me,curSelect,true)){
				choices[curSelect]->setState(led_red);
				return false;
			}
		}
		choices[which_clicked]->setState(led_red);
		if(!choices[which_clicked]->triggerFocusHandler(me,which_clicked,false)){
			if(!curSelect.empty())
				choices[curSelect]->setState(led_red);
			choices[which_clicked]->setState(led_off);
			return false;
		}
	}else return false;
	
	std::string savePrevSelect = prevSelect;
	prevSelect = curSelect;
	curSelect = which_clicked;
	if(!triggerFocusHandler(me,id,false)) {
		if(!curSelect.empty())
			choices[curSelect]->setState(led_red);
		choices[which_clicked]->setState(led_off);
		curSelect = prevSelect;
		prevSelect = savePrevSelect;
		return false;
	}
	return true;
}

bool cLedGroup::triggerFocusHandler(cDialog& me, std::string id, bool losingFocus){
	if(onFocus != NULL) return onFocus(me,id,losingFocus);
	return true;
}

void cLedGroup::disable(std::string /*id*/) {
	// TODO: Implement this
}

void cLedGroup::enable(std::string /*id*/) {
	// TODO: Implement this
}

void cLedGroup::show(std::string id){
	choices[id]->show();
}

void cLedGroup::hide(std::string id){
	choices[id]->hide();
}

void cLedGroup::setFormat(eFormat prop, short) throw(xUnsupportedProp) {
	throw xUnsupportedProp(prop);
}

short cLedGroup::getFormat(eFormat prop) throw(xUnsupportedProp) {
	throw xUnsupportedProp(prop);
}

void cLedGroup::setColour(sf::Color) throw(xUnsupportedProp) {
	// TODO: Colour is not supported
}

sf::Color cLedGroup::getColour() throw(xUnsupportedProp) {
	// TODO: Colour is not supported
	return sf::Color();
}

bool cLedGroup::isClickable(){
	return true;
}

cLed& cLedGroup::operator[](std::string id){
	ledIter iter = choices.find(id);
	if(iter == choices.end()) throw std::invalid_argument(id + " does not exist in the ledgroup.");
	return *(iter->second);
}

void cLedGroup::setSelected(std::string id){
	if(id == "") { // deselect all
		if(curSelect == "") return;
		eLedState was = choices[curSelect]->getState();
		choices[curSelect]->setState(led_off);
		if(choices[curSelect]->triggerFocusHandler(*parent,curSelect,true))
			curSelect = "";
		else
			choices[curSelect]->setState(was);
		return;
	}
	
	ledIter iter = choices.find(id);
	if(iter == choices.end()) throw std::invalid_argument(id + " does not exist in the ledgroup.");
	
	if(curSelect == ""){
		if(iter->second->triggerFocusHandler(*parent,curSelect,false)){
			iter->second->setState(led_red);
			curSelect = iter->first;
		}
	}else{
		eLedState a, b;
		a = choices[curSelect]->getState();
		b = iter->second->getState();
		choices[curSelect]->setState(led_off);
		iter->second->setState(led_red);
		if(!choices[curSelect]->triggerFocusHandler(*parent,curSelect,true)){
			choices[curSelect]->setState(a);
			iter->second->setState(b);
			return;
		}
		if(!iter->second->triggerFocusHandler(*parent,curSelect,false)){
			choices[curSelect]->setState(a);
			iter->second->setState(b);
			return;
		}
		curSelect = iter->first;
	}
}

std::string cLedGroup::getSelected(){
	return curSelect;
}

std::string cLedGroup::getPrevSelection(){
	return prevSelect;
}

void cLedGroup::draw(){
	if(!visible) return;
	ledIter iter = choices.begin();
	while(iter != choices.end()){
		iter->second->draw();
		iter++;
	}
}

void cButton::setBtnType(eBtnType newType){
	if(type == BTN_LED || newType == BTN_LED) return; // can't change type
	type = newType;
	switch(newType) {
		case BTN_SM:
			frame.width() = 23;
			frame.height() = 23;
			break;
		case BTN_REG: case BTN_DONE:
		case BTN_LEFT: case BTN_RIGHT:
		case BTN_UP: case BTN_DOWN:
			frame.width() = 63;
			frame.height() = 23;
			break;
		case BTN_LG:
			frame.width() = 102;
			frame.height() = 23;
			break;
		case BTN_HELP:
			frame.width() = 16;
			frame.height() = 13;
			break;
		case BTN_TALL:
		case BTN_TRAIT:
			frame.width() = 63;
			frame.height() = 40;
			break;
		case BTN_PUSH:
			frame.width() = 30;
			frame.height() = 30;
			break;
	}
}

eBtnType cButton::getBtnType(){
	return type;
}

cControl::storage_t cLedGroup::store() {
	storage_t storage = cControl::store();
	storage["led-select"] = getSelected();
	return storage;
}

void cLedGroup::restore(storage_t to) {
	cControl::restore(to);
	if(to.find("led-select") != to.end())
		setSelected(boost::any_cast<std::string>(to["led-select"]));
	else setSelected("");
}
