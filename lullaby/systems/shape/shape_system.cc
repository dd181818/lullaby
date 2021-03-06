/*
Copyright 2017 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS-IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "lullaby/systems/shape/shape_system.h"

#include "lullaby/modules/render/mesh_util.h"
#include "lullaby/systems/render/render_system.h"
#include "lullaby/generated/shape_def_generated.h"

namespace lull {

ShapeSystem::ShapeSystem(Registry* registry) : System(registry) {
  RegisterDef(this, ConstHash("SphereDef"));
  RegisterDef(this, ConstHash("RectMeshDef"));
  RegisterDependency<RenderSystem>(this);
}

void ShapeSystem::PostCreateComponent(Entity entity,
                                      const Blueprint& blueprint) {
  if (blueprint.Is<SphereDefT>()) {
    SphereDefT sphere;
    blueprint.Read(&sphere);
    MeshData mesh = CreateLatLonSphere(sphere.radius, sphere.num_parallels,
                                       sphere.num_meridians);

    auto* render_system = registry_->Get<RenderSystem>();
    render_system->SetMesh(entity, mesh);
  } else if (blueprint.Is<RectMeshDefT>()) {
    RectMeshDefT quad;
    blueprint.Read(&quad);
    MeshData mesh = CreateQuadMesh<VertexPT>(
        quad.size_x, quad.size_y, quad.verts_x, quad.verts_y,
        quad.corner_radius, quad.corner_verts);

    auto* render_system = registry_->Get<RenderSystem>();
    render_system->SetMesh(entity, mesh);
  } else {
    LOG(DFATAL) << "Unsupported shape.";
  }
}

}  // namespace lull
