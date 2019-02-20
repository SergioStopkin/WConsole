// Copyright © 2019 Sergio Stopkin.

/*
 * This file is part of WConsole. WConsole is free software:
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 * 
 * WConsole is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with WConsole. See the file COPYING. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WCONSOLE_IHEADER_H_
#define WCONSOLE_IHEADER_H_ 1

#include <iomanip>
#include <sstream>
#include <typeinfo>

#include "iprint.h"

namespace WConsole {

enum class DataPosition {
    Right,
    Left,
    Center
};

class IHeader : virtual public IPrint {
public:
    void SetDataPosition(const DataPosition pos) noexcept {
        data_pos_ = pos;
    }

    void ShowDataHeader (const bool show) noexcept {
        is_data_header_ = show;
    }

protected:
    bool         is_data_header_;
    DataPosition data_pos_;

    explicit IHeader(const bool         is_data_header = true,
                     const DataPosition data_position  = DataPosition::Center)
                   : is_data_header_(is_data_header),
                     data_pos_      (data_position) {}

    virtual ~IHeader() = default;

    template <typename T>
    void WriteDataToBuff(std::wstring & buff,
                         T              value,
                         uint           alignment,
                         int            precision) const noexcept {
        std::wstring wvalue;

        if (typeid(value) == typeid(float)
            || typeid(value) == typeid(double)
            || typeid(value) == typeid(long double)) {
            std::wstringstream wsstream;
            wsstream << std::fixed << std::setprecision(precision) << value;
            wvalue = wsstream.str();
        } else {
            wvalue = std::to_wstring(value);
        }

        const auto str_len = static_cast<uint>(wvalue.length());
        uint       diff    = 0;

        if (alignment > str_len) {
            switch (data_pos_) {
                case DataPosition::Left:
                    diff = alignment - str_len;
                    break;
                case DataPosition::Right:
                    diff = 0;//alignment - str_len - (alignment - str_len);
                    WriteSpacesToBuff(buff, alignment - str_len);
                    break;
                case DataPosition::Center:
                    diff = alignment - str_len - ((alignment - str_len) / 2);
                    WriteSpacesToBuff(buff, ((alignment - str_len) / 2));
                    break;
            }
        }

        buff += wvalue;

        if (alignment > str_len) {
            WriteSpacesToBuff(buff, diff);
        }
    }

};

} // namespace WConsole

#endif // WCONSOLE_IHEADER_H_
