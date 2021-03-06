#include "slider_attacks.h"
#include "common.h"

#include <cassert>
#include <iostream>

int exampleMain() {
  SliderAttacks slider_attacks;
  slider_attacks.Initialize();
  const U64 bitboard = 0xF7F6F3748CA5B610ULL;
  PrintBitBoard(bitboard);
  std::cout << std::endl;
  U64 attacks = slider_attacks.QueenAttacks(bitboard, 36);
  PrintBitBoard(attacks);
  std::cout << std::endl;
  assert(attacks == 0x4382c38509000ULL);
  attacks = slider_attacks.QueenAttacks(bitboard, 29);
  PrintBitBoard(attacks);
  assert(attacks == 0x70d8708804ULL);
  // TODO: Add more tests.
  return 0;
}
