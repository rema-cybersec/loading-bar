#ifndef LOADING_BAR_H
	#define LOADING_BAR_H

	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include <stdarg.h>

	#define DEFAULT_BAR_SIZE 100
	#define PERCENTAGE_PADDING_LEFT 6
	#define LINE_SPACE_EXTRA 20

	typedef enum BarMode {
		PERCENTAGE_MODE, MANUAL_MODE
	} BarMode;

	typedef struct BarInfo {
		int current;
		int size;
	} BarInfo;

	// either a percentage or a struct
	// note: later we can display
		// [|||--] 60% --> for percentage without data or
		// [|||--] 60% (3/5 downloaded) --> for percentage with data
	typedef union BarData {
		float percentage;
		BarInfo info;
	} BarData;

	typedef struct BarConfig {
		int bar_size;
	} BarConfig;

	// holds info of one of the bars
	// note: BarData isn't a pointer since it doesn't need heap allocation, stack allocation is enough.
	typedef struct Bar {
		int index;
		BarMode mode;
		BarData data;
		// char name[16]; // TODO add a name
		struct Bar *next;
	} Bar;

	// holds info on multiple bars which will render simultaniously (realized with SLL)
	// note: we're also holding count so we can go up and down in the terminal for rerenders
	typedef struct Bars {
		int count;
		BarConfig config;
		Bar *first;
		Bar *last;
	} Bars;

	void bars_config_init(Bars *bars) {
		bars->config.bar_size = DEFAULT_BAR_SIZE;
	}

	// Initialization function for struct Bars
	void bars_init(Bars *bars) {
		bars->count = 0;
		bars->first = NULL;
		bars_config_init(bars);
		bars->last = NULL;
	}

	// Heap version
	Bars *initialize_bars() {
		Bars * bars = (Bars *) malloc(sizeof(Bars));
		bars_init(bars);
		return bars;
	}

	// Free memory
	void bars_destroy(Bars *bars, bool destroySelf) {
		if(bars == NULL) {
			return;
		}
		Bar *curr = bars->first;
		while(curr != NULL) {
			Bar *temp = curr;
			curr = curr->next;
			free((void *) temp);
		}
		if(destroySelf) {
			free((void *) bars);
		}
	}

	void add_bar(Bars *bars, char mode, ...) {
		if(bars == NULL) {
			perror("Cannot add bar to a NULL struct.\n");
			return;
		}
		if(mode != 'p' && mode != 'm') {
			perror("Cannot insert bar of unknown mode. Modes: Percentage (p) or Manual (m)\n");
			return;
		}
		Bar *bar = (Bar *) malloc(sizeof(Bar));
		if(bar == NULL) {
			perror("Cannot insert bar. Memory allocation failed.\n");
			return;
		}
		bar->next = NULL;
		bar->index = bars->count;

		if(bars->first == NULL) {
			bars->first = bar;
		} else {
			bars->last->next = bar;
		}

		bars->last = bar;
		bars->count++;

		// filling data
		va_list(args);
		va_start(args, mode);
		if(mode == 'p') {
			bar->mode = PERCENTAGE_MODE;
			bar->data.percentage = (float) va_arg(args, double);
		} else {
			bar->mode = MANUAL_MODE;
			bar->data.info.current = va_arg(args, int);
			bar->data.info.size = va_arg(args, int);
		}
		va_end(args);
	}

	float bar_get_percentage(Bar *bar)
	{
		if(bar->mode == PERCENTAGE_MODE)
			return bar->data.percentage;
		return (float)bar->data.info.current / bar->data.info.size * 100;
	}

	void print_bar(Bar *bar, int bar_size) {
		float percentage = bar_get_percentage(bar);
	    int filled = (int) (percentage * bar_size / 100);
	    if (filled > bar_size) filled = bar_size;          
	    int empty = bar_size - filled;                     
	    printf("[");
	    for(int i = 0; i < filled; i++)
	        putchar('|');
	    for(int i = 0; i < empty; i++)
	        putchar('-');
	    printf("] %*.2f", PERCENTAGE_PADDING_LEFT, percentage);
	    if(bar->mode != PERCENTAGE_MODE) printf("\t(%d/%d)", bar->data.info.current, bar->data.info.size);
	    printf("\n");
	}

	void print_bars(Bars *bars) {
		if(bars == NULL) {
			perror("Unable to print bars. Bars doesn't exist");
			return;
		}
		Bar *curr = bars->first;
		while(curr != NULL) {
			print_bar(curr, bars->config.bar_size);
			curr = curr->next;
		}
	}

	void update_bars(Bars *bars) {
		if(bars == NULL) {
			perror("Unable to update bars. Bars doesn't exist");
			return;
		}
		printf("\033[%dA", bars->count);
		print_bars(bars);
		fflush(stdout);
	}

	void erase_bars(Bars *bars) {
		printf("\033[%dA", bars->count);
		for(int i = 0; i < bars->count; i++) {
			printf("%*c\n", bars->config.bar_size + LINE_SPACE_EXTRA, ' ');
		}
		printf("\033[%dA", bars->count);
	}

	Bar *get_bar(Bars *bars, int idx) {
		if(bars == NULL) {
			perror("Cannot get bar. Bars is NULL!");
			return NULL;
		}
		if(idx < 0 || idx >= bars->count) return NULL;
		Bar *curr = bars->first;
		for(int i = 0; i < idx; i++) {
			curr = curr->next;
		}
		return curr;
	}

	void update_bar_data(Bars *bars, int idx, ...) {
		if(bars == NULL) {
			return;
		}
		Bar *bar = get_bar(bars, idx);
		if(bar == NULL) {
			perror("Unable to update bar data. Bar doesn't exist!");
		}
		va_list(args);
		va_start(args, idx);
		if(bar->mode == PERCENTAGE_MODE) {
			bar->data.percentage = (float) va_arg(args, double);
		} else {
			bar->data.info.current = va_arg(args, int);
		}
		va_end(args);
	}

	void update_bar(Bars *bars, int idx) {
		if(bars == NULL) {
			perror("Unable to update bar. Cannot access NULL");
			return;
		}
		Bar *bar = get_bar(bars, idx);
		if(bar == NULL) {
			perror("Unable to update bar. Bar doesn't exist!");
		}
		printf("\033[%dA", bars->count - idx);
		print_bar(bar, bars->config.bar_size);
		printf("\033[%dB", bars->count - idx);
	}

#endif