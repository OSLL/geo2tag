package ru.spb.osll.objects;

public class GeoPoint {
	
	private double x;
	private double y;
	
    public GeoPoint() {}

    public GeoPoint(double x, double y) {
        this.x = x;
        this.y = y; 
    }
    
    public GeoPoint(GeoPoint p) { 
        this.x = p.x;
        this.y = p.y;
    }
    
    public final void set(float x, float y) {
        this.x = x;
        this.y = y;
    }
    
    public final void set(GeoPoint p) { 
        this.x = p.x;
        this.y = p.y;
    }
    
    public double x() { 
        return x;
    }
    
    public double y() { 
        return y;
    }

    public final boolean equals(float x, float y) { 
        return this.x == x && this.y == y; 
    }

}
