// 2022/5/8 19:23:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>


bool HAL_SDRAM::Test()
{
    uint16 *end = (uint16 *)(ADDRESS + SIZE);
    end--;

    uint16 *address = (uint16 *)ADDRESS;

    address++;

    while (address < end)
    {
        uint16 prev = *(address - 1);
        uint16 next = *(address + 1);

        uint16 value = std::rand() % 65535;

        *address = value;

        uint16 reading = *address;

        if (value != reading)
        {
            return false;
        }

        uint16 new_prev = *(address - 1);

        if (prev != new_prev)
        {
            return false;
        }

        uint16 new_next = *(address + 1);

        if (next != new_next)
        {
            return false;
        }

        address++;
    }

    return true;
}
