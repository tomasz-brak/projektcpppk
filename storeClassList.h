#ifndef PROJEKT_STORECLASSLIST_H
#define PROJEKT_STORECLASSLIST_H

#include <memory>
#include <vector>
#include "Serializable.h"

bool storeClass(const std::vector<std::shared_ptr<ISerializable>>&);

#endif

// filepath: storeClassList.h
