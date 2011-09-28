package ru.spb.osll.airomo;

import java.util.LinkedList;
import java.util.List;

class Buffer<T> {
	private int m_bufferSize = 50;
	LinkedList<T> m_queue = new LinkedList<T>();

	public Buffer(int bufferSize){
		m_bufferSize = bufferSize;
	}
	
	public void setBufferSize(int bufferSize){
		if (bufferSize > 200){
			m_bufferSize = 200;
		} else if (bufferSize < 1){
			m_bufferSize = 1;
		} else {
			m_bufferSize = bufferSize;
		}
		int removeCount = getCount() - m_bufferSize;
		for (int i = 0; i < removeCount; i++){
			m_queue.poll();
		}
	}
	
	public int getBufferSize(){
		return m_bufferSize;
	}

	public void add(T e) {
		m_queue.add(e); 
		if (m_queue.size() > m_bufferSize){
			m_queue.poll();
		}
	}
	
	public T getFirst(){
		if (hasElements()){
			return m_queue.get(0);
		} 
		return null;
	}

	public void removeFirst(){
		if (hasElements()){
			m_queue.remove(0);
		} 
	}
	
	public boolean hasElements(){
		return getCount() > 0;
	}
	
	public int getCount(){
		return m_queue.size();
	}
	
	List<T> getBufferData(){
		return m_queue;
	}
	
	public void clear(){
		m_queue.clear();
	}
}