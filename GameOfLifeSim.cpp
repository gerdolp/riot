#include "GameOfLifeSim.h"
#include <map>
#include <iostream>

GameOfLifeSim::GameOfLifeSim(int64_t aMinBounds, int64_t aMaxBounds)
	: mMinBounds(aMinBounds)
	, mMaxBounds(aMaxBounds)
{

}

bool GameOfLifeSim::IsCellAlive(const Cell& aCell, bool aWasAlive)
{
	if (aWasAlive)
	{
		if (aCell.NeighborCount < 2 || aCell.NeighborCount > 3)
			return false;
	}
	else
	{
		if (aCell.NeighborCount != 3)
			return false;
	}

	return true;
}

void GameOfLifeSim::IncrementNeighbors(SimulationMap& writeMap, const int64_t aCol, const int64_t aRow) const
{
	if (aCol > mMinBounds)
	{
		writeMap[aRow][aCol - 1].NeighborCount++;

		if(aRow > mMinBounds)
			writeMap[aRow - 1][aCol - 1].NeighborCount++;

		if(aRow < mMaxBounds)
			writeMap[aRow + 1][aCol - 1].NeighborCount++;
	}
	
	if (aRow > mMinBounds)
		writeMap[aRow - 1][aCol].NeighborCount++;

	if (aRow < mMaxBounds)
		writeMap[aRow + 1][aCol].NeighborCount++;
	 
	if (aCol < mMaxBounds)
	{
		writeMap[aRow][aCol + 1].NeighborCount++;

		if (aRow > mMinBounds)
			writeMap[aRow - 1][aCol + 1].NeighborCount++;
		
		if (aRow < mMaxBounds)
			writeMap[aRow + 1][aCol + 1].NeighborCount++;
	}
}

void GameOfLifeSim::TickSimulation(const SimulationMap& readMap, SimulationMap& writeMap)
{
	for (auto rowIt = readMap.begin(); rowIt != readMap.end(); rowIt++)
	{
		for (auto colIt = rowIt->second.begin(); colIt != rowIt->second.end(); colIt++)
		{
 			IncrementNeighbors(writeMap, colIt->first, rowIt->first);
		}
	}

	std::vector<Pair> cellsToRemove;
	for (auto rowIt = writeMap.begin(); rowIt != writeMap.end(); rowIt++)
	{
		for (auto colIt = rowIt->second.begin(); colIt != rowIt->second.end(); colIt++)
		{
			Cell& cell = colIt->second;
			
			auto readRowIt = readMap.find(rowIt->first);
			bool previouslyAlive = false;
			if (readRowIt != readMap.end())
			{
				auto readColIt = readRowIt->second.find(colIt->first);
				if (readColIt != readRowIt->second.end())
					previouslyAlive = readColIt->second.IsAlive;
			}

			if (IsCellAlive(cell, previouslyAlive))
			{
				cell.IsAlive = true;
				cell.NeighborCount = 0;
			}
			else
				cellsToRemove.emplace_back(colIt->first, rowIt->first);

		}
	}

	for (const Pair& removePair : cellsToRemove)
	{
		writeMap[removePair.Row].erase(removePair.Col);
		if (writeMap[removePair.Row].size() == 0)
			writeMap.erase(removePair.Row);
	}
}

void GameOfLifeSim::PrintAlive(const SimulationMap& readMap)
{
	for (auto rowIt = readMap.begin(); rowIt != readMap.end(); rowIt++)
	{
		for (auto colIt = rowIt->second.begin(); colIt != rowIt->second.end(); colIt++)
		{
			if (colIt->second.IsAlive)
				std::cout << colIt->first << ", " << rowIt->first << std::endl;
		}
	}

}

void GameOfLifeSim::Run(GameOfLifeSim::Pair startConditionArr[], uint32_t startConditionsCount, int tickCount)
{
	SimulationMap buffer1;
	SimulationMap buffer2;

	SimulationMap* readMap = &buffer1;
	SimulationMap* writeMap = &buffer2;

	for (uint32_t i = 0; i < startConditionsCount; i++)
	{
		const GameOfLifeSim::Pair& startingCondition = startConditionArr[i];
		(*readMap)[startingCondition.Row][startingCondition.Col].IsAlive = true;
	}

	int currentTickCount = 0;
	while (currentTickCount < tickCount)
	{
		TickSimulation(*readMap, *writeMap);
		SimulationMap* temp = readMap;
		readMap = writeMap;
		writeMap = temp;
		writeMap->clear();
		currentTickCount++;
	}
}
