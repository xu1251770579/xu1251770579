#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mp5.h"
#include "mp10.h"


//
// These variables hold the heads of two singly-linked lists of 
// requests.  
//
// The avaialble list consists of unpaired requests: partner 
// should be NULL, and next is used to form the list.   
// 
// The shared list consists of groups with non-empty start and end 
// vertex intersections.  Only one of the requests in the group is in 
// the list.  The others are linked through the first's partner field 
// and then through the next field of the others in the group.  The 
// next field of the first request in a group is used to form the
// shared list.
//
// Note: for MP2, you should only build groups of two in the shared
// list.
//

static request_t* available = NULL;
static request_t* shared = NULL;

/*copy from the mp10.pdf to introduce the function

Allocate vertex sets for the request and for use with matching.
Allocate a path for use with matching.
If any allocation fails, clean up (free any successful allocations) and return 0.
Fill in the vertex sets for the request. If either is empty, the function should return 0. Be sure to
free any vertex sets and path allocated by the function before returning.
Create minimaps for the request’s source and destination vertex sets.
Allocate id arrays for the vertex sets to be used for matching. Make these large enough that they
can hold any intersection with the request’s corresponding vertex set. Note that you must clean up
and return 0 if allocation fails.
Try to find a matching request in the available list. Skip any requests that have no overlap in
the source vertex set minimaps or no overlap in the destination vertex set minimaps. For those with
overlap, use merge_vertex_sets to check for non-empty intersections, then call dijkstra to
find a path (if both are non-empty). If everything succeeds, pair the two requests.
If no match is found, add the request to the available list and return 1 for success. Again, be
sure to free any dynamically allocated structures (specifically, those used for testing matches).
If a matching request is found, remove the matching request from the available list, free both
requests vertex sets (these should be replaced with the intersected vertex sets), and make both
requests point to the intersected vertex sets and the path filled in by dijkstra . Be sure to build
minimaps for both intersected vertex sets and for the path. Link the two requests as described
previously into the shared list and return 1 for success.
*/

int32_t
handle_request (graph_t* g, pyr_tree_t* p, heap_t* h, request_t* r)
{
    path_t* path;
    vertex_set_t* match_vs_from;
    vertex_set_t* match_vs_to;
    //initial r
    r->partner=NULL;
    r->path = NULL;
    r->next=NULL;
    //set memory
    if(NULL==(r->src_vs=new_vertex_set())){
        return 0;
    }
    if(NULL==(r->dst_vs=new_vertex_set())){
        free_vertex_set(r->src_vs);
        return 0;
    }
    if(NULL==(match_vs_from=new_vertex_set())){
        free_vertex_set(r->src_vs);
        free_vertex_set(r->dst_vs);
        return 0;
    }
    if(NULL==(match_vs_to=new_vertex_set())){
        free_vertex_set(r->src_vs);
        free_vertex_set(r->dst_vs);
        free_vertex_set(match_vs_from);
        return 0;
    }
    if(NULL==(path=new_path())){
        free_vertex_set(r->src_vs);
        free_vertex_set(r->dst_vs);
        free_vertex_set(match_vs_from);
        free_vertex_set(match_vs_to);
        return 0;
    }
    //build the src and dst
    find_nodes(&(r->from),r->src_vs,p,0);
    find_nodes(&(r->to),r->dst_vs,p,0);
    //check the vs set count isn't 0
    if(r->src_vs->count==0 || r->dst_vs->count==0){
        free_vertex_set(r->dst_vs);
        free_vertex_set(r->src_vs);
        free_vertex_set(match_vs_from);
        free_vertex_set(match_vs_to);
        free_path(path);
        return 0;
    }
    
    //build the minimap
    build_vertex_set_minimap(g,r->src_vs);
    build_vertex_set_minimap(g,r->dst_vs);
    //set the matchig vs's id memory
    if(NULL==(match_vs_from->id=realloc(match_vs_from->id,r->src_vs->count*sizeof(int32_t))) || NULL==(match_vs_to->id=realloc(match_vs_to->id,r->dst_vs->count*sizeof(int32_t)))){
        free_vertex_set(r->dst_vs);
        free_vertex_set(r->src_vs);
        free_vertex_set(match_vs_from);
        free_vertex_set(match_vs_to);
        free_path(path);
        return 0;
    }
    //check all available
    for(request_t** current = &available;*current != NULL;current = &((*current)->next)){
        //check the minimap
        if((0 != (((*current)->src_vs->minimap) & (r->src_vs->minimap))) && (0 != (((*current)->dst_vs->minimap) & (r->dst_vs->minimap)))){
            //check the merge_vertex_sets
            if(merge_vertex_sets(r->src_vs,(*current)->src_vs,match_vs_from) && merge_vertex_sets(r->dst_vs,(*current)->dst_vs,match_vs_to)){
                //build the minimap for matching set
                build_vertex_set_minimap(g,match_vs_from);
                build_vertex_set_minimap(g,match_vs_to);
                //check the dijkstra
                if(1== dijkstra(g,h,match_vs_from,match_vs_to,path)){
                    build_path_minimap(g,path);
                    //pair two requests
                    //and remove the current in available
                    r->partner=*current;
                    (*current)->partner= NULL;
                    *current=(*current)->next;
                    r->partner->next=NULL;
                    //point to same src_vs and dst_vs and path
                    free_vertex_set(r->src_vs);
                    free_vertex_set(r->dst_vs);
                    free_vertex_set((*current)->src_vs);
                    free_vertex_set((*current)->dst_vs);
                    r->src_vs=(*current)->src_vs=match_vs_from;
                    r->dst_vs=(*current)->dst_vs=match_vs_to;
                    r->path = (*current)->path = path;
                    //put r in shared
                    r->next = shared;
                    shared = r;
                    
                    return 1;
                }
            }
        }
        
    }
    //put the r in available
    r->next = available;
    available = r;
    //free the matching vs and path
    free_vertex_set(match_vs_to);
    free_vertex_set(match_vs_from);
    free_path(path);
    return 1;
}


void
print_results ()
{
    request_t* r;
    request_t* prt;

    printf ("Matched requests:\n");
    for (r = shared; NULL != r; r = r->next) {
        printf ("%5d", r->uid);
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    printf (" %5d", prt->uid);
	}
	printf (" src=%016lX dst=%016lX path=%016lX\n", r->src_vs->minimap,
		r->dst_vs->minimap, r->path->minimap);
    }

    printf ("\nUnmatched requests:\n");
    for (r = available; NULL != r; r = r->next) {
        printf ("%5d src=%016lX dst=%016lX\n", r->uid, r->src_vs->minimap,
		r->dst_vs->minimap);
    }
}


int32_t
show_results_for (graph_t* g, int32_t which)
{
    request_t* r;
    request_t* prt;

    // Can only illustrate one partner.
    for (r = shared; NULL != r; r = r->next) {
	if (which == r->uid) {
	    return show_find_results (g, r, r->partner);
	}
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    if (which == prt->uid) {
		return show_find_results (g, prt, r);
	    }
	}
    }

    for (r = available; NULL != r; r = r->next) {
        if (which == r->uid) {
	    return show_find_results (g, r, r);
	}
    }
    return 0;
}


static void
free_request (request_t* r)
{
    free_vertex_set (r->src_vs);
    free_vertex_set (r->dst_vs);
    if (NULL != r->path) {
	free_path (r->path);
    }
    free (r);
}

void
free_all_data ()
{
    request_t* r;
    request_t* prt;
    request_t* next;

    // All requests in a group share source and destination vertex sets
    // as well as a path, so we need free those elements only once.
    for (r = shared; NULL != r; r = next) {
	for (prt = r->partner; NULL != prt; prt = next) {
	    next = prt->next;
	    free (prt);
	}
	next = r->next;
	free_request (r);
    }

    for (r = available; NULL != r; r = next) {
	next = r->next;
	free_request (r);
    }
}


