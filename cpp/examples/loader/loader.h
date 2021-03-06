#pragma once
#include <string>
#include <unordered_map>
#include "rasterizer/sampler.h"
#include "scene.h"

struct aiMesh;
struct aiNode;
struct aiMaterial;
struct aiScene;
struct aiAnimation;

namespace rs {

extern void loadTexture(Texture& texture, const char* path);

class AssimpLoader {
   public:
    Scene load(const char* path);

   private:
    std::shared_ptr<Node> processNode(aiNode* ainode, std::shared_ptr<Node> parent);

    Mesh processMesh(const aiMesh* aimesh);
    Animation processAnimation(const aiAnimation* aianim);
    Material processMaterial(aiMaterial* material, const std::string& dir);

    std::unordered_map<std::string, Node*> m_nodeLookupTable;
};

}  // namespace rs
