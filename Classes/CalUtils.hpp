//
//  CalUtils.hpp
//  9Game
//
//  Created by mixi on 16/4/11.
//
//

#ifndef CalUtils_hpp
#define CalUtils_hpp

#include <stdio.h>
class Grid;
typedef std::vector<std::vector<int>>  Vec_Vec_Int;
typedef std::map<int, std::vector<Grid*>>  Grid_Map;
typedef std::vector<Grid*> Grid_Vec;

typedef struct {
    std::set<Grid*> gridSet;
    std::set<int> gridValueSet;
} GridPurCheckStruct;


class CC_DLL CalUtils
{
	Vec_Vec_Int _areaVec;
    Vec_Vec_Int _indexVec;
    
    // 放未解的
    Grid_Map _effectiveIndexAndGridMap;
    // 放已存在
    Grid_Map _existIndexAndGridMap;

    Grid_Vec _gridVec;
    Grid_Vec _effectiveGridVec;

    CC_SYNTHESIZE(int, _solveNum, SolveNum);

public:
	void calculate();
	bool calCellLimit(std::vector<int> Vec);
	Grid* checkCellLimit(int index);
    bool addPurOrUnPurValue();
    bool addPurOrUnPurValueByNewGrid(Grid* grid);
    void addSureValue(Grid* grid);
    bool scissorPurValue();
    bool scissorPurValueByIndex(int index);
    bool removeUnuseValue(int indexType, std::vector<int> useVec, int removeValue);
    bool calCellExclude();
    bool calCellExcludeByIndexList(std::vector<int> indexList);
    bool calCellTwoOrThree();
    Grid_Map getPurValueMap(int areaIndex);

    bool calCellTwoOrThreeFromPurMap(int index, Grid_Map& gridMap, GridPurCheckStruct& purStruct, Grid* grid, std::set<int>& searchPurValueSet);
    bool clearWithGridVec(int index, GridPurCheckStruct purStruct);
    CalUtils(Vec_Vec_Int areaV, Vec_Vec_Int indexV, Grid_Map effectIAG, Grid_Map existIAG, Grid_Vec gridV, Grid_Vec effectGV);
    CalUtils();
    ~CalUtils();
};

#endif /* CalUtils_hpp */
