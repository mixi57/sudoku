//
//  GameScene.hpp
//  9Game
//
//  Created by mixi on 15/11/14.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Grid.hpp"

class GameScene : public cocos2d::Layer
{
//  先抄流程
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void showData();
    void judgeOneByOne();
    void createGrids();
    std::vector<int> getGridsData();
    void calculate(Ref *pSender, Widget::TouchEventType eventType);
    
    bool addPurOrUnPurValue();
    
    bool calCellLimit();
    bool calCellExclude();

    void checkPurValue();
    // 裁剪
    bool scissorPurValue();

    bool removeUnuseValue(int indexType, std::vector<int> useVec, int removeValue);

    
    CREATE_FUNC(GameScene);
    
private:
    std::vector<int> _showGridsData;
    std::vector<std::vector<int>> _areaVec;
    std::vector<std::vector<int>> _indexVec;
    
    std::map<int, std::vector<Grid*>> _effectiveIndexAndGridMap;
    std::map<int, std::vector<Grid*>> _existIndexAndGridMap;
    GameScene();
    ~GameScene();
    
    // 最基础的 只有一个不确定数字 == 确定数字
    void checkOneAnswer();
    void twoThreeCalcute();
    
    void pairsAllResult();
    
    
    void updateEffectiveText();
    
    std::vector<Grid*> _gridVec;
    std::vector<Grid*> _effectiveGridVec;
    Text* _effectiveGridText;
    int _solveNum;

};

#endif /* GameScene_hpp */
