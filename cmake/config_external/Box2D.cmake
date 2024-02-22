# Defini le repertoire racine de Box2D
set(BOX2D_ROOT_PATH ${NEXUS_ROOT_PATH}/external/box2d)

# Liste tous les fichiers source de Box2D
set(BOX2D_SOURCE_FILES
    ${BOX2D_ROOT_PATH}/src/collision/b2_broad_phase.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_chain_shape.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_circle_shape.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_collide_circle.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_collide_edge.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_collide_polygon.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_collision.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_distance.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_dynamic_tree.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_edge_shape.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_polygon_shape.cpp
    ${BOX2D_ROOT_PATH}/src/collision/b2_time_of_impact.cpp
    ${BOX2D_ROOT_PATH}/src/common/b2_block_allocator.cpp
    ${BOX2D_ROOT_PATH}/src/common/b2_draw.cpp
    ${BOX2D_ROOT_PATH}/src/common/b2_math.cpp
    ${BOX2D_ROOT_PATH}/src/common/b2_settings.cpp
    ${BOX2D_ROOT_PATH}/src/common/b2_stack_allocator.cpp
    ${BOX2D_ROOT_PATH}/src/common/b2_timer.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_body.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_chain_circle_contact.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_chain_circle_contact.h
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_chain_polygon_contact.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_chain_polygon_contact.h
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_circle_contact.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_circle_contact.h
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_contact.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_contact_manager.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_contact_solver.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_contact_solver.h
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_distance_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_edge_circle_contact.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_edge_circle_contact.h
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_edge_polygon_contact.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_edge_polygon_contact.h
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_fixture.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_friction_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_gear_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_island.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_island.h
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_motor_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_mouse_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_polygon_circle_contact.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_polygon_circle_contact.h
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_polygon_contact.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_polygon_contact.h
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_prismatic_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_pulley_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_revolute_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_weld_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_wheel_joint.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_world.cpp
    ${BOX2D_ROOT_PATH}/src/dynamics/b2_world_callbacks.cpp
    ${BOX2D_ROOT_PATH}/src/rope/b2_rope.cpp)

# Ajout des sources de Box2D aux sources exertnes de 'nexus'
list(APPEND NEXUS_EXTERNAL_SOURCES ${BOX2D_SOURCE_FILES})

# Ajout du repertoire d'inclusion de Box2D dans la variable des inclusions
list(APPEND NEXUS_EXTERNAL_INCLUDES ${BOX2D_ROOT_PATH}/include)
