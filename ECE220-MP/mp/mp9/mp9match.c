#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"

/* 
 *  match_requests
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> the graph
 * 		   p -> the pyramid tree
 * 		   h -> the heap
 * 		   r1 -> the requests for first person
 * 		   r2 -> the requests for second person
 * 		   src_vs -> the vs of start point
 * 		   det_vs -> the vs of destination
 * 		   path -> the closet path 
 * OUTPUTS: find the closest path
 * RETURN VALUE: 0 if failed, otherwise 1
 * SIDE EFFECTS: none
 */
// this function find the closest path
int32_t
match_requests (graph_t* g, pyr_tree_t* p, heap_t* h,
		request_t* r1, request_t* r2,
		vertex_set_t* src_vs, vertex_set_t* dst_vs, path_t* path)
{	
	//initial the count 
	src_vs->count = 0;
	dst_vs->count = 0;
	//find the src_vs and det_vs 
	find_nodes(&(r1->from),src_vs,p,0);
	trim_nodes(g, src_vs, &(r2->from));
	find_nodes(&(r1->to),dst_vs,p,0);
	trim_nodes(g, dst_vs, &(r2->to));
	if(src_vs->count ==0 || dst_vs ->count == 0){
		return 0;
	}
	if(1 != dijkstra(g, h, src_vs, dst_vs, path)){
		return 0;
	}
	return 1;
}
