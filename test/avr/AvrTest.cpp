/**
 * @author	K
 * @date		19-04-2017.
 */

#include <pinlist.h>
#include <latch.h>

using namespace velo::embedded;

typedef PinList<Pa1, Pa2, Pa5, Pb3, Pb4> MyPins;

typedef ThreePinLatch<Pa0, Pa1, Pa3, 'R1'> ShiftReg1;

typedef Pin<ShiftReg1, 0> Rg0;
typedef Pin<ShiftReg1, 1> Rg1;
typedef Pin<ShiftReg1, 2> Rg2;
typedef Pin<ShiftReg1, 3> Rg3;
typedef Pin<ShiftReg1, 4> Rg4;
typedef Pin<ShiftReg1, 5> Rg5;
typedef Pin<ShiftReg1, 6> Rg6;
typedef Pin<ShiftReg1, 7> Rg7;

typedef PinList<Rg0, Rg1, Rg2> SomeList;

int main(int argc, char** argv, char** env)
{
	MyPins::write(0x55);

	SomeList::write(1);
}