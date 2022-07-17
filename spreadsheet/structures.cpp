#include "common.h"

#include <cctype>
#include <sstream>
#include <cmath>
#include <algorithm>

const int LETTERS = 26;
//const int MAX_POSITION_LENGTH = 17;
//const int MAX_POS_LETTER_COUNT = 3;

const Position Position::NONE = {-1, -1};

bool Position::operator==(const Position rhs) const {
    return row == rhs.row && col == rhs.col;
}

bool Position::operator<(const Position rhs) const {
    if (row < rhs.row) {
        return true;
    } else if (row == rhs.row) {
        return col < rhs.col;
    }
    return false;
}

bool Size::operator==(Size rhs) const {
    return rows == rhs.rows && cols == rhs.cols;
}

std::string_view ReadCol(std::string_view str) {
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] < 'A' || str[i] > 'Z') {
            return str.substr(0, i);
        }
    }
    return std::string_view();
}

int ReadRow(std::string_view str, int pos) {
    for (size_t i = pos; i < str.size(); ++i) {
        if (!std::isdigit(str[i])) {
            return -1;
        }
    }
    int row = std::atoi(str.substr(pos).data());
    return --row;
}

int ComputeProgramCol(std::string_view col_str) {
    int result = 0;
    int pwr = col_str.size() - 1;
    for (char c : col_str) {
        result += std::pow(LETTERS, pwr--) * (c - 'A' + 1);
    }
    return --result;
}

std::string ComputeUserCol(int col) {
    ++col;
    std::string result;
    while (col > 0) {
        result += 'A' + (col - 1) % LETTERS;
        col = (col - 1) / LETTERS;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

bool Position::IsValid() const {
    if (row >= MAX_ROWS || row < 0) {
        return false;
    }
    if (col >= MAX_COLS || col < 0) {
        return false;
    }
    return true;
}

std::string Position::ToString() const {
    if (!IsValid()) {
        return {};
    }
    std::string result = ComputeUserCol(col);
    result += std::to_string(row + 1);
    return result;
}

Position Position::FromString(std::string_view str) {
    std::string_view col_str = ReadCol(str);
    int row = ReadRow(str, col_str.size());
    if (col_str.empty() || row == -1) {
        return Position::NONE;
    }
    int col = ComputeProgramCol(col_str);
    if (row >= MAX_ROWS || row < 0) {
        return Position::NONE;
    }
    if (col >= MAX_COLS || col < 0) {
        return Position::NONE;
    }
    return {row, col};
}
