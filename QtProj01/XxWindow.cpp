#include "XxWindow.h"
#include <QQuickItem>

XxWindow::XxWindow(QWindow *parent) : QQuickWindow(parent)
{
	setFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	setColor(QColor(Qt::transparent));
	::SetWindowLong((HWND)winId(), GWL_STYLE, GetWindowLong((HWND)winId(), GWL_STYLE) | WS_THICKFRAME | WS_CAPTION);
}

XxWindow::~XxWindow()
{

}


long HitTestNow(QPoint point, QRect rcWindow)
{
	const int HIT_BORDER = 8;
	const int HIT_CAPTION = 24;

	if(!rcWindow.contains(point))
		return HTNOWHERE;

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

bool XxWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(eventType);

	const MSG *msg = static_cast<MSG*>(message);
	if(msg->message == WM_NCHITTEST) {
		int x = ((int)(short)LOWORD(msg->lParam)) - this->frameGeometry().x();
		int y = ((int)(short)HIWORD(msg->lParam)) - this->frameGeometry().y();

		QPoint point(x, y);
		QRect rcWindow(0, 0, frameGeometry().width(), frameGeometry().height());
		*result = HitTestNow(point, rcWindow);
		return true;
	}

	
	switch(msg->message) {
	case WM_NCCALCSIZE:
		return true;
	}

	return false;
}

void XxWindow::showEvent(QShowEvent * evt)
{
	//::SetWindowLong((HWND)winId(), GWL_STYLE, GetWindowLong((HWND)winId(), GWL_STYLE) | WS_THICKFRAME);
	
	QQuickWindow::showEvent(evt);
}
