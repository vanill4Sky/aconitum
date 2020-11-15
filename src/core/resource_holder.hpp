#pragma once

#include <algorithm>
#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

namespace aco
{

template <typename Resource>
class resource_holder
{
public:
	void load(std::string filename);
	/* overloaded function for loading shaders */
	template <typename Parameter>
	void load(std::string filename, const Parameter& second_param);

	Resource& get(const std::string& filename);
	const Resource& get(const std::string& filename) const;

private:
	void insert_resource(std::string&& filename, std::unique_ptr<Resource> resource);

	std::unordered_map<std::string, std::unique_ptr<Resource>> m_resources;
};

template<typename Resource>
inline void resource_holder<Resource>::load(std::string filename)
{
	// Create and load resource
	auto resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("resource_holder::load - Failed to load " + filename);

	// If loading successful, insert resource to map
	insert_resource(std::move(filename), std::move(resource));
}

template<typename Resource>
template<typename Parameter>
inline void resource_holder<Resource>::load(std::string filename, const Parameter& second_param)
{
	// Create and load resource
	auto resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(filename, second_param))
		throw std::runtime_error("resource_holder::load - Failed to load " + filename);

	// If loading successful, insert resource to map
	insert_resource(std::move(filename), std::move(resource));
}

template<typename Resource>
inline Resource& resource_holder<Resource>::get(const std::string& filename)
{
	auto found = m_resources.find(filename);
	assert(found != m_resources.end());

	return *found->second;
}

template<typename Resource>
inline const Resource& resource_holder<Resource>::get(const std::string& filename) const
{
	auto found = m_resources.find(filename);
	assert(found != m_resources.end());

	return *found->second;
}

template<typename Resource>
inline void resource_holder<Resource>::insert_resource(std::string&& filename, std::unique_ptr<Resource> resource)
{
	m_resources.try_emplace(filename, std::move(resource));
}

}

