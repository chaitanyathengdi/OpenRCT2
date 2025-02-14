/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "IniWriter.hpp"

#include "../core/IStream.hpp"
#include "../core/String.hpp"
#include "../platform/platform.h"

#include <sstream>

class IniWriter final : public IIniWriter
{
private:
    IStream* _stream;
    bool _firstSection = true;

public:
    explicit IniWriter(IStream* stream)
        : _stream(stream)
    {
    }

    void WriteSection(const std::string& name) override
    {
        if (!_firstSection)
        {
            WriteLine();
        }
        _firstSection = false;

        WriteLine("[" + name + "]");
    }

    void WriteBoolean(const std::string& name, bool value) override
    {
        WriteProperty(name, value ? "true" : "false");
    }

    void WriteInt32(const std::string& name, int32_t value) override
    {
        WriteProperty(name, std::to_string(value));
    }

    void WriteFloat(const std::string& name, float value) override
    {
        WriteProperty(name, std::to_string(value));
    }

    void WriteString(const std::string& name, const std::string& value) override
    {
        std::ostringstream buffer;
        buffer << '"';
        for (char c : value)
        {
            if (c == '\\' || c == '"')
            {
                buffer << '\\';
            }
            buffer << c;
        }
        buffer << '"';

        WriteProperty(name, buffer.str());
    }

    void WriteEnum(const std::string& name, const std::string& key) override
    {
        WriteProperty(name, key);
    }

private:
    void WriteProperty(const std::string& name, const std::string& value)
    {
        WriteLine(name + " = " + value);
    }

    void WriteLine()
    {
        _stream->Write(PLATFORM_NEWLINE, String::SizeOf(PLATFORM_NEWLINE));
    }

    void WriteLine(const std::string& line)
    {
        _stream->Write(line.c_str(), line.size());
        WriteLine();
    }
};

void IIniWriter::WriteString(const std::string& name, const utf8* value)
{
    WriteString(name, String::ToStd(value));
}

IIniWriter* CreateIniWriter(IStream* stream)
{
    return new IniWriter(stream);
}
