
test:
	g++ main.cpp -lwt -lwthttp -o test

clean:
	-rm *.o
	-rm test

run:
	./test --http-address=0.0.0.0 --http-port=8080 --docroot=.	
