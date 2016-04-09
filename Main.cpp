#include <iostream>
#include "Window.h"

int main()
{
    Window::init();

    Highway high;

    Window win(high);
    win.start();

    Window::term();
    return 0;
}