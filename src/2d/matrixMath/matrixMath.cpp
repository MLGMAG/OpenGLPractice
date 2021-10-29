#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main() {

    /*
     * Translation vector
     */

    // start vector
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);

    // matrix 4x4 with 'one' on main line
    glm::mat4 trans = glm::mat4(1.0f);

    // translation matrix. m[0][3] = 1, m[1][3] = 1, m[2][3] = 0; And main line with 'one'.
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

    vec = trans * vec;

    std::cout << vec.x << vec.y << vec.z << std::endl;

}
