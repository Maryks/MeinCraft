#ifndef TYPEDEFS_H
#define TYPEDEFS_H

//#include <cstdint>
#include <map>
#include <tuple>
#include <vector>

typedef unsigned char id_t;	// 1 byte for block IDs
typedef std::pair<int, int> xz_t;	// for x, z coordinates
typedef std::tuple<int, int, int> xyz_t;	// for x, y, z coordinates
typedef std::vector<float> coords_t;	// for coordinates of a vertex

#endif // !TYPEDEFS_H
