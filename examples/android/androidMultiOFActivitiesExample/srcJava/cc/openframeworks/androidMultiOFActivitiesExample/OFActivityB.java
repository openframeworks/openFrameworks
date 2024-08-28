package cc.openframeworks.androidMultiOFActivitiesExample;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;

public class OFActivityB extends cc.openframeworks.OFActivity{

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Button btn = new Button(this);
        btn.setText("Go To Activity A");
        btn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                startActivity(new Intent(OFActivityB.this, OFActivityA.class));
            }
        });
        RelativeLayout rootView = (RelativeLayout)findViewById(R.id.relativeLayout1);
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        params.addRule(RelativeLayout.CENTER_HORIZONTAL);
        params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        rootView.addView(btn, params);
        Log.d("OFActivityB", "onCreate");
    }
}
