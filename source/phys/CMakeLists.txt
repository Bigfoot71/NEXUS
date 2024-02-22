if(SUPPORT_PHYSICS_2D)
    set(NEXUS_SOURCES_PHYSICS_2D
        source/phys/2D/nxDrawing.cpp
        source/phys/2D/nxShapes.cpp
    )
endif()

if(SUPPORT_PHYSICS_3D)
    set(NEXUS_SOURCES_PHYSICS_3D
        source/phys/3D/rigid_objects/nxRigidObject.cpp
        source/phys/3D/rigid_objects/nxRigidCapsule.cpp
        source/phys/3D/rigid_objects/nxRigidSphere.cpp
        source/phys/3D/rigid_objects/nxRigidCube.cpp
        source/phys/3D/nxWorld.cpp
    )
endif()
