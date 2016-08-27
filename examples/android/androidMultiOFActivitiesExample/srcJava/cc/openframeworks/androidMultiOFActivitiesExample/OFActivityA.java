package cc.openframeworks.androidMultiOFActivitiesExample;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;
import cc.openframeworks.OFAndroid;
import cc.openframeworks.androidMultiOFActivitiesExample.R.id;


public class OFActivityA extends cc.openframeworks.OFActivity{

	@Override
    public void onCreate(Bundle savedInstanceState)
    { 
        super.onCreate(savedInstanceState);
        Button btn = new Button(this);
        btn.setText("Go To Activity B");
        btn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				startActivity(new Intent(OFActivityA.this, OFActivityB.class));
			}
		});
        RelativeLayout rootView = (RelativeLayout)findViewById(id.relativeLayout1);
        LayoutParams params = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        params.addRule(RelativeLayout.CENTER_HORIZONTAL);
        params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        rootView.addView(btn, params);
        Log.d("OFActivityA", "onCreate");
    }
	
}



