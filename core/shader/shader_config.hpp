#ifndef CORE__SHADER__SHADER_CONFIG_HPP
#define CORE__SHADER__SHADER_CONFIG_HPP

#include <unordered_map>
#include <vector>

#include "shader_info.hpp"

namespace ShaderInfo
{
    /// @brief Holds the requested configuration for a shader prior to building it.
    class ShaderConfig
    {
        private:
            /// @brief List of literals describing the features which the shader 
            /// should provide.
            std::unordered_map<ShaderFeature, bool> _requestedFeatures;

            /// @brief List of the features conflicting with a feature which was
            /// to be added to the config. Used by checkForConflicts.
            std::vector<ShaderFeature> _detectedConflicts;

            /// @brief Check for conflicts between the currently registered 
            /// features and a new to-be-added feature. If a conflict is 
            /// detected, _detectedConflicts will be filled with literals 
            /// describing the conflicting features.
            ///
            /// @param newFeature Literal describing the new feature about to 
            /// be added to the config.
            ///
            /// @return Whether or not conflicts were detected.
            bool checkForConflicts(ShaderFeature newFeature);

        public:
            ShaderConfig();

            /// @brief Add a new feature to the list of currently requested 
            /// shader features.
            ///
            /// @param newFeature Literal describing the new feature to add to
            /// the config.
            ///
            /// @exception If the new feature is found to be incompatible with 
            /// one of the features already requested in the config, the 
            /// funciton will throw an std::runtime_error.
            void addFeature(ShaderFeature newFeature);

            /// @brief Add a new feature to the list of currently requested 
            /// shader features.
            ///
            /// @param feature Literal describing the feature to remove from
            /// the config.
            void removeFeature(ShaderFeature feature);

            /// @brief Get the list of requested shader features in the config.
            ///
            /// @return The list of requested shader features in the config.
            std::unordered_map<ShaderFeature, bool> getRequestedFeatures();
    };

    using IncompatibleFeaturesMap = std::unordered_map<ShaderFeature, std::vector<ShaderFeature>>;

    /// @brief Stores which features are incompatible together. One entry is 
    /// mapped to the list of features which it is incompatible with.
    const IncompatibleFeaturesMap IncompatibleFeatures;

    /// @brief Get a map describing incompatibilities between shader features.
    /// 
    /// @return A map describing incompatibilities between shader features.
    IncompatibleFeaturesMap getIncompatibleFeatures();
}

using ShaderConfig = ShaderInfo::ShaderConfig;

#endif//CORE__SHADER__SHADER_CONFIG_HPP