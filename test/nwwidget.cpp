#include "nw_widget.h"
#include "nw_titlebar.h"
#include "nw_application.h"
int main(int argc, char* argv[])
{
    NWQAppGui a(argc, argv);
    NWQWidget w ;
    w.setToplevel();
    w.resize(300, 500);
    //w.setMinimumSize(200, 100);
    w.setMaximumSize(800, 1200);
    w.setAutoHide();
    w.show();
    w.titleBar()->enableBtn(NWQTitleBar::BTN_MIN, false);
    return a.exec();
}
