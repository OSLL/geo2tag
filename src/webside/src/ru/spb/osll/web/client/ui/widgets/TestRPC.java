package ru.spb.osll.web.client.ui.widgets;

import ru.spb.osll.web.client.GreetingService;
import ru.spb.osll.web.client.GreetingServiceAsync;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.image.ResImages;

import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.Widget;

public class TestRPC extends SimpleComposite {
	interface Images extends ResImages{
	}

	private final GreetingServiceAsync service = GWT.create(GreetingService.class);

	// Create a handler for the sendButton
	class MyHandler implements ClickHandler {

		public void onClick(ClickEvent event) {
			sendNameToServer();
		}

		private void sendNameToServer() {
			service.greetServer("bac1ca",
					new AsyncCallback<String>() {
						public void onFailure(Throwable caught) {
							Window.alert(caught.getMessage());
						}

						public void onSuccess(String result) {
							Window.alert(result);
						}
					});
		}
	}

	@Override
	protected Widget onInitialize() {
		Button btn = new Button(" Send ");
		btn.addClickHandler(new MyHandler());
		return btn;
	}
	
	@Override
	protected String getName() {
		return "Test GreetingService";
	}

	@Override
	protected ImageResource getImage() {
		Images images = GWT.create(Images.class);
		return images.peek();
	}
}
