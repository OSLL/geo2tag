package ru.spb.osll.GDS.maps;

import ru.spb.osll.objects.Mark;

import com.google.android.maps.GeoPoint;
import com.google.android.maps.OverlayItem;

public class MarkOverlayItem extends OverlayItem {
	
	private Mark m_mark;

	public MarkOverlayItem(Mark mark, String title, String snippet) {
		super(new GeoPoint((int) Math.round(mark.getLatitude() * 1E6),
						   (int) Math.round(mark.getLongitude() * 1E6)),
			  title, snippet);
		
		m_mark = mark;

	}
	
	public Mark getMark() {
		return m_mark;
	}

}
