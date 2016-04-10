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
:_solveNum(0)
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
    };*/
    {
        0, 4, 0,  0, 0, 6,  0, 7, 0,
        0, 0, 0,  0, 0, 3,  6, 1, 8,
        6, 1, 3,  7, 8, 9,  4, 0, 2,
        
        0, 9, 6,  2, 0, 8,  7, 0, 0,
        0, 0, 0,  6, 0, 0,  0, 3, 0,
        1, 7, 5,  0, 0, 0,  2, 8, 6,
        
        0, 3, 0,  0, 0, 1,  0, 0, 0,
        9, 8, 0,  0, 0, 7,  1, 0, 0,
        4, 6, 1,  0, 3, 2,  0, 9, 7,
    };
    // 已解 前两种解法无法解
    /*{
        5, 0, 7,  9, 0, 0,  3, 0, 2,
        0, 0, 2,  0, 0, 6,  5, 7, 0,
        1, 0, 0,  0, 0, 5,  0, 0, 9,
        
        4, 1, 9,  6, 3, 2,  0, 5, 0,
        0, 5, 0,  0, 9, 0,  0, 2, 0,
        2, 7, 0,  0, 5, 1,  9, 0, 0,
        
        8, 0, 5,  0, 0, 3,  0, 0, 4,
        0, 3, 1,  5, 0, 0,  2, 0, 0,
        7, 2, 4,  0, 0, 9,  6, 3, 5,
    };*/
    /*
    {
        0, 5, 3,  0, 6, 0,  0, 4, 0,
        0, 0, 0,  8, 0, 0,  0, 5, 0,
        0, 9, 1,  5, 0, 0,  0, 0, 0,
        
        0, 0, 0,  0, 9, 0,  0, 0, 1,
        0, 0, 0,  0, 1, 0,  2, 0, 0,
        7, 0, 5,  0, 0, 0,  0, 0, 4,
        
        0, 0, 0,  0, 0, 0,  0, 0, 6,
        1, 0, 6,  4, 0, 0,  0, 8, 0,
        0, 0, 0,  0, 5, 0,  9, 0, 0,
    };
    */

    /* 暂时没解开{
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
}

void GameScene::calculate(Ref *pSender, Widget::TouchEventType eventType)
{
    // first
    do{
        if (!calCellLimit()) {
            break;
        }
    } while(1);
    
    // second
    addPurOrUnPurValue();
    
    do{
        if (!calCellExclude()) {
            break;
        }
    } while(1);
    
    // check
//    checkPurValue();
    
    /*
    unsigned long curTime = MXTime::millisecondNow();
    log("curT %lu", curTime);
    bool needTurn = false;
    if (eventType == Widget::TouchEventType::ENDED) {
        for (std::vector<Grid*>::iterator iter = _effectiveGridVec.begin(); iter != _effectiveGridVec.end(); iter++) {
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
                iter = _effectiveGridVec.erase(iter);
                iter--;
            }
        }
    }
//    if (needTurn) {
//        calculate(pSender, eventType);
//    }
    log("use time %lu", MXTime::millisecondNow() - curTime);
    
     */
    
    updateEffectiveText();
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
    _effectiveGridText->setString(StringUtils::format("%lu", _effectiveGridVec.size() - _solveNum));
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

// 单元限制 一单元只有一个空格之类的
bool GameScene::calCellLimit()
{
    bool hasCal = false;
    for (auto iter = _effectiveIndexAndGridMap.begin(); iter != _effectiveIndexAndGridMap.end(); iter++) {
        std::vector<Grid*> vec = iter->second;
        if (vec.size() == 1) {
            Grid* grid = vec.at(0);
            int index = iter->first;
            std::vector<int> indexVec = _areaVec[index];
            for (auto indexIter = indexVec.begin(); indexIter != indexVec.end(); indexIter++) {
                if (*indexIter != index) {
                    Grid* typeGrid = _gridVec[*indexIter];
                    int value = typeGrid->getValue();
                    if (value != 0)
                    {
                        grid->addUnPurValue(value);
                    }
                }
            }
            if(grid->showText()){
                _solveNum++;
                if (!hasCal) {
                    hasCal = true;
                }
//                _effectiveGridVec.erase();
                
                std::vector<int> indexList = _indexVec[index];
                for (auto listIndexIter = indexList.begin(); listIndexIter != indexList.end(); listIndexIter++) {
                    std::map<int, std::vector<Grid*>>::iterator gridListIter = _effectiveIndexAndGridMap.find(*listIndexIter);
                    if (gridListIter != _effectiveIndexAndGridMap.end()) {
                        std::vector<Grid*> gridList = gridListIter->second;
                        for (auto areaGridIter = gridList.begin(); areaGridIter != gridList.end(); areaGridIter++) {
                            if (*areaGridIter == grid) {
                                gridList.erase(areaGridIter);
                                auto existIter =  _existIndexAndGridMap.find(*listIndexIter);
                                if (existIter == _existIndexAndGridMap.end()) {
                                    _existIndexAndGridMap[*listIndexIter] = {};
                                    existIter = _existIndexAndGridMap.find(*listIndexIter);
                                }
                                existIter->second.push_back(grid);
                                break;
                            }
                        }
                    }
                }
                iter = _effectiveIndexAndGridMap.erase(iter);
                iter--;
            }
            
        }
    }
    return hasCal;
}

// 检查是否只有一个可能性
bool GameScene::calCellExclude()
{
    bool hasCal = false;
    
    for (int offset = 0; offset < RowOrLineNum; offset++) {
        std::vector<Grid*> gridList = _effectiveIndexAndGridMap[offset];
        for (auto gridIter = gridList.begin(); gridIter != gridList.end(); gridIter++) {
            Grid* grid = *gridIter;
            if(grid->showText()){
                hasCal = true;
                _solveNum++;
                int index = grid->getIndex();
                std::vector<int> indexList = _indexVec[index];
                for (auto listIndexIter = indexList.begin(); listIndexIter != indexList.end(); listIndexIter++) {
                    std::map<int, std::vector<Grid*>>::iterator gridListIter = _effectiveIndexAndGridMap.find(*listIndexIter);
                    if (gridListIter != _effectiveIndexAndGridMap.end()) {
                        std::vector<Grid*> gridList = gridListIter->second;
                        for (auto areaGridIter = gridList.begin(); areaGridIter != gridList.end(); areaGridIter++) {
                            if (*areaGridIter == grid) {
                                gridList.erase(areaGridIter);
                                auto existIter =  _existIndexAndGridMap.find(*listIndexIter);
                                if (existIter == _existIndexAndGridMap.end()) {
                                    _existIndexAndGridMap[*listIndexIter] = {};
                                    existIter = _existIndexAndGridMap.find(*listIndexIter);
                                }
                                existIter->second.push_back(grid);
                                break;
                            }
                        }
                    }
                }
                
            };
        }
    }
//    for (auto cellIter = _effectiveIndexAndGridMap.begin(); cellIter != _effectiveIndexAndGridMap.end(); cellIter++) {
//        std::vector<Grid*>gridList = cellIter->second;
//        for (auto gridIter = gridList.begin(); gridIter != gridList.end(); gridIter++) {
//            Grid* grid = *gridIter;
//            int gridIndex = grid->getIndex();
//            std::vector<int>indexList = _indexVec[gridIndex];
//            for (auto areaIndexIter = indexList.begin(); areaIndexIter != indexList.end(); areaIndexIter++) {
//                std::vector<int>areaList = _areaVec[*areaIndexIter];
//                for (auto areaIter = areaList.begin(); areaIter != areaList.end(); areaIter++) {
//                    Grid* paramsGrid = _gridVec[*areaIter];
//                    int value = paramsGrid->getValue();
//                    if (value > 0) {
//                        
//                    }
//                }
//            }
//            
//            
//        }
//    }
    return hasCal;
}

// 根据现有线索推理其他格子的可能性
bool GameScene::addPurOrUnPurValue()
{
    for (int offset = 0; offset < RowOrLineNum ; offset++) {
        auto existIter = _existIndexAndGridMap.find(offset);
        if (existIter != _existIndexAndGridMap.end()) {
            std::vector<Grid*> gridVec = existIter->second;
            for (auto gridIter = gridVec.begin(); gridIter != gridVec.end(); gridIter++) {
                Grid* grid = *gridIter;
                int gridIndex = grid->getIndex();
                int value = grid->getValue();
                std::vector<int> indexList = _indexVec[gridIndex];
//                std::vector<int> areaIndexList = _areaVec[gridIndex];
                for (auto areaIter = indexList.begin(); areaIter != indexList.end(); areaIter++) {
                    auto effectiveGridMapIter = _effectiveIndexAndGridMap.find(*areaIter);
                    if (effectiveGridMapIter != _effectiveIndexAndGridMap.end()) {
                        std::vector<Grid*> gridMap= effectiveGridMapIter->second;
                        for (auto effectiveGridIter = gridMap.begin(); effectiveGridIter != gridMap.end(); effectiveGridIter++) {
                            Grid* paramsGrid = *effectiveGridIter;
                            paramsGrid->addUnPurValue(value);
                        }
                    }
                    
                }
            }
        }
        
    }
    return scissorPurValue();
}

// 裁剪
// 检查小方格之中是否存在某个数字只存在同行或同列的格子之中，是的话排杀
bool GameScene::scissorPurValue()
{
    bool hasCal = false;
    
    std::map<int, std::vector<Grid*>> purValueMap;
    for (auto areaIndex = RowOrLineNum * 2; areaIndex < RowOrLineNum * 3; areaIndex++) {
        std::vector<int> typeVec = _areaVec[areaIndex];
        purValueMap.clear();
        // 每条线中的处理
        for (int typeIndex = 0; typeIndex < typeVec.size(); typeIndex++) {
            int type = typeVec[typeIndex];
            Grid* typeGrid = _gridVec[type];
            int value = typeGrid->getValue();
            
            if (value == 0) {
                std::vector<int> purValue = typeGrid->getPurValue();
                for (std::vector<int>::iterator iter = purValue.begin(); iter != purValue.end(); iter++) {
                    std::map<int, std::vector<Grid*>>::iterator valueIter = purValueMap.find(*iter);
                    if (valueIter == purValueMap.end()) {
                        std::vector<Grid*> vec;
                        purValueMap.insert(std::pair<int, std::vector<Grid*>>(*iter, vec));
                        valueIter = purValueMap.find(*iter);
                    }
                    valueIter->second.push_back(typeGrid);
                }
            }
        }
        // 检查
         for (std::map<int, std::vector<Grid*>>::iterator iter = purValueMap.begin(); iter != purValueMap.end(); iter++) {
            if (iter->second.size() > 1) {
                // 如果同个数字都在同行或者同列 可以清掉其他的可能了
                Grid* firstGrid = iter->second[0];
                int rowIndex = firstGrid->getRowIndex();
                int lineIndex = firstGrid->getLineIndex();
                bool testRow = true;
                bool testLine = true;
                std::vector<int> useVec = {firstGrid->getIndex()};
                for (int i = 1; i < iter->second.size() && (testRow or testLine); ++i)
                {
                    Grid* grid = iter->second[i];
                    if (testRow) {
                        testRow = rowIndex == grid->getRowIndex();
                    }
                    if (testLine) {
                        testLine = lineIndex == grid->getLineIndex();
                    }
                    if (testRow or testLine)
                    {
                        useVec.push_back(grid->getIndex());
                    }
                }
                if (testRow)
                {
                    hasCal = removeUnuseValue(_indexVec[firstGrid->getIndex()][1], useVec, iter->first) or hasCal;
                } else if (testLine)
                {
                    hasCal = removeUnuseValue(_indexVec[firstGrid->getIndex()][0], useVec, iter->first) or hasCal;
                }
            }
        }   
    }

    return hasCal;
}

bool GameScene::removeUnuseValue(int indexType, std::vector<int> useVec, int removeValue)
{
    bool hasRemove = false;
    std::vector<int> gridIndexVec = _areaVec[indexType];
    for (int i = 0; i < gridIndexVec.size(); ++i)
    {
        int index = gridIndexVec[i];
        bool needBreak = false;
        for (int j = 0; j < useVec.size(); ++j)
        {
            if (index == useVec[j])
            {
                needBreak = true;
                break;
            }
        }
        if (needBreak) {
            continue;
        }
        Grid* grid = _gridVec[index];
        if (grid->getValue() == 0) {
            hasRemove = grid->removePurValue(removeValue) or hasRemove;
        }
    }
    return hasRemove;
}

// 检查一圈可能的数字
void GameScene::checkPurValue()
{
    // 检查每条分支 最好增加脏属性 就不用全部再检查一次了
    /*for (auto iter = _areaVec.begin(); iter != _areaVec.end(); iter++) {
        purValueMap.clear();
        std::vector<int> areaList = *iter;
        for (auto indexIter = areaList.begin(); indexIter != areaList.end(); indexIter++) {
            auto it = _existIndexAndGridMap.find(*indexIter);
            if(it != _existIndexAndGridMap.end())
            {
//                Grid* grid = it->second;
            }
        }
    }*/
    std::map<int, std::vector<Grid*>> purValueMap;
    // 参考线
    for (int id = 0; id < _areaVec.size(); id++) {
        std::vector<int> typeVec = _areaVec[id];
        purValueMap.clear();
        // 每条线中的处理
        for (int typeIndex = 0; typeIndex < typeVec.size(); typeIndex++) {
            int type = typeVec[typeIndex];
            Grid* typeGrid = _gridVec[type];
            int value = typeGrid->getValue();
            
            if (value == 0) {
                std::vector<int> purValue = typeGrid->getPurValue();
                for (std::vector<int>::iterator iter = purValue.begin(); iter != purValue.end(); iter++) {
                    std::map<int, std::vector<Grid*>>::iterator valueIter = purValueMap.find(*iter);
                    if (valueIter == purValueMap.end()) {
                        std::vector<Grid*> vec;
                        purValueMap.insert(std::pair<int, std::vector<Grid*>>(*iter, vec));
                        valueIter = purValueMap.find(*iter);
                    }
                    valueIter->second.push_back(typeGrid);
                }
            }
        }
        // 判断1⃣️ 如果只有一个可能性 那就是它了
        bool dirty = false;
        for (std::map<int, std::vector<Grid*>>::iterator iter = purValueMap.begin(); iter != purValueMap.end(); iter++) {
            if (iter->second.size() == 1) {
                Grid* grid = iter->second[0];
                grid->setValue(iter->first);
                
                // 复制黏贴是不好的事情
                int index = grid->getIndex();
                std::vector<int> indexList = _indexVec[index];
                for (auto listIndexIter = indexList.begin(); listIndexIter != indexList.end(); listIndexIter++) {
                    std::map<int, std::vector<Grid*>>::iterator gridListIter = _effectiveIndexAndGridMap.find(*listIndexIter);
                    if (gridListIter != _effectiveIndexAndGridMap.end()) {
                        std::vector<Grid*> gridList = gridListIter->second;
                        for (auto areaGridIter = gridList.begin(); areaGridIter != gridList.end(); areaGridIter++) {
                            if (*areaGridIter == grid) {
                                gridList.erase(areaGridIter);
                                auto existIter =  _existIndexAndGridMap.find(*listIndexIter);
                                if (existIter == _existIndexAndGridMap.end()) {
                                    _existIndexAndGridMap[*listIndexIter] = {};
                                    existIter = _existIndexAndGridMap.find(*listIndexIter);
                                }
                                existIter->second.push_back(grid);
                                break;
                            }
                        }
                    }
                }
                
                dirty = true;
            }
        }
        if (dirty) {
            //有优化空间
            addPurOrUnPurValue();
        }
    }

}
