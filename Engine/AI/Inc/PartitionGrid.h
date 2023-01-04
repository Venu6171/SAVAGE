#pragma once

#include "Common.h"

namespace SAVAGE::AI
{
	template <class T>
	class PartitionGrid
	{
	public:
		using cell = std::vector<T*>;
		void Initialize(uint32_t columns, uint32_t rows)
		{
			mCells.resize(columns * rows);
			mColumns = columns;
			mRows = rows;

			ClearCells();
		}

		void ClearCells()
		{
			for (auto& cells : mCells)
				cells.clear();
		}

		cell& GetCell(uint32_t column, uint32_t row)
		{
			SAVAGE::ASSERT(column < mColumns&& row < mRows, "[PartitionGrid] Invalid indices");
			return mCells[column + (row * mColumns)];
		}

		uint32_t GetColumns() const { return mColumns; }
		uint32_t GetRows() const { return mRows; }

	private:
		std::vector<cell> mCells;
		uint32_t mColumns;
		uint32_t mRows;
	};
}