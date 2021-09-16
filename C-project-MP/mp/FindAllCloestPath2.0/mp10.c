#include <stdint.h>
#include <stdio.h>

#include "mp10.h"

/* 
 *  mark_nodes
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> graph
 *         p -> pyrimid tree
 *         nnum -> the node num
 * OUTPUTS: find all node recursively and set the mm_bit
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function find all node and check 21-84 which is its parents' node 
void
mark_nodes(graph_t* g, pyr_tree_t* p, int32_t nnum)
{   
    static int32_t n;
    if(nnum >= 21 && nnum <=84){
        n = nnum;
    }
    if(nnum >= p->n_nodes){
        return;
    }
    if(4*nnum >= p->n_nodes){
        g->vertex[p->node[nnum].id].mm_bit = n - 21;
        return;
    }
    mark_nodes(g,p,4*nnum+1);
    mark_nodes(g,p,4*nnum+2);
    mark_nodes(g,p,4*nnum+3);
    mark_nodes(g,p,4*nnum+4);
}
/* 
 *  mark_vertex_minimap
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> graph
 *         p -> pyrimid_tree
 * OUTPUTS: write all mm_bit
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function call the mark_nodes to mark all nodes
int32_t 
mark_vertex_minimap (graph_t* g, pyr_tree_t* p)
{
    for(int32_t i=21; i<= 84;i++){
        mark_nodes(g,p,i);
    }
    return 1;
}

/* 
 *  build_vertex_set_minimap
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> graph
 *         vs -> vertex_set
 * OUTPUTS: set the minimap for vs
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function set the minimap for vs
void 
build_vertex_set_minimap (graph_t* g, vertex_set_t* vs)
{
    uint64_t temp_num = 1;
    vs->minimap = 0;
    for(int32_t i=0; i<vs->count;i++){
        vs->minimap = vs->minimap | (temp_num << g->vertex[vs->id[i]].mm_bit);
    }
}

/* 
 *  build_path_minimap
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> graph
 *         p -> path
 * OUTPUTS: set the minimap for path
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function set the minimap for path
void 
build_path_minimap (graph_t* g, path_t* p)
{
    uint64_t temp_num = 1;
    p->minimap=0;
    for(int32_t i=0; i<p->n_vertices;i++){
        p->minimap = p->minimap | (temp_num << g->vertex[p->id[i]].mm_bit);
    }
}

/* 
 *  merge_vertex_sets
 *	 
 *	 
 *	
 *	
 * INPUTS: v1 -> vs1
 *         v2 -> vs2
 *         vint -> the vs for matching
 * OUTPUTS: put the same element from v1 and v2 in vint
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function put the same element from v1 and v2 in vint
int32_t
merge_vertex_sets (const vertex_set_t* v1, const vertex_set_t* v2,
		   vertex_set_t* vint)
{	if(vint->count != 0){
        vint->count=0;
    }
    if(((v1->minimap)&(v2->minimap)) != 0){
        for(int32_t i=0; i<v1->count;i++){
            for(int32_t j=0; j<v2->count;j++){
                if(v1->id[i]==v2->id[j]){
                    vint->id[vint->count]=v1->id[i];
                    vint->count++;   
                }
            }
        }
    }
    if(vint->count==0){
        return 0;
    }
    return 1;
}

