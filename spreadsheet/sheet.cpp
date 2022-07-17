#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

void Row::SetCell(size_t col, std::string text, Sheet& sheet) {
    auto cell = row_.find(col);
    if (cell == row_.end()) {
        row_[col] = std::make_unique<Cell>(sheet);
    }
    row_[col]->Set(text);
    if (!text.empty()) {
        non_empty_cells_.insert(col);
    }
}

CellInterface* Row::operator[](size_t col) {
    auto cell = row_.find(col);
    if (cell == row_.end()) {
        return nullptr;
    }
    return cell->second.get();
}

const CellInterface* Row::operator[](size_t col) const {
    auto cell = row_.find(col);
    if (cell == row_.end()) {
        return nullptr;
    }
    return cell->second.get();
}

void Row::Clear(size_t col) {
    auto cell = row_.find(col);
    if (cell != row_.end()) {
        cell->second->Clear();
        non_empty_cells_.erase(col);

        row_.erase(cell);
    }
}

void Row::PrintValues(std::ostream& output, size_t cells_count) const {
    for (size_t i = 0; i < cells_count; ++i) {
        auto cell = row_.find(i);
        if (cell != row_.end()) {
        const CellInterface::Value value = cell->second->GetValue();
            std::visit([&output](auto& val) {
                output << val;
            }, value);
        }
        if (i < cells_count - 1) {
            output << '\t';
        }
    }
    output << '\n';
}

void Row::PrintTexts(std::ostream& output, size_t cells_count) const {
    for (size_t i = 0; i < cells_count; ++i) {
        auto cell = row_.find(i);
        if (cell != row_.end()) {
            output << cell->second->GetText();
        }
        if (i < cells_count - 1) {
            output << '\t';
        }
    }
    output << '\n';
}

size_t Row::Size() const {
    if (non_empty_cells_.empty()) {
        return 0;
    }
    return *non_empty_cells_.rbegin() + 1;
}

void Row::PrintEmptyRow(std::ostream& output, size_t cells_count) {
    for (size_t i = 0; i < cells_count - 1; ++i) {
        output << '\t';
    }
    output << '\n';
}

void Sheet::SetCell(Position pos, std::string text) {
    CheckPosition(pos);
    rows_[pos.row].SetCell(pos.col, text, *this);
    if (rows_[pos.row].Size() > 0 && !text.empty()) {
        non_empty_rows_.insert(pos.row);
    }
}

const CellInterface* Sheet::GetCell(Position pos) const {
    CheckPosition(pos);
    auto row = rows_.find(pos.row);
    if (row == rows_.end()) {
        return nullptr;
    }
    return row->second[pos.col];
}

CellInterface* Sheet::GetCell(Position pos) {
    CheckPosition(pos);
    auto row = rows_.find(pos.row);
    if (row == rows_.end()) {
        return nullptr;
    }
    return row->second[pos.col];
}

void Sheet::ClearCell(Position pos) {
    CheckPosition(pos);
    auto row = rows_.find(pos.row);
    if (row != rows_.end()) {
        row->second.Clear(pos.col);
        if (row->second.Size() == 0) {
            non_empty_rows_.erase(pos.row);
        }
    }
}

Size Sheet::GetPrintableSize() const {
    if (non_empty_rows_.empty()) {
        return {0, 0};
    }
    Size result;
    result.rows = *non_empty_rows_.rbegin() + 1;
    for (auto& row : rows_) {
        result.cols = std::max(result.cols, static_cast<int>(row.second.Size()));
    }
    return result;
}

void Sheet::PrintValues(std::ostream& output) const {
    Size printable_size = GetPrintableSize();
    for (int i = 0; i < printable_size.rows; ++i) {
        auto row = rows_.find(i);
        if (row != rows_.end()) {
            row->second.PrintValues(output, printable_size.cols);
        } else {
            Row::PrintEmptyRow(output, printable_size.cols);
        }
    }
}
void Sheet::PrintTexts(std::ostream& output) const {
    Size printable_size = GetPrintableSize();
    for (int i = 0; i < printable_size.rows; ++i) {
        auto row = rows_.find(i);
        if (row != rows_.end()) {
            row->second.PrintTexts(output, printable_size.cols);
        } else {
            Row::PrintEmptyRow(output, printable_size.cols);
        }
    }
}

Cell* Sheet::GetCellOrCreate(Position pos) {
    if (GetCell(pos) == nullptr) {
        SetCell(pos, ""s);
    }
    return dynamic_cast<Cell*>(GetCell(pos));
}

void Sheet::CheckPosition(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position"s);
    }
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}
