package ru.spb.osll.web.client;

import ru.spb.osll.web.client.ui.image.ResImages;

import com.google.gwt.resources.client.CssResource;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.resources.client.CssResource.NotStrict;

/**
 * The images and styles used throughout the Showcase.
 */
public interface GTResources extends ResImages {
	ImageResource geoLogo();

	ImageResource locale();

	@NotStrict
	@Source("GT.css")
	CssResource css();
}