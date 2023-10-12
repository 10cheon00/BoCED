#ifndef __COORD__
#define __COORD__

struct Coord {
  Coord();
  Coord(int, int);

  int x;
  int y;
  bool operator==(const Coord &other) {
    return x == other.x && y == other.y;
  }
  Coord &operator+=(const Coord &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Coord operator+(const Coord &other) {
    return Coord(x + other.x, y + other.y);
  }
};

#endif