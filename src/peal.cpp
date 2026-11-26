#include "rybinsk/peal.hpp"

namespace rybinsk
{
    // namespace

    Peal::Peal() {}

    Peal::Peal(int count)
    {
        if (count < 0)
        {
            throw PealError("Invalid count");
        }
        if (count > 0)
        {
            strokes_ = new Stroke[count]{};
            count_ = count;
        }
    }

    Peal::Peal(const Stroke *strokes, int count)
    {
        if (count < 0 || (strokes == nullptr && count > 0))
        {
            throw PealError("Invalid array");
        }
        if (count > 0)
        {
            strokes_ = new Stroke[count];
            count_ = count;
            for (int i = 0; i < count_; ++i)
            {
                strokes_[i] = strokes[i];
            }
        }
    }

    Peal::Peal(const Peal &other) : Peal(other.strokes_, other.count_) {}

    // noexcept: отсюда нельзя бросать исключение, процесс оборвётся.
    // Пока источник не опустошается, тесты на перемещение красные.
    Peal::Peal(Peal &&other) noexcept : strokes_(other.strokes_), count_(other.count_)
    {
        other.strokes_ = nullptr;
        other.count_ = 0;
    }

    Peal::~Peal()
    {
        delete[] strokes_; // Освободите strokes_. delete[] nullptr вызывать можно.
    }

    Peal &Peal::operator=(const Peal &other)
    {
        if (this == &other)
        {
            return *this;
        }

        Stroke *new_strokes = nullptr;

        if (other.count_ > 0)
        {
            new_strokes = new Stroke[other.count_];

            for (int i = 0; i < other.count_; ++i)
            {
                new_strokes[i] = other.strokes_[i];
            }
        }

        delete[] strokes_;

        strokes_ = new_strokes;
        count_ = other.count_;

        return *this;
    }

    // Заглушка ничего не делает. Для присваивания себе это случайно похоже
    // на правильный ответ, чужое перемещение она не выполняет.
    Peal &Peal::operator=(Peal &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        delete[] strokes_;

        strokes_ = other.strokes_;
        count_ = other.count_;

        other.strokes_ = nullptr;
        other.count_ = 0;

        return *this;
    }

    int Peal::size() const
    {
        return count_;
    }

    Stroke &Peal::operator[](int index)
    {
        if (index < 0 || index >= count_)
        {
            throw PealError("Invalid index");
        }

        return strokes_[index];
    }

    const Stroke &Peal::operator[](int index) const
    {
        if (index < 0 || index >= count_)
        {
            throw PealError("Invalid index");
        }

        return strokes_[index];
    }

    Peal Peal::operator+(const Peal &other) const
    {
        int common = count_ < other.count_
                         ? count_
                         : other.count_;

        int result_count = count_ + other.count_;

        for (int i = 0; i < common; ++i)
        {
            if (strokes_[i].bell == other.strokes_[i].bell)
            {
                --result_count;
            }
        }

        Peal result(result_count);

        int pos = 0;

        for (int i = 0; i < common; ++i)
        {
            Stroke a = strokes_[i];
            Stroke b = other.strokes_[i];

            if (a.bell < b.bell)
            {
                result[pos++] = a;
                result[pos++] = b;
            }
            else if (a.bell > b.bell)
            {
                result[pos++] = b;
                result[pos++] = a;
            }
            else
            {
                result[pos++] = a.beats >= b.beats ? a : b;
            }
        }

        for (int i = common; i < count_; ++i)
        {
            result[pos++] = strokes_[i];
        }

        for (int i = common; i < other.count_; ++i)
        {
            result[pos++] = other.strokes_[i];
        }

        return result;
    }

    Peal &Peal::operator+=(const Peal &other)
    {
        *this = *this + other;
        return *this;
    }

    bool Peal::operator==(const Peal &other) const
    {
        if (count_ != other.count_)
        {
            return false;
        }

        for (int i = 0; i < count_; ++i)
        {
            if (strokes_[i].bell != other.strokes_[i].bell ||
                strokes_[i].beats != other.strokes_[i].beats)
            {
                return false;
            }
        }

        return true;
    }

    bool Peal::operator!=(const Peal &other) const
    {
        return !(*this == other);
    }

    Peal::operator int() const
    {
        int sum = 0;

        for (int i = 0; i < count_; ++i)
        {
            sum += strokes_[i].beats;
        }

        return sum;
    }

} // namespace rybinsk
