#include "nww_login.h"
#include "nw_application.h"
int main(int argc, char* argv[])
{
    NWQAppGui a(argc, argv);
    IMLoginWidget w ;
    w.show();
    return a.exec();
}
