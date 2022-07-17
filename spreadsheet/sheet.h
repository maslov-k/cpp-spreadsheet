#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <unordered_map>
#include <set>

class Sheet;

class Row {
public:
    void SetCell(size_t col, std::string text, Sheet& sheet);

    CellInterface* operator[](size_t col);
    const CellInterface* operator[](size_t col) const;

    void Clear(size_t col);

    void PrintValues(std::ostream& output, size_t cells_count) const;
    void PrintTexts(std::ostream& output, size_t cells_count) const;

    static void PrintEmptyRow(std::ostream& output, size_t cells_count);

    size_t Size() const;

private:
    std::unordered_map<size_t, std::unique_ptr<Cell>> row_;
    std::set<size_t> non_empty_cells_;
};

class Sheet : public SheetInterface {
public:
    ~Sheet() = default;

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

    Cell* GetCellOrCreate(Position pos);

private:
    void CheckPosition(Position pos) const;

    std::unordered_map<size_t, Row> rows_;
    std::set<size_t> non_empty_rows_;
};
