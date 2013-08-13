CXXFLAGS=-std=c++11 -Wall -Werror -O2 -g

all: wavelet_tree_test

wavelet_tree_test: wavelet_tree.o

check: wavelet_tree_test
	./$<

clean:
	$(RM) *.o wavelet_tree_test
