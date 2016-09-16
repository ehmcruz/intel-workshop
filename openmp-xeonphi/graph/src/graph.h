#ifndef _SUM_H_
#define _SUM_H_

#include <stdint.h>

struct vertex_t;

typedef struct edge_t {
	struct vertex_t *src;
	struct vertex_t *dest;
} edge_t;

typedef struct vertex_t {
	uint32_t value;
	
	uint32_t pos; // pos in graph.vertices
	
	edge_t **linked;
	uint32_t arity;
} vertex_t;

typedef struct graph_t {
	vertex_t *vertices;
	edge_t *edges;
	vertex_t *tree_root; // in case it is a tree
	
	uint32_t n_vertices;
	uint32_t n_edges;
	
	// internal use only
	vertex_t *next_vertex;
	edge_t *next_edge;
	uint32_t used_vertices;
	uint32_t used_edges;
} graph_t;

void graph_init (graph_t *g, uint32_t n_vertices, uint32_t n_edges);
void graph_destroy (graph_t *g);
vertex_t* get_free_vertex(graph_t *g);
edge_t* get_free_edge(graph_t *g);
edge_t* graph_connect_vertices(graph_t *g, vertex_t *src, vertex_t *dest);

void create_tree (uint32_t arity, uint32_t nlevels);
uint32_t touch_all_nodes ();

#endif
