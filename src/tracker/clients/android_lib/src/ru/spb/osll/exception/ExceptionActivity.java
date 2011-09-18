package ru.spb.osll.exception;

import ru.spb.osll.R;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.TextView;

public class ExceptionActivity extends Activity{
	public static String STACKTRACE = "stacktrace";
	
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		setContentView(R.layout.exception_view);
		final String stackTrace = getIntent().getStringExtra(STACKTRACE);
		final TextView exeptionTextView = (TextView)findViewById(R.id.exeption_text);
		
		exeptionTextView.setMovementMethod(ScrollingMovementMethod.getInstance());
		exeptionTextView.setClickable(false);
		exeptionTextView.setLongClickable(false);

		exeptionTextView.append("Tracker has been crached!");
		exeptionTextView.append(stackTrace);
		
		findViewById(R.id.send).setOnClickListener(
			new View.OnClickListener() {
				public void onClick(View view) {
					Intent sendIntent = new Intent(Intent.ACTION_SEND);
					sendIntent.putExtra(Intent.EXTRA_EMAIL, new String[] { "bac1ca89@gmail.com" });
					sendIntent.putExtra(Intent.EXTRA_TEXT, stackTrace);
					sendIntent.putExtra(Intent.EXTRA_SUBJECT, "Tracker exception report");
					sendIntent.setType("message/rfc822");
					startActivity(sendIntent);
					finish();
				}
			}
		);

		findViewById(R.id.cancel).setOnClickListener(
			new View.OnClickListener() {
				public void onClick(View view) {
					finish();
				}
			}
		);
	}

	
}
