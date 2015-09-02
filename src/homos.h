/***************************************************************************
**
*A  homos.h                  graph homomorphisms              Julius Jonusas
**                                                            J. D. Mitchell 
**                                                            
**  Copyright (C) 2014-15 - Julius Jonusas and J. D. Mitchell 
**  This file is free software, see license information at the end.
**  
*/

#include "src/schreier-sims.h"
#include "bliss-0.72/bliss_C.h"
#include <setjmp.h>

struct homos_graph_struct {
  UIntL* neighbours;
  UIntS  nr_verts;
};

typedef struct homos_graph_struct HomosGraph;

void homo_hook_print ();

void GraphHomomorphisms (HomosGraph*  graph1, 
                         HomosGraph*  graph2,
                         void         (*hook)(void*        user_param,
	                                      const UIntS  nr,
	                                      const UIntS  *map       ),
                         void*        user_param_arg,
                         UIntL        max_results_arg,
                         int          hint_arg, 
                         bool         isinjective, 
                         int*         image,
                         UIntS*       map     );

HomosGraph* new_homos_graph (UIntS nr_verts);

void  add_edge_homos_graph (HomosGraph* graph, 
                            UIntS from_vert, 
                            UIntS to_vert);

void  free_homos_graph      (HomosGraph* graph);