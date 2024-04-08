#include "engine.h"

#include "../render/renderer.h"
#include "../entity/entitymanager.h"
#include "../physics/physics.h"

Engine::Engine(Render::Context* gl, int winWidth, int winHeight, SDL_Window* window)
    : m_gl(gl), m_winWidth(winWidth), m_winHeight(winHeight), m_window(window)
{
}

const std::shared_ptr<Physics>& Engine::GetPhysics()
{
    return m_physics;
}

void Engine::SetSlowDown(float slowdown)
{
    m_slowDown = slowdown;
    m_renderer->SetSlowdown(slowdown);
}

const std::shared_ptr<Renderer>& Engine::renderer()
{
    return m_renderer;
}

const std::shared_ptr<EntityManager>& Engine::emanager()
{
    return m_emanager;
}

const std::shared_ptr<Camera>& Engine::cam()
{
    return m_cam;
}

std::shared_ptr<Transform> Engine::sceneRoot()
{
    return m_sceneRoot;
}

Render::Context* Engine::gl()
{
    return m_gl;
}

void Engine::SetGL(Render::Context* gl)
{
    m_gl = gl;
}

float Engine::GetSlowdown()
{
    return m_slowDown;
}

void Engine::LoadScene(Matrix2D tilemap)
{
    m_scene = std::make_shared<Scene>();
    m_scene->tilemap(tilemap);

    LoadCubesForScene();
    m_emanager->ClearAllEntities();
}

void Engine::SetSceneTextures(std::vector<tdogl::Texture*> texturesForTiles, tdogl::Texture* floorTexture, tdogl::Texture* roofTexture)
{
    for (int i = 0; i < texturesForTiles.size(); i++)
    {
        m_scene->setTextureForTile(i + 1, texturesForTiles[i]);
    }

    m_scene->setFloorTexture(floorTexture);
    m_scene->setRoofTexture(roofTexture);
}

void Engine::sceneRender(float viewer_angle)
{
    m_renderer->SceneRender(viewer_angle, *m_scene, m_emanager);
}

void Engine::Reset(b2ContactListener* contactListener)
{
    m_slowDown = 1.0f;
    m_cam = std::make_shared<Camera>();
    m_renderer = std::make_shared<Renderer>(m_gl, m_cam, m_winWidth, m_winHeight, m_window);
    m_emanager = std::make_shared<EntityManager>();
    m_physics = std::make_shared<Physics>(contactListener);
    m_cam->position(cml::vector3f(0, 0, 0));
    m_cam->horizontalAngle(90);
    m_sceneRoot = std::make_shared<Transform>();
    m_unitTransform = std::make_shared<Transform>();
}

void Engine::PostUpdate()
{
    // Clean dead entities
    m_sceneRoot->UpdateClean();
    m_emanager->ClearDeadEntities();
}

void Engine::PreUpdate(uint32_t delta)
{
    // Update everything
    
    m_sceneRoot->Update(*m_unitTransform, delta * m_slowDown);
    m_physics->Step(m_slowDown);
}

void Engine::LoadCubesForScene()
{
    auto tilemap = m_scene->tilemap();
    for (int i = 0; i < tilemap.Cols(); i++)
    {
        for (int j = 0; j < tilemap.Rows(); j++)
        {
            if (tilemap.Get(i, j) != 0)
                m_physics->AddCubeBody(-i * 2, -j * 2, 0);
        }
    }
}
