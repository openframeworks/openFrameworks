#include "ofMain.h"

#include <QtGui>
#include <QtOpenGL>
#include <QOpenGLContext>
#include <QtGlobal>
#include "QtGLWidget.h"

QtGLWidget::QtGLWidget(ofAppQtWindow* _windowPtr){
	qt_set_sequence_auto_mnemonic(true);
	mousePressed = 0;
	setMouseTracking(true);
	setUpdateBehavior(QOpenGLWidget::PartialUpdate); // very important

//	ofAppPtr = _ofAppPtr;
	instance = _windowPtr;
}

QtGLWidget::~QtGLWidget(){
}

void QtGLWidget::initializeGL()
{
//	cout << "initializeGL" << endl;
}

void QtGLWidget::paintGL()
{
//	cout << "paintGL" << endl;
}

void QtGLWidget::resizeGL(int width, int height)
{
//	cout << "resizeGL" << endl;
	instance->events().notifyWindowResized(width, height);
}

//------------------------------------------------------------
static void rotateMouseXY(ofOrientation orientation, int w, int h, double &x, double &y) {
	int savedY;
	switch (orientation) {
	case OF_ORIENTATION_180:
		x = w - x;
		y = h - y;
		break;

	case OF_ORIENTATION_90_RIGHT:
		savedY = y;
		y = x;
		x = w - savedY;
		break;

	case OF_ORIENTATION_90_LEFT:
		savedY = y;
		y = h - x;
		x = savedY;
		break;

	case OF_ORIENTATION_DEFAULT:
	default:
		break;
	}
}

//void QtGLWidget::actionEvent(QActionEvent * event)
//{
//}
//void QtGLWidget::changeEvent(QEvent * event)
//{
//}
//void QtGLWidget::closeEvent(QCloseEvent * event)
//{
//}
//void QtGLWidget::contextMenuEvent(QContextMenuEvent * event)
//{
//}
//void QtGLWidget::dragEnterEvent(QDragEnterEvent * event)
//{
//}
//void QtGLWidget::dragLeaveEvent(QDragLeaveEvent * event)
//{
//}
//void QtGLWidget::dragMoveEvent(QDragMoveEvent * event)
//{
//}
//void QtGLWidget::dropEvent(QDropEvent * event)
//{
//}
//void QtGLWidget::enterEvent(QEvent * event)
//{
//}
//void QtGLWidget::focusInEvent(QFocusEvent * event)
//{
//}
//bool QtGLWidget::focusNextPrevChild(bool next)
//{
//	return false;
//}
//void QtGLWidget::focusOutEvent(QFocusEvent * event)
//{
//}
//void QtGLWidget::hideEvent(QHideEvent * event)
//{
//}
//void QtGLWidget::inputMethodEvent(QInputMethodEvent * event)
//{
//}

void QtGLWidget::keyPressEvent(QKeyEvent * event)
{
	static ofKeyEventArgs keyEventArgs;
	keyEventArgs.key = event->key();
	keyEventArgs.scancode = event->nativeScanCode();
	ofNotifyEvent(ofEvents().keyPressed, keyEventArgs);
}

void QtGLWidget::keyReleaseEvent(QKeyEvent * event)
{
	static ofKeyEventArgs keyEventArgs;
	keyEventArgs.key = event->key();
	keyEventArgs.scancode = event->nativeScanCode();
	ofNotifyEvent(ofEvents().keyReleased, keyEventArgs);
}

//void QtGLWidget::leaveEvent(QEvent * event)
//{
//}
//void QtGLWidget::mouseDoubleClickEvent(QMouseEvent * event)
//{
//}

void QtGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	double x = event->x();
	double y = event->y();

	rotateMouseXY(instance->orientation, instance->getWidth(), instance->getHeight(), x, y);

	if (!instance->buttonPressed) {
		instance->events().notifyMouseMoved(x*instance->pixelScreenCoordScale, y*instance->pixelScreenCoordScale);
	}
	else {
		instance->events().notifyMouseDragged(x*instance->pixelScreenCoordScale, y*instance->pixelScreenCoordScale, instance->buttonInUse);
	}
}

void QtGLWidget::mousePressEvent(QMouseEvent *event)
{
	/*
	lastPos = event->pos();
	mousePressed = event->button();
	static ofMouseEventArgs mouseEventArgs;
	mouseEventArgs.x = event->pos().x();
	mouseEventArgs.y = event->pos().y();
	mouseEventArgs.button = mousePressed;

	//ofAppPtr->mousePressed(event->pos().x(), event->pos().y(), event->button());
	ofNotifyEvent(ofEvents().mousePressed, mouseEventArgs);
	*/

	Qt::MouseButton qtButton = event->button();
	int ofButton;

	switch (qtButton) {
	case Qt::MouseButton::LeftButton:
		ofButton = OF_MOUSE_BUTTON_LEFT;
		break;
	case Qt::MouseButton::RightButton:
		ofButton = OF_MOUSE_BUTTON_RIGHT;
		break;
	case Qt::MouseButton::MiddleButton:
		ofButton = OF_MOUSE_BUTTON_MIDDLE;
		break;
	}

	instance->events().notifyMousePressed(instance->events().getMouseX(), instance->events().getMouseY(), ofButton);
	instance->buttonPressed = true;
	instance->buttonInUse = ofButton;
}

void QtGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	Qt::MouseButton qtButton = event->button();
	int ofButton;

	switch (qtButton) {
	case Qt::MouseButton::LeftButton:
		ofButton = OF_MOUSE_BUTTON_LEFT;
		break;
	case Qt::MouseButton::RightButton:
		ofButton = OF_MOUSE_BUTTON_RIGHT;
		break;
	case Qt::MouseButton::MiddleButton:
		ofButton = OF_MOUSE_BUTTON_MIDDLE;
		break;
	}

	instance->events().notifyMouseReleased(instance->events().getMouseX(), instance->events().getMouseY(), ofButton);
	instance->buttonPressed = false;
	instance->buttonInUse = ofButton;
}

//void QtGLWidget::moveEvent(QMoveEvent * event)
//{
//}
//bool QtGLWidget::nativeEvent(const QByteArray & eventType, void * message, long * result)
//{
//	return false;
//}
//void QtGLWidget::paintEvent(QPaintEvent * event)
//{
//}

//void QtGLWidget::resizeEvent(QResizeEvent * event)
//{
//	cout << "resizeEvent" << endl;
//	int w = event->size().width();
//	int h = event->size().height();
//	if (instance->windowMode == OF_WINDOW) {
//		instance->windowW = w * instance->pixelScreenCoordScale;
//		instance->windowH = h * instance->pixelScreenCoordScale;
//	}
//	instance->currentW = w;
//	instance->currentH = h;
//	instance->events().notifyWindowResized(w*instance->pixelScreenCoordScale, h*instance->pixelScreenCoordScale);
//	instance->nFramesSinceWindowResized = 0;
////	static ofResizeEventArgs resizeEventArgs(w, h);
////	ofNotifyEvent(ofEvents().windowResized, resizeEventArgs);
//}

//void QtGLWidget::showEvent(QShowEvent * event)
//{
//}
//void QtGLWidget::tabletEvent(QTabletEvent * event)
//{
//}
//void QtGLWidget::wheelEvent(QWheelEvent * event)
//{
//}
//
//

/*
int key = 0;
uint32_t codepoint = 0;
int keycode = event->key();
switch (keycode) {
case GLFW_KEY_ESCAPE:
key = OF_KEY_ESC;
break;
case GLFW_KEY_F1:
key = OF_KEY_F1;
break;
case GLFW_KEY_F2:
key = OF_KEY_F2;
break;
case GLFW_KEY_F3:
key = OF_KEY_F3;
break;
case GLFW_KEY_F4:
key = OF_KEY_F4;
break;
case GLFW_KEY_F5:
key = OF_KEY_F5;
break;
case GLFW_KEY_F6:
key = OF_KEY_F6;
break;
case GLFW_KEY_F7:
key = OF_KEY_F7;
break;
case GLFW_KEY_F8:
key = OF_KEY_F8;
break;
case GLFW_KEY_F9:
key = OF_KEY_F9;
break;
case GLFW_KEY_F10:
key = OF_KEY_F10;
break;
case GLFW_KEY_F11:
key = OF_KEY_F11;
break;
case GLFW_KEY_F12:
key = OF_KEY_F12;
break;
case GLFW_KEY_LEFT:
key = OF_KEY_LEFT;
break;
case GLFW_KEY_RIGHT:
key = OF_KEY_RIGHT;
break;
case GLFW_KEY_UP:
key = OF_KEY_UP;
break;
case GLFW_KEY_DOWN:
key = OF_KEY_DOWN;
break;
case GLFW_KEY_PAGE_UP:
key = OF_KEY_PAGE_UP;
break;
case GLFW_KEY_PAGE_DOWN:
key = OF_KEY_PAGE_DOWN;
break;
case GLFW_KEY_HOME:
key = OF_KEY_HOME;
break;
case GLFW_KEY_END:
key = OF_KEY_END;
break;
case GLFW_KEY_INSERT:
key = OF_KEY_INSERT;
break;
case GLFW_KEY_LEFT_SHIFT:
key = OF_KEY_LEFT_SHIFT;
break;
case GLFW_KEY_LEFT_CONTROL:
key = OF_KEY_LEFT_CONTROL;
break;
case GLFW_KEY_LEFT_ALT:
key = OF_KEY_LEFT_ALT;
break;
case GLFW_KEY_LEFT_SUPER:
key = OF_KEY_LEFT_SUPER;
break;
case GLFW_KEY_RIGHT_SHIFT:
key = OF_KEY_RIGHT_SHIFT;
break;
case GLFW_KEY_RIGHT_CONTROL:
key = OF_KEY_RIGHT_CONTROL;
break;
case GLFW_KEY_RIGHT_ALT:
key = OF_KEY_RIGHT_ALT;
break;
case GLFW_KEY_RIGHT_SUPER:
key = OF_KEY_RIGHT_SUPER;
break;
case GLFW_KEY_BACKSPACE:
key = OF_KEY_BACKSPACE;
break;
case GLFW_KEY_DELETE:
key = OF_KEY_DEL;
break;
case GLFW_KEY_ENTER:
key = OF_KEY_RETURN;
break;
case GLFW_KEY_KP_ENTER:
key = OF_KEY_RETURN;
break;
case GLFW_KEY_TAB:
key = OF_KEY_TAB;
break;
case GLFW_KEY_KP_0:
key = codepoint = '0';
break;
case GLFW_KEY_KP_1:
key = codepoint = '1';
break;
case GLFW_KEY_KP_2:
key = codepoint = '2';
break;
case GLFW_KEY_KP_3:
key = codepoint = '3';
break;
case GLFW_KEY_KP_4:
key = codepoint = '4';
break;
case GLFW_KEY_KP_5:
key = codepoint = '5';
break;
case GLFW_KEY_KP_6:
key = codepoint = '6';
break;
case GLFW_KEY_KP_7:
key = codepoint = '7';
break;
case GLFW_KEY_KP_8:
key = codepoint = '8';
break;
case GLFW_KEY_KP_9:
key = codepoint = '9';
break;
case GLFW_KEY_KP_DIVIDE:
key = codepoint = '/';
break;
case GLFW_KEY_KP_MULTIPLY:
key = codepoint = '*';
break;
case GLFW_KEY_KP_SUBTRACT:
key = codepoint = '-';
break;
case GLFW_KEY_KP_ADD:
key = codepoint = '+';
break;
case GLFW_KEY_KP_DECIMAL:
key = codepoint = '.';
break;
case GLFW_KEY_KP_EQUAL:
key = codepoint = '=';
break;
default:
//		codepoint = keycodeToUnicode(instance, scancode, mods);
key = codepoint;
break;
}

int modifiers = 0;
int mods = event->modifiers();
if (mods & GLFW_KEY_LEFT_SHIFT) {
modifiers |= OF_KEY_SHIFT;
modifiers |= OF_KEY_LEFT_SHIFT;
}
if (mods & GLFW_KEY_RIGHT_SHIFT) {
modifiers |= OF_KEY_SHIFT;
modifiers |= OF_KEY_RIGHT_SHIFT;
}
if (mods & GLFW_KEY_LEFT_ALT) {
modifiers |= OF_KEY_ALT;
modifiers |= OF_KEY_LEFT_ALT;
}
if (mods & GLFW_KEY_RIGHT_ALT) {
modifiers |= OF_KEY_ALT;
modifiers |= OF_KEY_RIGHT_ALT;
}
if (mods & GLFW_KEY_LEFT_CONTROL) {
modifiers |= OF_KEY_CONTROL;
modifiers |= OF_KEY_LEFT_CONTROL;
}
if (mods & GLFW_KEY_RIGHT_CONTROL) {
modifiers |= OF_KEY_CONTROL;
modifiers |= OF_KEY_RIGHT_CONTROL;
}
if (mods & GLFW_KEY_LEFT_CONTROL) {
modifiers |= OF_KEY_CONTROL;
modifiers |= OF_KEY_LEFT_CONTROL;
}
if (mods & GLFW_KEY_RIGHT_CONTROL) {
modifiers |= OF_KEY_CONTROL;
modifiers |= OF_KEY_RIGHT_CONTROL;
}
if (mods & GLFW_KEY_LEFT_SUPER) {
modifiers |= OF_KEY_SUPER;
modifiers |= OF_KEY_LEFT_SUPER;
}
if (mods & GLFW_KEY_RIGHT_SUPER) {
modifiers |= OF_KEY_SUPER;
modifiers |= OF_KEY_RIGHT_SUPER;
}

static ofKeyEventArgs keyEventArgs(
ofKeyEventArgs::Pressed,
event->key(),
keycode,
event->nativeScanCode(),
codepoint,
modifiers);

ofAppPtr->keyPressed(key);
windowPtr->events().notifyKeyPressed(key);
*/

