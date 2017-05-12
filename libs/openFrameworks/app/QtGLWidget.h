#pragma once

#include "ofBaseApp.h"
#include "ofApp.h"
#include "ofAppQtWindow.h"
//
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

extern void qt_set_sequence_auto_mnemonic(bool b);
class ofAppQtWindow;

class QtGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:

	QtGLWidget(ofAppQtWindow* _windowPtr);// shared_ptr<::ofApp> app, ofAppQtWindow* _windowPtr, QWidget *parent = 0
    ~QtGLWidget();
	
//	shared_ptr<ofApp> ofAppPtr;
	ofAppQtWindow* instance;

//	void swapBuffers();

protected:
	void initializeGL();					// required from Qt
    void paintGL();							// required from Qt
    void resizeGL(int width, int height);	// required from Qt

	// TODO:
	// fill out Qt callbacks

	//ofCoreEvents&  getEvents();
	//ofAppQtWindow * setCurrent(QOpenGLWidget* windowP);
	//void 	actionEvent(QActionEvent *event);
	//void 	changeEvent(QEvent *event);
	//void 	closeEvent(QCloseEvent *event);
	//void 	contextMenuEvent(QContextMenuEvent *event);
	////	void 	create(WId window = 0, bool initializeWindow = true, bool destroyOldWindow = true)
	////	void 	destroy(bool destroyWindow = true, bool destroySubWindows = true)
	//void 	dragEnterEvent(QDragEnterEvent *event);
	//void 	dragLeaveEvent(QDragLeaveEvent *event);
	//void 	dragMoveEvent(QDragMoveEvent *event);
	//void 	dropEvent(QDropEvent *event);
	//void 	enterEvent(QEvent *event);
	//void 	focusInEvent(QFocusEvent *event);
	////	bool 	focusNextChild()
	//bool 	focusNextPrevChild(bool next);
	//void 	focusOutEvent(QFocusEvent *event);
	////	bool 	focusPreviousChild()
	//void 	hideEvent(QHideEvent *event);
	//void 	inputMethodEvent(QInputMethodEvent *event);
	void 	keyPressEvent(QKeyEvent *event);
	void 	keyReleaseEvent(QKeyEvent *event);
	//void 	leaveEvent(QEvent *event);
	//void 	mouseDoubleClickEvent(QMouseEvent *event);
	void 	mouseMoveEvent(QMouseEvent *event);
	void 	mousePressEvent(QMouseEvent *event);
	void 	mouseReleaseEvent(QMouseEvent *event);
	//void 	moveEvent(QMoveEvent *event);
	//bool 	nativeEvent(const QByteArray &eventType, void *message, long *result);
	//void 	paintEvent(QPaintEvent *event);
	//void 	resizeEvent(QResizeEvent *event); // implementing this event will cause resizeGL to stop working..
	//void 	showEvent(QShowEvent *event);
	//void 	tabletEvent(QTabletEvent *event);
	//void 	wheelEvent(QWheelEvent *event);

private:
		QPoint lastPos;
		int mousePressed;	

	public slots:

signals:

};

