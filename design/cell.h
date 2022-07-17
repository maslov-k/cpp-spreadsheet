#pragma once

#include "common.h"

#include <set>

class Sheet;

class Cell : public CellInterface {
public:
    Cell(Sheet& sheet);
    ~Cell() override;

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;

    std::vector<Position> GetReferencedCells() const override;
    bool IsReferenced() const;

private:
    class Impl;
    class EmptyImpl;
    class TextImpl;
    class FormulaImpl;

    std::set<Cell*> parents_;
    std::set<Cell*> children_;

    void SetParents();
    void DeleteParents();

    void ClearCache();
    void ClearChildrenCache();

    void CheckCircularRef(const std::vector<Position>& parents, const Cell* root);

    Sheet& sheet_;
    std::unique_ptr<Impl> impl_;
};
