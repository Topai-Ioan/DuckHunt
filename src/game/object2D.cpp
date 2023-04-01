#include "game/object2D.h"


#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "object2D.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D::CreateRectangle(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float lungime,
    float latime,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(latime, 0, 0), color),
        VertexFormat(corner + glm::vec3(latime, lungime, 0), color),
        VertexFormat(corner + glm::vec3(0, lungime, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D::CreateTriangle(
    const std::string& name,
    glm::vec3 triangleApex,
    float base,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = triangleApex;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(base, 0, 0), color),
        VertexFormat(corner + glm::vec3(base/2,height, 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}
Mesh* object2D::Create90Triangle(
    const std::string & name,
    glm::vec3 triangleApex,
    float base,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = triangleApex;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(base, 0, 0), color),
        VertexFormat(corner + glm::vec3(0,base, 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    int circleLines,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    for (int i = 0; i < circleLines; i++)
    {
        vertices
            .push_back(VertexFormat(glm::vec3(1 * cos(2 * M_PI / circleLines * i), 1 * sin(2 * M_PI / circleLines * i), 0), color));
    }


    std::vector<unsigned int> indices;

    for (int i = 0; i <= circleLines; i++)
    {
        indices.push_back(i);
    }

    
    Mesh* circle = new Mesh(name);

    circle->SetDrawMode(GL_TRIANGLE_FAN);


    circle->InitFromData(vertices, indices);
    return circle;
}
