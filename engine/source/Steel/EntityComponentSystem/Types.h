#pragma once

namespace Types
{
    enum Type
    {
        Undefined,

        Int,
        Long,
        UInt32,
        UInt64,
        Char,
        Bool,
        Float,
        Double,
        Vector2,
        Vector3,
        Color,

        String,

        Enum,

        EntityID,
        ResourceID,

        List,

        Script,
    };

    static std::string ToString(Type type)
    {
        switch (type)
        {
            case Undefined:
                return "Undefined";
            case Int:
                return "Int";
            case Long:
                return "Long";
            case UInt32:
                return "UInt32";
            case UInt64:
                return "UInt64";
            case Char:
                return "Char";
            case Bool:
                return "Bool";
            case Float:
                return "Float";
            case Double:
                return "Double";
            case Vector2:
                return "Vector2";
            case Vector3:
                return "Vector3";
            case Color:
                return "Color";
            case String:
                return "String";
            case Enum:
                return "Enum";
            case EntityID:
                return "EntityID";
            case ResourceID:
                return "ResourceID";
            case List:
                return "List";
            case Script:
                return "Script";
        }

        return "";
    }
}
