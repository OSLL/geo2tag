package ru.spb.osll.GDS.maps;

import java.util.ArrayList;

import ru.spb.osll.GDS.GDSUtil;
import ru.spb.osll.objects.Mark;

import android.app.AlertDialog;
import android.content.Context;
import android.graphics.drawable.Drawable;

import com.google.android.maps.ItemizedOverlay;
import com.google.android.maps.OverlayItem;

public class EventsItemizedOverlay extends ItemizedOverlay<OverlayItem> {
	
	private ArrayList<MarkOverlayItem> m_items = new ArrayList<MarkOverlayItem>();
	private Context m_context;
	
	public EventsItemizedOverlay(Drawable defaultMarker) {
		super(boundCenter(defaultMarker));
		populate();
	}
	
	public EventsItemizedOverlay(Drawable defaultMarker, Context context) {
		  super(boundCenter(defaultMarker));
		  m_context = context;
		  populate();
	}

	@Override
	protected OverlayItem createItem(int i) {
		return m_items.get(i);
	}

	@Override
	public int size() {
		return m_items.size();
	}
	
	@Override
	protected boolean onTap(int index) {
		MarkOverlayItem item = m_items.get(index);
		AlertDialog.Builder dialog = new AlertDialog.Builder(m_context);
		dialog.setTitle(item.getMark().getTitle());
		dialog.setMessage(item.getMark().getUser() + " ("
				+ GDSUtil.getTimeFromUtcString(item.getMark().getTime()) + "):\n"
				+ item.getMark().getDescription());
		dialog.show();
	return true;
	}
	
	public void setEvents(Mark[] marks) {
		m_items.clear();
		for (Mark mark : marks) {
			m_items.add(new MarkOverlayItem(mark, "Event", ""));
		}
		populate();
	}
	
	public void addOverlay(MarkOverlayItem overlay) {
	    m_items.add(overlay);
	    populate();
	}

}
