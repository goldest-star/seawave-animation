#include "hierarchy_mesh_drawable.hpp"

#include <set>

namespace vcl {

static void assert_valid_hierarchy(const hierarchy_mesh_drawable& hierarchy);






void hierarchy_mesh_drawable::add(const hierarchy_mesh_drawable_node& node)
{
    name_map[node.name] = static_cast<int>(elements.size());
    elements.push_back(node);
    assert_valid_hierarchy(*this);
}


// Shortcut to add a new node
void hierarchy_mesh_drawable::add(const mesh_drawable& element,
                             const std::string& name,
                             const std::string& name_parent,
                             const affine_transform& transform)
{
    hierarchy_mesh_drawable_node node(element, name, name_parent, transform);
    add(node);
}

// Shortcut to add directly an initial translation instead of an affine transform
void hierarchy_mesh_drawable::add(const mesh_drawable& element,
                             const std::string& name,
                             const std::string& name_parent,
                             const vec3& translation)
{
    hierarchy_mesh_drawable_node node(element, name, name_parent, translation);
    add(node);
}


void hierarchy_mesh_drawable::set_shader_for_all_elements(GLuint shader)
{
    const size_t N = elements.size();
    for(size_t k=0; k<N; ++k)
        elements[k].element.shader = shader;
}

hierarchy_mesh_drawable_node& hierarchy_mesh_drawable::operator[](const std::string& name)
{
    auto it = name_map.find(name);
    if(it==name_map.end())
    {
        std::cerr<<"Error: cannot find element ["<<name<<"] in hierarchy_mesh_drawable"<<std::endl;
        std::cerr<<"Possibles element names are: ";
        for( auto const& s : name_map ) { std::cerr<<"["<<s.first<<"] "; }
        abort();
    }
    const size_t index = it->second;
    assert_vcl_no_msg(index<elements.size());

    return elements[index];
}
const hierarchy_mesh_drawable_node& hierarchy_mesh_drawable::operator[](const std::string& name) const
{
    auto it = name_map.find(name);
    if(it==name_map.end())
    {
        std::cerr<<"Error: cannot find element ["<<name<<"] in hierarchy_mesh_drawable"<<std::endl;
        std::cerr<<"Possibles element names are: ";
        for( auto const& s : name_map ) { std::cerr<<"["<<s.first<<"] "; }
        abort();
    }

    const size_t index = it->second;
    assert_vcl_no_msg(index<elements.size());

    return elements[index];
}




void hierarchy_mesh_drawable::update_local_to_global_coordinates()
{
    assert_vcl_no_msg(elements.size()>0);

    assert_valid_hierarchy(*this);

    const std::string name_root_parent = elements[0].name_parent;

    const size_t N = static_cast<int>(elements.size());
    for(size_t k=0; k<N; ++k)
    {
        hierarchy_mesh_drawable_node& element = elements[k];

        const std::string parent_name = element.name_parent;

        // Case of root element (or same parent) - local=global
        if( parent_name == name_root_parent ) {
            element.global_transform = element.transform;
        }
        // Else apply hierarchical transformation
        else
        {
            const affine_transform& local = element.transform;
            const affine_transform& global_parent = (*this)[parent_name].global_transform;

            element.global_transform = global_parent * local;
        }
    }
}

void draw(const hierarchy_mesh_drawable& hierarchy, const camera_scene& camera, int shader)
{
    const size_t N = hierarchy.elements.size();
    for(size_t k=0; k<N; ++k)
    {
        const hierarchy_mesh_drawable_node& node = hierarchy.elements[k];

        // copy of the mesh drawable (lightweight element) - to preserve its uniform parameters
        mesh_drawable visual_element = node.element;
        const affine_transform& T = node.global_transform;

        // Update local uniform values (and combine them with uniform already stored in the mesh)
        visual_element.uniform.transform = T * visual_element.uniform.transform;

        if(shader==-1) // Use the shader associated to the current visual_element
            vcl::draw(visual_element, camera);
        else // use the shader set in the argument
            vcl::draw(visual_element, camera, shader);
    }
}

void assert_valid_hierarchy(const hierarchy_mesh_drawable& hierarchy)
{
    if(hierarchy.elements.size()==0)
        return ;

    {
        // Check that elements and name_map have the same size
        const size_t N1 = hierarchy.elements.size();
        const size_t N2 = hierarchy.name_map.size();
        if(N1!=N2) {
            std::cerr<<"Error: Hierarchy not valid - Number of element ("<<N1<<") != Name_map.size() ("<<N2<<")"<<std::endl;
            abort();
        }
    }

    {
        // Check that the name of the parent of root node doesn't belong to the hierarchy
        const std::string root_name_parent = hierarchy.elements[0].name_parent;
        const auto it = hierarchy.name_map.find(root_name_parent);
        if(it!=hierarchy.name_map.end()) {
            std::cerr<<"Error: Hierarchy not valid - name of the root node ("<<root_name_parent<<") cannot be an element of the hierarchy"<<std::endl;
            abort();
        }
    }

    {
        // Check that all names stored in map are designating the corresponding elements
        for(const auto& e : hierarchy.name_map)
        {
            const std::string& name = e.first;
            const int index = e.second;

            if(index>=int(hierarchy.elements.size()) || index<0)
            {
                std::cerr<<"Error: Hierarchy not valid - Incorrect index stored in name_map: "<<name<<";"<<index<<std::endl;
                abort();
            }

            const hierarchy_mesh_drawable_node& designated_element = hierarchy.elements[index];
            if(designated_element.name != name)
            {
                std::cerr<<"Error: Hierarchy not valid - Incoherent name between map ("<<name<<";"<<index<<") and element ("<<designated_element.name<<") "<<std::endl;
                abort();
            }
        }
    }

    {
        // Check that all parents are defined before their children (excepted for root parent)
        std::set<std::string> parent_names_visited;
        parent_names_visited.insert(hierarchy.elements[0].name_parent);
        const size_t N = hierarchy.elements.size();
        for(size_t k=0; k<N; ++k)
        {
            const std::string& name = hierarchy.elements[k].name;
            const std::string& parent_name = hierarchy.elements[k].name_parent;
            if(parent_names_visited.find(parent_name)==parent_names_visited.end())
            {

                std::cerr<<"Error: Hierarchy not valid"<<std::endl;
                std::cerr<<"Element ("<<name<<","<<k<<") has parent name ("<<parent_name<<") used before being defined"<<std::endl;
                abort();
            }
            else
                parent_names_visited.insert(name);
        }
    }


}




}
