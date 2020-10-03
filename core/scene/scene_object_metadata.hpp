#ifndef CORE__SCENE__SCENE_OBJECT_METADATA_HPP
#define CORE__SCENE__SCENE_OBJECT_METADATA_HPP

// Stores useful information about the management of an object within a scene
struct SceneObjectMetadata
{
    // ID of the object this metadata refers to
    unsigned int id;
    // ID of the parent object of the object this metadata refers to
    unsigned int parentId;
    // ID of the graph node containing the object
    unsigned int objectNodeId;
    // ID of the graph node containing the world transform of the object
    unsigned int transformNodeId;
    // ID of the graph node containing the update flag of the object
    unsigned int updateNodeId;
    // ID of the subscription to the transform notifier of the object
    unsigned int transformSubscriberId;
};

#endif//CORE__SCENE__SCENE_OBJECT_METADATA_HPP