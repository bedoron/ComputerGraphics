#include <afxwin.h>

class WindowExample :public CFrameWnd
{
    public:
    WindowExample()
    {
        Create(NULL,"MFC Tutorial Part 5 CoderSource Dialog");
    }
    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP( WindowExample, CFrameWnd)

END_MESSAGE_MAP()