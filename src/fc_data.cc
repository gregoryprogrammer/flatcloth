#include "fc_data.h"
#include "components.h"

struct DynamicToVertex {
    DynamicId dynamic_id;
    VertexId vertex_id;
};

static Vertices vertices;
static Texcoords texcoords;
static Faces faces;
static Dynamics dynamics;
static Constraints constraints;
static std::vector<DynamicToVertex> dynamics_to_vertices{};

void update_vertices_data()
{
    for (auto &dtv : dynamics_to_vertices) {
        auto vertex = get_dynamic(dtv.dynamic_id).position_now;
        set_vertex(dtv.vertex_id, vertex);
    }
}

void make_move()
{
    for (auto &dynamic : dynamics) {

        if (dynamic.pinned == 1) {
            continue;
        }

        auto *pos_now = &dynamic.position_now;
        auto *pos_prev = &dynamic.position_prev;
        auto *pos_next = &dynamic.position_next;

        auto *vel_now = &dynamic.velocity_now;
        auto *vel_next = &dynamic.velocity_next;

        *pos_prev = *pos_now;
        *pos_now = *pos_next;

        *vel_now = *vel_next;
    }
}

void solve_verlet(float dt)
{
    for (auto &dynamic : dynamics) {
        auto *pos_now = &dynamic.position_now;
        auto *pos_prev = &dynamic.position_prev;
        auto *pos_next = &dynamic.position_next;

        auto *vel_now = &dynamic.velocity_now;
        auto *vel_next = &dynamic.velocity_next;
        auto *acc = &dynamic.acceleration_now;

        *pos_next = *pos_now * 2.0f - *pos_prev + *acc * dt * dt;
        *vel_next = (*pos_next - *pos_prev) * (1 / dt) * 0.5f;

        *acc -= *vel_now * 0.15f;
    }
}

void satisfy_constraints()
{
    constexpr int NUM_ITERATIONS = 16;
    for (const auto &constraint : constraints) {
        for (int j = 0; j < NUM_ITERATIONS; ++j) {

            auto d0_id = constraint.d0;
            auto d1_id = constraint.d1;

            auto pos0 = get_dynamic(d0_id).position_now;
            auto pos1 = get_dynamic(d1_id).position_now;

            auto delta = pos1 - pos0;
            auto delta_length = glm::length(delta);
            auto diff = (delta_length - constraint.rest_length) / delta_length;

            pos0 += delta * 0.5f * diff;
            pos1 -= delta * 0.5f * diff;

            if (dynamics[d0_id.value].pinned == 0) {
                dynamics[d0_id.value].position_now = pos0;
            }
            if (dynamics[d1_id.value].pinned == 0) {
                dynamics[d1_id.value].position_now = pos1;
            }
        }
    }
}

VertexId add_vertex(Vertex vertex)
{
    auto vertex_id = VertexId {static_cast<IdType>(vertices.size())};
    vertices.push_back(vertex);
    return vertex_id;
}

Vertex get_vertex(VertexId vertex_id)
{
    return vertices[vertex_id.value];
}

void set_vertex(VertexId vertex_id, Vertex vertex)
{
    vertices[vertex_id.value] = vertex;
}

TexcoordId add_texcoord(Texcoord texcoord)
{
    auto texcoord_id = TexcoordId {static_cast<IdType>(texcoords.size())};
    texcoords.push_back(texcoord);
    return texcoord_id;
}

Texcoord get_texcoord(TexcoordId texcoord_id)
{
    return texcoords[texcoord_id.value];
}

FaceId add_face(Face face)
{
    auto face_id = FaceId {static_cast<IdType>(faces.size())};
    faces.push_back(face);
    return face_id;
}

Face get_face(FaceId face_id)
{
    return faces[face_id.value];
}

DynamicId add_dynamic(VertexId vertex_id)
{
    auto vertex = get_vertex(vertex_id);
    auto dynamic = Dynamic(vertex);
    auto dynamic_id = DynamicId {static_cast<IdType>(dynamics.size())};
    dynamics.push_back(dynamic);

    dynamics_to_vertices.push_back({dynamic_id, vertex_id});
    return dynamic_id;
}

Dynamic get_dynamic(DynamicId dynamic_id)
{
    return dynamics[dynamic_id.value];
}

void dynamic_move_by(DynamicId dynamic_id, Vertex translation)
{
    auto func = [&translation](auto &vec) {
        vec += translation;
    };

    func(dynamics[dynamic_id.value].position_now);
    func(dynamics[dynamic_id.value].position_prev);
    func(dynamics[dynamic_id.value].position_next);
}

void dynamic_move_to(DynamicId dynamic_id, Vertex vertex)
{
    auto func = [&vertex](auto &vec) {
        vec = vertex;
    };

    func(dynamics[dynamic_id.value].position_now);
    func(dynamics[dynamic_id.value].position_prev);
    func(dynamics[dynamic_id.value].position_next);
}


void dynamic_pin(DynamicId dynamic_id)
{
    dynamics[dynamic_id.value].pinned = 1;
}

void dynamic_add_force(DynamicId dynamic_id, Vertex force)
{
    dynamics[dynamic_id.value].acceleration_now += force;
}

void dynamic_set_force(DynamicId dynamic_id, Vertex force)
{
    dynamics[dynamic_id.value].acceleration_now = force;
}

ConstraintId add_constraint(DynamicId dynamic_id_0, DynamicId dynamic_id_1)
{
    Constraint constraint;

    auto pos0 = get_dynamic(dynamic_id_0).position_now;
    auto pos1 = get_dynamic(dynamic_id_1).position_now;

    constraint.d0 = dynamic_id_0;
    constraint.d1 = dynamic_id_1;
    auto distance = glm::distance(pos0, pos1);
    constraint.rest_length = distance;

    auto constraint_id = ConstraintId {static_cast<IdType>(constraints.size())};
    constraints.push_back(std::move(constraint));
    return constraint_id;
}

