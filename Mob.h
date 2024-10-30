#pragma once

#include <iostream>
#include <glm/glm.hpp>

class Mob {
	float vertices[180];
    glm::vec3 positions;
    bool use;
public:
    // �����������, ���������������� ������ ������ ����� ������ �������������
    Mob(float* verticesArray, glm::vec3 cubePositions) {
        positions = cubePositions;

        for (int i = 0; i < 180; ++i) {
            vertices[i] = verticesArray[i];
        }
        use = 1;
    }
    // ����� ��� ��������� ������� ������
    const float* getVertices() const {
        return vertices;
    }

    const glm::vec3 getPosition() const {
        return positions;
    }

    void kill() {
        use = 0;
    }

    bool getUse() {
        return use;
    }
};