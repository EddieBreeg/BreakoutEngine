template <class Res>
brk::ResourceRef<Res>::ResourceRef(Resource* ptr)
	: m_Ptr{ ptr }
{
	static_cast<Resource*>(m_Ptr)->m_RefCount++;
}
