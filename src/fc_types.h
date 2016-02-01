#ifndef FLATCLOTH_TYPES_H
#define FLATCLOTH_TYPES_H

#include "opengl.h"

#include <vector>
#include <glm/glm.hpp>

extern struct Settings {
        int32_t width{960};
        int32_t height{540};
        uint32_t fullscreen{0};
        uint32_t focus{1};
        uint32_t physics_run{1};
        float ratio_w {16.0f};
        float ratio_h {8.0f};
        float ratio{16.0f / 9.0f};
} settings;

typedef uint32_t IdType;

typedef struct {
        IdType value;
} VertexId, TexcoordId, FaceId, MeshId, DynamicId, ConstraintId;

typedef glm::vec3 Vertex;
typedef glm::vec2 Texcoord;
typedef glm::vec4 Color;

struct FaceVertex {
        VertexId vertex_id{0};
        TexcoordId texcoord_id{0};
        GLuint index {0};  // index in gl array
};

struct Face {
        FaceVertex fv0{};
        FaceVertex fv1{};
        FaceVertex fv2{};
};

struct Quad {
        Face f0;
        Face f1;
};

struct Dynamic {
        Vertex position_now{0.0f, 0.0f, 0.0f};
        Vertex position_prev{0.0f, 0.0f, 0.0f};
        Vertex position_next{0.0f, 0.0f, 0.0f};
        Vertex velocity_now{0.0f, 0.0f, 0.0f};
        Vertex velocity_next{0.0f, 0.0f, 0.0f};
        Vertex acceleration_now{0.0f, 0.0f, 0.0f};
        uint32_t pinned{0};

        explicit Dynamic(Vertex vertex) :
        position_now(vertex),
        position_prev(vertex),
        position_next(vertex)
        { }
};

struct Constraint {
        DynamicId d0{0};
        DynamicId d1{0};
        float rest_length{0.0f};
};

typedef std::vector<Vertex> Vertices;
typedef std::vector<VertexId> VerticesIds;
typedef std::vector<Texcoord> Texcoords;
typedef std::vector<TexcoordId> TexcoordsIds;
typedef std::vector<Face> Faces;
typedef std::vector<FaceId> FacesIds;

typedef std::vector<Dynamic> Dynamics;
typedef std::vector<DynamicId> DynamicsIds;
typedef std::vector<Constraint> Constraints;
typedef std::vector<ConstraintId> ConstraintsIds;

#endif //  FLATCLOTH_TYPES_H
