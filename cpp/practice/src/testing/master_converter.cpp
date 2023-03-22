
#include "master_converter.h"
#include "converter.h"

bool MasterConverter::master_run()
{
    return Converter().run();
}
