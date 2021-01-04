#ifndef RENDERBOI__CORE__SHADER__SHADER_CONFIG_HPP
#define RENDERBOI__CORE__SHADER__SHADER_CONFIG_HPP

#include <unordered_set>
#include <vector>

#include "shader_feature.hpp"

namespace Renderboi
{

/// @brief Holds the requested configuration for a shader prior to building it.
class ShaderConfig
{
private:
    /// @brief Set of literals describing the features which the shader 
    /// should provide.
    std::unordered_set<ShaderFeature> _requestedFeatures;

    /// @brief List of the features which were detected to be a problem
    /// upon adding a new feature. Used by _checkForConflicts and 
    /// _checkRequirementsAreMet.
    mutable std::vector<ShaderFeature> _problems;

    /// @brief Tells whether or not the config the last computed config
    /// vector is outdated, due to a recent change in the config state.
    mutable bool _configVectorOutdated;

    /// @brief Check for conflicts between the currently registered 
    /// features and a new to-be-added feature. If conflicts are 
    /// detected, _problems will be filled with literals describing
    /// those.
    ///
    /// @param newFeature Literal describing the new feature about to 
    /// be added to the config.
    ///
    /// @return Whether or not conflicts were detected.
    bool _checkForConflicts(const ShaderFeature newFeature) const;

    /// @brief Check that the config currently meets the requirements 
    /// for a new to-be-added feature. If requirements are unmet,
    /// _problems will be filled with literals describing those.
    ///
    /// @param newFeature Literal describing the new feature about to 
    /// be added to the config.
    ///
    /// @return Whether or not all requirements are met.
    bool _checkRequirementsAreMet(const ShaderFeature newFeature) const;

public:
    ShaderConfig();

    /// @brief Get a config object for the minimum functionality a 
    /// shader should provide.
    static const ShaderConfig& MinimalConfig();

    /// @brief Add a new feature to the list of currently requested 
    /// shader features.
    ///
    /// @param newFeature Literal describing the new feature to add to
    /// the config.
    ///
    /// @exception If the new feature is found to be incompatible with 
    /// one of the features already requested in the config, the 
    /// funciton will throw an std::runtime_error. If the requirements
    /// of the new feature are not met at the time this function is 
    /// called, it will throw an std::runtime_error.
    void addFeature(const ShaderFeature newFeature);

    /// @brief Add a new feature to the list of currently requested 
    /// shader features, and add any unmet requirements along with it.
    ///
    /// @param newFeature Literal describing the new feature to add to
    /// the config.
    ///
    /// @exception If the new feature is found to be incompatible with 
    /// one of the features already requested in the config, the 
    /// funciton will throw an std::runtime_error. If another feature 
    /// needs to be added as a requirement, and it is found to be 
    /// incompatible with one of the features already requested in the 
    /// config, the funciton will throw an std::runtime_error. 
    void addFeatureWithRequirements(const ShaderFeature newFeature);

    /// @brief Add a new feature to the list of currently requested 
    /// shader features.
    ///
    /// @param feature Literal describing the feature to remove from
    /// the config.
    void removeFeature(const ShaderFeature feature);

    /// @brief Get the list of requested shader features in the config.
    ///
    /// @return The list of requested shader features in the config.
    const std::vector<ShaderFeature>& getRequestedFeatures() const;

    /// @brief Get a map describing requirements between shader features.
    /// 
    /// @return A map describing requirements between shader features.
    static const std::unordered_map<ShaderFeature, std::vector<ShaderFeature>>& FeatureRequirements();

    /// @brief Get a map describing incompatibilities between shader features.
    /// 
    /// @return A map describing incompatibilities between shader features.
    static const std::unordered_map<ShaderFeature, std::vector<ShaderFeature>>& IncompatibleFeatures();
};

}//namespace Renderboi

#endif//RENDERBOI__CORE__SHADER__SHADER_CONFIG_HPP