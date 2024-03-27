#pragma once

#include <glrayfw/core/matrix2d.h>
#include <memory>
#include <vector>

namespace Render
{
    class Context;
}

namespace tdogl
{
    class Texture;
}

class b2ContactListener;
class Physics;
class Renderer;
class EntityManager;
class Camera;
class Transform;
class Scene;

class Engine
{
public:
    Engine(Render::Context* gl, int winWidth, int winHeight);

    const std::shared_ptr<Physics>& GetPhysics();
    void SetSlowDown(float slowdown);
    const std::shared_ptr<Renderer>& renderer();
    const std::shared_ptr<EntityManager>& emanager();
    const std::shared_ptr<Camera>& cam();
    std::shared_ptr<Transform> sceneRoot();
    Render::Context* gl();
    void SetGL(Render::Context* gl);
    float GetSlowdown();
    void LoadScene(Matrix2D tilemap);
    void SetSceneTextures(std::vector<tdogl::Texture*> texturesForTiles, tdogl::Texture* floorTexture, tdogl::Texture* roofTexture);
    void sceneRender(float viewer_angle);
    void Reset(b2ContactListener* contactListener);
    void PostUpdate();
    void PreUpdate(uint32_t delta);

private:
    void LoadCubesForScene();

    float m_slowDown = 1.0f;
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<EntityManager> m_emanager;
    std::shared_ptr<Physics> m_physics;
    std::shared_ptr<Camera> m_cam;
    std::shared_ptr<Transform> m_sceneRoot;
    Render::Context* m_gl;
    std::shared_ptr<Scene> m_scene;
    int m_winWidth = 0;
    int m_winHeight = 0;
    std::shared_ptr<Transform> m_unitTransform;
};
