//
//  Grid.hpp
//  9Game
//
//  Created by mixi on 15/11/14.
//
//

#ifndef Grid_hpp
#define Grid_hpp

#include <stdio.h>
#include "ui/cocosGUI.h"

using namespace cocos2d::ui;

class Grid : public Widget
{
    Grid();
    ~Grid();
    
    virtual bool init();
    
    bool _isStart;
    
    
    int _index;
    int _value;
    int _gatherValue;
    
    CC_SYNTHESIZE(int, _rowIndex, RowIndex);
    CC_SYNTHESIZE(int, _lineIndex, LineIndex);
    
    Text* _text;
    ImageView* _imgBg;
    std::vector<Text*>_purText;
    bool _isInit;
    
public:
    
    static Grid* create(int num, int index);
    static Grid* create(int num, int rowIndex, int lineIndex, int index);
    
    bool showText();
    void addPurValue(int num);
    void addUnPurValue(int num);
    void setPurValues(std::vector<int> values);
    void setUnPurValues(std::vector<int> values);
    void clearValues();
    void removePurText();
    int getIndex();
    int getValue();
    void setValue(int value);
    std::vector<int> getPurValue();
    bool removePurValue(int value);
    
    std::vector<int> _purValue; // 可能出现的数据
    std::vector<int> _noPurValue; // 不可能出现的数据
    
    std::vector<Text*> getPurText();
    CREATE_FUNC(Grid);
};

#endif /* Grid_hpp */
