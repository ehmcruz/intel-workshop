#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <omp.h>

#include "graph.h"

extern struct timeval xeonphi_time_start, xeonphi_time_end;

void graph_init (graph_t *g, uint32_t n_vertices, uint32_t n_edges)
{
	uint32_t i, j;
	
	printf("total malloc memory: %u MB\n", ( (n_vertices * sizeof(vertex_t)) + (n_edges * sizeof(edge_t)) ) / (1024*1024) );
	
	g->vertices = (vertex_t*)_Offload_shared_malloc(n_vertices * sizeof(vertex_t));
	assert(g->vertices != NULL);
	
	g->edges = (edge_t*)_Offload_shared_malloc(n_edges * sizeof(edge_t));
	assert(g->edges != NULL);
		
	for (i=0; i<n_vertices; i++) {
		g->vertices[i].pos = i;
		g->vertices[i].arity = 0;
		g->vertices[i].linked = NULL;
	}
	
	g->n_vertices = n_vertices;
	g->n_edges = n_edges;
	
	g->next_vertex = g->vertices;
	g->next_edge = g->edges;
	g->used_vertices = 0;
	g->used_edges = 0;
}

void graph_destroy (graph_t *g)
{
	_Offload_shared_free(g->vertices);
	_Offload_shared_free(g->edges);
}

vertex_t* get_free_vertex(graph_t *g)
{
	assert(g->used_vertices < g->n_vertices);
	g->used_vertices++;
	g->next_vertex->arity = 0;
	return g->next_vertex++;
}

edge_t* get_free_edge(graph_t *g)
{
	assert(g->used_edges < g->n_edges);
	g->used_edges++;
	return g->next_edge++;
}

edge_t* graph_connect_vertices(graph_t *g, vertex_t *src, vertex_t *dest)
{
	edge_t *e;
	
	e = get_free_edge(g);
	
/*	e->src = src;*/
	e->dest = dest;
	e->next = src->linked;
	
	src->linked = e;

	src->arity++;
	
	return e;
}

static uint32_t intpow(uint32_t base, uint32_t exp)
{
	uint32_t r = 1, i;
	
	for (i=0; i<exp; i++)
		r *= base;
	
	return r;
}

// --------------------------------------------------------------

_Cilk_shared graph_t tree;

static vertex_t* create_node (int level, int arity)
{
	vertex_t *v, *child;
	int i;
	
	if (level == 0)
		return NULL;
	
	v = get_free_vertex(&tree);
	
	for (i=0; i<arity; i++) {
		child = create_node(level-1, arity);

		if (child != NULL)
			graph_connect_vertices(&tree, v, child);
	}
	
	return v;
}

uint32_t create_tree (uint32_t arity, uint32_t nlevels)
{
	uint32_t n_vertices, n_edges;
	
	n_vertices = (intpow(arity, nlevels) - 1) / (arity - 1);
	n_edges = n_vertices - 1;
	
	printf("arity: %u\nnumber of levels: %u\nnumber of vertices: %u\nnumber of edges: %u\n", arity, nlevels, n_vertices, n_edges);
	
	graph_init(&tree, n_vertices, n_edges);
	printf("tree allocated\n");
	tree.tree_root = create_node(nlevels, arity);
	printf("tree built\n");
	
	return n_vertices;
}

_Cilk_shared uint32_t touch_node_seq (vertex_t *node)
{
	uint32_t n = 1;
	edge_t *e;

#ifndef __MIC__
	printf("we want xeon phi to run this\n");
	exit(1);
#endif
	
	for (e=node->linked; e!=NULL; e=e->next) {
		n += touch_node_seq(e->dest);
	}

	return n;
}

_Cilk_shared uint32_t touch_node_parallel (vertex_t *node)
{
	uint32_t n = 1;
	edge_t *e;

#ifndef __MIC__
	printf("we want xeon phi to run this\n");
	exit(1);
#endif
	
	for (e=node->linked; e!=NULL; e=e->next) {
		#pragma omp task shared(n)
		{
			int my_n;
			
			my_n = touch_node_parallel(e->dest);
			
			#pragma omp atomic
			n += my_n;
		}
	}
	
	#pragma omp taskwait

	return n;
}

_Cilk_shared uint32_t touch_node_prepare (vertex_t *node)
{
	uint32_t n;

	#pragma omp parallel
	{
		#pragma omp single
		{
			n = touch_node_parallel(node);
		}
	}

	return n;
}

uint32_t touch_all_nodes ()
{
	uint32_t n;
	
	n = _Cilk_offload touch_node_seq(tree.tree_root);
	
	gettimeofday(&xeonphi_time_start, NULL);
	
	n = _Cilk_offload touch_node_prepare(tree.tree_root);
	
	gettimeofday(&xeonphi_time_end, NULL);

	return n;
}

