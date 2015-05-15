package cc.openframeworks;

import android.app.Activity;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.ScaleGestureDetector.OnScaleGestureListener;
import android.view.View;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.View.OnClickListener;

class OFGestureListener extends SimpleOnGestureListener implements OnClickListener, OnScaleGestureListener {
	

	OFGestureListener(Activity activity){
		gestureDetector = new GestureDetector(activity,this);
		scaleDetector = new ScaleGestureDetector(activity, this);
        touchListener = new View.OnTouchListener() {
        	
            public boolean onTouch(View v, MotionEvent event) {
            	final int action = event.getAction();
            	final int pointerIndex = (action & MotionEvent.ACTION_POINTER_INDEX_MASK) 
                >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
                final int pointerId = event.getPointerId(pointerIndex);
                switch((action & MotionEvent.ACTION_MASK)){
                case MotionEvent.ACTION_MOVE:
                {
            		for(int i=0; i<event.getHistorySize(); i++)
            		{            			
		                for(int j=0; j<event.getPointerCount(); j++){	                			
	                		OFAndroid.onTouchMoved(event.getPointerId(j), event.getHistoricalX(j, i), event.getHistoricalY(j, i), event.getHistoricalPressure(j, i), event.getHistoricalTouchMajor(j, i), event.getHistoricalTouchMinor(j, i), event.getHistoricalOrientation(j, i));
	                	}            			
                	}
	            	for(int i=0; i<event.getPointerCount(); i++){
	            		OFAndroid.onTouchMoved(event.getPointerId(i), event.getX(i), event.getY(i), event.getPressure(i), event.getTouchMajor(i), event.getTouchMinor(i), event.getOrientation(i));
	            	}
                }
	            	break;
                case MotionEvent.ACTION_POINTER_UP:
                case MotionEvent.ACTION_UP:
                	OFAndroid.onTouchUp(pointerId, event.getX(pointerIndex), event.getY(pointerIndex), event.getPressure(pointerIndex), event.getTouchMajor(pointerIndex), event.getTouchMinor(pointerIndex), event.getOrientation(pointerIndex));
                	break;
                case MotionEvent.ACTION_POINTER_DOWN:
                case MotionEvent.ACTION_DOWN:
                	OFAndroid.onTouchDown(pointerId, event.getX(pointerIndex), event.getY(pointerIndex), event.getPressure(pointerIndex), event.getTouchMajor(pointerIndex), event.getTouchMinor(pointerIndex), event.getOrientation(pointerIndex));
                	break;
                case MotionEvent.ACTION_CANCEL:
                	OFAndroid.onTouchCancelled(pointerId,event.getX(),event.getY());
                	break;
                }
                return gestureDetector.onTouchEvent(event) || scaleDetector.onTouchEvent(event);
            }
            
        };
	}
	
	// Gesture listener
	public void onClick(View view) {
	}

	@Override
	public boolean onDoubleTap(MotionEvent event) {
		final int action = event.getAction();
		final int pointerIndex = (action & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
        final int pointerId = event.getPointerId(pointerIndex);

        OFAndroid.onTouchDoubleTap(pointerId, event.getX(pointerIndex), event.getY(pointerIndex), event.getPressure(pointerIndex));

		return true;
		//return super.onDoubleTap(e);
	}
	
	@Override
	public boolean onDoubleTapEvent(MotionEvent event) {
		return super.onDoubleTapEvent(event);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent event) {
		return super.onSingleTapConfirmed(event);
	}

	@Override
	public boolean onDown(MotionEvent event) {
		return true;
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
		/*boolean res = super.onFling(e1, e2, velocityX, velocityY);
		return res;*/
		if(e1==null || e2==null) return false;
		
		final float xDistance = Math.abs(e1.getX() - e2.getX());
		final float yDistance = Math.abs(e1.getY() - e2.getY());

		if(xDistance > OFGestureListener.swipe_Max_Distance || yDistance > OFGestureListener.swipe_Max_Distance)
			return false;

		velocityX = Math.abs(velocityX);
		velocityY = Math.abs(velocityY);
        boolean result = false;

        if(velocityX > OFGestureListener.swipe_Min_Velocity && xDistance > OFGestureListener.swipe_Min_Distance){
        	if(e1.getX() > e2.getX()) // right to left
        		OFAndroid.onSwipe(e1.getPointerId(0),SWIPE_LEFT);
        	else
        		OFAndroid.onSwipe(e1.getPointerId(0),SWIPE_RIGHT);
   
        	result = true;
        }else if(velocityY > OFGestureListener.swipe_Min_Velocity && yDistance > OFGestureListener.swipe_Min_Distance){
        	if(e1.getY() > e2.getY()) // bottom to up 
        		OFAndroid.onSwipe(e1.getPointerId(0),SWIPE_UP);
        	else
        		OFAndroid.onSwipe(e1.getPointerId(0),SWIPE_DOWN);
   
        	result = true;
        }

        return result;
	}

	@Override
	public void onLongPress(MotionEvent arg0) {
	}

	@Override
	public boolean onScroll(MotionEvent arg0, MotionEvent arg1, float arg2,	float arg3) {
		return super.onScroll(arg0, arg1, arg2, arg3);
	}

	@Override
	public void onShowPress(MotionEvent arg0) {
	}

	@Override
	public boolean onSingleTapUp(MotionEvent event) {
		return super.onSingleTapUp(event);
	}
	
	// Scale listener
	@Override
	public boolean onScale(ScaleGestureDetector detector) {
		return OFAndroid.onScale(detector);
	}

	@Override
	public boolean onScaleBegin(ScaleGestureDetector detector) {
		return OFAndroid.onScaleBegin(detector);
	}

	@Override
	public void onScaleEnd(ScaleGestureDetector detector) {
		OFAndroid.onScaleEnd(detector);
	}

    private GestureDetector gestureDetector;
    private ScaleGestureDetector scaleDetector;
    View.OnTouchListener touchListener;
    public static int swipe_Min_Distance = 100;
    public static int swipe_Max_Distance = 350;
    public static int swipe_Min_Velocity = 100;
    public final static int SWIPE_UP    = 1;
    public final static int SWIPE_DOWN  = 2;
    public final static int SWIPE_LEFT  = 3;
    public final static int SWIPE_RIGHT = 4;
}
