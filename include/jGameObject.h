#pragma once

#include "jModel.h"

#include <memory>

namespace HUAR
{
struct Transform2dComponent{
    glm::vec2 translation{};
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    glm::mat2 mat2(){
        const float s = glm::sin(rotation);
        const float c = glm::cos(rotation);
        glm::mat2 rotMatrix{{c,s},{-s,c}};  
        glm::mat2 scaleMat{{scale.x, 0.f}, {0.f, scale.y}};

        return rotMatrix * scaleMat;
    }
};

class JinGameObject{
public:
    using id_t = unsigned int;
    static JinGameObject createGameObject(){
        static id_t currentId = 0;
        return JinGameObject{currentId++};
    }

    JinGameObject(const JinGameObject &) = delete;
    JinGameObject &operator=(const JinGameObject &) = delete;   
    JinGameObject(JinGameObject &&) = default;
    JinGameObject &operator=(JinGameObject &&) = default;

    id_t getId() const {return id;}

    std::shared_ptr<JinModel> model{};
    glm::vec3 color;
    Transform2dComponent transform2d{};

private:
    JinGameObject(id_t objId): id{objId}{}
    id_t id;
};
}