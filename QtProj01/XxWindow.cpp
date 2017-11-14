#include "XxWindow.h"
#include <QQuickItem>
#include <QtCore>
#include <QtWidgets/QtWidgets>

XxWindow::XxWindow(QWindow *parent) : QQuickWindow(parent)
{
	setFlags(Qt::Window);
	setColor(QColor(Qt::transparent));
}

XxWindow::~XxWindow()
{

}

bool HandleGetMinMaxInfoMsg(const MSG *msg, long *result)
{
	QRect maxRect = QApplication::desktop()->availableGeometry();
	//QSize szMin = this->minimumSize();
	//QSize szMax = this->maximumSize();
	PMINMAXINFO lpMMI = (PMINMAXINFO)msg->lParam;
	lpMMI->ptMaxSize.x = maxRect.width();
	lpMMI->ptMaxSize.y = maxRect.height();
	lpMMI->ptMaxPosition.x = 0;
	lpMMI->ptMaxPosition.y = 0;
	//lpMMI->ptMinTrackSize.x = szMin.width();
	//lpMMI->ptMinTrackSize.y = szMin.height();
	//lpMMI->ptMaxTrackSize.x = szMax.width();
	//lpMMI->ptMaxTrackSize.y = szMax.height();
	lpMMI->ptMinTrackSize.x = GetSystemMetrics(SM_CXMINTRACK);
	lpMMI->ptMinTrackSize.y = GetSystemMetrics(SM_CYMINTRACK);
	lpMMI->ptMaxTrackSize.x = GetSystemMetrics(SM_CXMAXTRACK);
	lpMMI->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK);

	return true;
}

long HitTestNow(HWND hWnd, QPoint point, QRect rcWindow)
{
	const int HIT_BORDER = 8;
	const int HIT_CAPTION = 24;

	if(!rcWindow.contains(point))
		return HTNOWHERE;

	if(IsZoomed(hWnd))
		return HTCAPTION;

	long RESULTS[4][3] = 
	{
		HTTOPLEFT,HTTOP,HTTOPRIGHT,
		HTLEFT, /*HTCLIENT*/HTCAPTION, HTRIGHT,
		HTLEFT, /*HTCLIENT*/HTCAPTION, HTRIGHT,
		HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT,
	};

	int row = 0, col = 0;
	if(point.y() < HIT_BORDER)
		row = 0;
	else if(point.y() < HIT_BORDER + HIT_CAPTION)
		row = 1;
	else if(point.y() < rcWindow.height() - HIT_BORDER)
		row = 2;
	else
		row = 3;

	if(point.x() < HIT_BORDER)
		col = 0;
	else if(point.x() < rcWindow.width() - HIT_BORDER)
		col = 1;
	else
		col = 2;
	return RESULTS[row][col];
}

bool XxWindow::event(QEvent * evt)
{
	return QQuickWindow::event(evt);
}

bool XxWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(eventType);

	const MSG *msg = static_cast<MSG*>(message);
	if(msg->message == WM_NCHITTEST) {
		int x = ((int)(short)LOWORD(msg->lParam)) - this->frameGeometry().x();
		int y = ((int)(short)HIWORD(msg->lParam)) - this->frameGeometry().y();

		QPoint point(x, y);
		QRect rcWindow(0, 0, frameGeometry().width(), frameGeometry().height());
		*result = HitTestNow(msg->hwnd, point, rcWindow);
		return true;
	}
	
	switch(msg->message) {
	case WM_ERASEBKGND:
		*result = 0;
		return true;
	case WM_GETMINMAXINFO:
		*result = 0;
		return true;
	case WM_NCCALCSIZE:
	{
		*result = 0;
		return true;
		int xFrame = 0; /*左右边框的厚度*/
		int yFrame = 0; /*下边框的厚度*/
		int nTHight = 0; /*标题栏的高度*/
		NCCALCSIZE_PARAMS * p = (NCCALCSIZE_PARAMS *)(msg->lParam);
		if(msg->wParam)
		{
			RECT rcDst = p->rgrc[0];
			RECT rcSrc = p->rgrc[1];

			p->rgrc[0] = rcDst;
			p->rgrc[1] = rcDst;
			p->rgrc[2] = rcSrc;
		}
		else
		{
			RECT * rc = (RECT *)msg->lParam;

			rc->left = rc->left + xFrame;
			rc->top = rc->top + nTHight;
			rc->right = rc->right - xFrame;
			rc->bottom = rc->bottom - yFrame;
		}
		*result = 0;
		return true;
	}
	default:
		break;
	}

	return false;
}

void XxWindow::showEvent(QShowEvent * evt)
{
	//::SetWindowLong((HWND)winId(), GWL_STYLE, GetWindowLong((HWND)winId(), GWL_STYLE) | WS_THICKFRAME);
	
	QQuickWindow::showEvent(evt);
}
