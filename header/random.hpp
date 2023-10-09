#ifndef TRABALHO_GRAFOS_RANDOM_HPP
#define TRABALHO_GRAFOS_RANDOM_HPP

#include <chrono>
#include <random>

namespace Random
{
inline std::mt19937 generate()
{
  std::random_device dev{};
  std::seed_seq seed{
    static_cast<std::seed_seq::result_type>(
      std::chrono::steady_clock::now().time_since_epoch().count()),
    dev(), dev(), dev(), dev(), dev(), dev(), dev()
  };

  return std::mt19937{ seed };
}

inline std::mt19937 mt{ generate() };

inline int get(int min, int max)
{
  return std::uniform_int_distribution{ min, max }(mt);
}

inline double get(double min, double max)
{
  return std::uniform_real_distribution{ min, max }(mt);
}

}

#endif