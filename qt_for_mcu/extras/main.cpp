#include "extras.h"

#include <qul/application.h>
#include <qul/qul.h>

#ifdef SIM_BACKEND
extern void simulateStatusBar();
#endif

int main()
{
    Qul::initHardware();
    Qul::initPlatform();
    Qul::Application app;
    static extras item;
    app.setRootItem(&item);

#ifdef SIM_BACKEND
    simulateStatusBar();
#endif

    app.exec();
}
