#pragma once

#include <QQuickWindow>

class XxWindow : public QQuickWindow
{
	Q_OBJECT
public:
	explicit XxWindow(QWindow *parent = 0);
	~XxWindow();

protected:
	bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

	void showEvent(QShowEvent * evt) override;
};
