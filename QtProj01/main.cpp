#include <QDir>
#include <QGuiApplication>
#include <QQmlFileSelector>
#include <QQuickView> 
#include <QColor>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "XxWindow.h"

int qtmain(int argc, char *argv[])
{
	qInfo() << "version" << QT_VERSION << std::endl;

	QGuiApplication app(argc, argv);
	qmlRegisterType<XxWindow>("com.thexujie", 1, 0, "XxWindow");
	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("test.qml")));
	return app.exec();
}

int qtmain2(int argc, char * argv[])
{	
	//QString qml("qrc:///clocks.qml");
	QString qml("test.qml");
	//QString qml("E:\\vsfiles2\\QtProj0\\QtProj01\\test.qml");
	//QString qml("E:\\vsfiles2\\QtProj0\\QtProj01\\clocks.qml");

	QGuiApplication app(argc, argv);
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	//qmlRegisterType<XxWindow>();
	qmlRegisterType<XxWindow>("com.thexujie", 1, 0, "XxWindow");

	//QQuickView view;
	//view.setFlag(Qt::FramelessWindowHint);
	//view.setColor(QColor(Qt::transparent));
	//view.rootContext()->setContextProperty("mainwindow", &view);

	////加载qml，qml添加到资源文件中可避免qml暴露
	//view.setSource(QUrl(qml));
	//view.show();
	//view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);

	QQmlApplicationEngine engine;
		//2.加载qml文件 
	engine.load(QUrl(qml));

	return app.exec();

	//QString qml("qrc:///demos/clocks/clocks.qml");
 //   QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
 //   QGuiApplication app(argc,argv);
 //   app.setOrganizationName("QtProject");
 //   app.setOrganizationDomain("qt-project.org");
 //   app.setApplicationName(QFileInfo(app.applicationFilePath()).baseName());
 //   QQuickView view;
 //   if (qgetenv("QT_QUICK_CORE_PROFILE").toInt()) {
 //       QSurfaceFormat f = view.format();
 //       f.setProfile(QSurfaceFormat::CoreProfile);
 //       f.setVersion(4, 4);
 //       view.setFormat(f);
 //   }
 //   view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
 //   new QQmlFileSelector(view.engine(), &view);
 //   view.setSource(QUrl(qml));
 //   if (view.status() == QQuickView::Error)
 //       return -1;
 //   view.setResizeMode(QQuickView::SizeRootObjectToView);
 //   if (QGuiApplication::platformName() == QLatin1String("qnx") ||
 //         QGuiApplication::platformName() == QLatin1String("eglfs")) {
 //       view.showFullScreen();
 //   } else {
 //       view.show();
 //   }
 //   return app.exec();
}

// Convert a wchar_t to char string, equivalent to QString::toLocal8Bit()
// when passed CP_ACP.
static inline char *wideToMulti(int codePage, const wchar_t *aw)
{
	const int required = WideCharToMultiByte(codePage, 0, aw, -1, NULL, 0, NULL, NULL);
	char *result = new char[required];
	WideCharToMultiByte(codePage, 0, aw, -1, result, required, NULL, NULL);
	return result;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	int argc;
	wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
	if(!argvW)
		return -1;
	char **argv = new char *[argc + 1];
	for(int i = 0; i < argc; ++i)
		argv[i] = wideToMulti(CP_ACP, argvW[i]);
	argv[argc] = Q_NULLPTR;
	LocalFree(argvW);
	const int exitCode = qtmain(argc, argv);
	for(int i = 0; i < argc && argv[i]; ++i)
		delete[] argv[i];
	delete[] argv;
	return exitCode;
}
