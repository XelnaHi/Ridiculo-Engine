//
// Created by Teo on 2026-11-11.
//

#include "Renderer.h"

#include <array>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

#include "../custom_obj_loader/CustomMesh.h"
#include "../custom_obj_loader/OBJLoader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Shader.h"
#include "../model_loader/Model.h"
#include "glm/gtc/type_ptr.hpp"
#include "../GameState/GameState.h"
#include "../InputSystem/InputSystem.h"
#include "../Controller/Controller.h"
#include "../Player/Player.h"
#include "../Camera/Camera.h"
#include "../Levels/LevelManager.h"


int Renderer::drawCalls = 0;
std::map<std::string, Shader> Renderer::shaderMap;

bool Renderer::isPerspective = false;
bool Renderer::isProjectionDirty = true;

void Renderer::LoadObj(const std::string &path) {
    if (useCustomLoader) {
        OBJLoader::LoadMesh(path);
    } else {
        Model::Load(path);
    }
}

void Renderer::RenderObj(const std::string &name, Shader &shader, const glm::vec3 &pos, float rot,
                         const glm::vec3 &scale) {
    if (!useCustomLoader) {
        Model *model = Model::GetModel(name);
        if (!model) {
            std::cout << "[ERROR] Model not found: " << name << std::endl;
            return;
        }
        model->draw(shader, pos, rot, scale);
        return;
    }

    CustomMesh *mesh = OBJLoader::GetMesh(name);

    if (!mesh) {
        std::cout << "RenderObj: couldn't find mesh name: " << name << std::endl;
        return;
    }

    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot), glm::vec3(0, 1, 0));
    model = glm::scale(model, scale);

    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "u_model"), 1, GL_FALSE, glm::value_ptr(model));
    mesh->Render();
    drawCalls++;
}

void Renderer::InitShaders() {
    const Shader houseShader("shaders/2DHouse/vHouse.glsl", "shaders/2DHouse/fHouse.glsl");
    shaderMap.emplace("HouseShader", houseShader);

    const Shader suzanneShader("shaders/Week3/vSuzanne.glsl", "shaders/Week3/fSuzanne.glsl");
    shaderMap.emplace("SuzanneShader", suzanneShader);

    const Shader assimpTextureShader("shaders/Week3/TextureShaders/vAssimpTextureShader.glsl",
                                     "shaders/Week3/TextureShaders/fAssimpTextureShader.glsl");
    shaderMap.emplace("AssimpTextureShader", assimpTextureShader);

    const Shader combinedShader("shaders/Week3/TextureNormalCombinedShaders/vAssimpCombinedShader.glsl",
                                "shaders/Week3/TextureNormalCombinedShaders/fAssimpCombinedShader.glsl");
    shaderMap.emplace("CombinedShader", combinedShader);

    // Cache view for all shaders
    for (auto &[name, shader]: shaderMap) {
        shader.use();

        // Apply directional light components for all shaders
        // I was a little confused here, regarding the y-component in the directional light.
        // Remember to read this vector as directional vector. A positive y-value would mean that the light is
        // traveling upwards, which is generally not the case when we want to use a directional light which often
        // mimics how we perceive light coming from *the sun*.
        glUniform3f(glGetUniformLocation(shader.ID, "lightDirection"), -0.2f, -0.5f, -0.5f);
        glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), 0.5f, 1.0f, 1.0f);
    }
}

void Renderer::Render() {
    for (int i = 0; i < GameState::controllers.size(); i++) {
        int viewportWidth = SCR_WIDTH / GameState::controllers.size();

        glViewport(viewportWidth * i, 0, viewportWidth, SCR_HEIGHT);

        glm::mat4 projection = glm::mat4(1.0f);
        auto pos = glm::vec3(0.0f, 0.0f, 0.0f);
        auto scale = glm::vec3(0.5f, 0.5f, 0.5f);

        glm::mat4 view;
        view = GameState::controllers[i].player->camera->GetViewMatrix();


        for (auto &[name, shader]: shaderMap) {
            shader.use();
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "u_view"), 1, GL_FALSE, glm::value_ptr(view));
        }


        // Checks if we have pressed the keybind indicating that we want to switch projection modes
        if (isProjectionDirty) {
            if (!isPerspective) {
                projection = glm::perspective(glm::radians(45.0f), (float) viewportWidth / SCR_HEIGHT, 0.1f,
                                              100.0f);
            } else {
                float aspect = static_cast<float>(viewportWidth) / static_cast<float>(SCR_HEIGHT);
                float orthoHeight = 2.0f;
                float orthoWidth = orthoHeight * aspect;
                projection = glm::ortho(-orthoWidth / 2, orthoWidth / 2, -orthoHeight / 2, orthoHeight / 2, 0.1f,
                                        100.0f);
            }

            // Set the projection mode for all shaders. This is unlikely to change dynamically, so we want to refrain from setting this value over and
            // over unless necessary, thus we gate it behind the above if statement.
            for (auto &[name, shader]: shaderMap) {
                shader.use();
                // Apply projection for all shaders
                glUniformMatrix4fv(glGetUniformLocation(shader.ID, "u_projection"),
                                   1, GL_FALSE, glm::value_ptr(projection));
            }

            // Our input has been processed and is no longer considered "dirty" (dirty meaning we have a desired change that has not yet
            // been processed
            isProjectionDirty = false;
        }

        // Caching the uniform locations. I would like to have this done pre-render-loop, but I'm not sure how. If the shader program
        // is switched to another, the values have to be reset anyway, so I'm not sure if my desired "optimization" is actually viable

        // Set the matrices to identity matrices in order to give them clean slates. If we dont, we risk using old matrix values
        // which can skew our transformation matrices as we start applying translation, rotation and scale.

        // Local space = coordinate space local to the object
        // World space = coordinates relative to a shared game world origin
        // View space = often referred to as the camera. This is the result of transforming world space coordinates into coordinates that are in front of the users view.
        // Clip space = refers to the space within which fragments are eligible for rendering. Coordinates outside of this range are not rendered, ie clipped. Working with coordinates within the range [-1, 1] is generally considered unintuitive which is why we use a projection matrix to specify an easier-to-work-with range for each axis, ie [600, 800] which the projection matrix then converts to NDC coordinates. Coordinates outside of this custom range (for example [650, 550]) is thus mapped to be outside of the range [-1,1] and is subsequently not rendered on screen (clipped). The viewing box that projection creates is called a frustum. Once all vertices have been mapped to clip space the vertex shader performs the perspective division, meaning it divides the x, y and z components by the vector's homogenous w component. After this, the resulting coordinates are mapped to screen coordinates by using the dimensions set in glViewport(width, height) which then gets turned into fragments.

        // Orthographic projection = a cube-like frustum defined by a near-plane, far-plane, width, height and length. Coordinates in front of the near plane, and coordinates behind the far plane are clipped. This type of projection doesn't touch the w component which means that if the w remains = 1.0f, x, y and z components will remain intact during perspective division. This projection mode is usually used for 2D rendering.

        // Perspective projection = we can roughly describe perspective as objects appearing smaller the further away they are from us, as well as objects appearing as if coinciding, or "travelling towards"  a shared point in the distance. Perspective projection mimics this by increasing the value of the w component the further away an object is from the origin. When perspective division happens, this means that x, y and z coordinates are scaled based on the value of the w component. Perspective projection defines a clip space within the range of [-w, w]. Coordinates outside of this range fall outside of the mapped-to NDC coordinates, and are thus not rendered.

        // Similarly to orthographic projection, perspective projection creates a frustum with a near and far plane, with the caveat that this frustum is non-uniform (whereas the ortho proj is uniformly cube-like) and includes a field of view specified in radians or degrees. Furthermore, it uses an aspect ratio equal to width divided by height.


        // This is essentially our camera. After having applied the model matrix, the view matrix is applied, translating all objects 4 units away from the "camera", creating a sense of distance.

        // IMPORTANT MENTAL NOTE!!
        // I keep mixing up application of rotation, scale and translation when multiplying a transformation matrix with a vector, and the act of *building the transformation itself*. Depending on what order you multiply the rotation, scale and translation matrices in order to build the transformation matrix does have an impact on the final result due to matrices non-commutative nature.

        // Multiplying matrices in the following order; I * T * R * S can then be read as S (scale) being applied to the vector first, then R (rotation) and then T (translation). We get different "behaviors" depending on what order we multiply these matrices when building them. If we were to to R * T, instead of T * R, we would get an orbiting result, because we would first translate the object away from the origin, and then rotate it around the origin as opposed to first rotating it around the origin, and then translating it away from the origin.

            Shader &suzanneShader = shaderMap.at("SuzanneShader");

        // We only render the other character if we actually have two players in our controllers/playerinputs list.
        if (GameState::controllers.size() > 1) {
            if (i == 0) {
                pos = GameState::controllers[1].player->camera->Position;
                Camera *cam = GameState::controllers[1].player->camera;
                float yawDeg = glm::degrees(atan2(cam->Front.x, cam->Front.z));

                scale = glm::vec3(0.5f, 0.5f, 0.5f);
                RenderObj(LevelManager::activeLevel->playerInfoList[i].playerModel, suzanneShader, pos, yawDeg, scale);
            } else {
                pos = GameState::controllers[0].player->camera->Position;
                Camera *cam = GameState::controllers[0].player->camera;
                float yawDeg = glm::degrees(atan2(cam->Front.x, cam->Front.z));
                scale = glm::vec3(0.5f, 0.5f, 0.5f);
                RenderObj(LevelManager::activeLevel->playerInfoList[i].playerModel, suzanneShader, pos, yawDeg, scale);
            }

        }
        for (int i = 0; i < LevelManager::activeLevel->gameObjects.size(); ++i) {
            pos = LevelManager::activeLevel->gameObjects[i].position;
            scale = LevelManager::activeLevel->gameObjects[i].scale;
            RenderObj(LevelManager::activeLevel->gameObjects[i].modelPath, suzanneShader, pos, 0, scale);
        }


        // Usually, all render commands go to the back buffer, while the front buffer contains the final output image.
        // Since rendering isn't instant, but rather pixel by pixel, only rendering to a single buffer would cause those leftover render artifacts to be visible.
        // After all rendering commands are done, we swap the buffers.
    }
}
