//
//  Grid.cpp
//  9Game
//
//  Created by mixi on 15/11/14.
//
//

#include "Grid.hpp"

USING_NS_CC;
Grid::Grid()
: _isStart(false)
, _index(0)
, _text(nullptr)
, _value(0)
, _gatherValue(0)
, _rowIndex(-1)
, _lineIndex(-1)
, _isInit(false)
{
}

Grid::~Grid()
{
}

bool Grid::init()
{
    for (int i = 1; i <= 9; i++) {
        _gatherValue += i;
    }
    _imgBg = ImageView::create("gridBg.png");
    _imgBg->setContentSize(Size::Size(80, 80));
    _imgBg->ignoreContentAdaptWithSize(false);
    addChild(_imgBg, -1);
    return true;
}

Grid* Grid::create(int num, int index)
{
    Grid * grid = new Grid();
    
    if(!grid->init()){
        return nullptr;
    };
    if(num > 0){
        grid->_isStart = true;
        grid->_value = num;
        grid->_purValue.push_back(num);
    }
    grid->_index = index;
    grid->showText();
    return grid;
}

Grid* Grid::create(int num, int rowIndex, int lineIndex, int index)
{
    Grid * grid = new Grid();
    
    if(!grid->init()){
        return nullptr;
    };
    if(num > 0){
        grid->_isStart = true;
        grid->_value = num;
        grid->_purValue.push_back(num);
    }
    grid->_rowIndex = rowIndex;
    grid->_lineIndex = lineIndex;
    grid->_index = index;
    grid->showText();
    return grid;
}


void Grid::addPurValue(int num)
{
    _isInit = true;
    for (int i = 0; i < _purValue.size(); i++) {
        if (_purValue[i] == num) {
            return;
        }
    }
    _purValue.push_back(num);
}

bool Grid::addUnPurValue(int num)
{
    _isInit = true;
    for (int i = 0; i < _noPurValue.size(); i++) {
        if (_noPurValue[i] == num) {
            return false;
        }
    }
    if (num == 0) {
        printf("why 0");
    }
    _noPurValue.push_back(num);
    return true;
}

bool Grid::removePurValue(int value)
{
    bool var = false;
    bool exist = false;
    for (int i = 0; i < _noPurValue.size(); i++) {
        if (_noPurValue[i] == value) {
            exist = true;
            break;
        }
    }
    if (!exist) {
        var = true;
        _noPurValue.push_back(value);
    }
    for (std::vector<int>::iterator iter = _purValue.begin(); iter != _purValue.end(); ++iter) {
        if (*iter == value) {
            _purValue.erase(iter);
            var = true;
            break;
        }
    }
    return var;
}

void Grid::setPurValues(std::vector<int> values)
{
    _purValue.clear();
    _purValue = values;
}

void Grid::clearValues()
{
    _purValue.clear();
    _noPurValue.clear();
}

void Grid::setUnPurValues(std::vector<int> values)
{
    _noPurValue.clear();
    _noPurValue = values;
}

bool Grid::showText()
{
    if(_value > 0){
        if(!_text){
            _text = Text::create();
            _text->setFontName("fonts/arial.ttf");
            _text->setFontSize(30);
            addChild(_text);
        }
//        _text->setEnabled(true);
        _text->setString(StringUtils::format("%d", _value)); //(_purValue[1]);
        if (_isStart) {
            _text->enableOutline(Color4B(255, 0, 0, 255));
        }
        removePurText();
    } else {
        if (_isInit) {
            if (_purValue.size() > 1 || _noPurValue.size() < 8){
                std::vector<Text*> textVec = getPurText();
                for (int i = 0; i < _noPurValue.size(); i++) {
                    int index = _noPurValue[i] - 1;
                    textVec[index]->setVisible(false);
                }
            } else if (_noPurValue.size() == 8){
                std::vector<Text*> textVec = getPurText();
                int tolValue = 0;
                for (int i = 0; i < _noPurValue.size(); i++) {
                    tolValue += _noPurValue[i];
                }
                _value = _gatherValue - tolValue;
                showText();
                return true;
            }
        }
    }
    return false;
}

int Grid::getIndex()
{
    return _index;
}

int Grid::getValue()
{
    return _value;
}

void Grid::setValue(int value)
{
    _value = value;
    showText();
}

std::vector<int> Grid::getPurValue()
{
    std::vector<int> valueVec;
    if(_value == 0){
        if (_purValue.size() > 0) {
            return _purValue;
        }
        std::vector<bool> existValueVec(9, true);
        for (std::vector<int>::iterator iter = _noPurValue.begin(); iter != _noPurValue.end(); iter++) {
            existValueVec[*iter - 1] = false;
        }
        for (int i = 1; i <= existValueVec.size(); i++) {
            if (existValueVec[i - 1]) {
                valueVec.push_back(i);
            }
        }
    }
    return valueVec;
}

std::vector<Text*> Grid::getPurText()
{
    if (_purText.size() == 0) {
        for (int i = 0; i < 9; i++) {
            Text* text = Text::create(StringUtils::format("%d", i + 1), "", 20);
            text->setPosition(Vec2(-20, -40) + Vec2(i % 3 * 20, (3 - i / 3) * 20));
//            text->setEnabled(false);
            _purText.push_back(text);
            addChild(text);
        }
    }
    return _purText;
}

void Grid::removePurText()
{
    for (std::vector<Text*>::iterator iter = _purText.begin(); iter != _purText.end(); iter++) {
        Text * text = *iter;
        text->removeFromParent();
        _purText.erase(iter);
        iter--;
    }
}