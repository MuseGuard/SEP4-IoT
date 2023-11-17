#include "package_builder.h"
#include <string.h>

Package package_builder_build() {

  Package package;
  package.data = "package\n";
  package.size = strlen(package.data);
  // add more fields and assign values as needed
  return package;
}
