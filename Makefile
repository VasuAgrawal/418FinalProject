default:
	g++ -std=c++11 -o bst-driver src/bst.cpp src/main.cpp

clean:
	rm -f bst-driver