#default:
	#g++ -std=c++11 -o bst-driver src/bst.cpp src/main.cpp

coarse:
	g++ -std=c++11 -o coarse src/coarse_grained_bst.cpp src/main.cpp -D COARSE

clean:
	rm -f bst-driver
