#include "JsonParser.h"

#include "StringHash.h"

#include <cctype>

bool JsonParser::parse(std::istream &jsonStream)
{
    _typeId = 0;
    _parsedAttributes.clear();
    _parsedArrays.clear();

    char c;

    // Skip whitespace and find opening brace
    while (jsonStream.get(c) && std::isspace(c))
    {
    }
    if (c != '{')
    {
        return false;
    }

    // Parse key-value pairs
    while (jsonStream.get(c))
    {
        // Skip whitespace
        while (std::isspace(c) && jsonStream.get(c))
        {
        }

        // Check for end of object
        if (c == '}')
        {
            return true;
        }

        // Expect opening quote for key
        if (c != '"')
        {
            return false;
        }

        // Read key
        std::string key;
        while (jsonStream.get(c) && c != '"')
        {
            if (c == '\\' && jsonStream.get(c))
            {
                // Handle escape sequences
                switch (c)
                {
                case 'n':
                    key += '\n';
                    break;
                case 't':
                    key += '\t';
                    break;
                case 'r':
                    key += '\r';
                    break;
                case '\\':
                    key += '\\';
                    break;
                case '"':
                    key += '"';
                    break;
                default:
                    key += c;
                    break;
                }
            }
            else
            {
                key += c;
            }
        }

        // Skip whitespace and find colon
        while (jsonStream.get(c) && std::isspace(c))
        {
        }
        if (c != ':')
        {
            return false;
        }

        // Skip whitespace and find value
        while (jsonStream.get(c) && std::isspace(c))
        {
        }

        if (c == '"')
        {
            // Read string value
            std::string value;
            while (jsonStream.get(c) && c != '"')
            {
                if (c == '\\' && jsonStream.get(c))
                {
                    // Handle escape sequences
                    switch (c)
                    {
                    case 'n':
                        value += '\n';
                        break;
                    case 't':
                        value += '\t';
                        break;
                    case 'r':
                        value += '\r';
                        break;
                    case '\\':
                        value += '\\';
                        break;
                    case '"':
                        value += '"';
                        break;
                    default:
                        value += c;
                        break;
                    }
                }
                else
                {
                    value += c;
                }
            }

            // Store the key-value pair
            if (key == "type")
            {
                _typeId = StringHash::hash(value);
            }
            else
            {
                _parsedAttributes[key] = value;
            }
        }
        else if (c == '[')
        {
            // Read array value
            std::vector<std::string> values;

            while (jsonStream.get(c))
            {
                // Skip whitespace
                while (std::isspace(c) && jsonStream.get(c))
                {
                }

                // Check for end of array
                if (c == ']')
                {
                    break;
                }

                // Expect opening quote for array element
                if (c != '"')
                {
                    return false;
                }

                // Read array element
                std::string element;
                while (jsonStream.get(c) && c != '"')
                {
                    if (c == '\\' && jsonStream.get(c))
                    {
                        switch (c)
                        {
                        case 'n':
                            element += '\n';
                            break;
                        case 't':
                            element += '\t';
                            break;
                        case 'r':
                            element += '\r';
                            break;
                        case '\\':
                            element += '\\';
                            break;
                        case '"':
                            element += '"';
                            break;
                        default:
                            element += c;
                            break;
                        }
                    }
                    else
                    {
                        element += c;
                    }
                }

                values.push_back(std::move(element));

                // Skip whitespace and find comma or end of array
                while (jsonStream.get(c) && std::isspace(c))
                {
                }

                if (c == ']')
                {
                    break;
                }

                if (c != ',')
                {
                    return false;
                }
            }

            _parsedArrays[key] = std::move(values);
        }
        else
        {
            return false;
        }

        // Skip whitespace and find comma or end of object
        while (jsonStream.get(c) && std::isspace(c))
        {
        }

        if (c == '}')
        {
            return true;
        }

        if (c != ',')
        {
            return false;
        }
    }

    return false;
}

std::string JsonParser::valueOf(std::string_view attribute) const
{
    auto it = _parsedAttributes.find(std::string(attribute));
    if (it != _parsedAttributes.end())
    {
        return it->second;
    }
    return {};
}

std::vector<std::string> JsonParser::arrayOf(std::string_view attribute) const
{
    auto it = _parsedArrays.find(std::string(attribute));
    if (it != _parsedArrays.end())
    {
        return it->second;
    }
    return {};
}
