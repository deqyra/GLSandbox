#ifndef RENDERBOI__TOOLBOX__SCENE__SCENE_OBJECT_METADATA_HPP
#define RENDERBOI__TOOLBOX__SCENE__SCENE_OBJECT_METADATA_HPP

/// @brief Packs together information about the management of an object within
/// a scene.
struct SceneObjectMetadata
{
    /// @brief ID of the object this metadata refers to.
    unsigned int id;
    
    /// @brief ID of the parent object of the object this metadata refers to.
    unsigned int parentId;
    
    /// @brief ID of the graph node containing the object this metadata refers
    /// to.
    unsigned int objectNodeId;
    
    /// @brief ID of the graph node containing the world transform of the object
    /// this metadata refers to.
    unsigned int transformNodeId;
    
    /// @brief ID of the graph node containing the update flag of the object 
    /// this metadata refers to.
    unsigned int updateNodeId;
    
    /// @brief ID of the subscription to the transform notifier of the object 
    /// this metadata refers to.
    unsigned int transformSubscriberId;
};

#endif//RENDERBOI__TOOLBOX__SCENE__SCENE_OBJECT_METADATA_HPP