#ifndef MATH_H
#define MATH_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

glm::mat4 createMat4(const glm::quat &rotation, const glm::vec4 &position);

void normalizeXYZ(float &x, float &y, float &z);

#endif
