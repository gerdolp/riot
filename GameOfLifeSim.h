#pragma once
#include <cstdint>
#include <unordered_map>
class GameOfLifeSim
{
public:

	GameOfLifeSim(int64_t aMinBounds, int64_t aMaxBounds);

	struct Pair
	{
		Pair(const int64_t aCol, const int64_t aRow)
			: Col(aCol)
			, Row(aRow)
		{

		}

		Pair()
			: Col(0)
			, Row(0)
		{

		}

		int64_t Col;
		int64_t Row;
	};

	struct Cell
	{
		Cell()
			: IsAlive(false)
			, NeighborCount(0)
		{

		}

		int8_t NeighborCount;
		bool IsAlive;

	};

	void Run(Pair startingConditionsArr[], uint32_t startConditionsCount, int tickCount);

private:
	using SimulationMap = std::unordered_map<int64_t, std::unordered_map<int64_t, Cell>>;

	void TickSimulation(const SimulationMap& readMap, SimulationMap& writeMap);
	void PrintAlive(const SimulationMap& readMap);

	inline void IncrementNeighbors(SimulationMap& writeMap, const int64_t aX, const int64_t aY) const;
	inline bool IsCellAlive(const Cell& aCell, bool aWasAlive);

	int64_t mMinBounds;
	int64_t mMaxBounds;
};


