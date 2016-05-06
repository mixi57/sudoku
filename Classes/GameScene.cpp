//
//  GameScene.cpp
//  9Game
//
//  Created by mixi on 15/11/14.
//
//

#include "GameScene.hpp"
#include "MXTime.hpp"

#define RowOrLineNum 9
USING_NS_CC;
Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    // init
    for (int i = 0; i < RowOrLineNum; i++) {
        for (int j =0; j < RowOrLineNum; j++) {
            _indexVec.push_back(std::vector<int>{});
        }
    }
    // 统一以0开始
    // horizontal
    int typeIndex = 0;
    for (int line = 1; line <= RowOrLineNum; line++) {
        std::vector<int> data;
        for (int row = 1; row <= RowOrLineNum; row++) {
            int index = (line - 1) * RowOrLineNum + (row - 1);
            data.push_back(index);
            _indexVec[index].push_back(typeIndex);
        }
        _areaVec.push_back(data);
        typeIndex++;
    }
    
    // vertical
    for (int row = 1; row <= RowOrLineNum; row++) {
        std::vector<int> data;
        for (int line = 1; line <= RowOrLineNum; line++) {
            int index = (line - 1) * RowOrLineNum + (row - 1);
            data.push_back(index);
            _indexVec[index].push_back(typeIndex);
        }
        _areaVec.push_back(data);
        typeIndex++;
    }
    
    // little grid
    int rowIndex = 1, lineIndex = 1;
    int row = 1, line = 1;
    while(lineIndex <= RowOrLineNum and rowIndex <= RowOrLineNum) {
        std::vector<int> data;
        for (line = lineIndex; line < lineIndex + 3; line++) {
            for (row = rowIndex; row < rowIndex + 3; row++) {
                int index = (line - 1) * RowOrLineNum + (row - 1);
                data.push_back(index);
                _indexVec[index].push_back(typeIndex);
            }
        }
        rowIndex += 3;
        if(rowIndex > RowOrLineNum){
            rowIndex = 1;
            lineIndex += 3;
        }
        _areaVec.push_back(data);
        typeIndex++;
    }

    createGrids();
    
    Button* calBtn = Button::create("common_btn_xiaolv.png");
    calBtn->setPosition(Vec2(1000, 400));
    calBtn->setTitleFontSize(30);
    calBtn->setTitleFontName("fonts/stfont.ttf");
    calBtn->setTitleText("步算");
    calBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::calculate, this));
    
    addChild(calBtn);
    
    Button* checkBtn = Button::create("common_btn_xiaolv.png");
    checkBtn->setPosition(Vec2(1000, 200));
    checkBtn->setTitleFontSize(30);
    checkBtn->setTitleFontName("fonts/stfont.ttf");
    checkBtn->setTitleText("计算");
    checkBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::checkAllValue, this));
    
    addChild(checkBtn);

    
    _effectiveGridText = Text::create();
    _effectiveGridText->setFontName("fonts/stfont.ttf");
    _effectiveGridText->setFontSize(30);
    _effectiveGridText->setPosition(Vec2(1000, 300));
    addChild(_effectiveGridText);
    
    updateEffectiveText();
    
    
    return true;
}

GameScene::GameScene()
//:_unsolvedNum(9 * 9)
{
    _indexVec.clear();
    _areaVec.clear();
    _effectiveIndexAndGridMap.clear();
    _existIndexAndGridMap.clear();
}

GameScene::~GameScene()
{
}

std::vector<int> GameScene::getGridsData()
{
    // test
    std::vector<int> data = std::vector<int>
    /*{
        0, 2, 1,  6, 0, 0,  0, 7, 0,
        9, 5, 0,  0, 0, 0,  0, 8, 2,
        0, 6, 0,  0, 0, 9,  5, 3, 0,
        
        0, 7, 0,  1, 0, 0,  0, 0, 0,
        0, 0, 8,  2, 0, 0,  0, 0, 5,
        2, 0, 0,  8, 0, 0,  0, 4, 0,
        
        0, 9, 0,  0, 0, 4,  0, 0, 3,
        6, 0, 0,  0, 0, 1,  8, 0, 0,
        0, 0, 0,  0, 0, 2,  0, 9, 0,
    };*/
    // 初级 一次算完 0s 6次算完
    /*{
        3, 1, 0,  7, 0, 0,  9, 0, 0,
        0, 0, 0,  0, 0, 6,  1, 2, 0,
        0, 6, 2,  0, 4, 8,  3, 7, 0,
        
        0, 0, 0,  0, 0, 0,  0, 4, 0,
        0, 0, 6,  5, 0, 0,  0, 0, 2,
        4, 0, 0,  3, 2, 0,  7, 0, 6,
        
        0, 0, 0,  0, 8, 5,  2, 0, 7,
        0, 2, 0,  0, 0, 0,  0, 8, 3,
        0, 8, 0,  2, 9, 3,  4, 5, 1,
    }; 一次解完 简单版*/
    /*{
        0, 4, 0,  0, 0, 6,  0, 7, 0,
        0, 0, 0,  0, 0, 3,  6, 1, 8,
        6, 1, 3,  7, 8, 9,  4, 0, 2,
        
        0, 9, 6,  2, 0, 8,  7, 0, 0,
        0, 0, 0,  6, 0, 0,  0, 3, 0,
        1, 7, 5,  0, 0, 0,  2, 8, 6,
        
        0, 3, 0,  0, 0, 1,  0, 0, 0,
        9, 8, 0,  0, 0, 7,  1, 0, 0,
        4, 6, 1,  0, 3, 2,  0, 9, 7,
    }; 一次解完 简单版*/
    // 已解 前两种解法无法解
   {
        5, 0, 7,  9, 0, 0,  3, 0, 2,
        0, 0, 2,  0, 0, 6,  5, 7, 0,
        1, 0, 0,  0, 0, 5,  0, 0, 9,
        
        4, 1, 9,  6, 3, 2,  0, 5, 0,
        0, 5, 0,  0, 9, 0,  0, 2, 0,
        2, 7, 0,  0, 5, 1,  9, 0, 0,
        
        8, 0, 5,  0, 0, 3,  0, 0, 4,
        0, 3, 1,  5, 0, 0,  2, 0, 0,
        7, 2, 4,  0, 0, 9,  6, 3, 5,
    };// 简单解法无法解开 新加入 2 3 相同排除法 我好厉害的解完了
     /**/
    /*{
        0, 5, 3,  0, 6, 0,  0, 4, 0,
        0, 0, 0,  8, 0, 0,  0, 5, 0,
        0, 9, 1,  5, 0, 0,  0, 0, 0,
        
        0, 0, 0,  0, 9, 0,  0, 0, 1,
        0, 0, 0,  0, 1, 0,  2, 0, 0,
        7, 0, 5,  0, 0, 0,  0, 0, 4,
        
        0, 0, 0,  0, 0, 0,  0, 0, 6,
        1, 0, 6,  4, 0, 0,  0, 8, 0,
        0, 0, 0,  0, 5, 0,  9, 0, 0,
    }; */// 使用 简单解法 2 3排除 都不能解决
    
    /*{
     0, 2, 1,  0, 0, 6,  0, 0, 7,
     0, 0, 0,  7, 0, 0,  0, 0, 0,
     7, 4, 0,  8, 0, 5,  0, 0, 2,
     
     4, 3, 8,  0, 6, 0,  0, 0, 5,
     1, 5, 0,  9, 8, 4,  0, 3, 6,
     9, 6, 0,  0, 0, 0,  8, 4, 1,
     
     2, 7, 0,  6, 0, 9,  0, 1, 8,
     0, 0, 0,  0, 7, 3,  0, 0, 0,
     6, 0, 0,  0, 0, 8,  5, 7, 0,
     }; *//*完成 测试高级排除法  4次循环可以解决*/
    /*{
     0, 5, 0,  0, 0, 0,  0, 2, 0,
     4, 0, 0,  2, 0, 6,  0, 0, 7,
     0, 0, 8,  0, 3, 0,  1, 0, 0,
     
     0, 1, 0,  0, 0, 0,  0, 6, 0,
     0, 0, 9,  0, 0, 0,  5, 0, 0,
     0, 7, 0,  0, 0, 0,  0, 9, 0,
     
     0, 0, 5,  0, 8, 0,  3, 0, 0,
     7, 0, 0,  9, 0, 1,  0, 0, 4,
     0, 2, 0,  0, 0, 0,  0, 7, 0,
     };*/ /*新加的排除法能解开*/
    /*{
        0, 5, 7,  0, 0, 0,  0, 2, 0,
        4, 0, 0,  2, 0, 6,  0, 0, 7,
        2, 0, 8,  0, 3, 0,  1, 0, 0,
        
        0, 1, 0,  0, 0, 0,  7, 6, 0,
        0, 0, 9,  0, 7, 0,  5, 0, 0,
        0, 7, 0,  0, 0, 0,  0, 9, 0,
        
        0, 0, 5,  0, 8, 0,  3, 1, 0,
        7, 0, 0,  9, 0, 1,  0, 0, 4,
        0, 2, 0,  0, 0, 0,  0, 7, 0,
    }; */// 事例
    /* 暂时没解开
    {
        0, 5, 0,  0, 0, 0,  0, 3, 0,
        0, 0, 0,  0, 4, 0,  0, 0, 0,
        4, 2, 0,  5, 8, 0,  0, 0, 0,
        
        0, 0, 0,  0, 5, 0,  0, 0, 0,
        0, 0, 0,  0, 0, 0,  0, 0, 0,
        0, 0, 0,  0, 0, 4,  0, 0, 0,
        
        0, 0, 0,  0, 3, 0,  0, 0, 0,
        0, 0, 0,  0, 2, 0,  0, 0, 0,
        0, 0, 0,  0, 0, 0,  3, 0, 0,
    };*/
    /*{
     0, 9, 0,  0, 0, 0,  4, 8, 0,
     0, 6, 0,  0, 0, 5,  0, 0, 9,
     8, 7, 0,  0, 9, 3,  0, 0, 0,
     
     0, 0, 3,  6, 0, 0,  0, 2, 7,
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     7, 2, 0,  0, 0, 8,  6, 0, 0,
     
     0, 0, 0,  3, 1, 0,  0, 7, 2,
     6, 0, 0,  2, 0, 0,  0, 9, 0,
     0, 1, 7,  0, 0, 0,  0, 5, 0,
     };
     {
     0, 0, 0,  0, 0, 0,  0, 2, 0,
     0, 5, 0,  7, 0, 0,  0, 0, 4,
     1, 0, 3,  8, 0, 0,  0, 5, 0,
     
     0, 0, 0,  2, 0, 1,  0, 0, 9,
     9, 0, 5,  4, 0, 8,  2, 0, 6,
     7, 0, 0,  6, 0, 3,  0, 0, 0,
     
     0, 9, 0,  0, 0, 6,  3, 0, 8,
     3, 0, 0,  0, 0, 4,  0, 6, 0,
     0, 1, 0,  0, 0, 0,  0, 0, 0,
     };
     {
     0, 6, 0,  0, 0, 0,  0, 3, 9,
     5, 0, 0,  1, 7, 0,  0, 0, 0,
     0, 4, 0,  0, 0, 9,  7, 0, 0,
     
     0, 0, 5,  9, 0, 3,  8, 0, 0,
     0, 7, 0,  0, 0, 0,  0, 5, 0,
     0, 0, 8,  5, 0, 7,  6, 0, 0,
     
     0, 0, 6,  7, 0, 0,  0, 8, 0,
     0, 0, 0,  0, 2, 5,  0, 0, 6,
     3, 9, 0,  0, 0, 0,  0, 4, 0,
     };
    {
        0, 0, 0,  0, 7, 0,  0, 1, 4,
        9, 0, 0,  0, 0, 0,  8, 2, 0,
        0, 0, 0,  3, 0, 9,  0, 0, 5,
        
        0, 2, 0,  5, 0, 0,  1, 0, 0,
        0, 5, 0,  8, 0, 4,  0, 3, 0,
        0, 0, 9,  0, 0, 7,  0, 4, 0,
        
        6, 0, 0,  9, 0, 1,  0, 0, 0,
        0, 9, 7,  0, 0, 0,  0, 0, 8,
        5, 4, 0,  0, 8, 0,  0, 0, 0,
    };
    {
     0, 0, 6,  8, 0, 1,  5, 0, 3,
     0, 5, 0,  7, 0, 0,  1, 0, 8,
     0, 0, 0,  0, 5, 4,  0, 2, 6,
     
     0, 0, 5,  9, 0, 3,  0, 0, 0,
     2, 0, 0,  6, 0, 5,  3, 0, 9,
     0, 0, 0,  0, 7, 2,  8, 0, 0,
     
     0, 4, 0,  5, 3, 0,  0, 8, 0,
     8, 0, 3,  0, 0, 0,  0, 0, 0,
     5, 0, 9,  2, 0, 8,  6, 3, 7,
     };
     {
     0, 5, 3,  0, 0, 0,  7, 1, 0,
     0, 0, 0,  0, 3, 4,  0, 0, 0,
     0, 0, 1,  5, 0, 0,  2, 0, 0,
     
     0, 0, 0,  2, 0, 0,  0, 0, 6,
     6, 0, 0,  0, 0, 0,  0, 0, 9,
     8, 0, 0,  0, 0, 1,  0, 0, 0,
     
     0, 0, 2,  0, 0, 5,  3, 0, 0,
     0, 0, 0,  8, 7, 0,  0, 0, 0,
     0, 4, 9,  0, 0, 0,  5, 2, 0,
     };*/
     /*{
     2, 0, 5,  8, 9, 6,  0, 7, 0,
     0, 0, 0,  7, 3, 5,  8, 0, 0,
     8, 0, 7,  4, 2, 1,  0, 9, 5,

     3, 0, 0,  1, 0, 9,  5, 8, 0,
     1, 7, 0,  5, 8, 4,  0, 6, 0,
     0, 5, 8,  3, 0, 2,  0, 0, 0,

     4, 9, 0,  2, 5, 8,  7, 0, 0,
     7, 0, 1,  9, 4, 3,  0, 5, 8,
     5, 8, 0,  6, 1, 7,  9, 0, 0,
     };*/

    /*
     {
     5, 0, 0,  0, 8, 0,  9, 0, 7,
     0, 0, 0,  6, 0, 0,  0, 0, 0,
     6, 0, 0,  0, 0, 5,  8, 0, 0,
     
     3, 0, 0,  7, 2, 9,  0, 1, 0,
     0, 1, 0,  0, 0, 0,  0, 2, 0,
     0, 7, 0,  3, 1, 8,  0, 0, 9,
     
     0, 0, 3,  8, 0, 0,  0, 0, 5,
     0, 0, 0,  0, 0, 7,  0, 0, 0,
     4, 0, 8,  0, 9, 0,  0, 0, 1,
     };//未解答
    */
    /*
     {
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     0, 0, 0,  0, 0, 0,  0, 0, 0,
     };
     */
    
    return data;
}

void GameScene::createGrids()
{
    int interval = 80;
    Vec2 leftTopPos = Vec2(60, -20);
    std::vector<int> data = getGridsData();
    for (int line = 0, row; line < RowOrLineNum; line++) {
        for (row = 0; row < RowOrLineNum; row++) {
            int index = line * RowOrLineNum + row;
            Grid* grid = Grid::create(data[index], row, line, index);
            grid->setPosition(leftTopPos + Vec2(row * interval, (RowOrLineNum - line) * interval));
            addChild(grid);
            _gridVec.push_back(grid);
            if (data[index] == 0) {
                _effectiveGridVec.push_back(grid);
                std::vector<int> indexVec = _indexVec[index];
                for (auto indexIter = indexVec.begin(); indexIter != indexVec.end(); indexIter++) {
                    int newIndex = *indexIter;
                    std::map<int, std::vector<Grid*>>::iterator iter = _effectiveIndexAndGridMap.find(newIndex);
                    if (iter == _effectiveIndexAndGridMap.end()) {
                        _effectiveIndexAndGridMap[newIndex] = {};
                        iter = _effectiveIndexAndGridMap.find(newIndex);
                        //                    _effectiveIndexAndGridMap.insert(std::pair<int, std::vector<Grid*>>(index,));
                    }
                    iter->second.push_back(grid);
                }
            } else{
                std::vector<int> indexVec = _indexVec[index];
                for (auto indexIter = indexVec.begin(); indexIter != indexVec.end(); indexIter++) {
                    int newIndex = *indexIter;
                    std::map<int, std::vector<Grid*>>::iterator iter = _existIndexAndGridMap.find(newIndex);
                    if (iter == _existIndexAndGridMap.end()) {
                        _existIndexAndGridMap[newIndex] = {};
                        iter = _existIndexAndGridMap.find(newIndex);
                        //                    _effectiveIndexAndGridMap.insert(std::pair<int, std::vector<Grid*>>(index,));
                    }
                    iter->second.push_back(grid);
                }
            }
        }
    }
    _calUtils = CalUtils::CalUtils(_areaVec, _indexVec, _effectiveIndexAndGridMap, _existIndexAndGridMap, _gridVec, _effectiveGridVec);
}

void GameScene::calculate(Ref *pSender, Widget::TouchEventType eventType)
{
    if (eventType == Widget::TouchEventType::ENDED) {
        _calUtils.calculate();
        updateEffectiveText();
    }
}

void GameScene::checkAllValue(Ref *pSender, Widget::TouchEventType eventType)
{
    if (eventType == Widget::TouchEventType::ENDED) {
        for(auto areaIndex : _areaVec){
            // Grid_Vec
            std::vector<int> gridIndexVec = areaIndex;
            int sum = 0;
            for (auto gridIndex: gridIndexVec)
            {
                Grid* grid = _gridVec[gridIndex];
                sum += grid->getValue();
            } 
            if (sum != 45)
            {
                printf("has value false");
            }
        }
    }
}

void GameScene::checkOneAnswer()
{
    /*for (std::vector<Grid*>::iterator iter = _effectiveGridVec.begin(); iter != _effectiveGridVec.end(); iter++) {
        Grid* grid = *iter;
        grid->clearValues();
        int gridIndex = grid->getIndex();
        
        // 参考线
        std::vector<int> ids = _indexVec[gridIndex];
        for (int id = 0; id < ids.size(); id++) {
            std::vector<int> typeVec = _areaVec[ids[id]];
            for (int typeIndex = 0; typeIndex < typeVec.size(); typeIndex++) {
                int type = typeVec[typeIndex];
                if (type != gridIndex) {
                    Grid* typeGrid = _gridVec[type];
                    int value = typeGrid->getValue();
                    if (value > 0) {
                        grid->addUnPurValue(value);
                    }
                }
            }
        }
        if(grid->showText()){
            if (!needTurn) {
                needTurn = true;
            }
            iter = _effectiveGridVec. erase(iter);
            iter--;
        }
    }*/
}

void GameScene::updateEffectiveText()
{
    _effectiveGridText->setString(StringUtils::format("%lu", _effectiveGridVec.size() - _calUtils.getSolveNum()));
}

void GameScene::twoThreeCalcute()
{
    //最蠢的方法

        // 参考线
        for (int id = 0; id < _areaVec.size(); id++) {
            std::vector<int> typeVec = _areaVec[id];
            std::vector<std::map<int, std::vector<int>>> needCalVec;
            // 每条线中的处理
            for (int typeIndex = 0; typeIndex < typeVec.size(); typeIndex++) {
                int type = typeVec[typeIndex];
//                if (type != gridIndex) {
                    Grid* typeGrid = _gridVec[type];
                    int value = typeGrid->getValue();
                    
                    if (value == 0) {
                        std::vector<int> purValue = typeGrid->_purValue;
                        std::map<int, std::vector<int>> mapValue;
                        mapValue.insert(std::pair<int, std::vector<int>>(type, purValue));
                        needCalVec.push_back(mapValue);
                    }
//                }
            }
            //两个格子以上才有意义
            long needCalSize = needCalVec.size();
            if (needCalSize > 2) {
                for (int i = 0; i < needCalSize - 1; i++) {
                    for (int j = i + 1; j < needCalSize ; j++) {
                        
                    }
                }
            }
        }
//        if(grid->showText()){
////            if (!needTurn) {
////                needTurn = true;
////            }
//            iter = _effectiveGridVec. erase(iter);
//            iter--;
//        }
    
    
}

void GameScene::pairsAllResult()
{
    for(std::vector<Grid*>::iterator iter = _effectiveGridVec.begin();
        iter != _effectiveGridVec.end();
        iter++){
        Grid* grid = *iter;
        int index = grid->getIndex();
    }
}

