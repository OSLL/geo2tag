package ru.spb.osll.exception;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.Thread.UncaughtExceptionHandler;

import android.content.Context;
import android.content.Intent;
import android.os.Process;

public class ExceptionHandler implements UncaughtExceptionHandler {
	private final Context m_context;

	public ExceptionHandler(Context context) {
		m_context = context;
	}

	public void uncaughtException(Thread thread, Throwable exception) {
		StringWriter stackTrace = new StringWriter();
		exception.printStackTrace(new PrintWriter(stackTrace));
		System.err.println(stackTrace);

		Intent intent = new Intent(m_context, ExceptionActivity.class);
		intent.putExtra(ExceptionActivity.STACKTRACE, stackTrace.toString());
		m_context.startActivity(intent);

		Process.killProcess(Process.myPid());
		System.exit(10);
	}
	
}
