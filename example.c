#include "loading_bar.h"
#include <stdlib.h>
#include <string.h>

void test_heap();
void test_stack();

int main() {

	// test_heap();
	// test_stack();

	return 0;
}

void test_stack() {
	Bars loading_bar_object;
	Bars *loading_bar = &loading_bar_object;
	bars_init(loading_bar);
	loading_bar->config.bar_size = 100;

	// printf("Size: %ld", sizeof(Bar));

	int a = 0, b = 0, c = 0, d = 0, e = 232;

	add_bar(loading_bar, 'p', a);
	add_bar(loading_bar, 'p', b);
	add_bar(loading_bar, 'p', c);
	add_bar(loading_bar, 'm', d, e);
	add_bar(loading_bar, 'p', b);
	add_bar(loading_bar, 'p', a);
	// add_bar(loading_bar, 'm', b, e);

	print_bars(loading_bar);
	for(int i = 0; i < 1000000; i++) {
		if(b < 100 && i % 3789 == 0) {
			b++;
		}
		if(a < 100 && i % 4520 == 0) {
			a++;
		}
		if(d < e && i % 2121 == 0) {
			d++;
		}
		if(c < 100 && i % 6121 == 0) {
			c++;
		}

		if(i % 20 == 0) {
			update_bar_data(loading_bar, 0, a/1.0);
			update_bar_data(loading_bar, 1, b/1.0);
			update_bar_data(loading_bar, 2, c/1.0);
			update_bar_data(loading_bar, 3, d);
			update_bar_data(loading_bar, 4, b);
			update_bar_data(loading_bar, 5, a);
			// update_bar_data(loading_bar, 6, b > e ? e : b*2);
			update_bars(loading_bar);
			if(a == 100 && b == 100 && c == 100 && d == e) {
				break;
			}
		}
	}

	// erase_bars(loading_bar);
	bars_destroy(loading_bar, false);
}

void test_heap() {
	Bars *loading_bar = initialize_bars();
	loading_bar->config.bar_size = 100;

	// printf("Size: %ld", sizeof(Bar));

	int a = 0, b = 0, c = 0, d = 0, e = 232;

	add_bar(loading_bar, 'p', a);
	add_bar(loading_bar, 'p', b);
	add_bar(loading_bar, 'p', c);
	add_bar(loading_bar, 'm', d, e);
	add_bar(loading_bar, 'p', b);
	add_bar(loading_bar, 'p', a);
	// add_bar(loading_bar, 'm', b, e);

	print_bars(loading_bar);
	for(int i = 0; i < 1000000; i++) {
		if(b < 100 && i % 3789 == 0) {
			b++;
		}
		if(a < 100 && i % 4520 == 0) {
			a++;
		}
		if(d < e && i % 2121 == 0) {
			d++;
		}
		if(c < 100 && i % 6121 == 0) {
			c++;
		}

		if(i % 20 == 0) {
			update_bar_data(loading_bar, 0, a/1.0);
			update_bar_data(loading_bar, 1, b/1.0);
			update_bar_data(loading_bar, 2, c/1.0);
			update_bar_data(loading_bar, 3, d);
			update_bar_data(loading_bar, 4, b);
			update_bar_data(loading_bar, 5, a);
			// update_bar_data(loading_bar, 6, b > e ? e : b*2);
			update_bars(loading_bar);
			if(a == 100 && b == 100 && c == 100 && d == e) {
				break;
			}
		}
	}

	// erase_bars(loading_bar);
	bars_destroy(loading_bar, true);
}