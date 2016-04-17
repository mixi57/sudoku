//
//  CalUtils.cpp
//  9Game
//
//  Created by mixi on 16/4/11.
//
//
#define RowOrLineNum 9

#include "CalUtils.hpp"
#include "Grid.hpp"

CalUtils::CalUtils()
{
}

CalUtils::~CalUtils()
{
}

CalUtils::CalUtils(Vec_Vec_Int areaV, Vec_Vec_Int indexV, Grid_Map effectIAG, Grid_Map existIAG, Grid_Vec gridV, Grid_Vec effectGV)
{
	_areaVec = areaV;
	_indexVec = indexV;
	_effectiveIndexAndGridMap = effectIAG;
	_existIndexAndGridMap = existIAG;
	_solveNum = 0;
    _gridVec = gridV;
    _effectiveGridVec = effectGV;
}

Grid* CalUtils::checkCellLimit(int index)
{
    Grid* sureGrid = nullptr;
    std::vector<Grid*> vec = _effectiveIndexAndGridMap[index];

    // 该线上只有一个可能性 那肯定是它啦
    if (vec.size() == 1) {
        Grid* grid = vec.at(0);
        std::vector<int> indexVec = _areaVec[index];
        for (auto indexIter = indexVec.begin(); indexIter != indexVec.end(); indexIter++) {
            if (*indexIter != index) {
                Grid* typeGrid = _gridVec[*indexIter];
                int value = typeGrid->getValue();
                if (value != 0){
                    grid->addUnPurValue(value);
                }
            }
        }
        if(grid->showText()){
            addSureValue(grid);
            sureGrid = grid;
        }
    }
    return sureGrid;

}
// 单元限制 一单元只有一个空格之类的 再分出来吧 这个叫做单元限制法
// 参数是 链下标向量
bool CalUtils::calCellLimit(std::vector<int> Vec)
{
	bool hasCal = false;
    std::vector<int> dirtyIndexVec;
    for (auto iter = Vec.begin(); iter != Vec.end(); iter++) {
        int curIndex = *iter;
        Grid* sureGrid = checkCellLimit(curIndex);
        if (sureGrid != nullptr) {
            auto mapIter = _effectiveIndexAndGridMap.find(curIndex);
            _effectiveIndexAndGridMap.erase(mapIter);
            hasCal = true;
            std::vector<int> gridIndexList = _indexVec[sureGrid->getIndex()];
            for (int i = 0; i < gridIndexList.size(); ++i)
            {
                int index = gridIndexList[i];
                if (index < curIndex) {
                    dirtyIndexVec.push_back(index);
                }
            }
        }
    }
    if (dirtyIndexVec.size() > 0)
    {
        calCellLimit(dirtyIndexVec);
    }
    return hasCal;
}

// 根据现有线索推理其他格子的可能性 理想状态应该只计算一次是全局的
bool CalUtils::addPurOrUnPurValue()
{
	bool hasChange = false;
    for (int offset = 0; offset < RowOrLineNum ; offset++) {
        auto existIter = _existIndexAndGridMap.find(offset);
        if (existIter != _existIndexAndGridMap.end()) {
            std::vector<Grid*> gridVec = existIter->second;
            for (auto gridIter = gridVec.begin(); gridIter != gridVec.end(); gridIter++) {
                Grid* grid = *gridIter;
            	hasChange = addPurOrUnPurValueByNewGrid(grid) or hasChange;
            }
        }
    }
    return hasChange;
}

// 把这步分出来 通过一个已知的数字确定三条线的可能性
bool CalUtils::addPurOrUnPurValueByNewGrid(Grid* grid)
{
	bool hasChange = false;
	int gridIndex = grid->getIndex();
    int value = grid->getValue();
    std::vector<int> indexList = _indexVec[gridIndex];
    for (auto areaIter = indexList.begin(); areaIter != indexList.end(); areaIter++) {
        auto effectiveGridMapIter = _effectiveIndexAndGridMap.find(*areaIter);
        if (effectiveGridMapIter != _effectiveIndexAndGridMap.end()) {
            std::vector<Grid*> gridMap = effectiveGridMapIter->second;
            for (auto effectiveGridIter = gridMap.begin(); effectiveGridIter != gridMap.end(); effectiveGridIter++) {
                Grid* paramsGrid = *effectiveGridIter;
                if (paramsGrid->addUnPurValue(value)) {
                    hasChange = true;
                    if (paramsGrid->showText()) {
                        addSureValue(paramsGrid);
                    }
                }
            }
        }
    }
    return hasChange;
}
bool _isFirst = true;
void CalUtils::calculate()
{
    if (_isFirst)//(_solveNum == 0)
    {
        std::vector<int> calVec;
        for (auto iter = _effectiveIndexAndGridMap.begin(); iter !=  _effectiveIndexAndGridMap.end(); ++iter)
        {
            calVec.push_back(iter->first);
        }
        calCellLimit(calVec);
        
        addPurOrUnPurValue();

        scissorPurValue();

        calCellExclude();

        // calCellTwoOrThree();
        // _isFirst = false;

    } else {
        
    }
}

void CalUtils::addSureValue(Grid* grid)
{
	int index = grid->getIndex();
	_solveNum++;
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

	addPurOrUnPurValueByNewGrid(grid);
}

Grid_Map CalUtils::getPurValueMap(int areaIndex)
{
    Grid_Map purValueMap;
    std::vector<int> typeVec = _areaVec[areaIndex];
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
    return purValueMap;
}
bool CalUtils::scissorPurValueByIndex(int areaIndex, std::map<int, std::vector<Grid_Vec>>& gridMap, bool needSave)
{
    bool hasCal = false;
    std::map<int, std::vector<Grid*>> purValueMap = getPurValueMap(areaIndex);
    // 检查
     for (std::map<int, std::vector<Grid*>>::iterator iter = purValueMap.begin(); iter != purValueMap.end(); iter++) {
        if (iter->second.size() > 1) {
            // 如果同个数字都在同行或者同列 可以清掉其他的可能了
            if (needSave)
            {
                auto gridMapIter = gridMap.find(iter->first);
                if (gridMapIter == gridMap.end())
                {
                    gridMap[iter->first] = {};
                    gridMapIter = gridMap.find(iter->first);
                }
                gridMapIter->second.push_back(iter->second);
            } else {
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

// 裁剪
// 检查小方格之中是否存在某个数字只存在同行或同列的格子之中，是的话排杀
// 其实应该扩散 不应该只限定在小方格中 
bool CalUtils::scissorPurValue()
{
    bool hasCal = false;
    std::map<int, std::vector<Grid_Vec>> gridMap;
    // 小方格
    for (auto areaIndex = RowOrLineNum * 2; areaIndex < RowOrLineNum * 3; areaIndex++) {
        if (scissorPurValueByIndex(areaIndex, gridMap)) {
            hasCal = true;
        }
    }

    for (auto areaIndex = 0; areaIndex < RowOrLineNum; areaIndex++) {
        if (scissorPurValueByIndex(areaIndex, gridMap, true)) {
            hasCal = true;
        }
    }

    for (auto iter = gridMap.begin(); iter != gridMap.end(); ++iter)
    {
        //  查找数目为2？  3的呢 同数目吧
        long gridSameNumSize = iter->second.size();
        if (gridSameNumSize > 1)
        {
            if (iter->first == 5) {
                printf("");
            }
            std::vector<Grid_Vec> gridVecVec = iter->second;
            std::map<long, std::map<long, std::vector<Grid_Vec>>> existNumMap;
            
            bool isTheSamePur = false;
            std::vector<int> useVec;
            std::vector<int> useRowIndexVec;
            int samePurNum = 0;
            bool hasSame = false;
            
            for (auto gridVVIter = gridVecVec.begin(); gridVVIter < gridVecVec.end(); ++gridVVIter)
            {
                long size = gridVVIter->size();
                if (size <= gridSameNumSize)
                {
                    int indexSum = 0;
                    for(auto gridIter = gridVVIter->begin(); gridIter != gridVVIter->end(); ++gridIter)
                    {
                        indexSum += (*gridIter)->getRowIndex();
                    }
                    auto sameIter = existNumMap.find(size);

                    if (sameIter != existNumMap.end())
                    {
                        std::map<long, std::vector<Grid_Vec>> vecMap = sameIter->second;
                        if (vecMap.find(indexSum) != vecMap.end()) {
                            for (auto vecMapIter = vecMap[indexSum].begin(); vecMapIter != vecMap[indexSum].end(); ++vecMapIter) {
                                for (auto vecGridVecIter = vecMapIter->begin(), gridIter = gridVVIter->begin(); vecGridVecIter != vecMapIter->end(); ++vecGridVecIter, ++gridIter){
                                    Grid* oldGrid = *vecGridVecIter;
                                    Grid* newGrid = *gridIter;
                                    if (oldGrid->getRowIndex() == newGrid->getRowIndex()) {
                                        isTheSamePur = true;
                                        if (!hasSame) {
                                            useVec.push_back(oldGrid->getIndex());
                                            useRowIndexVec.push_back(oldGrid->getRowIndex());
                                        }
                                    } else {
                                        isTheSamePur = false;
                                        break;
                                    }
                                }
                                if (isTheSamePur) {
                                    if (!hasSame) {
                                        hasSame = true;
                                    }
                                    samePurNum++;
//                                    useVecVec.push_back(useVec);
//                                    usrVecMap[lineIndex] = useVec;
                                }
                            }
                        }
                    }
                    if (isTheSamePur) {
                        if (samePurNum == size) {
                            for (auto useGridIndexIter = useVec.begin(); useGridIndexIter != useVec.end(); ++useGridIndexIter) {
                                int useGridIndex = *useGridIndexIter;
                                removeUnuseValue(_indexVec[useGridIndex][2], useRowIndexVec, iter->first);
                            }
                        }
                    }

                    if (!isTheSamePur)
                    {
//                        std::map<long, Grid_Vec> gridVecMap = {indexSum, *gridVVIter};
                        if (sameIter == existNumMap.end()) {
                            existNumMap[size] = {};
                            sameIter = existNumMap.find(size);
                        }
                        std::map<long, std::vector<Grid_Vec>> vecMap = sameIter->second;
                        if (vecMap.find(indexSum) == vecMap.end()) {
                            existNumMap[size][indexSum] = {};
                        }
                        
                        existNumMap[size][indexSum].push_back(*gridVVIter);
                    }
                }
            }
        }
    }
    return hasCal;
}

bool CalUtils::removeUnuseValue(int indexType, std::vector<int> useVec, int removeValue)
{
    bool hasRemove = false;
    std::vector<int> gridIndexVec = _areaVec[indexType];
    std::set<int> removeGridCellSet;
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
            if (grid->removePurValue(removeValue))
            {
                hasRemove = true;
                std::vector<int> indexList = _indexVec[grid->getIndex()];
                for (auto iter = indexList.begin(); iter != indexList.end(); ++iter) {
                    removeGridCellSet.insert(*iter);
                }
            }
        }
    }

    for (auto iter = removeGridCellSet.begin(); iter != removeGridCellSet.end(); ++iter) {
        
    }

    return hasRemove;
}

// 检查是否只有一个可能性 并没有检查？
// 单元排除法 结合 addPurOrUnPurValue
bool CalUtils::calCellExclude()
{
    bool hasCal = false;

    for (auto areaIndex = 0; areaIndex < _areaVec.size(); ++areaIndex) {
        Grid_Map gridMap = getPurValueMap(areaIndex);
        for (auto iter = gridMap.begin(); iter != gridMap.end(); ++iter)
        {
            Grid_Vec gridVec = iter->second;
            if (gridVec.size() == 1)
            {
                gridVec[0]->setValue(iter->first);
                addSureValue(gridVec[0]);
                hasCal = true;
            }            
        }
    }    
    for (int offset = 0; offset < RowOrLineNum; offset++) {
        std::vector<Grid*> gridList = _effectiveIndexAndGridMap[offset];
        for (auto gridIter = gridList.begin(); gridIter != gridList.end(); gridIter++) {
            Grid* grid = *gridIter;
            if(grid->showText()){
                hasCal = true;
                addSureValue(grid);
            };
        }
    }
    return hasCal;
}

// 单元排除法 结合 addPurOrUnPurValue
bool CalUtils::calCellExcludeByIndexList(std::vector<int> indexList)
{
    bool hasCal = false;
    for (int offset = 0; offset < indexList.size(); offset++) {
        int areaIndex = indexList[offset];
        std::vector<Grid*> gridList = _effectiveIndexAndGridMap[areaIndex];
        for (auto gridIter = gridList.begin(); gridIter != gridList.end(); gridIter++) {
            Grid* grid = *gridIter;
            if(grid->showText()){
                hasCal = true;
                // _solveNum++;
                addSureValue(grid);
            };
        }
    }
    return hasCal;

}

bool CalUtils::calCellTwoOrThree()
{
    bool hasCal = false;
    for (int i = 0; i < _effectiveIndexAndGridMap.size(); ++i)
    {
        Grid_Vec vec = _effectiveIndexAndGridMap[i];
        long vecSize = vec.size();
        if (vecSize > 0) {
            // 生成链表
            Grid_Map gridMap;
            long unSolveGridNum = vec.size();
            for (int j = 0; j < unSolveGridNum; ++j) {
                Grid* grid = vec[j];
                std::vector<int> gridPurValVec = grid->getPurValue();
                long gridPurValNum = gridPurValVec.size();
                // 如果一个格子可能的数字是剩下格子的数目的话，他一定不是我们要的，排除，减少后面工作量 
                // 打脸 78 78
                // if (unSolveGridNum == gridPurValNum) {
                //     continue;
                // }
                for (std::vector<int>::iterator purValueIter = gridPurValVec.begin(); purValueIter != gridPurValVec.end(); ++purValueIter) {
                    int value = *purValueIter;
                    auto iter = gridMap.find(value);
                    if (iter == gridMap.end()) {
                        gridMap[value] = {};
                        iter = gridMap.find(value);
                        iter->second.push_back(grid);
                    } else {
                        //iter->second.push_back(grid);
                        // 改在这里排序吧 插入排序？
                        Grid_Vec& newVec = iter->second;
                        bool isTheMax = true;
                        for (auto iter = newVec.begin(); iter != newVec.end(); ++iter) {
                            long num = (*iter)->getPurValue().size();
                            if (num > gridPurValNum) {
                                newVec.insert(iter, grid);
                                isTheMax = false;
                                break;
                            }
                        }
                        if (isTheMax) {
                            iter->second.push_back(grid);
                        }
                    }
                }
                // 排下序
                /*for (auto iter = gridMap.begin(); iter != gridMap.end(); ++iter) {
                    //std::sort(iter->second.begin(), iter->second.end(), cmp);
                }*/
            }

            std::vector<GridPurCheckStruct> GridPurCheckStructVec;
            for (auto mapIter = gridMap.begin(); mapIter != gridMap.end(); ++mapIter) {
                
                Grid_Vec gridVec = mapIter->second;
                if (gridVec.size() > 1) {
                    GridPurCheckStruct purStruct;

                    int value = mapIter->first;
                    for (int j = 0; j < gridVec.size(); ++j) {
                        Grid* grid = gridVec[j];
                        std::vector<int> purValVec = grid->getPurValue();
                        for (int vIndex = 0; vIndex < purValVec.size(); ++vIndex) {
                            int purValue = purValVec[vIndex];
                            purStruct.gridValueSet.insert(purValue);
                        }
                        bool calSuc = false;
                        for (int vIndex = 0; vIndex < purValVec.size(); ++vIndex) {
                            int purValue = purValVec[vIndex];
                            if (purValue != value) {
                                std::vector<Grid*> purValueVec = gridMap[purValue];
                                for (auto iter = purValueVec.begin(); iter != purValueVec.end(); ++iter){
                                    if (*iter != grid) {
                                        purStruct.gridSet.insert(grid);
                                        std::set<int> purSearchValueVec = {value, purValue};
                                        if (calCellTwoOrThreeFromPurMap(i, gridMap, purStruct, *iter, purSearchValueVec)) {
                                            calSuc = true;
                                        }
                                    }
                                }
                            }
                        }
                        if (!calSuc) {
                            for (auto gridMapIter = gridMap.begin(); gridMapIter != gridMap.end(); ++gridMapIter) {
                                for (auto gridInMapIter = gridMapIter->second.begin(); gridInMapIter != gridMapIter->second.end(); ++gridInMapIter) {
                                    Grid* gridInMap = *gridInMapIter;
                                    if (gridInMap == grid) {
                                        gridMapIter->second.erase(gridInMapIter);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                
            }
        }

    }
    return hasCal;
}

bool CalUtils::calCellTwoOrThreeFromPurMap(int index, Grid_Map& gridMap, GridPurCheckStruct& purStruct, Grid* grid, std::set<int>& searchPurValueSet)
{
    GridPurCheckStruct newPurStruct;
    // GridPurCheckStructVec.insert(newPurStruct);
    newPurStruct.gridSet = purStruct.gridSet;
    newPurStruct.gridValueSet = purStruct.gridValueSet;

    std::vector<int> purValue = grid->getPurValue();
    bool existPurValue = false;
    int lastPurValue = *(-- --searchPurValueSet.end());
    int nextCheckValue = 0;
    for (int i = 0; i < purValue.size(); ++i) {
        int value = purValue[i];
        if (!existPurValue) {   
            if (searchPurValueSet.find(value) == searchPurValueSet.end()) {
                existPurValue = true;
                nextCheckValue = value;
                //break;
            } else {
                if (lastPurValue != value) {
                    existPurValue = true;
                    nextCheckValue = value;
                }
            }
        }
        newPurStruct.gridValueSet.insert(value);
    }
    
    /*// 这时候已经不可能了
    if (newPurStruct.gridValueSet.size() >= gridMap.size()) {
        return false;
    }*/
    
    if (nextCheckValue != 0) {
        Grid_Vec purValueVec = gridMap[nextCheckValue];
        for (auto iter = purValueVec.begin(); iter != purValueVec.end(); ++iter) {
            if (*iter != grid) {
                if (*iter == *newPurStruct.gridSet.begin()) {
                    if (newPurStruct.gridSet.size() == newPurStruct.gridValueSet.size()) {
                        clearWithGridVec(index, newPurStruct);
                        return true;
                    }
                    return false;
                } else if (newPurStruct.gridSet.find(*iter) != newPurStruct.gridSet.end()) {
                    return false;
                    
                }
                newPurStruct.gridSet.insert(grid);
                searchPurValueSet.insert(nextCheckValue);
                if (calCellTwoOrThreeFromPurMap(index, gridMap, newPurStruct, *iter, searchPurValueSet)) {
                    return true;
                };
            }
        }
    }
    return false;
}

bool CalUtils::clearWithGridVec(int index, GridPurCheckStruct purStruct)
{
    Grid_Vec gridVec = _effectiveIndexAndGridMap[index];

    if (gridVec.size() > purStruct.gridSet.size()) {
        for (auto iter = gridVec.begin(); iter != gridVec.end(); iter++) {
            Grid* grid = *iter;
            bool isExist = purStruct.gridSet.find(grid) != purStruct.gridSet.end();
            if (!isExist) {
                bool removeSuc = false;
                for (auto valueIter = purStruct.gridValueSet.begin(); valueIter != purStruct.gridValueSet.end(); ++valueIter)
                {
                    if (grid->removePurValue(*valueIter)){
                        removeSuc = true;
                    }
                }
                if (removeSuc) {
                    if (grid->showText())
                    {
                        addSureValue(grid);
                    }
                }
            }
        }
    }
    return false;
}
    
