package cc.openframeworks;

import android.util.Log;
import android.view.InputDevice;
import android.view.InputEvent;
import android.view.KeyEvent;
import android.view.MotionEvent;

import java.util.ArrayList;

public class OFAndroidController {

    public static final String PS5_Controller_NAME = "Sony Interactive Entertainment Wireless Controller";
    public static final String PS5_Controller_NAME_GENERIC = "Wireless Controller";
    public static final String PS4_Controller_NAME = "Sony Interactive Entertainment Wireless Controller Touchpad";
    public static final String XBOX_Controller_NAME = "Microsoft X-Box One pad (Firmware 2015)";
    public static final String STEAM_Controller_NAME = "Valve Software Wired Controller";
    public static final String NINTENDO_Controller_NAME = "Nintendo Wired Controller";


    public static final int VendorERROR = 0;
    public static final int VendorPS = 1356;
    public static final int VendorMS = 1118;
    public static final int VendorNS = 1406;
    public static final int VendorValve = 10462;

    public static final int ProductID_PS5 = 3302;
    public static final int ProductID_PS4 = 2508;
    public static final int ProductID_PS4_2 = 1476;
    public static final int ProductID_XBOX = 765;
    public static final int ProductID_STEAM = 4354;

    public enum ControllerType {
        NOT_SET,
        NONE,
        ACTION_DPAD,
        ACTION_X,
        ACTION_A,
        ACTION_B
    }

    public enum Keys {
        KEYCODE_BUTTON_A,
        KEYCODE_BUTTON_B,
        KEYCODE_BUTTON_C,
        KEYCODE_BUTTON_Y,
        KEYCODE_BUTTON_X,
        KEYCODE_BUTTON_Z,
        KEYCODE_BUTTON_L1,
        KEYCODE_BUTTON_R1,
        KEYCODE_BUTTON_L2,
        KEYCODE_BUTTON_R2,
        KEYCODE_DPAD_UP,
        KEYCODE_DPAD_LEFT,
        KEYCODE_DPAD_RIGHT,
        KEYCODE_DPAD_DOWN,
        KEYCODE_DPAD_CENTER

    }

    public static ControllerType getControllerType(InputDevice device) {


        if(OFActivity.LOG_INPUT) Log.i( "OF", "getControllerType number:" + device.getControllerNumber() +

                " vendorid:" + device.getVendorId() +
                " descriptor:" + device.getDescriptor() + " name:" + device.getName());

        int[] keyMap = new int[15];
        keyMap[0] = (int)KeyEvent.KEYCODE_BUTTON_A;
        keyMap[1] = (int)KeyEvent.KEYCODE_BUTTON_B;
        keyMap[2] = (int)KeyEvent.KEYCODE_BUTTON_C;
        keyMap[3] = (int)KeyEvent.KEYCODE_BUTTON_Y;

        keyMap[4] = (int)KeyEvent.KEYCODE_BUTTON_X;
        keyMap[5] = (int)KeyEvent.KEYCODE_BUTTON_Z;

        keyMap[6] = (int)KeyEvent.KEYCODE_BUTTON_L1;
        keyMap[7] = (int)KeyEvent.KEYCODE_BUTTON_R1;
        keyMap[8] = (int)KeyEvent.KEYCODE_BUTTON_L2;
        keyMap[9] = (int)KeyEvent.KEYCODE_BUTTON_R2;

        keyMap[10] = (int)KeyEvent.KEYCODE_DPAD_UP;
        keyMap[11] = (int)KeyEvent.KEYCODE_DPAD_LEFT;
        keyMap[12] = (int)KeyEvent.KEYCODE_DPAD_RIGHT;
        keyMap[13] = (int)KeyEvent.KEYCODE_DPAD_DOWN;
        keyMap[14] = (int) KeyEvent.KEYCODE_DPAD_CENTER;


        boolean[] hasMap = device.hasKeys(keyMap);

        if(device.getVendorId() == 0 && device.getProductId() == 0 ) {
            return ControllerType.ACTION_X;
        }
        if(device.getVendorId() == VendorPS) {
            return ControllerType.ACTION_X;
        }else if(device.getVendorId() == VendorNS) {
            return ControllerType.ACTION_B;
        } else {
            return ControllerType.ACTION_A;
        }

//        if(!hasMap[0] && // A
//           hasMap[1] && // B
//           hasMap[4] && // X // Y
//           hasMap[3]) {
//            return ControllerType.ACTION_A;
//        }

    }

    final static int UP = 0;
    final static int LEFT = 1;
    final static int RIGHT = 2;
    final static int DOWN = 3;
    final static int CENTER = 4;


    public int getDirectionPressed(InputEvent event) {
        if (!isDpadDevice(event)) {
            return -1;
        }

        // If the input event is a MotionEvent, check its hat axis values.
        if (event instanceof MotionEvent) {

            // Use the hat axis value to find the D-pad direction
            MotionEvent motionEvent = (MotionEvent) event;
            float xaxis = motionEvent.getAxisValue(MotionEvent.AXIS_HAT_X);
            float yaxis = motionEvent.getAxisValue(MotionEvent.AXIS_HAT_Y);

            // Check if the AXIS_HAT_X value is -1 or 1, and set the D-pad
            // LEFT and RIGHT direction accordingly.
            if (Float.compare(xaxis, -1.0f) == 0) {
                directionPressed = OFAndroidController.LEFT;
            } else if (Float.compare(xaxis, 1.0f) == 0) {
                directionPressed = OFAndroidController.RIGHT;
            }
            // Check if the AXIS_HAT_Y value is -1 or 1, and set the D-pad
            // UP and DOWN direction accordingly.
            else if (Float.compare(yaxis, -1.0f) == 0) {
                directionPressed = OFAndroidController.UP;
            } else if (Float.compare(yaxis, 1.0f) == 0) {
                directionPressed = OFAndroidController.DOWN;
            }
        }
        // If the input event is a KeyEvent, check its key code.
        else if (event instanceof KeyEvent) {

            // Use the key code to find the D-pad direction.
            KeyEvent keyEvent = (KeyEvent) event;
            if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_LEFT) {
                directionPressed = OFAndroidController.LEFT;
            } else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_RIGHT) {
                directionPressed = OFAndroidController.RIGHT;
            } else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_UP) {
                directionPressed = OFAndroidController.UP;
            } else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_DOWN) {
                directionPressed = OFAndroidController.DOWN;
            } else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_CENTER) {
                directionPressed = OFAndroidController.CENTER;
            }
        }
        return directionPressed;
    }

    public static boolean isDpadDevice(InputEvent event) {
        // Check that input comes from a device with directional pads.
        if ((event.getSource() & InputDevice.SOURCE_DPAD)
                != InputDevice.SOURCE_DPAD) {
            return true;
        } else {
            return false;
        }
    }
    
    public static int CONTROLLER_OFFSET = 400;

    public static boolean isActionKey(int keyCode) {
        // Here we treat Button_A and DPAD_CENTER as the primary action
        // keys for the game.
        return keyCode == KeyEvent.KEYCODE_DPAD_CENTER
                || keyCode == KeyEvent.KEYCODE_BUTTON_A;
    }

    public static boolean isControllerBackKey(int keyCode) {
        // Here we treat Button_A and DPAD_CENTER as the primary action
        // keys for the game.
        return keyCode == KeyEvent.KEYCODE_DPAD_CENTER
                || keyCode == KeyEvent.KEYCODE_BUTTON_A;
    }

    public static float getCenteredAxis(MotionEvent event,
                                         InputDevice device, int axis, int historyPos) {
        final InputDevice.MotionRange range =
                device.getMotionRange(axis, event.getSource());

        // A joystick at rest does not always report an absolute position of
        // (0,0). Use the getFlat() method to determine the range of values
        // bounding the joystick axis center.
        if (range != null) {
            final float flat = range.getFlat();
            final float value =
                    historyPos < 0 ? event.getAxisValue(axis):
                            event.getHistoricalAxisValue(axis, historyPos);

            // Ignore axis values that are within the 'flat' region of the
            // joystick axis center.
            if (Math.abs(value) > flat) {
                return value;
            }
        }
        return 0;
    }

    public static void processJoystickInput(MotionEvent event,
                                            int historyPos) {

        InputDevice inputDevice = event.getDevice();

        // Calculate the horizontal distance to move by
        // using the input value from one of these physical controls:
        // the left control stick, hat axis, or the right control stick.
        float x = getCenteredAxis(event, inputDevice,
                MotionEvent.AXIS_X, historyPos);
        if (x == 0) {
            x = getCenteredAxis(event, inputDevice,
                    MotionEvent.AXIS_HAT_X, historyPos);
        }
        if (x == 0) {
            x = getCenteredAxis(event, inputDevice,
                    MotionEvent.AXIS_Z, historyPos);
        }

        // Calculate the vertical distance to move by
        // using the input value from one of these physical controls:
        // the left control stick, hat switch, or the right control stick.
        float y = getCenteredAxis(event, inputDevice,
                MotionEvent.AXIS_Y, historyPos);
        if (y == 0) {
            y = getCenteredAxis(event, inputDevice,
                    MotionEvent.AXIS_HAT_Y, historyPos);
        }
        if (y == 0) {
            y = getCenteredAxis(event, inputDevice,
                    MotionEvent.AXIS_RZ, historyPos);
        }
        if(OFActivity.LOG_INPUT)
            Log.i("OF", "processJoystickInput: " + " x:" + x + " y::" + y);
        OFAndroid.onAxisMoved(inputDevice.getId(), event.getDeviceId(), inputDevice.getProductId(), x, y);
    }


    final static int DPAD_UP       = 0;
    final static int DPAD_LEFT     = 1;
    final static int DPAD_RIGHT    = 2;
    final static int DPAD_DOWN     = 3;
    final static int DPAD_CENTER   = 4;
    static int directionPressed = -1;
    static int keyCode = -1;
    static int lastKeyCode = -1;
    static int deviceCode = -1;

    static boolean DPAD_UP_DOWN  = false;
    static boolean DPAD_LEFT_DOWN     = false;
    static boolean DPAD_RIGHT_DOWN    = false;
    static boolean DPAD_DOWN_DOWN     = false;
    static boolean DPAD_CENTER_DOWN   = false;

    public static boolean genericMotionEvent (MotionEvent event){
        if(OFActivity.LOG_INPUT) Log.i("OF", "dispatchGenericMotionEvent" + " event:" + event.toString() + " deviceId:" + event.getDeviceId());

        if(deviceCode == -1)
            deviceCode = event.getDeviceId();

        if (OFAndroidController.isDpadDevice(event)) {
            return  false;
//            int press = dpad.getDirectionPressed(event);
//            int action = event.getAction();
//            switch (press) {
//                case Dpad.LEFT:
//                    if (event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS || event.getActionMasked() == MotionEvent.ACTION_DOWN) {
//                        return OFAndroid.keyDown(KeyEvent.KEYCODE_DPAD_LEFT + CONTROLLER_OFFSET, KeyEvent.KEYCODE_DPAD_LEFT);
//                    }
//                    else if (event.getActionMasked() == MotionEvent.ACTION_BUTTON_RELEASE || event.getActionMasked() == MotionEvent.ACTION_CANCEL  || event.getActionMasked() == MotionEvent.ACTION_UP) {
//                        return OFAndroid.keyUp(KeyEvent.KEYCODE_DPAD_LEFT + CONTROLLER_OFFSET, KeyEvent.KEYCODE_DPAD_LEFT);
//                    }
//                    return true;
//                case Dpad.RIGHT:
//                    if (event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS || event.getActionMasked() == MotionEvent.ACTION_DOWN) {
//                        return OFAndroid.keyDown(KeyEvent.KEYCODE_DPAD_RIGHT + CONTROLLER_OFFSET, KeyEvent.KEYCODE_DPAD_RIGHT);
//                    }
//                    else if (event.getActionMasked() == MotionEvent.ACTION_BUTTON_RELEASE || event.getActionMasked() == MotionEvent.ACTION_CANCEL  || event.getActionMasked() == MotionEvent.ACTION_UP) {
//                        return OFAndroid.keyUp(KeyEvent.KEYCODE_DPAD_RIGHT + CONTROLLER_OFFSET, KeyEvent.KEYCODE_DPAD_RIGHT);
//                    }
//                    return true;
//                case Dpad.UP:
//                    if (event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS || event.getActionMasked() == MotionEvent.ACTION_DOWN) {
//                        return OFAndroid.keyDown(KeyEvent.KEYCODE_DPAD_UP + CONTROLLER_OFFSET, KeyEvent.KEYCODE_DPAD_UP);
//                    }
//                    else if (event.getActionMasked() == MotionEvent.ACTION_BUTTON_RELEASE || event.getActionMasked() == MotionEvent.ACTION_CANCEL  || event.getActionMasked() == MotionEvent.ACTION_UP) {
//                        return OFAndroid.keyUp(KeyEvent.KEYCODE_DPAD_UP + CONTROLLER_OFFSET, KeyEvent.KEYCODE_DPAD_UP);
//                    }
//                case Dpad.DOWN:
//                    if (event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS || event.getActionMasked() == MotionEvent.ACTION_DOWN) {
//                        return OFAndroid.keyDown(KeyEvent.KEYCODE_DPAD_DOWN + CONTROLLER_OFFSET, KeyEvent.KEYCODE_DPAD_DOWN);
//                    }
//                    else if (event.getActionMasked() == MotionEvent.ACTION_BUTTON_RELEASE || event.getActionMasked() == MotionEvent.ACTION_CANCEL || event.getActionMasked() == MotionEvent.ACTION_UP) {
//                        return OFAndroid.keyUp(KeyEvent.KEYCODE_DPAD_DOWN + CONTROLLER_OFFSET, KeyEvent.KEYCODE_DPAD_DOWN);
//                    }
//                    return true;
//
//            }
        }
        //if(event.getDeviceId() != deviceCode) return true;
        try {
            if ((event.getSource() & InputDevice.SOURCE_JOYSTICK) ==
                    InputDevice.SOURCE_JOYSTICK &&
                    event.getAction() == MotionEvent.ACTION_MOVE) {
                if (event.getAction() == MotionEvent.ACTION_MOVE) {
                    // process the joystick movement...
                    final int historySize = event.getHistorySize();
                    // Process the movements starting from the
                    // earliest historical position in the batch
                    for (int i = 0; i < historySize; i++) {
                        // Process the event at historical position i
                        OFAndroidController.processJoystickInput(event, i);
                    }
                    OFAndroidController.processJoystickInput(event, -1);
                    return true;
                }

            }
            if ((event.getSource() & InputDevice.SOURCE_DPAD) == InputDevice.SOURCE_DPAD ||
                    (event.getSource() & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD ||
                    (event.getSource() & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK ||
                    (event.getSource() & InputDevice.SOURCE_CLASS_BUTTON) == InputDevice.SOURCE_CLASS_BUTTON ||
                    (event.getSource() & InputDevice.SOURCE_DPAD) > InputDevice.SOURCE_DPAD - 13 && (event.getSource() & InputDevice.SOURCE_DPAD) < InputDevice.SOURCE_GAMEPAD + 500
            ) {
                // If the input event is a MotionEvent, check its hat axis values.
                directionPressed = -1;
//                if ((InputEvent) event instanceof KeyEvent) {    // If the input event is a KeyEvent, check its key code.
//
//                    // Use the key code to find the D-pad direction.
//                    KeyEvent keyEvent = (KeyEvent) (InputEvent) event;
//                    boolean isKeyDown = event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS;
//                    if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_LEFT) {
//                        directionPressed = DPAD_LEFT;
//                        keyCode = KeyEvent.KEYCODE_DPAD_LEFT;
//                        DPAD_LEFT_DOWN = isKeyDown;
//                    } else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_RIGHT) {
//                        directionPressed = DPAD_RIGHT;
//                        keyCode = KeyEvent.KEYCODE_DPAD_RIGHT;
//                        DPAD_RIGHT_DOWN = isKeyDown;
//                    } else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_UP) {
//                        directionPressed = DPAD_UP;
//                        keyCode = KeyEvent.KEYCODE_DPAD_UP;
//                        DPAD_UP_DOWN = isKeyDown;
//                    } else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_DOWN) {
//                        directionPressed = DPAD_DOWN;
//                        keyCode = KeyEvent.KEYCODE_DPAD_DOWN;
//                        DPAD_DOWN_DOWN = isKeyDown;
//                    } else if (keyEvent.getKeyCode() == KeyEvent.KEYCODE_DPAD_CENTER) {
//                        directionPressed = DPAD_CENTER;
//                        keyCode = KeyEvent.KEYCODE_DPAD_CENTER;
//                        DPAD_CENTER_DOWN = isKeyDown;
//                    }
//                    if (event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS) {
//                        return OFAndroid.keyDown(keyCode + 400, keyCode);
//                    }
//                    else if (event.getActionMasked() == MotionEvent.ACTION_BUTTON_RELEASE || event.getActionMasked() == MotionEvent.ACTION_CANCEL) {
//                        return OFAndroid.keyUp(keyCode + 400, keyCode);
//                    }
//                    else {
//                        return true;
//                    }
//                } else

                 if (event instanceof MotionEvent) {

                    // Use the hat axis value to find the D-pad direction
                    MotionEvent motionEvent = (MotionEvent) event;
                    Log.i("OF", "onGenericMotionEvent motionEvent:" + motionEvent);
                    float xaxis = motionEvent.getAxisValue(MotionEvent.AXIS_HAT_X);
                    float yaxis = motionEvent.getAxisValue(MotionEvent.AXIS_HAT_Y);
                    boolean isKeyDown = true;
                    // Check if the AXIS_HAT_X value is -1 or 1, and set the D-pad
                    // LEFT and RIGHT direction accordingly.
                    if (Float.compare(xaxis, -1.0f) == 0) {
                        directionPressed = DPAD_LEFT;
                        keyCode = KeyEvent.KEYCODE_DPAD_LEFT;
                    } else if (Float.compare(xaxis, 1.0f) == 0) {
                        directionPressed = DPAD_RIGHT;
                        keyCode = KeyEvent.KEYCODE_DPAD_RIGHT;
                    }
                    // Check if the AXIS_HAT_Y value is -1 or 1, and set the D-pad
                    // UP and DOWN direction accordingly.
                    else if (Float.compare(yaxis, -1.0f) == 0) {
                        directionPressed = DPAD_UP;
                        keyCode = KeyEvent.KEYCODE_DPAD_UP;
                    } else if (Float.compare(yaxis, 1.0f) == 0) {
                        directionPressed = DPAD_DOWN;
                        keyCode = KeyEvent.KEYCODE_DPAD_DOWN;
                    } else if(Float.compare(xaxis, 0.0f) == 0 &&  Float.compare(yaxis, 0.0f) == 0) {
                        boolean value = true;
                        if(keyCode != -10)
                            value = OFAndroid.keyUp(keyCode + CONTROLLER_OFFSET, keyCode);
                        keyCode = -1; lastKeyCode = -1;
                        return value;
                    }
                    if (event.getActionMasked() == MotionEvent.ACTION_MOVE || event.getActionMasked() == MotionEvent.ACTION_BUTTON_PRESS) {
                        return OFAndroid.keyDown(keyCode + CONTROLLER_OFFSET, keyCode);
                    }
                    else if (event.getActionMasked() == MotionEvent.ACTION_BUTTON_RELEASE || event.getActionMasked() == MotionEvent.ACTION_CANCEL) {
                        boolean value = OFAndroid.keyUp(keyCode + CONTROLLER_OFFSET, keyCode);
                        keyCode = -1;
                        return value;
                    }
                    else
                        return true;

                }
            } else if (event.isFromSource(InputDevice.SOURCE_CLASS_POINTER)) { // PS4 Touch Pad
                switch (event.getAction()) {
                    case MotionEvent.ACTION_HOVER_MOVE:
                        if(OFAndroid.gestureListener != null) return OFAndroid.gestureListener.onTouch(event);
                        return true;
                    case MotionEvent.ACTION_SCROLL:
                        if(OFAndroid.gestureListener != null) return OFAndroid.gestureListener.onScroll(event);
                        return true;
                }
            }
        } catch(Exception exeception) {
            Log.i("OF", "onGenericMotionEvent Exception:" + exeception.getMessage());
        }
        return true;
    }

    public ArrayList<Integer> getGameControllerIds() {
        ArrayList<Integer> gameControllerDeviceIds = new ArrayList<Integer>();
        int[] deviceIds = InputDevice.getDeviceIds();
        for (int deviceId : deviceIds) {
            InputDevice dev = InputDevice.getDevice(deviceId);
            int sources = dev.getSources();

            // Verify that the device has gamepad buttons, control sticks, or both.
            if (((sources & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD)
                    || ((sources & InputDevice.SOURCE_JOYSTICK)
                    == InputDevice.SOURCE_JOYSTICK)) {
                // This device is a game controller. Store its device ID.
                if (!gameControllerDeviceIds.contains(deviceId)) {
                    gameControllerDeviceIds.add(deviceId);
                }
            }
        }
        return gameControllerDeviceIds;
    }

    public static InputDevice getGameControllerForID(int deviceID) {
        InputDevice dev = InputDevice.getDevice(deviceID);
        return dev;
    }

}
