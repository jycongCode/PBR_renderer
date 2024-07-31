# PBR_renderer
This project performs as a practice after learning learnopengl tutorial \
Implements a simple physics based rendering pipeline 

**In which you can**:
- load obj format files
- create pbr materials using pbr textures
- tune parameters to improve visual quality
- controll cameras to view from all angels
- skybox

**paramters tunable:**
- hdr: exposure
- gamma correction: gamma value
- metallic & roughness
- light color, light intensity
- albedo color
- model scaling
- camera focus point height
- skybox trigger

!["model-d-i-hdr-gamma"](/results/model-d-i-hdr-gamma.png "model using direct lighting and indirect lighting")
!["model-d-i-hdr-gamma"](/results/model-i-hdr-gamma.png "model only using indirect lighting")
!["model-d-i-hdr-gamma"](/results/model-d-hdr-gamma.png "model only using direct lighting")

!["sphere-d-i-hdr-gamma"](/results/sphere-d-i-hdr-gamma.png "sphere using direct lighting and indirect lighting")
!["sphere-d-i-hdr-gamma"](/results/sphere-i-hdr-gamma.png "sphere only using indirect lighting")
!["sphere-d-i-hdr-gamma"](/results/sphere-d-hdr-gamma.png "sphere only using direct lighting")
