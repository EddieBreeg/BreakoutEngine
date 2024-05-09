#include "ResourceManager.hpp"

void brk::ResourceManager::Unload(Resource* res)
{
	m_Resources.erase(res->m_Id);
	delete res;
}
