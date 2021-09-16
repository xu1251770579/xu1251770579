#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp10.h"
#include "stdlib.h"

#define MY_INFINITY   1000000000

/* 
 *  find_nodes
 *	 
 *	 
 *	
 *	
 * INPUTS:  loc -> a center point and distance from the point
 *          vs -> is the vertex set which contain the vertex in the loc
 *          p ->is the pyramid-tree
 *          nnum -> is the index of the node 
 * OUTPUTS: vs contain the vertexs in the range of the local 
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// thid function find the node in the range of loc
void
find_nodes (locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum)
{
    // Do not modify the following line nor add anything above in the function.
    record_fn_call ();
    int32_t* id_copy;
    //check if the nnum out the range, if out the range->find the node
    if (4*nnum >= p->n_nodes){
        //check if the nodes in the loc
        if (1==in_range(loc, p->node[nnum].x, p->node[nnum].y_left)){
            if(vs->id_array_size == vs->count ){
                //check whether the id is null,if is null molloc a null memory
                if(NULL == (id_copy = realloc(vs->id,2*(vs->id_array_size)*sizeof(*(vs->id))))){
                    return;
                }
                vs->id = id_copy;
                vs->id_array_size *= 2;
            }
            //sort it in an increasing order
            for(int32_t i = 0; i <= vs->count; i++){
                if(i==vs->count ||vs->id[i] > p->node[nnum].id){
                    for(int32_t j = vs->count -1; j >= i;j--){
                        vs->id[j+1] = vs->id[j];
                    }
                    vs->id[i] = p->node[nnum].id;
                    break;
                }
            }
            vs->count++;
            return;
        }
        return;
    }else{
        //recursively check
        if (p->node[nnum].x > loc->x - loc->range){
            if (p->node[nnum].y_left > loc->y - loc->range){
                find_nodes (loc, vs, p, 4*nnum +1 );
            }
            if (p->node[nnum].y_left < loc->y + loc->range){
                find_nodes (loc, vs, p, 4*nnum+3);
            }
        }
        if (p->node[nnum].x < loc->x + loc->range){
            if (p->node[nnum].y_right > loc->y - loc->range){
                find_nodes (loc, vs, p, 4*nnum +2 );
            }
            if (p->node[nnum].y_right < loc->y + loc->range){
                find_nodes (loc, vs, p, 4*nnum+4);
            }
        }
    }
}

/* 
 *  trim_nodes
 *	 
 *	 
 *	
 *	
 * INPUTS:  g -> is the all the vertex
 *          vs -> contain the vertex that need to be modified   
 *          loc -> is the constraint of the vs
 * OUTPUTS: remove the node in vs which is not in the loc
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function remove the nodes in vs that is not in the loc

void
trim_nodes (graph_t* g, vertex_set_t* vs, locale_t* loc)
{
    //check each nodes in the vs
    for(int32_t i=vs->count -1;i >= 0; i--){
        if (1 != in_range(loc, g->vertex[vs->id[i]].x, g->vertex[vs->id[i]].y)){
            vs->id[i] = vs->id[vs->count -1];
            vs->count--;
        }
    }
    return;
}

//just for swap 
void
heap_swap(int32_t* a, int32_t* b){
    int32_t i = *a;
    *a = *b;
    *b = i;
    return;
}
/* 
 *  heapify
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> the graph
 *         h -> the heap
 *         root ->the start point of heapify
 * OUTPUTS: sort the root node 
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function sort the root node to the right place below it

void
heapify (graph_t* g, heap_t* h, int32_t root){
    //initial the smallest
    int32_t smallest = root;
    int32_t left = root*2 + 1; 
    int32_t right = root*2 + 2;
    if(left < h->n_elts && g->vertex[h->elt[smallest]].from_src > g->vertex[h->elt[left]].from_src){
        smallest = left;
    } 
    if(right < h->n_elts && g->vertex[h->elt[smallest]].from_src > g->vertex[h->elt[right]].from_src){
        smallest = right;
    } 
    if(smallest != root ){ 
        heap_swap(&(h->elt[root]),&(h->elt[smallest]));
        heap_swap(&(g->vertex[h->elt[root]].heap_id),&(g->vertex[h->elt[smallest]].heap_id) );
        heapify( g, h, smallest);
    }
}

/* 
 *  heap_sort_reduce
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> the graph
 *         h -> heap
 *         heap_id -> is the heap_id that be reduced
 * OUTPUTS: this sort from the bottom node, sort with upper nodes
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function sort upper, with the reduced nodes in the heap

void
heap_sort_reduce(graph_t* g, heap_t* h,int32_t heap_id){
    //check if the node is root
    if(heap_id == 0){
        return;
    }
    //check the child with there parents nodes
    if(g->vertex[h->elt[heap_id]].from_src < g->vertex[h->elt[(heap_id - 1)/2]].from_src){
        heap_swap(&(h->elt[heap_id]),&(h->elt[(heap_id-1)/2]));
        heap_swap(&(g->vertex[h->elt[heap_id]].heap_id),&(g->vertex[h->elt[(heap_id -1)/2]].heap_id));
        heap_sort_reduce(g,h,(heap_id-1)/2);
    }
}
/* 
 *  heap_initial
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> the graph
 * 	       h -> the heap
 * OUTPUTS: the sorted heap
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function sorted the heap
void
heap_initial(graph_t* g, heap_t* h){
    for(int32_t i = (h->n_elts)/2; i>=0; i--){
        heapify(g,h,i);
    }
    //for(int32_t i=0;i<h->n_elts;i++){
    //    printf("%d ",g->vertex[h->elt[i]].from_src);
    //}
    //printf("\n");
}
/* 
 *  heap_remove_lowest
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> graph
 *         h -> the heap
 * OUTPUTS: change the root node with the last node and the number of node--
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function remove the root 
void
heap_remove_lowest(graph_t* g, heap_t* h){
    g->vertex[h->elt[0]].done = 1;
    heap_swap(&(h->elt[0]),&(h->elt[h->n_elts-1]));
    heap_swap(&(g->vertex[h->elt[0]].heap_id),&(g->vertex[h->elt[h->n_elts-1]].heap_id) );
    h->n_elts--;
    //printf("%d",h->n_elts);
}
/* 
 *  heap_reduce
 *	 
 *	 
 *	
 *	
 * INPUTS: g -> graph
 *         h -> heap
 * OUTPUTS: reduce the from_source distance by the root node
 * RETURN VALUE: void
 * SIDE EFFECTS: none
 */
// this function reduce the from_sourse on the neighber of the root node
void
heap_reduce(graph_t* g, heap_t* h){
    // for each neighbor
    for(int32_t i = 0; i < g->vertex[h->elt[0]].n_neighbors;i++){
        //check whether the new way is close to the initial distance 
        if((g->vertex[g->vertex[h->elt[0]].neighbor[i]].done == 0) && 
        (g->vertex[g->vertex[h->elt[0]].neighbor[i]].from_src > g->vertex[h->elt[0]].from_src + g->vertex[h->elt[0]].distance[i])){
            g->vertex[g->vertex[h->elt[0]].neighbor[i]].from_src = g->vertex[h->elt[0]].from_src + g->vertex[h->elt[0]].distance[i];
            g->vertex[g->vertex[h->elt[0]].neighbor[i]].pred = h->elt[0];
            //if change -> sort it
            heap_sort_reduce(g,h,g->vertex[g->vertex[h->elt[0]].neighbor[i]].heap_id);
        }
    }
    
}
/* 
 *  dijkstra
 *	 
 *	 
 *	
 *	
 * INPUTS: g-> graph
 *         h -> heap
 *         src-> the source set
 *         dest -> the destination set
 *         path -> used for containe the cloest path from source to dest
 * OUTPUTS: all the vertex in the graph has a closet distance from the source,and find the cloest path from source to dest 
 * RETURN VALUE: 0 if failed, otherwise 1
 * SIDE EFFECTS: none
 */
// this function find the cloest path from source to destination
int32_t
dijkstra (graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest,
          path_t* path)
{   //initial the done to 0 means not done with vertex, prev to -1 means no prev node yet
    for(int32_t i=0;i<g->n_vertices;i++){
        g->vertex[i].pred = -1;
        g->vertex[i].done = 0;
        g->vertex[i].from_src = MY_INFINITY;
    }
    //initial the start point
    for(int32_t i=0;i<src->count;i++){
        g->vertex[src->id[i]].from_src = 0;
    }
    //initial the heap
    h->n_elts = g->n_vertices;
    for(int32_t i=0;i< g->n_vertices;i++){
        h->elt[i] = i;
        g->vertex[i].heap_id = i;
    }
    heap_initial(g, h);
    //each time done one node
    for(int32_t i = 0; i <g->n_vertices; i++){
        heapify(g,h,0);
        heap_reduce(g,h);
        heap_remove_lowest(g,h);
    }
    //find the closest destination
    int32_t closest_dis = MY_INFINITY,closest;
    for(int32_t i=0;i<dest->count;i++){
        if(g->vertex[dest->id[i]].from_src < closest_dis){
            closest = dest->id[i];
            closest_dis = g->vertex[closest].from_src;
        }
    }
    if(closest_dis == MY_INFINITY){
        return 0;
    }
    //fill in the path in right direction
    path->tot_dist = closest_dis;
    path->n_vertices = 0;
    int32_t curruent = closest;
    while(g->vertex[curruent].from_src != 0){
        path->n_vertices++;
        curruent = g->vertex[curruent].pred;
    }
    //set a suitable large size id
    if(NULL == (path->id = malloc(((path->n_vertices)+1) * 4))){
        return 0;
    }
    //if the vertex _set is too large failed
    //if(path->n_vertices>=MAX_IN_VERTEX_SET){
    //    return 0;
    //}
    curruent =closest;
    for(int32_t i = path->n_vertices; i>=0;i--){
        path->id[i] = curruent;
        curruent = g->vertex[curruent].pred;
        if(curruent==-1&&i!=0){
            return 0;
        }
    }
    path->n_vertices++;
    //add the start vertex
    

    return 1;
}

