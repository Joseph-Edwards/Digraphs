#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.hh"
extern "C" {
#include "bliss_C.h"
}

/*
  Copyright (c) 2003-2015 Tommi Junttila
  Released under the GNU Lesser General Public License version 3.

  This file is part of bliss.

  bliss is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, version 3 of the License.

  bliss is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with bliss.  If not, see <http://www.gnu.org/licenses/>.
*/

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wsign-compare"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#endif

struct bliss_digraphs_graph_struct {
  bliss_digraphs::Graph* g;
};

extern "C"
BlissGraph *bliss_digraphs_new(const unsigned int n)
{
  BlissGraph *graph = new bliss_digraphs_graph_struct;
  assert(graph);
  graph->g = new bliss_digraphs::Graph(n);
  assert(graph->g);
  return graph;
}

extern "C"
BlissGraph *bliss_digraphs_read_dimacs(FILE *fp)
{
  bliss_digraphs::Graph *g = bliss_digraphs::Graph::read_dimacs(fp);
  if(!g)
    return 0;
  BlissGraph *graph = new bliss_digraphs_graph_struct;
  assert(graph);
  graph->g = g;
  return graph;
}

extern "C"
void bliss_digraphs_write_dimacs(BlissGraph *graph, FILE *fp)
{
  assert(graph);
  assert(graph->g);
  graph->g->write_dimacs(fp);
}

extern "C"
void bliss_digraphs_clear(BlissGraph *graph)
{
  assert(graph);
  assert(graph->g);
  graph->g->clear();
}

  extern "C"
void bliss_digraphs_change_color(BlissGraph* graph, const unsigned int vertex, const unsigned int color)
{
  assert(graph);
  assert(graph->g);
  graph->g->change_color(vertex, color);
}


extern "C"
void bliss_digraphs_release(BlissGraph *graph)
{
  assert(graph);
  assert(graph->g);
  delete graph->g; graph->g = 0;
  delete graph;
}

extern "C"
void bliss_digraphs_write_dot(BlissGraph *graph, FILE *fp)
{
  assert(graph);
  assert(graph->g);
  graph->g->write_dot(fp);
}

extern "C"
unsigned int bliss_digraphs_get_nof_vertices(BlissGraph *graph)
{
  assert(graph);
  assert(graph->g);
  return graph->g->get_nof_vertices();
}

extern "C"
unsigned int bliss_digraphs_add_vertex(BlissGraph *graph, unsigned int l)
{
  assert(graph);
  assert(graph->g);
  return graph->g->add_vertex(l);
}

extern "C"
void bliss_digraphs_add_edge(BlissGraph *graph, unsigned int v1, unsigned int v2)
{
  assert(graph);
  assert(graph->g);
  graph->g->add_edge(v1, v2);
}

extern "C"
int bliss_digraphs_cmp(BlissGraph *graph1, BlissGraph *graph2)
{
  assert(graph1);
  assert(graph1->g);
  assert(graph2);
  assert(graph2->g);
  return (*graph1->g).cmp(*graph2->g);
}

extern "C"
unsigned int bliss_digraphs_hash(BlissGraph *graph)
{
  assert(graph);
  assert(graph->g);
  return graph->g->get_hash();
}

extern "C"
BlissGraph *bliss_digraphs_permute(BlissGraph *graph, const unsigned int *perm)
{
  assert(graph);
  assert(graph->g);
  assert(graph->g->get_nof_vertices() == 0 || perm);
  BlissGraph *permuted_graph = new bliss_digraphs_graph_struct;
  assert(permuted_graph);
  permuted_graph->g = graph->g->permute(perm);
  return permuted_graph;
}

void fill_size(BlissStats* stats, const bliss_digraphs::Stats& s)
{
#ifdef BLISS_IN_GAP
   std::vector<int> sizes = s.get_group_size().get_mults();
   stats->group_size = (int*)malloc(sizes.size() * sizeof(int));
   stats->group_size_len = sizes.size();
   for(int i = 0; i < sizes.size(); ++i) {
       stats->group_size[i] = sizes[i];
   }
#endif
}



extern "C"
void
bliss_digraphs_find_automorphisms(BlissGraph *graph,
			 void (*hook)(void *user_param,
				      unsigned int n,
				      const unsigned int *aut),
			 void *hook_user_param,
			 BlissStats *stats)
{
  bliss_digraphs::Stats s;
  assert(graph);
  assert(graph->g);
  graph->g->find_automorphisms(s, hook, hook_user_param);

  if(stats)
    {
      stats->group_size_approx = s.get_group_size_approx();
      stats->nof_nodes = s.get_nof_nodes();
      stats->nof_leaf_nodes = s.get_nof_leaf_nodes();
      stats->nof_bad_nodes = s.get_nof_bad_nodes();
      stats->nof_canupdates = s.get_nof_canupdates();
      stats->nof_generators = s.get_nof_generators();
      stats->max_level = s.get_max_level();
      fill_size(stats, s);
    }
}


extern "C"
const unsigned int *
bliss_digraphs_find_canonical_labeling(BlissGraph *graph,
			      void (*hook)(void *user_param,
					   unsigned int n,
					   const unsigned int *aut),
			      void *hook_user_param,
			      BlissStats *stats)
{
  bliss_digraphs::Stats s;
  const unsigned int *canonical_labeling = 0;
  assert(graph);
  assert(graph->g);

  canonical_labeling = &(*graph->g->canonical_form(s, hook, hook_user_param));

  if(stats)
    {
      stats->group_size_approx = s.get_group_size_approx();
      stats->nof_nodes = s.get_nof_nodes();
      stats->nof_leaf_nodes = s.get_nof_leaf_nodes();
      stats->nof_bad_nodes = s.get_nof_bad_nodes();
      stats->nof_canupdates = s.get_nof_canupdates();
      stats->nof_generators = s.get_nof_generators();
      stats->max_level = s.get_max_level();
      fill_size(stats, s);

    }

  return canonical_labeling;
}


extern "C"
void bliss_digraphs_free_blissstats(BlissStats *stats)
{
#ifdef BLISS_IN_GAP
    free(stats->group_size);
#endif
}

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
