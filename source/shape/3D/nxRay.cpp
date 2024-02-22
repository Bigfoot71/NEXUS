#include "shape/3D/nxRay.hpp"

#include "shape/3D/nxCylinder.hpp"
#include "shape/3D/nxCapsule.hpp"
#include "shape/3D/nxSphere.hpp"
#include "shape/3D/nxPlane.hpp"
#include "shape/3D/nxMesh.hpp"
#include "shape/3D/nxCube.hpp"
#include "shape/3D/nxLine.hpp"
#include "shape/3D/nxAABB.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

shape3D::RayCollision shape3D::Ray::CollisionPoint(const nexus::math::Vec3& point) const
{
    if (position.Direction(point) == direction)
    {
        return { point, -direction, static_cast<float>(position.Distance(point)), true };
    }

    return { position, { 0, 0, 0 }, 0, false };
}

shape3D::RayCollision shape3D::Ray::CollisionAABB(const nexus::shape3D::AABB& aabb) const
{
    RayCollision collision{ position, {}, 0, false };;

    // Note: If ray.position is inside the box, the distance is negative (as if the ray was reversed)
    // Reversing ray.direction will give use the correct result.
    bool insideBox = (position.x > aabb.min.x) && (position.x < aabb.max.x) &&
                     (position.y > aabb.min.y) && (position.y < aabb.max.y) &&
                     (position.z > aabb.min.z) && (position.z < aabb.max.z);

    const math::Vec3 dir = insideBox ? -direction : direction;

    float t[11]{};

    t[8] = 1.0f / dir.x;
    t[9] = 1.0f / dir.y;
    t[10] = 1.0f / dir.z;

    t[0] = (aabb.min.x - position.x) * t[8];
    t[1] = (aabb.max.x - position.x) * t[8];
    t[2] = (aabb.min.y - position.y) * t[9];
    t[3] = (aabb.max.y - position.y) * t[9];
    t[4] = (aabb.min.z - position.z) * t[10];
    t[5] = (aabb.max.z - position.z) * t[10];

    t[6] = std::max(std::max(std::min(t[0], t[1]), std::min(t[2], t[3])), std::min(t[4], t[5]));
    t[7] = std::min(std::min(std::max(t[0], t[1]), std::max(t[2], t[3])), std::max(t[4], t[5]));

    collision.hit = !(t[7] < 0 || t[6] > t[7]);
    collision.distance = t[6];
    collision.point = position + (dir * collision.distance);

    // Get box center point
    collision.normal = (aabb.min + aabb.max) * 0.5f;

    // Get vector center point->hit point
    collision.normal = collision.point - collision.normal;

    // Scale vector to unit cube
    // NOTE: We use an additional .01 to fix numerical errors
    collision.normal = collision.normal * 2.01f;
    collision.normal = collision.normal / (aabb.max - aabb.min);

    // The relevant elements of the vector are now slightly larger than 1.0f (or smaller than -1.0f)
    // and the others are somewhere between -1.0 and 1.0 casting to int is exactly our wanted normal!
    collision.normal.x = std::floor(collision.normal.x);
    collision.normal.y = std::floor(collision.normal.y);
    collision.normal.z = std::floor(collision.normal.z);

    collision.normal.Normalize();

    if (insideBox)
    {
        // Fix result
        collision.distance = -collision.distance;
        collision.normal = -collision.normal;
    }

    return collision;
}

shape3D::RayCollision shape3D::Ray::CollisionCube(const nexus::shape3D::Cube& cube) const
{
    return this->CollisionAABB(cube.GetAABB());
}

shape3D::RayCollision shape3D::Ray::CollisionPlane(const nexus::shape3D::Plane& plane) const
{
    return this->CollisionAABB(plane.GetAABB());
}

shape3D::RayCollision shape3D::Ray::CollisionSphere(const nexus::shape3D::Sphere& sphere) const
{
    RayCollision collision{ position, {}, 0, false };;

    math::Vec3 raySpherePos = sphere.center - position;
    float vector = raySpherePos.Dot(direction);
    float distance = raySpherePos.Length();

    float d = sphere.radius * sphere.radius - (distance * distance - vector * vector);

    collision.hit = d >= 0.0f;

    // Check if ray origin is inside the sphere to calculate the correct collision point
    if (distance < sphere.radius)
    {
        collision.distance = vector + sqrtf(d);

        // Calculate collision point
        collision.point = position + (direction * distance);

        // Calculate collision normal (pointing outwards)
        collision.normal = -(collision.point - sphere.center).Normalized();
    }
    else
    {
        collision.distance = vector - sqrtf(d);

        // Calculate collision point
        collision.point = position + (direction * collision.distance);

        // Calculate collision normal (pointing inwards)
        collision.normal = (collision.point - sphere.center).Normalized();
    }

    return collision;
}

shape3D::RayCollision shape3D::Ray::CollisionCapsule(const nexus::shape3D::Capsule& capsule) const
{
    const math::Vec3 capsCenter = capsule.GetCenter();
    const float capsHeight = capsule.GetHeight();

    // Calculate the collision with the top hemisphere
    const Sphere topSphere(capsCenter + math::Vec3(0, capsHeight * 0.5f, 0), capsule.radius);
    const RayCollision topCollision = this->CollisionSphere(topSphere);

    // Calculate the collision with the bottom hemisphere
    const Sphere bottomSphere(capsCenter - math::Vec3(0, capsHeight * 0.5f, 0), capsule.radius);
    const RayCollision bottomCollision = this->CollisionSphere(bottomSphere);

    // Calculate the collision with the cylindrical part of the capsule
    const Cylinder cylinder(capsCenter, capsule.radius, capsHeight);
    const RayCollision cylinderCollision = this->CollisionCylinder(cylinder);

    // Check which collision is the closest
    RayCollision closestCollision;
    if (topCollision.hit && (!bottomCollision.hit || topCollision.distance < bottomCollision.distance))
    {
        closestCollision = topCollision;
    }
    else if (bottomCollision.hit && (!topCollision.hit || bottomCollision.distance < topCollision.distance))
    {
        closestCollision = bottomCollision;
    }
    else
    {
        closestCollision = cylinderCollision;
    }

    return closestCollision;
}

shape3D::RayCollision shape3D::Ray::CollisionCylinder(const nexus::shape3D::Cylinder& cylinder) const
{
    RayCollision collision{ position, {}, 0, false };

    // Calculate the direction vector of the cylinder axis
    math::Vec3 cylinderAxis = math::Vec3(0, 1, 0); // Assuming the cylinder is aligned with the y-axis
    math::Vec3 cylinderTop = cylinder.center + cylinderAxis * (cylinder.height * 0.5f);
    math::Vec3 cylinderBottom = cylinder.center - cylinderAxis * (cylinder.height * 0.5f);

    // Calculate the vector from the cylinder center to the ray origin
    math::Vec3 rayToCenter = cylinder.center - position;

    // Calculate the projection of the ray-to-center vector onto the cylinder axis
    float projection = rayToCenter.Dot(cylinderAxis);

    // Calculate the closest point on the cylinder axis to the ray
    math::Vec3 closestPoint;
    if (projection <= -cylinder.height * 0.5f)
    {
        closestPoint = cylinderBottom;
    }
    else if (projection >= cylinder.height * 0.5f)
    {
        closestPoint = cylinderTop;
    }
    else
    {
        closestPoint = cylinder.center + cylinderAxis * projection;
    }

    // Calculate the vector from the closest point on the cylinder axis to the ray origin
    math::Vec3 closestPointToRayOrigin = position - closestPoint;

    // Calculate the distance between the closest point on the cylinder axis and the ray origin
    float distanceToAxis = closestPointToRayOrigin.Length();

    // Check if the closest point is within the cylinder radius
    if (distanceToAxis <= cylinder.radius)
    {
        // Calculate the collision point on the cylinder surface
        math::Vec3 collisionPoint = closestPoint + cylinderAxis * (direction.Dot(closestPointToRayOrigin));

        // Calculate the distance between the ray origin and the collision point
        float distanceToCollision = (collisionPoint - position).Length();

        // Check if the collision point is within the ray length
        if (distanceToCollision <= direction.Length())
        {
            collision.hit = true;
            collision.distance = distanceToCollision;
            collision.point = collisionPoint;
            collision.normal = (collision.point - cylinder.center).Normalized();
        }
    }

    return collision;
}

shape3D::RayCollision shape3D::Ray::CollisionRay(nexus::shape3D::Ray& ray) const
{
    RayCollision collision{ position, {}, 0, false };

    // Calculate direction of the other ray
    const math::Vec3 otherRayDir = ray.direction;

    // Check if the rays are parallel
    float crossProduct = direction.Cross(otherRayDir).Length();

    // Rays are parallel, no intersection
    if (crossProduct < 1e-5f) return collision;

    // Calculate vector between ray origins
    const math::Vec3 rayVector = ray.position - position;

    // Calculate parameters for both rays
    const float t = rayVector.Cross(otherRayDir).Dot(otherRayDir.Cross(direction)) / crossProduct;

    const float u = rayVector.Cross(direction).Dot(otherRayDir.Cross(direction)) / crossProduct;

    // Check if the intersection occurs within both rays
    if (t >= 0 && u >= 0 && u <= 1)
    {
        collision.hit = true;
        collision.distance = t;
        collision.point = position + direction * t;

        // Calculate collision normal as the cross product of the direction vectors
        collision.normal = direction.Cross(otherRayDir).Normalized();
    }

    return collision;
}

shape3D::RayCollision shape3D::Ray::CollisionLine(const nexus::shape3D::Line& line) const
{
    // Convert the line to a ray
    Ray lineRay(line.start, line.start.Direction(line.end));

    // Use the CollisionRay function to check for intersection between the ray and the line
    RayCollision collision = this->CollisionRay(lineRay);

    // Check if the collision point is within the line segment
    if (collision.hit)
    {
        // Calculate the length of the line segment
        float lineLength = line.start.Distance(line.end);

        // Check if the collision distance is within the line segment
        if (collision.distance >= 0 && collision.distance <= lineLength)
        {
            // Collision occurred within the line segment, return the collision information
            return collision;
        }
        else
        {
            // Collision occurred outside the line segment, set hit to false
            collision.hit = false;
        }
    }

    return collision;
}

shape3D::RayCollision shape3D::Ray::CollisionMesh(const nexus::shape3D::Mesh& mesh, const math::Mat4& transform) const
{
    RayCollision collision{ position, {}, 0, false };

    // Test against all triangles in mesh
    for (Uint32 i = 0; i < mesh.vertices.size(); i++)
    {
        math::Vec3 v0, v1, v2;

        if (!mesh.indices.empty())
        {
            v0 = mesh.vertices[mesh.indices[i * 3 + 0]].Transformed(transform);
            v1 = mesh.vertices[mesh.indices[i * 3 + 1]].Transformed(transform);
            v2 = mesh.vertices[mesh.indices[i * 3 + 2]].Transformed(transform);
        }
        else
        {
            v0 = mesh.vertices[i * 3 + 0].Transformed(transform);
            v1 = mesh.vertices[i * 3 + 1].Transformed(transform);
            v2 = mesh.vertices[i * 3 + 2].Transformed(transform);
        }

        RayCollision triHitInfo = CollisionTriangle(v0, v1, v2);

        if (triHitInfo.hit)
        {
            // Save the closest hit triangle
            if (!collision.hit || collision.distance > triHitInfo.distance) collision = triHitInfo;
        }
    }

    return collision;
}

shape3D::RayCollision shape3D::Ray::CollisionTriangle(const math::Vec3& v0, const math::Vec3& v1, const math::Vec3& v2) const
{
    RayCollision collision{ position, {}, 0, false };

    // Find vectors for two edges sharing V1
    math::Vec3 edge1 = v1 - v0;
    math::Vec3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate u parameter
    math::Vec3 p = direction.Cross(edge2);

    // If determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
    float det = edge1.Dot(p);

    // Avoid culling!
    if (det > -1e-5f && det < 1e-5f) return collision;

    float invDet = 1.0f / det;

    // Calculate distance from V1 to ray origin
    math::Vec3 tv = position - v0;

    // Calculate u parameter and test bound
    float u = tv.Dot(p) * invDet;

    // The intersection lies outside the triangle
    if ((u < 0.0f) || (u > 1.0f)) return collision;

    // Prepare to test v parameter
    math::Vec3 q = tv.Cross(edge1);

    // Calculate V parameter and test bound
    float v = direction.Dot(q) * invDet;

    // The intersection lies outside the triangle
    if (v < 0.0f || u + v > 1.0f) return collision;

    float t = edge2.Dot(q) * invDet;

    if (t > 1e-5f)
    {
        // Ray hit, get hit point and normal
        collision.hit = true;
        collision.distance = t;
        collision.normal = edge1.Cross(edge2).Normalized();
        collision.point = position + (direction * t);
    }

    return collision;
}

shape3D::RayCollision shape3D::Ray::CollisionQuad(const math::Vec3& v0, const math::Vec3& v1, const math::Vec3& v2, const math::Vec3& v3) const
{
    RayCollision collision = CollisionTriangle(v0, v1, v3);
    if (!collision.hit) collision = CollisionTriangle(v1, v2, v3);
    return collision;
}
