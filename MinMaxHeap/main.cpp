/*
	PA 3-4 mooc Min-Max Heap Final Version 2015-12-19
	By Chen Yazheng
	Reference: 
    M. D. Atkinson, J.R. Sack, N. Santoro, and T. Strothotte, Communications of the ACM, October, 1986, Min-Max Heaps and Generalized Priority Queues.

*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;


const int MAXN = int(1e5) << 1 | 63;
const int MAXP = int(1e9) << 1;
const unsigned int INF = 0xffffffff;

void init_IO(int argc, char const *argv[]) {
	//accelerate I/O
    const int BUF_SIZE = 1 << 20;
    setvbuf(stdin , new char[BUF_SIZE], _IOFBF, BUF_SIZE);
    setvbuf(stdout, new char[BUF_SIZE], _IOFBF, BUF_SIZE);

#ifdef _SAMPLE_
	// local test
	if (argc > 1) { // input data file exists ?
		FILE *fin;
		if ((fin = fopen(argv[1], "r")) != NULL) {
			fclose(fin);
			freopen(argv[1], "r", stdin);
			freopen("output.txt", "w", stdout);
		}
	} else { // default data file
		freopen("sample.in", "r", stdin);
		freopen("sample.out", "w", stdout);	
	}
#endif
}

void swap(int &a, int &b) {
	int c = a; a = b; b = c;
}

const int DEFAULTSIZE = MAXN | 31;
typedef int Position;
class MinMaxHeap {
public:
	MinMaxHeap(int size = DEFAULTSIZE) {
		t = new int[size];
		last = 0;
	}

	void display() {
		for (int i = 1; i <= last; ++i) {
			printf("%d ", t[i]);
		}
		putchar('\n');
	}

	void insert(int);

	int getMax() {
		if (last == 0) return 0;
		int maxp = maxChild(1);
		if (maxp) return t[maxp];
		else return t[1];
	}

	int getMin() {
		if (last == 0) return 0;
		return t[1];
	}

	int deleteMax();
	int deleteMin();

	void heapify();

private:
	bool onMinLevel(Position);

	void SiftDown   (Position);
	void SiftDownMin(Position);
	void SiftDownMax(Position);

	void BubbleUp   (Position);
	void BubbleUpMin(Position);
	void BubbleUpMax(Position);

	Position minChild(Position);
	Position maxChild(Position);
	Position minChildOrGrandchild(Position);
	Position maxChildOrGrandchild(Position);

	int last;
	int *t;
};

void MinMaxHeap::insert(int v) {
	t[++last] = v;
	BubbleUp(last);
}

void MinMaxHeap::heapify() {
	assert(last < MAXN);
	for(int i = last; i >= 1; --i) {
		SiftDown(i);
	}
}

int MinMaxHeap::deleteMin() {
	if (last) {
		int min = t[1];
		if (last > 1) {
			t[1] = t[last--];
			SiftDown(1);
		} else {
			last = 0;
		}
		return min;	
	} else {
		return 0;
	}
}

int MinMaxHeap::deleteMax() {
	if (last == 0) return 0;
	if (last == 1) return deleteMin();
	// last > 1
	int maxp = maxChild(1);
	int max = t[maxp];
	if (last != maxp) {
		t[maxp] = t[last--];
		SiftDown(maxp);
	} else { // maxp == last
		last--;
	}
	return max;
}

bool MinMaxHeap::onMinLevel(Position p) {
	if (p == 1) return true;
	bool minLevel = true;
	for (; p > 1; minLevel = !minLevel, p >>= 1);
	return minLevel;
}

void MinMaxHeap::SiftDown(Position p) {
	if (onMinLevel(p))
		SiftDownMin(p);
	else 
		SiftDownMax(p);
}

void MinMaxHeap::SiftDownMin(Position p) {
	Position minp = minChildOrGrandchild(p);
	if (minp && minp != p) {
		if (minp == p << 1 || minp == ((p << 1) | 1)) { // minp is a child of p
			if (t[minp] < t[p]) {
				swap(t[minp], t[p]);
			}
		} else {// minp is a grandchild of p, isGrandChild(minp, p)
			if (t[minp] < t[p]) {
				swap(t[minp], t[p]);
				if (t[minp] > t[minp >> 1]) {
					swap(t[minp], t[minp >> 1]);
				}
				SiftDownMin(minp);
			}
		}
	}
}

void MinMaxHeap::SiftDownMax(Position p) {
	Position maxp = maxChildOrGrandchild(p);
	if (maxp && maxp != p) {
		if ( maxp == p << 1 || maxp == ((p << 1) | 1) ) { // maxp is a child of p
			if (t[maxp] > t[p]) {
				swap(t[maxp], t[p]);
			} 
		} else { // grandchild
				if (t[maxp] > t[p]) {
					swap(t[maxp], t[p]);
					if (t[maxp] < t[maxp >> 1]) {
						swap(t[maxp], t[maxp >> 1]);
					}
					SiftDownMax(maxp);
				}
			}
	}
}

void MinMaxHeap::BubbleUp(Position p) {
	if (p == 1) return;
	if (onMinLevel(p)) {
		if (p >> 1 && t[p] > t[p>>1]) {
			swap(t[p], t[p>>1]);
			BubbleUpMax(p>>1);
		} else {
			BubbleUpMin(p);
		}
	} else { // on maxLevel
		if (p >> 1 && t[p] < t[p>>1]) {
			swap(t[p], t[p>>1]);
			BubbleUpMin(p>>1);
		} else {
			BubbleUpMax(p);
		}
	}
}

void MinMaxHeap::BubbleUpMin(Position p) {
	int gp = p >> 2;	// grandparent
	while (gp && t[p] < t[gp]) {
		swap(t[p], t[gp]);
		p = gp;
		gp = p >> 2;
	}
}

void MinMaxHeap::BubbleUpMax(Position p) {
	int gp = p >> 2;
	while (gp && t[p] > t[gp]) {
		swap(t[p], t[gp]);
		p = gp;
		gp = p >> 2;
	}
}

Position MinMaxHeap::minChild(Position p) {
	int minp = 0;
	if (p << 1 <= last) {
		minp = p << 1;
		if ( ((p<<1)|1) <= last && t[(p<<1)|1] < t[minp] ) {
			minp = (p<<1)|1;
		}
		return minp;
	} // else (p << 1) > last
	return 0;
}

Position MinMaxHeap::maxChild(Position p) {
	int maxp = 0;
	if (p << 1 <= last) {
		maxp = p << 1;
		if ( ((p<<1)|1) <= last && t[(p<<1)|1] >= t[maxp] ) {
			maxp = (p<<1)|1;
		}
		return maxp;
	} // else no children
	return 0;
}

Position MinMaxHeap::minChildOrGrandchild(Position p) {
	int minp = minChild(p);
	if (minp) {
		int lminp = minChild(p<<1);
		int rminp = minChild((p<<1)|1);
		if (lminp && t[lminp] < t[minp]) {
			minp = lminp;
		}
		if (rminp && t[rminp] < t[minp]) {
			minp = rminp;
		}
		return minp;
	} // else 
	return 0;
}

Position MinMaxHeap::maxChildOrGrandchild(Position p) {
	int maxp = maxChild(p);
	if (maxp) {
		int lmaxp = maxChild(p<<1);
		int rmaxp = maxChild((p<<1)|1);
		if (lmaxp && t[lmaxp] > t[maxp]) {
			maxp = lmaxp;
		}
		if (rmaxp && t[rmaxp] > t[maxp]) {
			maxp = rmaxp;
		}
		return maxp;
	} // else 
	return 0;
}

void test() {
	MinMaxHeap t;
	int m;
	scanf("%d\n", &m);
	for (int i = 0; i < m; ++i)//while (1)
	{
		char T;
		int pts;		
		int nval = 0;

		if ((nval = scanf("%c%d\n", &T, &pts)) == 2) {
			t.insert(pts);
		} else if (nval == 1) {
			switch (T) {
				case 'H': printf("%d\n", t.deleteMax());
				break;
				case 'L': printf("%d\n", t.deleteMin());
				break;
				case 'D': t.display();
				break;
				default: t.display();
				break;
			}
		} else {
			break;
		}
	}
}

MinMaxHeap h;
int  work() {
	int n;
	scanf("%d\n", &n);
	for (int i = 0; i < n; ++i)
	{
		char T;
		int pts;
		int nval = 0;
		if ((nval = scanf("%c%d\n", &T, &pts)) == 2) {
			h.insert(pts);
		} else if (nval == 1) {
			switch (T) {
				case 'H': printf("%d\n", h.deleteMax());
				break;
				case 'L': printf("%d\n", h.deleteMin());
				break;
				case 'D': h.display();
				break;
				default:  return 256; // t.display();
				break;
			}
		} else return 256;
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	init_IO(argc, argv);
	
#ifdef _TEST_
	test();
#endif
	return work();
}