#include "lpc.h"
#include "fintek.h"

int main(int argc, char* argv[])
{
    LPC_Enable();

    // Check LPC.
    if (!Fintek_Check())
    {
        return 2;
    }

    Fintek_View_Current_State();
    Fintek_View_Range_Tables();

    return 0;
}
