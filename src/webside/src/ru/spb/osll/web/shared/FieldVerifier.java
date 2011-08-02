package ru.spb.osll.web.shared;

public class FieldVerifier {

	public static boolean isValidName(String name) {
		if (name == null) {
			return false;
		}
		return name.length() > 3;
	}
	
}
