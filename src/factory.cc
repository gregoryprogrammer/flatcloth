#include "factory.h"
#include "fc_tools.h"
#include "fc_data.h"
#include "fc_types.h"
#include "components.h"

Entity FACTORY_create_cloth(World *world, glm::vec3 position, int w, int h)
{
    auto step = 1.0f / std::max(w, h);
    auto tx_step = 1.0f / (w - 1);
    auto ty_step = 1.0f / (h - 1);

    std::vector<VertexId> vertices_ids;
    std::vector<DynamicId> dynamics_ids;
    std::vector<TexcoordId> texcoords_ids;

    std::vector<FaceId> faces_ids;

    // grid
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            Vertex vertex(step * i, step * j, 0.0f);
            Texcoord texcoord(tx_step * i, ty_step * j);

            texcoord.y = 1.0f - texcoord.y;

            auto vertex_id = add_vertex(vertex);
            auto dynamic_id = add_dynamic(vertex_id);
            auto texcoord_id = add_texcoord(texcoord);

            vertices_ids.push_back(vertex_id);
            dynamics_ids.push_back(dynamic_id);
            texcoords_ids.push_back(texcoord_id);
        }
    }

    // index order, quad
    // 3 2
    // 0 1
    //
    // grid
    // 6 7 8
    // 3 4 5
    // 0 1 2
    for (int j = 0; j < h - 1; ++j) {
        for (int i = 0; i < w - 1; ++i) {

            auto v0_local = (GLuint)(i + j * w);
            auto v1_local = (GLuint)(i + j * w + 1);
            auto v2_local = (GLuint)(i + (j + 1) * w + 1);
            auto v3_local = (GLuint)(i + (j + 1) * w);

            auto v0 = vertices_ids[v0_local];
            auto v1 = vertices_ids[v1_local];
            auto v2 = vertices_ids[v2_local];
            auto v3 = vertices_ids[v3_local];

            auto t0 = texcoords_ids[v0_local];
            auto t1 = texcoords_ids[v1_local];
            auto t2 = texcoords_ids[v2_local];
            auto t3 = texcoords_ids[v3_local];

            auto face_0 = Face {
                {v0, t0, v0_local},
                {v1, t1, v1_local},
                {v3, t3, v3_local}};
            auto face_1 = Face {
                {v3, t3, v3_local},
                {v1, t1, v1_local},
                {v2, t2, v2_local}};

            auto f0 = add_face(face_0);
            auto f1 = add_face(face_1);

            faces_ids.push_back(f0);
            faces_ids.push_back(f1);

            auto d0_id = dynamics_ids[v0_local];
            auto d1_id = dynamics_ids[v1_local];
            auto d2_id = dynamics_ids[v2_local];
            auto d3_id = dynamics_ids[v3_local];

            // constraints diagonal
            add_constraint(d0_id, d2_id);
            add_constraint(d1_id, d3_id);
        }
    }

    // constraints horizontal
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w - 1; ++i) {
            auto v0 = i + j * w;
            auto v1 = i + j * w + 1;

            auto d0_id = dynamics_ids[v0];
            auto d1_id = dynamics_ids[v1];

            add_constraint(d0_id, d1_id);
        }
    }

    // constraints horizontal x2
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w - 2; ++i) {
            auto v0 = i + j * w;
            auto v1 = i + j * w + 2;

            auto d0_id = dynamics_ids[v0];
            auto d1_id = dynamics_ids[v1];

            add_constraint(d0_id, d1_id);
        }
    }

    // constraints vertical
    for (int j = 0; j < h - 1; ++j) {
        for (int i = 0; i < w; ++i) {
            auto v0 = i + j * w;
            auto v3 = i + (j + 1) * w;

            auto d0_id = dynamics_ids[v0];
            auto d3_id = dynamics_ids[v3];

            add_constraint(d0_id, d3_id);
        }
    }

    // constraints vertical
    for (int j = 0; j < h - 2; ++j) {
        for (int i = 0; i < w; ++i) {
            auto v0 = i + j * w;
            auto v3 = i + (j + 2) * w;

            auto d0_id = dynamics_ids[v0];
            auto d3_id = dynamics_ids[v3];

            add_constraint(d0_id, d3_id);
        }
    }

    auto texture = get_texture("assets/graphics/fabric.png");
    std::vector<TextureGL> textures {
        {texture.id, GL_TEXTURE_2D, "u_texture_diffuse_0"}
    };

    auto cloth = world->create_entity();
    assign_component<Transform>(cloth);
    auto mesh = assign_component<Mesh>(cloth);
    auto mesh_gl = assign_component<MeshGL>(cloth);
    auto dynamic_body = assign_component<DynamicBody>(cloth);

    mesh->vertices_ids = vertices_ids;
    mesh->texcoords_ids = texcoords_ids;
    mesh->faces_ids = faces_ids;
    mesh->textures = textures;

    mesh_gl->two_sided = 1;

    dynamic_body->dynamics_ids = dynamics_ids;

    DYNAMIC_BODY_translate(cloth, position);

    auto pin_0 = (h - 1) * w;
    auto pin_1 = w * h - 1;

    dynamic_pin(dynamics_ids[pin_0]);
    dynamic_pin(dynamics_ids[pin_1]);

    return cloth;
}

Entity FACTORY_create_box(World *world, Vertex position, float edge)
{
    // bottom
    // v2  v3
    // v0  v1
    auto v0 = add_vertex({0.0f, 0.0f, edge});
    auto v1 = add_vertex({edge, 0.0f, edge});
    auto v2 = add_vertex({0.0f, 0.0f, 0.0f});
    auto v3 = add_vertex({edge, 0.0f, 0.0f});

    // top
    // v7  v6
    // v5  v4
    auto v4 = add_vertex({edge, edge, edge});
    auto v5 = add_vertex({0.0f, edge, edge});
    auto v6 = add_vertex({edge, edge, 0.0f});
    auto v7 = add_vertex({0.0f, edge, 0.0f});

    // y flipped
    auto t0 = add_texcoord({0.0f, 1.0f});
    auto t1 = add_texcoord({1.0f, 1.0f});
    auto t2 = add_texcoord({0.0f, 0.0f});
    auto t3 = add_texcoord({1.0f, 0.0f});

    std::vector<VertexId> vertices_ids;
    std::vector<TexcoordId> texcoords_ids;

    for (int i = 0; i < 6; ++i) {
        texcoords_ids.push_back(t0);
        texcoords_ids.push_back(t1);
        texcoords_ids.push_back(t2);
        texcoords_ids.push_back(t3);
    }

    auto q_bottom = make_quad(
    {v2, t0, 0},
    {v3, t1, 1},
    {v0, t2, 2},
    {v1, t3, 3});

    vertices_ids.push_back(v2);
    vertices_ids.push_back(v3);
    vertices_ids.push_back(v0);
    vertices_ids.push_back(v1);

    auto q_top = make_quad(
    {v5, t0, 4},
    {v4, t1, 5},
    {v7, t2, 6},
    {v6, t3, 7});

    vertices_ids.push_back(v5);
    vertices_ids.push_back(v4);
    vertices_ids.push_back(v7);
    vertices_ids.push_back(v6);

    auto q_front = make_quad(
    {v0, t0, 8},
    {v1, t1, 9},
    {v5, t2, 10},
    {v4, t3, 11});

    vertices_ids.push_back(v0);
    vertices_ids.push_back(v1);
    vertices_ids.push_back(v5);
    vertices_ids.push_back(v4);

    auto q_left = make_quad(
    {v2, t0, 12},
    {v0, t1, 13},
    {v7, t2, 14},
    {v5, t3, 15});

    vertices_ids.push_back(v2);
    vertices_ids.push_back(v0);
    vertices_ids.push_back(v7);
    vertices_ids.push_back(v5);

    auto q_back = make_quad(
    {v3, t0, 16},
    {v2, t1, 17},
    {v6, t2, 18},
    {v7, t3, 19});

    vertices_ids.push_back(v3);
    vertices_ids.push_back(v2);
    vertices_ids.push_back(v6);
    vertices_ids.push_back(v7);

    auto q_right = make_quad(
    {v1, t0, 20},
    {v3, t1, 21},
    {v4, t2, 22},
    {v6, t3, 23});

    vertices_ids.push_back(v1);
    vertices_ids.push_back(v3);
    vertices_ids.push_back(v4);
    vertices_ids.push_back(v6);

    std::vector<FaceId> faces_ids;
    faces_ids.push_back(add_face(q_bottom.f0));
    faces_ids.push_back(add_face(q_bottom.f1));
    faces_ids.push_back(add_face(q_top.f0));
    faces_ids.push_back(add_face(q_top.f1));
    faces_ids.push_back(add_face(q_front.f0));
    faces_ids.push_back(add_face(q_front.f1));
    faces_ids.push_back(add_face(q_left.f0));
    faces_ids.push_back(add_face(q_left.f1));
    faces_ids.push_back(add_face(q_back.f0));
    faces_ids.push_back(add_face(q_back.f1));
    faces_ids.push_back(add_face(q_right.f0));
    faces_ids.push_back(add_face(q_right.f1));

    auto texture_diffuse = get_texture("assets/graphics/uvrefmap.png");

    std::vector<TextureGL> textures {
        {texture_diffuse.id, GL_TEXTURE_2D, "u_texture_diffuse_0"}
    };

    auto box = world->create_entity();
    auto transform = assign_component<Transform>(box);
    assign_component<MeshGL>(box);
    auto mesh = assign_component<Mesh>(box);
    auto ball = assign_component<Ball>(box);
    auto dynamic_body = assign_component<DynamicBody>(box);

    transform->position = position;
    transform->origin = {edge / 2, edge / 2, edge / 2};

    auto position_id = add_vertex({0.0f, 0.0f, 0.0f});
    auto dynamic_id = add_dynamic(position_id);

    ball->radius = edge / 2;
    ball->position_id = position_id;

    dynamic_body->dynamics_ids.push_back(dynamic_id);

    DYNAMIC_BODY_move_to(box, position);

    mesh->vertices_ids = vertices_ids;
    mesh->texcoords_ids = texcoords_ids;
    mesh->faces_ids = faces_ids;
    mesh->textures = textures;

    return box;
}

Entity FACTORY_create_camera(World *world, Vertex position)
{
    auto camera = world->create_entity();
    auto transform = assign_component<Transform>(camera);
    assign_component<Camera>(camera);
    transform->position = position;
    return camera;
}

Entity FACTORY_create_skybox(World *world, const std::string &path)
{
    // bottom
    // v2  v3
    // v0  v1
    auto v0 = add_vertex({-0.5f, -0.5f,  0.5f});
    auto v1 = add_vertex({ 0.5f, -0.5f,  0.5f});
    auto v2 = add_vertex({-0.5f, -0.5f, -0.5f});
    auto v3 = add_vertex({ 0.5f, -0.5f, -0.5f});

    // top
    // v7  v6
    // v5  v4
    auto v4 = add_vertex({ 0.5f, 0.5f,  0.5f});
    auto v5 = add_vertex({-0.5f, 0.5f,  0.5f});
    auto v6 = add_vertex({ 0.5f, 0.5f, -0.5f});
    auto v7 = add_vertex({-0.5f, 0.5f, -0.5f});

    auto fake_texcoord_id = add_texcoord({0.0f, 0.0f});

    auto q_right = make_quad(
    {v3, fake_texcoord_id},
    {v1, fake_texcoord_id},
    {v6, fake_texcoord_id},
    {v4, fake_texcoord_id});

    auto q_left = make_quad(
    {v0, fake_texcoord_id},
    {v2, fake_texcoord_id},
    {v5, fake_texcoord_id},
    {v7, fake_texcoord_id});

    auto q_top = make_quad(
    {v7, fake_texcoord_id},
    {v6, fake_texcoord_id},
    {v5, fake_texcoord_id},
    {v4, fake_texcoord_id});

    auto q_bottom = make_quad(
    {v0, fake_texcoord_id},
    {v1, fake_texcoord_id},
    {v2, fake_texcoord_id},
    {v3, fake_texcoord_id});

    auto q_back = make_quad(
    {v2, fake_texcoord_id},
    {v3, fake_texcoord_id},
    {v7, fake_texcoord_id},
    {v6, fake_texcoord_id});

    auto q_front = make_quad(
    {v1, fake_texcoord_id},
    {v0, fake_texcoord_id},
    {v4, fake_texcoord_id},
    {v5, fake_texcoord_id});

    std::vector<FaceId> faces_ids;
    faces_ids.push_back(add_face(q_right.f0));
    faces_ids.push_back(add_face(q_right.f1));
    faces_ids.push_back(add_face(q_left.f0));
    faces_ids.push_back(add_face(q_left.f1));
    faces_ids.push_back(add_face(q_top.f0));
    faces_ids.push_back(add_face(q_top.f1));
    faces_ids.push_back(add_face(q_bottom.f0));
    faces_ids.push_back(add_face(q_bottom.f1));
    faces_ids.push_back(add_face(q_back.f0));
    faces_ids.push_back(add_face(q_back.f1));
    faces_ids.push_back(add_face(q_front.f0));
    faces_ids.push_back(add_face(q_front.f1));

    auto texture = get_cube_texture(path);

    auto entity = world->create_entity();
    auto skybox = assign_component<Skybox>(entity);
    skybox->faces_ids = faces_ids;
    skybox->texture = {texture.id, GL_TEXTURE_CUBE_MAP, "u_texture_skybox_0"};

    return entity;
}

Entity FACTORY_create_sprite2D(World *world, const std::string &pic)
{
    auto texture = get_texture(pic);

    log_info("tex w = %d", texture.w);

    auto xw = (float)texture.w * (settings.ratio_w / settings.width);
    auto xh = (float)texture.h * (settings.ratio_h / settings.height);

    auto v0 = add_vertex({0.0f, 0.0f, 0.0f});
    auto v1 = add_vertex({xw, 0.0f, 0.0f});
    auto v2 = add_vertex({0.0f, xh, 0.0f});
    auto v3 = add_vertex({xw, xh, 0.0f});

    auto t0 = add_texcoord({0.0f, 1.0f});
    auto t1 = add_texcoord({1.0f, 1.0f});
    auto t2 = add_texcoord({0.0f, 0.0f});
    auto t3 = add_texcoord({1.0f, 0.0f});

    auto quad = make_quad(
    {v0, t0, 0},
    {v1, t1, 1},
    {v2, t2, 2},
    {v3, t3, 3});

    std::vector<VertexId> vertices_ids;
    std::vector<TexcoordId> texcoords_ids;
    std::vector<FaceId> faces_ids;

    vertices_ids.push_back(v0);
    vertices_ids.push_back(v1);
    vertices_ids.push_back(v2);
    vertices_ids.push_back(v3);

    texcoords_ids.push_back(t0);
    texcoords_ids.push_back(t1);
    texcoords_ids.push_back(t2);
    texcoords_ids.push_back(t3);

    faces_ids.push_back(add_face(quad.f0));
    faces_ids.push_back(add_face(quad.f1));


    std::vector<TextureGL> textures {
        {texture.id, GL_TEXTURE_2D, "u_texture_diffuse_0"}
    };

    auto crosshair = world->create_entity();
    assign_component<Transform>(crosshair);
    auto mesh = assign_component<Mesh>(crosshair);
    assign_component<MeshGL>(crosshair);
    assign_component<Sprite2D>(crosshair);

    mesh->vertices_ids = vertices_ids;
    mesh->texcoords_ids = texcoords_ids;
    mesh->faces_ids = faces_ids;
    mesh->textures = textures;

    return crosshair;
}

Entity FACTORY_create_floor(World *world, int w, int h, float edge)
{
    std::vector<VertexId> vertices_ids;
    std::vector<TexcoordId> texcoords_ids;

    std::vector<FaceId> faces_ids;

    // grid
    GLuint p = 0;
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            Vertex vertex_0(edge * i, 0.0f, edge * j + edge);
            Vertex vertex_1(edge * i + edge, 0.0f, edge * j + edge);
            Vertex vertex_2(edge * i, 0.0f, edge * j);
            Vertex vertex_3(edge * i + edge, 0.0f, edge * j);

            vertex_0.x -= edge * (w >> 1);
            vertex_1.x -= edge * (w >> 1);
            vertex_2.x -= edge * (w >> 1);
            vertex_3.x -= edge * (w >> 1);

            vertex_0.z -= edge * (h >> 1);
            vertex_1.z -= edge * (h >> 1);
            vertex_2.z -= edge * (h >> 1);
            vertex_3.z -= edge * (h >> 1);

            Texcoord texcoord_0(0.0f, 1.0f);
            Texcoord texcoord_1(1.0f, 1.0f);
            Texcoord texcoord_2(0.0f, 0.0f);
            Texcoord texcoord_3(1.0f, 0.0f);

            auto v0 = add_vertex(vertex_0);
            auto v1 = add_vertex(vertex_1);
            auto v2 = add_vertex(vertex_2);
            auto v3 = add_vertex(vertex_3);

            auto t0 = add_texcoord(texcoord_0);
            auto t1 = add_texcoord(texcoord_1);
            auto t2 = add_texcoord(texcoord_2);
            auto t3 = add_texcoord(texcoord_3);

            vertices_ids.push_back(v0);
            vertices_ids.push_back(v1);
            vertices_ids.push_back(v2);
            vertices_ids.push_back(v3);

            texcoords_ids.push_back(t0);
            texcoords_ids.push_back(t1);
            texcoords_ids.push_back(t2);
            texcoords_ids.push_back(t3);

            auto face_0 = Face {
                {v0, t0, p + 0},
                {v1, t1, p + 1},
                {v2, t2, p + 2}};
            auto face_1 = Face {
                {v2, t2, p + 2},
                {v1, t1, p + 1},
                {v3, t3, p + 3}};

            auto f0 = add_face(face_0);
            auto f1 = add_face(face_1);

            faces_ids.push_back(f0);
            faces_ids.push_back(f1);

            p += 4;
        }
    }

    auto texture = get_texture("assets/graphics/floor.png");
    std::vector<TextureGL> textures {
        {texture.id, GL_TEXTURE_2D, "u_texture_diffuse_0"}
    };

    auto floor = world->create_entity();
    assign_component<Transform>(floor);

    auto mesh = assign_component<Mesh>(floor);
    assign_component<MeshGL>(floor);

    mesh->vertices_ids = vertices_ids;
    mesh->texcoords_ids = texcoords_ids;
    mesh->faces_ids = faces_ids;
    mesh->textures = textures;


    return floor;
}
