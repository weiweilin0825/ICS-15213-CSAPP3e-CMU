#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <inttypes.h>
#include "cachelab.h"
#define cmd_length 20

int hit_cnt = 0;
int miss_cnt = 0;
int eviction_cnt = 0;
int s, e, b;
int verbose = 0;

typedef struct che{
    int valid, tag, call;
}CacheLine;

CacheLine **create_cache(int s, int e){
    int s_size = 1 << s;
    CacheLine **cache = (CacheLine **) malloc(sizeof(CacheLine *) * s_size);
    for (int i = 0; i < s_size; i ++) {
        cache[i] = (CacheLine *)malloc(sizeof(CacheLine) * e);
    }
    for (int i = 0; i < s_size; i ++) {
        for (int j = 0; j < e; j ++) {
            cache[i][j].valid = 0;
            cache[i][j].call = cache[i][j].tag = -1;
        }
    }
    return cache;
}

/*
 * evict: LRU algo to replace the lest recently used one
 */
void evict(CacheLine **cache, int set, int tag) {
    if(verbose)
        printf("eviction ");
    eviction_cnt ++;
    int max_call = -2, max_call_line = -1;
    for (int i = 0; i < e; i ++) {
        if (max_call < cache[set][i].call) {
            max_call = cache[set][i].call;
            max_call_line = i;
        }
    }
    cache[set][max_call_line].tag = tag;
    cache[set][max_call_line].call = -1;
    cache[set][max_call_line].valid = 1;
}

/*
 * hit:hit count ++ 
 */
void hit(){
    if (verbose)
        printf("hit ");
    hit_cnt ++;
}

/*
 * miss: miss count ++ and store the tag into cache
 */
void miss(CacheLine **cache, int set, int tag) {
    if (verbose)
        printf("miss ");
    miss_cnt ++;
    for (int i = 0; i < e; i ++) {
        if (cache[set][i].valid == 0) {
            cache[set][i].valid = 1;
            cache[set][i].tag = tag;
            cache[set][i].call = -1;
            return;
        }
    }
    evict(cache, set, tag);
}

/*
 * store the value into cache, operate like load
 */

void store(CacheLine **cache, unsigned long address){
    int set = (address >> b) & (-1U >> (32 - s));
    int tag = address >> (s + b);
    for (int i = 0; i < e; i ++) {
        if (cache[set][i].tag == tag && cache[set][i].valid) {
            cache[set][i].call = -1;
            hit();
            return;
        }
    }
    miss(cache, set, tag);
    return;
}

void load(CacheLine **cache, unsigned long address){
    int set = (address >> b) & (-1U >> (32 - s));
    int tag = address >> (s + b);
    for (int i = 0; i < e; i ++) {
        if (cache[set][i].tag == tag && cache[set][i].valid) {
            cache[set][i].call = -1;
            hit();
            return;
        }
    }
    miss(cache, set, tag);
    return;
}

/*
 *modify the cache
 */
void modify(CacheLine **cache, unsigned long address){
    load(cache, address);
    store(cache, address);
}

/*
 * Increase the count of every line's last time call
 */
void call_count_incre(CacheLine **cache) {
    int set_cnt = 1 << s;
    for (int i = 0; i < set_cnt; i ++) {
        for (int j = 0; j < e; j ++) {
            cache[i][j].call ++;
        }
    }
}

/*
 *read the trace from trace file and operate cache
 *count the hits, misses, eviction in cnt array
 */

void trace_read(CacheLine **cache, char *trace_loc, const int verbose) {
    char op;
    int size;
    unsigned long address;
    char cmd[cmd_length];
    FILE *trace = fopen(trace_loc, "r");
    while (fgets(cmd, cmd_length, trace) != NULL) {
        sscanf(cmd," %c %lx,%d", &op, &address, &size);
        if (verbose)
            printf("%c %lx,%d ", op, address, size);
        switch (op) {
            case 'M':
                modify(cache, address);
                break;
            case 'L':
                load(cache, address);
                break;
            case 'S':
                store(cache, address);
                break;
        }
        if (verbose)
            printf("\n");
        call_count_incre(cache);
    }
    fclose(trace);
}

int main(int argc, char *argv[]) {   
    int cmd_opt = 0;
    char *trace_loc;
    //parse, -h and -v are optional argument
    while((cmd_opt = getopt(argc, argv, "vs:E:b:t:")) != -1) {
        switch (cmd_opt) {
            // Optional options
            case 'v':
                verbose = 1;
                break;
            // Necessary options
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                e = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_loc = optarg;
        }
    }
    int s_size = 1 << s;
    CacheLine **cache = create_cache(s, e);
    trace_read(cache, trace_loc, verbose);
    for (int i = 0; i < s_size; i ++)
        free(cache[i]);
    free(cache);
    printSummary(hit_cnt, miss_cnt, eviction_cnt);
    return 0;
}
