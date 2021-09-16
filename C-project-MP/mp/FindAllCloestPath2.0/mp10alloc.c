#include <stdint.h>
#include <stdlib.h>

#include "mp10.h"


vertex_set_t*
new_vertex_set ()
{
    vertex_set_t* vs;
    if(NULL == (vs = malloc(sizeof(*vs)))){
        return NULL;
    }
    vs->count = 0;
    if(NULL==(vs->id = malloc(sizeof(int32_t)))){
        free(vs);
        return NULL;
    }
    vs->id_array_size = 1;
    return vs;
}


void
free_vertex_set (vertex_set_t* vs)
{
    if(vs->id != NULL){
        free(vs->id);
    }
    free(vs);
}


path_t*
new_path ()
{
    path_t* path;
    if(NULL == (path = malloc(sizeof(*path)))){
        return NULL;
    }
    path->id = NULL;
    path->tot_dist = 0;
    path->n_vertices = 0;
    return path;
}


void
free_path (path_t* path)
{
    if(path->id != NULL){
        free(path->id);
    }
    free(path);
}

