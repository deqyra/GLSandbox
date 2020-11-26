#include "shader_builder.hpp"

#include <algorithm>
#include <cstdarg>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <sstream>

#include "shader_info.hpp"
#include "shader_feature.hpp"
#include "shader_stage.hpp"
#include <cpptools/string_tools.hpp>

#define INFO_BUFFER_SIZE 2048

ShaderBuilder::NamedStringToLoadStatusMap ShaderBuilder::_namedStringLoadStatus = ShaderBuilder::NamedStringToLoadStatusMap();
const unsigned int ShaderBuilder::shadingLanguageVersion = 420;
const std::string ShaderBuilder::shadingLanguageProfile = "core";

ShaderProgram ShaderBuilder::MinimalShaderProgram()
{
    static ShaderProgram minimal = buildShaderProgramFromConfig(ShaderConfig::MinimalConfig());
    return minimal;
}

ShaderProgram ShaderBuilder::buildShaderProgramFromConfig(ShaderConfig config)
{
    const std::vector<ShaderInfo::ShaderFeature>& features = config.getRequestedFeatures();
    std::unordered_set<ShaderInfo::ShaderStage> requestedStages;

    // Find out which shader stage were requested in the features
    for (auto it = features.begin(); it != features.end(); it++)
    {
        auto jt = ShaderInfo::FeatureStages().find(*it);
        if (jt == ShaderInfo::FeatureStages().end())
        {
            std::string s = "ShaderBuilder: cannot build shader program from config, unknown feature \"" + 
                            std::to_string(*it) + "\" (" + std::to_string((unsigned int) *it) + ") was requested.";
            throw std::runtime_error(s.c_str());
        }

        ShaderInfo::ShaderStage stage = jt->second;
        requestedStages.insert(stage);
    }

    std::vector<Shader> shaders;
    // Generate all shader stages
    for (auto it = requestedStages.begin(); it != requestedStages.end(); it++)
    {
        Shader shader = buildShaderStageFromConfig(*it, config);
        shaders.push_back(shader);
    }

    // Link them together and return
    return linkShaders(shaders);
}

ShaderProgram ShaderBuilder::linkShaders(std::vector<Shader> shaders)
{
    std::unordered_set<ShaderInfo::ShaderStage> stageCount;
    std::vector<unsigned int> locations;

    // Check if there is no more than one shader per stage
    for (auto it = shaders.begin(); it != shaders.end(); it++)
    {
        ShaderInfo::ShaderStage stage = it->stage();
        auto jt = stageCount.find(stage);
        if (jt == stageCount.end())
        {
            stageCount.insert(stage);
            // Store locations aside
            locations.push_back(it->location());
        }
        else
        {
            std::string s = "ShaderBuilder: cannot link shaders, several objects were provided for stage \"" + std::to_string(stage) + "\".";
            throw std::runtime_error(s.c_str());
        }        
    }

    // Check if there is at least a vertex shader and a fragment shader
    if (stageCount.find(ShaderInfo::ShaderStage::Vertex) == stageCount.end())
    {
        std::string s = "ShaderBuilder: cannot link shaders, a vertex shader is required but none was provided.";
        throw std::runtime_error(s.c_str());
    }
    if (stageCount.find(ShaderInfo::ShaderStage::Fragment) == stageCount.end())
    {
        std::string s = "ShaderBuilder: cannot link shaders, a fragment shader is required but none was provided.";
        throw std::runtime_error(s.c_str());
    }

    // Do the linking
    unsigned int programLocation = makeShaderProgram(locations);
    if (!programLocation)
    {
        throw std::runtime_error("ShaderBuilder: the provided shaders could not be linked together. More info was printed at std::cerr.");
    }

    // Aggregate supported features
    std::vector<ShaderInfo::ShaderFeature> supportedFeatures = aggregateShaderFeatures(shaders);

    // Return the shader instance
    return ShaderProgram(programLocation, supportedFeatures);
}

Shader ShaderBuilder::buildShaderStageFromConfig(ShaderInfo::ShaderStage stage, ShaderConfig config)
{
    std::vector<ShaderInfo::ShaderFeature> requested = filterFeaturesByStage(config.getRequestedFeatures(), stage);

    auto it = StageTemplatePaths().find(stage);
    if (it == StageTemplatePaths().end())
    {
        std::string s = "ShaderBuilder: cannot find template path for stage \"" + std::to_string(stage) + "\" (" + std::to_string(unsigned int (stage)) + ").";
        throw std::runtime_error(s.c_str());
    }

	// Open template 
	std::ifstream file(it->second);

	if (!file.is_open())
    {
		std::string s = "ShaderBuilder: Shader template \"" + it->second + "\" could not be found.";
        throw std::runtime_error(s.c_str());
    }

	std::string source;
    // Add directives for version and extension usage, and define macros for requested features
    source += GenerateVersionDirective()
            + GenerateExtensionDirectives()
            + GenerateDefineDirectives(requested);

    // Add all file contents
	std::string line;
	while (std::getline(file, line))
		source += line + "\n";

    return buildShaderStageFromText(stage, source, requested);
}

Shader ShaderBuilder::buildShaderStageFromFile(ShaderInfo::ShaderStage stage, std::string filename, std::vector<ShaderInfo::ShaderFeature> supportedFeatures)
{
	// Open input file 
	std::ifstream file(filename);

	if (!file.is_open())
    {
		std::string s = "ShaderBuilder: Shader source \"" + filename + "\" could not be found.";
        throw std::runtime_error(s.c_str());
    }

    // Read all of its contents
	std::string source("");
	std::string line;
	while (std::getline(file, line))
		source += line + "\n";

    // If no info was provided about supported features, try to find some
    if (!supportedFeatures.size())
    {
        auto it = ShaderInfo::FeaturesSupportedByFile().find(filename);
        if (it != ShaderInfo::FeaturesSupportedByFile().end())
            supportedFeatures = it->second;
    }

    return buildShaderStageFromText(stage, source, supportedFeatures);
}

Shader ShaderBuilder::buildShaderStageFromText(ShaderInfo::ShaderStage stage, const std::string& text, std::vector<ShaderInfo::ShaderFeature> supportedFeatures)
{
    // In case the shader makes use of #include directives, process them
    processIncludeDirectives(text);

    // Get GL macro corresponding to requested shader stage
    auto it = ShaderInfo::StageMacros().find(stage);
    if (it == ShaderInfo::StageMacros().end())
    {
        std::string s = "ShaderBuilder: Unknown requested shader stage \"" + std::to_string(stage) + "\" (" + std::to_string((unsigned int) stage) + ").";
        throw std::runtime_error(s.c_str());
    }
    unsigned int shaderType = ShaderInfo::StageMacros().at(stage);

	// Compile into shader
	const char* source = text.c_str();
	unsigned int location;
	location = glCreateShader(shaderType);
	glShaderSource(location, 1, &source, nullptr);
	glCompileShaderIncludeARB(location, 0, nullptr, nullptr);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetShaderiv(location, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(location, INFO_BUFFER_SIZE, nullptr, info);
        std::string filename = dumpSource(stage, text);
		std::cerr << "Shader compilation failed:\n" << info << '\n';
        std::cerr << "Source was dumped to " << filename << std::endl;
		throw std::runtime_error("Shader compilation failed. See std::cerr for more info.");
	}

	return Shader(location, stage, supportedFeatures);
}

// There must be `count` arguments after `count`, all of type `unsigned int`.
unsigned int ShaderBuilder::makeShaderProgram(std::vector<unsigned int> locations)
{
	unsigned int program = glCreateProgram();

	for (auto it = locations.begin(); it != locations.end(); it++)
	{
		glAttachShader(program, *it);
	}

	// Link all shaders
	glLinkProgram(program);

	// Print errors if any
	int success;
	char info[INFO_BUFFER_SIZE];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, INFO_BUFFER_SIZE, NULL, info);
		std::cerr << "Shader linking failed:\n" << info << std::endl;
		return 0;
	}

	return program;
}

void ShaderBuilder::processIncludeDirectives(const std::string& text)
{
    std::unordered_set<std::string> includeDirectives = findIncludeDirectivesInSource(text, true);
    
    for (auto it = includeDirectives.begin(); it != includeDirectives.end(); it++)
    {
        // Find whether the named string for the requested include directive is loaded
        auto jt = _namedStringLoadStatus.find(*it);
        // If loaded already, skip
        if (jt != _namedStringLoadStatus.end() && jt->second == true) continue;

        // Find the source file needed to create the requested named string
        auto kt = ShaderInfo::IncludeFilenames().find(*it);
        // If info about the requested include cannot be found, throw
        if (kt == ShaderInfo::IncludeFilenames().end())
        {
            std::string s = "Shader: Info about include directive <" + *it + "> cannot be found.";
            throw std::runtime_error(s.c_str());
        }

        // Otherwise, create the named string
        makeNamedString(kt->first, kt->second);
    }
}

std::unordered_set<std::string> ShaderBuilder::findIncludeDirectivesInSource(std::string text, bool recursive)
{
    static const std::string includeStringStart = "#include";
    std::unordered_set<std::string> includeDirectives;

    // Remove comments from source
    StringTools::stripComments(text);
    // Find include directives
    size_t offset = text.find(includeStringStart, 0);
    while (offset != std::string::npos)
    {
        // Find the last EOL before the current include directive position
        size_t lastEol = text.find_last_of('\n', offset);
        if (lastEol == std::string::npos) lastEol = 0;
        // Get the substring from the last EOL to the current directive position
        std::string before = text.substr(lastEol, offset - lastEol);
        // If there are non whitespace chars before the include directive, skip
        if (!StringTools::stringIsWhitespace(before)) continue;

        // Add size of the directive head to the offset
        offset += includeStringStart.size();
        // Find the next EOL
        size_t eol = text.find('\n', offset);

        // Length of the string starting after the directive head,
        // going down to the next EOL, or end of the string altogether.
        size_t len = std::string::npos;
        if (eol != std::string::npos)
        {
            len = eol - offset;
        }

        // Get the string after the directive head until the next EOL
        std::string includeArgument = text.substr(offset, len);
        // Remove all leading or trailing whitespace
        StringTools::trim(includeArgument);

        // Check delimiter consistency
        char firstDelimiter = includeArgument[0];
        char secondDelimiter = 0;
        switch (firstDelimiter)
        {
            case '<': secondDelimiter = '>'; break;
            case '"': secondDelimiter = '"'; break;
        }

        // If the last delimiter doesn't match the first, throw
        if (secondDelimiter == 0 || secondDelimiter != includeArgument.back())
        {
            throw std::runtime_error("Badly formatted #include directive: argument \"" + includeArgument + "\" is illegal.");
        }

        // Strip delimiting chars
        std::string filename = includeArgument.substr(1, includeArgument.size() - 2);

        // If any delimiting chars are left in the string after stripping, that makes the argument illegal
        if ((filename.find(secondDelimiter) != std::string::npos) || (filename.find(firstDelimiter) != std::string::npos))
        {
            throw std::runtime_error("Badly formatted #include directive: argument \"" + filename + "\" is illegal.");
        }
        includeDirectives.insert(filename);

        offset = text.find(includeStringStart, offset);
    }

    if (recursive)
    {
        // Process potential include directives in all files found to be included
        for (auto it = includeDirectives.begin(); it != includeDirectives.end(); it++)
        {
            // Retrieve filename for include directive
            auto jt = ShaderInfo::IncludeFilenames().find(*it);
            if (jt == ShaderInfo::IncludeFilenames().end())
            {
                std::string s = "ShaderBuilder: missing info about include directive \"" + *it + "\", cannot recursively find include filenames.";
                throw std::runtime_error(s.c_str());
            }

            // Read file
            std::string all = StringTools::readFileIntoString(jt->second);
            // Process include contained in it
            std::unordered_set<std::string> newIncludeDirectives = findIncludeDirectivesInSource(all, true);

            // Add them all to the current include directives
            includeDirectives.merge(newIncludeDirectives);
        }
    }

    return includeDirectives;
}

void ShaderBuilder::makeNamedString(std::string name, std::string sourceFilename)
{
    std::string all = StringTools::readFileIntoString(sourceFilename);
	const char* source = all.c_str();

    // Create the named string
    glNamedStringARB(GL_SHADER_INCLUDE_ARB, -1, name.c_str(), -1, source);

    // Update load status
    _namedStringLoadStatus[name] = true;
}

std::vector<ShaderInfo::ShaderFeature> ShaderBuilder::aggregateShaderFeatures(const std::vector<Shader>& shaders)
{
    std::vector<ShaderInfo::ShaderFeature> supportedFeatures;
    for (auto it = shaders.begin(); it != shaders.end(); it++)
    {
        const std::vector<ShaderInfo::ShaderFeature>& features = it->getSupportedFeatures();
        std::copy(features.begin(), features.end(), std::back_inserter(supportedFeatures));
    }

    return supportedFeatures;
}

std::vector<ShaderInfo::ShaderFeature> ShaderBuilder::filterFeaturesByStage(const std::vector<ShaderInfo::ShaderFeature>& features, ShaderInfo::ShaderStage stage)
{
    std::vector<ShaderInfo::ShaderFeature> result;

    std::function<bool(ShaderInfo::ShaderFeature)> filter = [stage](ShaderInfo::ShaderFeature v) -> bool
    {
        auto it = ShaderInfo::FeatureStages().find(v);
        if (it == ShaderInfo::FeatureStages().end()) return false;
        return it->second == stage;
    };

    std::copy_if(features.begin(), features.end(), std::inserter(result, result.begin()), filter);

    return result;
}

std::string ShaderBuilder::dumpSource(ShaderInfo::ShaderStage stage, const std::string& text)
{
    static std::string dumpFolder = "output/";
    std::filesystem::create_directories(dumpFolder);

    std::time_t t = std::time(nullptr);
    std::tm tm = {};
    ::localtime_s(&tm, &t);

    std::stringstream ss;
    ss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");

    const ShaderInfo::ShaderStageFileExtensionMap& fileExtensions = ShaderInfo::StageFileExtensions();
    std::string filename = dumpFolder + std::to_string(stage) + "_shader_dump_" + ss.str() + '.' + fileExtensions.at(stage);

    std::ofstream file;
    file.open(filename, std::ios::out);
    file << text;
    file.close();

    return std::filesystem::absolute(filename).string();
}

const std::string& ShaderBuilder::GenerateVersionDirective()
{
    static std::string s = "#version " + std::to_string(shadingLanguageVersion) + " " + shadingLanguageProfile + "\n";
    return s;
}

const std::string& ShaderBuilder::GenerateExtensionDirectives()
{
    static bool runOnce = false;
    static std::string s = "";

    if (!runOnce)
    {
        std::function<std::string(std::string, std::pair<std::string, std::string>)> addDirective = [](std::string aggregate, std::pair<std::string, std::string> tuplet) -> std::string
        {
            return aggregate + "#extension " + tuplet.first + " : " + tuplet.second + "\n";
        };

        const std::unordered_map<std::string, std::string>& extensions = ShadingLanguageExtensions(); 
        s = std::accumulate(extensions.cbegin(), extensions.cend(), (std::string)(""), addDirective);
        runOnce = true;
    }

    return s;
}

const std::unordered_map<std::string, std::string>& ShaderBuilder::ShadingLanguageExtensions()
{
    static bool runOnce = false;
    static std::unordered_map<std::string, std::string> map;
    if (!runOnce)
    {
        map["GL_ARB_shading_language_include"] = "require";

        runOnce = true;
    }

    return map;
}

const std::unordered_map<ShaderInfo::ShaderStage, std::string>& ShaderBuilder::StageTemplatePaths()
{
    static bool runOnce = false;
    static std::unordered_map<ShaderInfo::ShaderStage, std::string> map;

    if (!runOnce)
    {
        map[ShaderInfo::ShaderStage::Vertex]    = "assets/shaders/templates/vertex_shader.vert";
        map[ShaderInfo::ShaderStage::Geometry]  = "assets/shaders/templates/geometry_shader.geom";
        map[ShaderInfo::ShaderStage::Fragment]  = "assets/shaders/templates/fragment_shader.frag";

        runOnce = true;
    }

    return map;
}

const std::unordered_map<ShaderInfo::ShaderFeature, std::string>& ShaderBuilder::FeatureDefineMacros()
{
    static bool runOnce = false;
    static std::unordered_map<ShaderInfo::ShaderFeature, std::string> map;

    if (!runOnce)
    {
        map[ShaderInfo::ShaderFeature::VertexMVP]                   = "VERTEX_MVP";
        // map[ShaderInfo::ShaderFeature::VertexFishEye]               = "VERTEX_FISH_EYE";        // IMPLEMENT VERT LENS
        // map[ShaderInfo::ShaderFeature::GeometryShowNormals]         = "GEOMETRY_SHOW_NORMALS";  // IMPLEMENT GEOM NORMALS
        map[ShaderInfo::ShaderFeature::FragmentFullColor]           = "FRAGMENT_FULL_COLOR";
        map[ShaderInfo::ShaderFeature::FragmentDepthView]           = "FRAGMENT_DEPTH_VIEW";
        map[ShaderInfo::ShaderFeature::FragmentMeshMaterial]        = "FRAGMENT_MESH_MATERIAL";
        map[ShaderInfo::ShaderFeature::FragmentBypassVertexColor]   = "FRAGMENT_BYPASS_VERTEX_COLOR";
        // map[ShaderInfo::ShaderFeature::FragmentFlatShading]         = "FRAGMENT_FLAT_SHADING";  // IMPLEMENT FRAG FLAT
        map[ShaderInfo::ShaderFeature::FragmentPhong]               = "FRAGMENT_PHONG";
        map[ShaderInfo::ShaderFeature::FragmentBlinnPhong]          = "FRAGMENT_BLINN_PHONG";
        map[ShaderInfo::ShaderFeature::FragmentGammaCorrection]     = "FRAGMENT_GAMMA_CORRECTION";
        // map[ShaderInfo::ShaderFeature::FragmentOutline]             = "FRAGMENT_OUTLINE";       // IMPLEMENT FRAG OUTLINE
        // map[ShaderInfo::ShaderFeature::FragmentCubemap]             = "FRAGMENT_CUBEMAP";       // IMPLEMENT FRAG CUBEMAP
        // map[ShaderInfo::ShaderFeature::FragmentBlending]            = "FRAGMENT_BLENDING";      // IMPLEMENT FRAG BLENDING
        // map[ShaderInfo::ShaderFeature::FragmentShadows]             = "FRAGMENT_SHADOWS";       // IMPLEMENT FRAG SHADOWS

        runOnce = true;
    }

    return map;
}

std::string ShaderBuilder::GenerateDefineDirectives(const std::vector<ShaderInfo::ShaderFeature>& features)
{
    static const std::unordered_map<ShaderInfo::ShaderFeature, std::string>& defineMacros = FeatureDefineMacros();

    std::function<std::string(std::string, ShaderInfo::ShaderFeature)> addDirective = [](std::string aggregate, ShaderInfo::ShaderFeature feature) -> std::string
    {
        auto it = defineMacros.find(feature);
        if (it == defineMacros.cend())
        {
            std::string s = "ShaderBuilder: cannot generate #define directive for feature \"" + std::to_string(feature) + "\" (" + std::to_string(unsigned int (feature)) + ").";
            throw std::runtime_error(s.c_str());
        }
        return aggregate + "#define " + it->second + "\n";
    };

    return std::accumulate(features.cbegin(), features.cend(), (std::string)(""), addDirective);
}