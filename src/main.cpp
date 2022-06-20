#include "Application.h"

int main()
{
    auto app = Application();

    if (app.Init("Painter", 0, 0) < 0)
    {
        return -1;
    }

    while (app.Running())
    {
        app.Update();
    }

    return 0;
}