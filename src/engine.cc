#include "engine.h"
#include "fc_types.h"
#include "fc_tools.h"
#include "opengl.h"
#include "input.h"
#include "shader.h"
#include "factory.h"
#include "texture.h"
#include "ecs.h"
#include "components.h"
#include "render_system.h"
#include "skybox_system.h"
#include "dynamic_system.h"
#include "camera_system.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <cstdio>

static Entity camera;
static Entity crosshair;
static Entity logo;

static World world;
static RenderSystem render_system_mesh;
static RenderSystem render_system_sprite2D;
static SkyboxSystem skybox_system;
static DynamicSystem dynamic_system;
static CameraSystem camera_system;

static std::vector<Entity> clothes;

void Engine::init()
{
        uint32_t init_flags = SDL_INIT_EVERYTHING;
        uint32_t window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

        if (SDL_Init(init_flags) != 0) {
                log_error("Engine", "cannot initialize sdl: %s", SDL_GetError());
                exit(1);
        }
        SDL_GL_LoadLibrary(NULL);

        m_window = SDL_CreateWindow("flatcloth", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.width,
                                    settings.height, window_flags);
        if (m_window == nullptr) {
                log_error("Engine", "cannot create window: %s", SDL_GetError());
                exit(1);
        }

        SDL_SetRelativeMouseMode(SDL_TRUE);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_glcontext = SDL_GL_CreateContext(m_window);
        SDL_GL_MakeCurrent(m_window, m_glcontext);

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
                log_error("Engine", "cannot initialize GLEW");
                exit(1);
        }

        if (!GLEW_VERSION_3_2) {
                log_error("Engine", "OpenGL 3.2 API is not available");
                exit(1);
        }

        log_info("Engine", "GL_VERSION: %s", glGetString(GL_VERSION));
        log_info("Engine", "GL_RENDERER: %s", glGetString(GL_RENDERER));
        log_info("Engine", "GL_VENDOR: %s", glGetString(GL_VENDOR));
        log_info("Engine", "GL_SHADING_LANGUAGE_VERSION: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

        GLint max_vertex_attribs;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);
        log_info("Engine", "maximum nr of vertex attributes supported: %d", max_vertex_attribs);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glPointSize(3.0f);
        glClearColor(0.1f, 0.15f, 0.25f, 1.0f);

        auto mesh_vs = "assets/shaders/mesh_textured_vs.glsl";
        auto mesh_fs = "assets/shaders/mesh_textured_fs.glsl";
        auto skybox_vs = "assets/shaders/skybox_vs.glsl";
        auto skybox_fs = "assets/shaders/skybox_fs.glsl";

        m_shaders["mesh_textured"] = std::make_unique<Shader>(mesh_vs, mesh_fs);
        m_shaders["skybox"] = std::make_unique<Shader>(skybox_vs, skybox_fs);

        FACTORY_create_skybox(&world, "assets/graphics/skybox/");
        FACTORY_create_floor(&world, 10, 10, 1.0f);

        camera = FACTORY_create_camera(&world, {0.0f, 0.0f, 0.0f});
        crosshair = FACTORY_create_sprite2D(&world, "assets/graphics/crosshair.png");
        logo = FACTORY_create_sprite2D(&world, "assets/graphics/ninja.png");

        TRANSFORM_move_to(logo, {-8.0f, -4.5f, 0.0f});
        TRANSFORM_set_origin(crosshair, {0.5f, 0.5f, 0.0f});

        clothes.push_back(FACTORY_create_cloth(&world, {-3.5f, 0.5f, -2.0f}, 4, 4));
        clothes.push_back(FACTORY_create_cloth(&world, {-2.0f, 0.5f, -2.0f}, 8, 8));
        clothes.push_back(FACTORY_create_cloth(&world, {-0.5f, 0.5f, -2.0f}, 10, 10));
        clothes.push_back(FACTORY_create_cloth(&world, {1.0f, 0.5f, -2.0f}, 16, 16));
        clothes.push_back(FACTORY_create_cloth(&world, {2.5f, 1.0f, -2.0f}, 16, 16));

        std::for_each(clothes.begin(), clothes.end(), [](auto &cloth){DYNAMIC_BODY_add_force(cloth, {0.0f, 10.0f, 10.f});});

        render_system_mesh.with_entities<Transform, Mesh, MeshGL>();
        render_system_mesh.without_entities<Sprite2D>();

        render_system_sprite2D.with_entities<Transform, Mesh, MeshGL, Sprite2D>();

        skybox_system.with_entities<Skybox>();

        dynamic_system.with_entities<DynamicBody>();

        camera_system.with_entities<Transform, Camera>();

        render_system_mesh.register_world(&world);
        render_system_sprite2D.register_world(&world);
        skybox_system.register_world(&world);
        dynamic_system.register_world(&world);
        camera_system.register_world(&world);

        render_system_mesh.init();
        render_system_sprite2D.init();
        skybox_system.init();
        camera_system.init();
}

void Engine::cleanup()
{
        std::for_each(m_shaders.begin(), m_shaders.end(),
                        [](auto &shader){ shader.second->destroy(); });
        SDL_GL_DeleteContext(m_glcontext);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        log_info("Engine", "bye bye");
}

int Engine::poll_events()
{
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

                switch (event.type) {
                case SDL_QUIT:
                        return 1;
                case SDL_KEYDOWN:
                        break;
                case SDL_MOUSEBUTTONDOWN:

                        if (settings.focus == 0) {
                                SDL_SetRelativeMouseMode(SDL_FALSE);
                                settings.focus = 1;
                        }

                        if (event.button.button == SDL_BUTTON_LEFT) {

                                // TODO grab cloth

                        } else if (event.button.button == SDL_BUTTON_RIGHT) {
                                std::for_each(clothes.begin(), clothes.end(), [](auto &cloth){DYNAMIC_BODY_add_force(cloth, {0.0f, 50.0f, 500.f});});
                        }


                        break;
                case SDL_MOUSEMOTION: {
                        // event.motion.xrel - mouse x move (relative)
                        auto xrel = event.motion.xrel;
                        auto yrel = event.motion.yrel;
                        if (settings.focus == 1) {
                                camera_system.mouse_movement(xrel, yrel);
                                camera_system.update_camera_vectors();
                        }
                        break;
                }
                case SDL_WINDOWEVENT:

                        switch (event.window.event) {

                        case SDL_WINDOWEVENT_SHOWN:
                                log_debug("Window", "%d: shown", event.window.windowID);
                                break;

                        case SDL_WINDOWEVENT_HIDDEN:
                                log_debug("Window", "%d: hidden", event.window.windowID);
                                break;

                        case SDL_WINDOWEVENT_EXPOSED:
                                log_debug("Window", "%d: exposed", event.window.windowID);
                                break;

                        case SDL_WINDOWEVENT_RESTORED:
                                log_debug("Window", "%d: restored", event.window.windowID);
                                break;

                        case SDL_WINDOWEVENT_MINIMIZED:
                                log_debug("Window", "%d: minimized", event.window.windowID);
                                break;

                        case SDL_WINDOWEVENT_MAXIMIZED:
                                log_debug("Window", "%d: maximized", event.window.windowID);
                                break;

                        case SDL_WINDOWEVENT_MOVED:
                                log_debug("Window", "%d: moved to %d, %d", event.window.windowID,
                                        event.window.data1, event.window.data2);
                                break;

                        case SDL_WINDOWEVENT_RESIZED:
                                settings.width = event.window.data1;
                                settings.height = event.window.data2;
                                viewport(settings.width, settings.height, settings.ratio);
                                log_debug("Window", "%d: resized to %d, %d", event.window.windowID,
                                        settings.width, settings.height);
                                break;

                        case SDL_WINDOWEVENT_LEAVE:  // mouse leave window
                                log_debug("Window", "%d: mouse left", event.window.windowID);
                                break;

                        case SDL_WINDOWEVENT_ENTER:  // mouse entered window
                                log_debug("Window", "%d: mouse entered", event.window.windowID);
                                break;

                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                                log_debug("Window", "%d: focus gained", event.window.windowID);
                                settings.focus = 1;
                                SDL_SetRelativeMouseMode(SDL_TRUE);
                                break;

                        case SDL_WINDOWEVENT_FOCUS_LOST:
                                log_debug("Window", "%d: focus lost", event.window.windowID);
                                settings.focus = 0;
                                break;

                        case SDL_WINDOWEVENT_CLOSE:
                                log_debug("Window", "%d: closed", event.window.windowID);
                                break;

                        default:
                                break;
                        }

                default:
                        break;
                }
        }
        return 0;
}

int Engine::logic()
{
        int quit = 0;

        if (key_pressed(SDL_SCANCODE_Q)) {
                quit = 1;
        } else if (key_pressed(SDL_SCANCODE_ESCAPE)) {

                if (settings.fullscreen == 0) {
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                        settings.focus = 0;
                }

        } else if (key_just_pressed(SDL_SCANCODE_F)) {

                if (settings.fullscreen == 1) {
                        SDL_SetWindowFullscreen(m_window, 0);
                        settings.fullscreen = 0;
                } else {
                        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        settings.fullscreen = 1;
                }

        } else if (key_just_pressed(SDL_SCANCODE_P)) {
                settings.physics_run = 1 - settings.physics_run;
        } else if (key_just_pressed(SDL_SCANCODE_C)) {
                CAMERA_toggle_freefly(camera);
        }

        if (settings.physics_run == 1) {

                dynamic_system.pre_update();
                satisfy_constraints();
                solve_verlet(0.01f);
                make_move();
                update_vertices_data();
                dynamic_system.post_update();
        }
        camera_system.temporary_steering(0.016f);

        return quit;
}

void Engine::render()
{
        render_system_mesh.update_vbo();
        render_system_sprite2D.update_vbo();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto u_view_3D = CAMERA_get_view(camera);
        auto u_view_2D = glm::mat4(1.0f);
        auto u_projection_3D = glm::perspective(CAMERA_fov(camera), settings.ratio, 0.1f, 100.0f);
        // left, right, bottom, top, zNear, zFar
        auto u_projection_2D = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, 0.0f, 1.0f);

        auto &shader_skybox = m_shaders["skybox"];
        auto &shader_mesh_textured = m_shaders["mesh_textured"];

        // remove any translation component of the view matrix
        auto u_view_skybox = glm::mat4(glm::mat3(CAMERA_get_view(camera)));

        shader_skybox->use();
        shader_skybox->uniform_mat4("u_view", glm::value_ptr(u_view_skybox));
        shader_skybox->uniform_mat4("u_projection", glm::value_ptr(u_projection_3D));
        skybox_system.render(shader_skybox);

        shader_mesh_textured->use();

        // 3D
        shader_mesh_textured->uniform_mat4("u_view", glm::value_ptr(u_view_3D));
        shader_mesh_textured->uniform_mat4("u_projection", glm::value_ptr(u_projection_3D));
        render_system_mesh.draw(shader_mesh_textured);

        // 2D
        shader_mesh_textured->uniform_mat4("u_view", glm::value_ptr(u_view_2D));
        shader_mesh_textured->uniform_mat4("u_projection", glm::value_ptr(u_projection_2D));
        render_system_sprite2D.draw(shader_mesh_textured);

        SDL_GL_SwapWindow(m_window);
}

void Engine::loop()
{
        int quit = 0;
        init();
        while (quit == 0) {
                quit += poll_events();
                input_update();
                quit += logic();
                render();
        }
        cleanup();
}

