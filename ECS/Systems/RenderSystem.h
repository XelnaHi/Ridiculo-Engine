//
// Created by Teo on 2026-11-27.
//

#ifndef RIDICULOENGINE_DRAWSYSTEM_H
#define RIDICULOENGINE_DRAWSYSTEM_H
#include "System.h"
#include "../../Shader.h"
#include "../Components/ModelComponent.h"
#include "../Components/Transform.h"
#include "../Coordinator/Coordinator.h"
#include "../../model_loader/Model.h"

extern Coordinator gCoordinator;

class RenderSystem : public System {
public:
    void Draw(Shader& shader) {
        for (auto entity : mEntities) {
            auto& modelComp = gCoordinator.GetComponent<ModelComponent>(entity);
            auto & transform = gCoordinator.GetComponent<Transform>(entity);

            Model* model = Model::GetModel(modelComp.modelName);

            if (model) {
                model->draw(shader, transform.Position, 0.0f, transform.Scale );
            }

        }
    }
};

#endif //RIDICULOENGINE_DRAWSYSTEM_H
