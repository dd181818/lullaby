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

#include "lullaby/base/blueprint_type.h"

#include <string.h>
#include "lullaby/util/logging.h"

namespace lull {

BlueprintType BlueprintType::CreateFromSchemaNameHash(HashValue type) {
  return BlueprintType(0, type);
}

bool BlueprintType::operator==(const BlueprintType& rhs) const {
  if (type_ == 0 || rhs.type_ == 0) {
    return name_ == rhs.name_;
  } else {
    return type_ == rhs.type_;
  }
}

bool BlueprintType::operator!=(const BlueprintType& rhs) const {
  return !(*this == rhs);
}

HashValue BlueprintType::GetSchemaNameHash() const { return name_; }

HashValue BlueprintType::GenerateSchemaNameHashFromTypeName(const char* name) {
  // The generated typename is suffixed with a "T" (to distinguish it from
  // the flatbuffer type).  We need to drop that "T" (hence the -1) before
  // hashing the type name to be consistent with the flatbuffer name.
  const size_t len = strlen(name) - 1;

  // The names generated by the flatc compiler do not include the namespace for
  // legacy reasons.  This is enforced by the --no-union-value-namespacing
  // flag that we pass into flatc when generating the code.  As a result, we
  // also need to drop any namespaces in front of the typenames here.
  // Iterate backwards until we encounter a namespace delimiter (ie. colon) and,
  // if so, only hash that name (and ignore the namespaces).
  for (size_t iter = len; iter > 0; --iter) {
    if (name[iter] == ':') {
      ++iter;  // Start after the colon.
      return Hash(&name[iter], len - iter);
    }
  }

  // Hash the entire name (since it has no namespaces).
  return Hash(name, len);
}

}  // namespace lull