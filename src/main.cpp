#include "pch.hpp"
#include "wrenbind17/wrenbind17.hpp"

namespace wren = wrenbind17;

class Vector2
{
public:
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    float getX() const
    {
        return x;
    }

    float getY() const
    {
        return y;
    }

    void setX(float x)
    {
        this->x = x;
    }

    void setY(float y)
    {
        this->y = y;
    }

    float operator[](int index) const
    {
        if (index == 0) return x;
        if (index == 1) return y;
        throw std::out_of_range("Index out of range");
    }

    float &operator[](int index)
    {
        if (index == 0) return x;
        if (index == 1) return y;
        throw std::out_of_range("Index out of range");
    }

    Vector2 operator+(const Vector2 &other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2 &other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    Vector2 operator-() const
    {
        return Vector2(-x, -y);
    }

    std::string toString() const
    {
        std::ostringstream oss;
        oss << "Vector2(" << x << ", " << y << ")";
        return oss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector2 &vec)
    {
        os << "Vector2(" << vec.x << ", " << vec.y << ")";
        return os;
    }
};

int main()
{
    typedef Vector2 (Vector2::*Vector2Sub)(const Vector2&) const;
    typedef Vector2 (Vector2::*Vector2Neg)() const;

    wren::VM vm;
    auto &m = vm.module("math");
    auto &cls = m.klass<Vector2>("Vector2");

    cls.ctor<float, float>();
    cls.prop<&Vector2::getX, &Vector2::setX>("x");
    cls.prop<&Vector2::getY, &Vector2::setY>("y");
    cls.func<&Vector2::operator+ >(wren::OPERATOR_ADD);
    cls.func<static_cast<Vector2Sub>(&Vector2::operator-)>(wren::OPERATOR_SUB);
    cls.func<static_cast<Vector2Neg>(&Vector2::operator-)>(wren::OPERATOR_NEG);
    cls.func<&Vector2::operator*>(wren::OPERATOR_MUL);
    cls.func<&Vector2::operator/ >(wren::OPERATOR_DIV);
    cls.func<static_cast<float (Vector2::*)(int) const>(&Vector2::operator[])>(wren::OPERATOR_GET_INDEX);
    cls.func<static_cast<float &(Vector2::*)(int)>(&Vector2::operator[])>(wren::OPERATOR_SET_INDEX);
    cls.func<&Vector2::toString>("toString");

    std::ofstream outFile("Vector2.wren");
    cls.generate(outFile);
    outFile.close();

    const char* script = R"(
        import "math" for Vector2

        var v1 = Vector2.new(1.0, 2.0)
        var v2 = Vector2.new(3.0, 4.0)

        System.print("v1: %(v1.toString())\n\n")
        System.print("v2: %(v2.toString())\n\n")

        var v3 = v1 + v2
        System.print("v1 + v2: %(v3.toString())\n\n")

        var v4 = v1 - v2
        System.print("v1 - v2: %(v4.toString())\n\n")

        var v5 = v1 * 2.0
        System.print("v1 * 2.0: %(v5.toString())\n\n")

        var v6 = v1 / 2.0
        System.print("v1 / 2.0: %(v6.toString())\n\n")

        var v7 = -v1
        System.print("-v1: %(v7.toString())\n\n")

        System.print("v1[0]: %(v1[0])\n\n")
        System.print("v1[1]: %(v1[1])\n\n")

        v1[0] = 5.0
        v1[1] = 6.0
        System.print("v1 after set: %(v1.toString())\n\n")
    )";

    vm.runFromSource("test", script);

    return 0;
}
