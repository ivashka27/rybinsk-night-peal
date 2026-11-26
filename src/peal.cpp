#include "rybinsk/peal.hpp"

#include <algorithm>
#include <climits>
#include <utility>

namespace rybinsk {

Peal::Peal() = default;

Peal::Peal(int count) {
  if (count < 0) {
    throw PealError("Negative count");
  }
  if (count > 0) {
    strokes_ = new Stroke[count]{};
    count_ = count;
  }
}

Peal::Peal(const Stroke* strokes, int count) {
  if(count < 0 || (strokes == nullptr && count > 0)) {
    throw PealError("Invalid strokes or count");
  }
  if (count > 0) {
    strokes_ = new Stroke[count];
    count_ = count;
    for (int i = 0; i < count_; ++i) {
      strokes_[i] = strokes[i];
    }
  }
}

Peal::Peal(const Peal& other) : Peal(other.strokes_, other.count_) {}

Peal::Peal(Peal&& other) noexcept
    : strokes_(std::exchange(other.strokes_, nullptr)),
      count_(std::exchange(other.count_, 0)) {}

Peal::~Peal() {
  delete[] strokes_;
}

Peal& Peal::operator=(const Peal& other) {
  if (this != &other) {
    Peal temp(other);
    std::swap(strokes_, temp.strokes_);
    std::swap(count_, temp.count_);
  }
  return *this;
}

Peal& Peal::operator=(Peal&& other) noexcept {
  if (this != &other) {
    delete[] strokes_;
    strokes_ = std::exchange(other.strokes_, nullptr);
    count_ = std::exchange(other.count_, 0);
  }
  return *this;
}

int Peal::size() const {
  return count_;
}

Stroke& Peal::operator[](int index) {
  if (index < 0 || index >= count_) {
    throw PealError("Index out of bounds");
  }
  return strokes_[index];
}

const Stroke& Peal::operator[](int index) const {
  if (index < 0 || index >= count_) {
    throw PealError("Index out of bounds");
  }
  return strokes_[index];
}

Peal Peal::operator+(const Peal& other) const {
  int min_len = std::min(count_, other.count_);
  int max_len = std::max(count_, other.count_);

  long long res_count = static_cast<long long>(max_len) - min_len;
  for (int i = 0; i < min_len; ++i) {
    res_count += (strokes_[i].bell == other.strokes_[i].bell) ? 1 : 2;
  }

  if (res_count > INT_MAX) {
    throw PealError("Resulting peal is too large");
  }

  Peal res(static_cast<int>(res_count));
  int pos = 0;

  for (int i = 0; i < min_len; ++i) {
    const Stroke& a = strokes_[i];
    const Stroke& b = other.strokes_[i];
    if(a.bell == b.bell) {
      res.strokes_[pos++] = (a.beats >= b.beats) ? a : b;
    } else if (a.bell < b.bell) {
      res.strokes_[pos++] = a;
      res.strokes_[pos++] = b;
    } else {
      res.strokes_[pos++] = b;
      res.strokes_[pos++] = a;
    }
  }

  const Peal& longer = (count_ > other.count_) ? *this : other;
  for(int i = min_len; i < max_len; ++i) {
    res.strokes_[pos++] = longer.strokes_[i];
  }

  return res;
}

Peal& Peal::operator+=(const Peal& other) {
  *this = *this + other;
  return *this;
}

bool Peal::operator==(const Peal& other) const {
  if (count_ != other.count_) {
    return false;
  }
  for (int i = 0; i < count_; ++i) {
    if (strokes_[i].bell != other.strokes_[i].bell ||
        strokes_[i].beats != other.strokes_[i].beats) {
      return false;
    }
  }
  return true;
}

bool Peal::operator!=(const Peal& other) const {
  return !(*this == other);
}

Peal::operator int() const {
  int sum = 0;
  for (int i = 0; i < count_; ++i) {
    sum += strokes_[i].beats;
  }
  return sum;
}

} // namespace rybinsk