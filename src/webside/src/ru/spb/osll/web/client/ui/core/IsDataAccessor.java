package ru.spb.osll.web.client.ui.core;

public interface IsDataAccessor<T> {
	String extractData(T object);
}
