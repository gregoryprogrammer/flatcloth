#ifndef FLATCLOTH_FCDATA_H
#define FLATCLOTH_FCDATA_H

#include "fc_types.h"
#include "ecs.h"

void update_vertices_data();
void make_move();
void solve_verlet(float dt);
void satisfy_constraints();

VertexId add_vertex(Vertex vertex);
Vertex get_vertex(VertexId vertex_id);
void set_vertex(VertexId vertex_id, Vertex vertex);

TexcoordId add_texcoord(Texcoord texcoord);
Texcoord get_texcoord(TexcoordId texcoord_id);

FaceId add_face(Face face);
Face get_face(FaceId face_id);

DynamicId add_dynamic(VertexId vertex_id);
Dynamic get_dynamic(DynamicId dynamic_id);

void dynamic_move_by(DynamicId dynamic_id, Vertex translation);
void dynamic_move_to(DynamicId dynamic_id, Vertex vertex);
void dynamic_pin(DynamicId dynamic_id);
void dynamic_add_force(DynamicId dynamic_id, Vertex force);
void dynamic_set_force(DynamicId dynamic_id, Vertex force);

ConstraintId add_constraint(DynamicId dynamic_id_0, DynamicId dynamic_id_1);

void entity_translate(Entity entity, Vertex translation);
void entity_add_force(Entity entity, Vertex force);
void entity_set_force(Entity entity, Vertex force);

#endif //  FLATCLOTH_FCDATA_H
