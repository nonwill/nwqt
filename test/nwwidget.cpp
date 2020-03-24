#include "nw_widget.h"
#include "nw_titlebar.h"
#include "nw_application.h"
//#include "eventdispatcher_libuv.h"
//#include "uv.h"
int main(int argc, char* argv[])
{
//    LIBUV_QAPP(a, argc, argv);

    NWQAppGui a(argc, argv);
    NWQWidget w ;
    QString title("标题");
    w.setWindowTitle(title);
    w.setToplevel();
    w.resize(300, 500);
    //w.setMinimumSize(200, 100);
    w.setMaximumSize(800, 1200);
    w.setAutoHide();
    w.show();
    w.titleBar()->enableBtn(NWQTitleBar::BTN_MIN, false);
    //uv_run(loop, UV_RUN_DEFAULT);
    return a.exec();
}
